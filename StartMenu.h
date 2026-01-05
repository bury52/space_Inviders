//
// Created by bogus on 5.01.2026.
//

#ifndef SPACE_INVADERS_STARTMENU_H
#define SPACE_INVADERS_STARTMENU_H

#include <ranges>
#include "TomlReader.h"
#include "SFML/Graphics/Drawable.hpp"

class StartMenu : public sf::Drawable {
public:
    std::optional<Game> &game;
    const std::vector<Game_TOML> &games;
    sf::Font &font;
    sf::Text main_text;
    sf::Text game_text;
    std::vector<Button<sf::Text> > games_button = {};
    Button<sf::Text> resume_button;

    explicit StartMenu(std::optional<Game> &game, const std::vector<Game_TOML> &games, sf::Vector2u window_size,
                       sf::Font &font)
        : game(game),
          games(games),
          font(font),
          main_text(font, "Space Invaders", 50),
          game_text(font, "games:"),
          resume_button(
              [&] {
                  auto text_ = std::make_unique<sf::Text>(font,"resume");
                  text_->setPosition({200,150});
                  text_->setFillColor(sf::Color::Red);
                  return std::move(text_);
              }(),[&]{game->is_pause = false;}
              ) {
        // main_text
        main_text.setFillColor(sf::Color::Green);
        main_text.setOrigin(main_text.getLocalBounds().getCenter());
        main_text.setPosition({static_cast<float>(window_size.x / 2), 50});

        // game_text
        game_text.setFillColor(sf::Color::Green);
        game_text.setPosition({50, 150});

        games_button = games | std::ranges::views::transform([&](const auto &e) {
            Button button = Button(std::make_unique<sf::Text>(font, e.name), [&] { game = Game(); });
            return button;
        }) | std::ranges::to<std::vector>();

        float x = 50;
        for (auto &value: games_button) {
            value.move({x, 200});
            x += value.getBounds().size.x + 25;
        }
    }

    void onMouseButtonPressed(const sf::Event::MouseButtonPressed &event) {
        for (auto &button: games_button) {
            button.onMouseButtonPressed(event);
        }
        if (game && game->is_pause) {
            resume_button.onMouseButtonPressed(event);
        }
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        if (game) {
            if (!game->is_pause) {
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
