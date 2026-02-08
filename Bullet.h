//
// Created by bogus on 5.11.2025.
//

#ifndef SPACE_INVADERS_BULLET_H
#define SPACE_INVADERS_BULLET_H
#include <variant>

#include "concepts.h"
#include "enum.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Transformable.hpp"
// pocisk
class Bullet : public sf::Drawable, public sf::Transformable {
public:
    // spełnienie konceptu CollisionObject
    sf::FloatRect getBounds() const {
        return getTransform().transformRect(shape.getGlobalBounds());
    };

    TurnState turn = TurnState::Up;
    sf::RectangleShape shape = sf::RectangleShape();
    float bullet_speed = 600;
    int damage = 3;

protected:
    // funkcja rysowania z sf::Drawable
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(shape, states);
    };
};

#endif //SPACE_INVADERS_BULLET_H
