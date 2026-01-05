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
    const std::vector<Game_TOML> &games;
    sf::Font &font;
    sf::Text main_text;
    sf::Text game_text;
    std::vector<Button<sf::Text>> games_button = {};

    explicit StartMenu(const std::vector<Game_TOML> &games, sf::Vector2u window_size, sf::Font &font)
        : games(games)
          , font(font),
          main_text(font, "Space Invaders", 50),
          game_text(font, "games:"){

        // main_text
        main_text.setFillColor(sf::Color::Green);
        main_text.setOrigin(main_text.getLocalBounds().getCenter());
        main_text.setPosition({static_cast<float>(window_size.x / 2), 50});

        // game_text
        game_text.setFillColor(sf::Color::Green);
        game_text.setPosition({50, 150});

        games_button =  games | std::ranges::views::transform([&](const auto& e) {
            Button button = Button(std::make_unique<sf::Text>(font, e.name), [&]{ std::cout << e.name << std::endl; });
            return button;
        }) | std::ranges::to<std::vector>();

        float x = 50;
        for (auto& value : games_button) {
            value.move({x,200});
            x += value.getBounds().size.x + 25;
        }

    }

    void onMouseButtonPressed(const sf::Event::MouseButtonPressed &event) {
        for (auto& button : games_button) {
            button.onMouseButtonPressed(event);
        }
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        target.draw(main_text, states);
        target.draw(game_text, states);
        for (auto& value : games_button) {
            target.draw(value, states);
        }
    };
};

#endif //SPACE_INVADERS_STARTMENU_H
