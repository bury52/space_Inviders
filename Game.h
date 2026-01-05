//
// Created by bogus on 5.01.2026.
//

#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <SFML/Graphics/Drawable.hpp>

class Game : public sf::Drawable {
public:
    bool is_pause = false;

    void onKeyPressed(const sf::Event::KeyPressed &event) {
        if (event.scancode == sf::Keyboard::Scancode::Escape) {
            is_pause = true;
        }
    }
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    };
};

#endif //SPACE_INVADERS_GAME_H
