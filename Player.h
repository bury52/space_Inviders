//
// Created by bogus on 20.10.2025.
//

#ifndef SPACE_INVADERS_PLAYER_H
#define SPACE_INVADERS_PLAYER_H
#include "enum.h"
#include "concepts.h"
#include "structs.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"

class Player : public sf::Drawable, public sf::Transformable {
public:
    explicit Player(const sf::Vector2f &border_x,const float& scale,const Res& res)
        : border_x(border_x), res(res), sprite(res.player) {
        setScale({scale, scale});
    }

    sf::FloatRect getBounds() const {
        return getTransform().transformRect(sprite.getGlobalBounds());
    };

    void collision(Bullet& collider) {
        sprite.setColor(sf::Color::Red);
    }

    void onKeyPressed(const sf::Event::KeyPressed &event, Shooter auto& shooter) {
        if (event.scancode == sf::Keyboard::Scancode::A) {
            turn = TurnState::Left;
        } else if (event.scancode == sf::Keyboard::Scancode::D) {
            turn = TurnState::Right;
        } else if (event.scancode == sf::Keyboard::Scancode::W) {
            shooter.shoot(TurnState::Up, sf::Vector2f(getBounds().getCenter().x,getBounds().position.y),bullet_speed,damage);
        }
    }

    void onKeyReleased(const sf::Event::KeyReleased &event) {
        if ((event.scancode == sf::Keyboard::Scancode::A && turn == TurnState::Left) ||
            (event.scancode == sf::Keyboard::Scancode::D && turn == TurnState::Right)) {
            turn = TurnState::None;
        }
    }

    void update(const sf::Time &elapsed) {
        if (turn == TurnState::Left) {
            move({elapsed.asSeconds() * -player_speed, 0});
        } else if (turn == TurnState::Right) {
            move({elapsed.asSeconds() * player_speed, 0});
        }

        if (getPosition().x < border_x.x) {
            setPosition({border_x.x, getPosition().y});
        } else {
            if (float border_position_x = border_x.y - getBounds().size.x;
                getPosition().x > border_position_x) {
                setPosition({border_position_x, getPosition().y});
            }
        }
    }

private:
    TurnState turn = TurnState::None;
    float player_speed = 400;
    float bullet_speed = 600;
    int damage = 3;
    sf::Vector2f border_x;
    const Res& res;
    sf::Sprite sprite;


protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(sprite, states);
    };
};

#endif //SPACE_INVADERS_PLAYER_H
