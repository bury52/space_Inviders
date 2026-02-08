//
// Created by bogus on 5.01.2026.
//

#ifndef SPACE_INVADERS_STARTMENU_H
#define SPACE_INVADERS_STARTMENU_H

#include <ranges>
#include "TomlReader.h"
#include "SFML/Graphics/Drawable.hpp"
// menu gry
class StartMenu : public sf::Drawable {
public:
    std::optional<Game> &game;
    const std::vector<Game_TOML> &games;
    sf::Font &font;
    sf::Text main_text;
    sf::Text game_text;
    std::vector<Button<UIButton> > games_button = {};
    Button<UIButton> resume_button;

    // --- in game ---

    sf::RectangleShape bottom_line = sf::RectangleShape();
    sf::Text health_text;
    int old_health_player = 0;
    sf::Text game_over_text;
    const sf::Time delay_default = sf::seconds(3);
    sf::Time delay = delay_default;
    sf::Text next_level_text;
    sf::Vector2u window_size_;
    // konstruktor
    explicit StartMenu(std::optional<Game> &game, const std::vector<Game_TOML> &games,
                       const std::function<void(const Game_TOML &)> &create_game, sf::Vector2u window_size,
                       sf::Font &font)
        : game(game),
          games(games),
          font(font),
          main_text(font, "Space Invaders", 50),
          game_text(font, "games:"),
          resume_button(
              [&] {
                  auto text_ = std::make_unique<UIButton>(sf::Text(font, "resume"));
                  text_->setPosition({200, 150});
                  text_->text.setFillColor(sf::Color::Red);
                  return std::move(text_);
              }(), [&] { game->is_pause = false; }
          ),
          health_text(font, "health:", 40),
          game_over_text(font, "game over", 150),
          next_level_text(font, "next level", 150), window_size_(window_size) {
        // main_text
        main_text.setFillColor(sf::Color::Green);
        main_text.setOrigin(main_text.getLocalBounds().getCenter());
        main_text.setPosition({static_cast<float>(window_size.x / 2), 50});

        // game_text
        game_text.setFillColor(sf::Color::Green);
        game_text.setPosition({50, 150});

        games_button = games | std::views::transform([&](const auto &e) {
            Button button = Button(std::make_unique<UIButton>(sf::Text(font, e.name)), [&] { create_game(e); });
            return button;
        }) | std::ranges::to<std::vector>();

        float x = 50;
        for (auto &value: games_button) {
            value.move({x, 200});
            x += value.getBounds().size.x + 25;
        }

        // --- in game ---

        bottom_line.setPosition({0, static_cast<float>(window_size.y - 110)});
        bottom_line.setSize({static_cast<float>(window_size.x), 5});
        bottom_line.setFillColor(sf::Color::Green);

        health_text.setFillColor(sf::Color::Green);
        health_text.setPosition({50, static_cast<float>(window_size.y - 85)});

        game_over_text.setFillColor(sf::Color::Red);
        game_over_text.setOrigin(game_over_text.getLocalBounds().getCenter());
        game_over_text.setPosition({static_cast<float>(window_size.x / 2), static_cast<float>(window_size.y / 2)});

        next_level_text.setFillColor(sf::Color::Green);
        next_level_text.setOrigin(next_level_text.getLocalBounds().getCenter());
        next_level_text.setPosition({static_cast<float>(window_size.x / 2), static_cast<float>(window_size.y / 2)});
    }

    // klikanie przycisku
    void onMouseButtonPressed(const sf::Event::MouseButtonPressed &event) {
        for (auto &button: games_button) {
            button.onMouseButtonPressed(event);
        }
        if (game && game->is_pause) {
            resume_button.onMouseButtonPressed(event);
        }
    }
    // scrollowanie
    void onMouseWheelScrolled(const sf::Event::MouseWheelScrolled &event) {
        if (!game || game->is_pause) {
            for (auto& button : games_button) {
                button.move({event.delta * 10,0});
            }
        }
    }
    // Aktualizacja stanu, pobieranie informacji z gry np. zdrowia
    void update(const sf::Time &elapsed) {
        if (game) {
            if (game->player.getHealth() <= 0) {
                delay -= elapsed;
                if (delay <= sf::seconds(0)) {
                    game = std::nullopt;
                    delay = delay_default;
                }
            } else {
                // pokazywanie aktualnego życia
                if (game->player.getHealth() != old_health_player) {
                    std::string h = " []";
                    std::string current = "health:";
                    current.reserve(current.length() + h.length() * game->player.getHealth());
                    for (int i = 0; i < game->player.getHealth(); ++i) {
                        current.append(h);
                    }
                    health_text.setString(current);
                }

                if (game->current_enemy.size() == 0) {
                    delay -= elapsed;
                    next_level_text.setString(game->end_title);
                    next_level_text.setOrigin(next_level_text.getLocalBounds().getCenter());
                    next_level_text.setPosition({static_cast<float>(window_size_.x / 2), static_cast<float>(window_size_.y / 2)});
                    if (delay <= sf::seconds(0)) {
                        if (game->game_toml_.levels.size() > game->level + 1) {
                            game->next_level();
                        }else {
                            game = std::nullopt;
                        }
                        delay = delay_default;
                    }
                }
            }
        }
    }

protected:
    // funkcja rysowania z sf::Drawable
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        if (game) {
            if (!game->is_pause) {
                target.draw(bottom_line, states);
                target.draw(health_text, states);
                if (game->player.getHealth() <= 0) {
                    target.draw(game_over_text, states);
                }else if (game->current_enemy.size() == 0) {
                    target.draw(next_level_text, states);
                }
                return;
            }
            target.draw(resume_button, states);
        }

        target.draw(main_text, states);
        target.draw(game_text, states);
        for (auto &value: games_button) {
            target.draw(value, states);
        }
    };
};

#endif //SPACE_INVADERS_STARTMENU_H
