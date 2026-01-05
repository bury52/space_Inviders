//
// Created by bogus on 5.01.2026.
//

#ifndef SPACE_INVADERS_STARTMENU_H
#define SPACE_INVADERS_STARTMENU_H

#include "SFML/Graphics/Drawable.hpp"

class StartMenu : public sf::Drawable {
public:
    sf::Font &font;
    sf::Text main_text;
    sf::Text game_text;
    Button button = Button(font,"tak",[]{ std::cout << "tak"; });

    explicit StartMenu(sf::Vector2u window_size, sf::Font &font)
        : font(font),
          main_text(font, "Space Invaders", 50),
          game_text(font, "games:") {
        // main_text
        main_text.setFillColor(sf::Color::Green);
        main_text.setOrigin(main_text.getLocalBounds().getCenter());
        main_text.setPosition({static_cast<float>(window_size.x / 2), 50});

        // game_text
        game_text.setFillColor(sf::Color::Green);
        game_text.setPosition({50, 150});

        button.setPosition({50,200});

    }

    void onMouseButtonPressed(const sf::Event::MouseButtonPressed& event) {
        button.onMouseButtonPressed(event);
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        target.draw(main_text, states);
        target.draw(game_text, states);
        target.draw(button, states);
    };
};

#endif //SPACE_INVADERS_STARTMENU_H
