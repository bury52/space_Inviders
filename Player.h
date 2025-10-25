//
// Created by bogus on 20.10.2025.
//

#ifndef SPACE_INVADERS_PLAYER_H
#define SPACE_INVADERS_PLAYER_H
#include "enum.h"
#include "Robot.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"

class Player : public sf::Drawable, public sf::Transformable {
public:
    explicit Player(const sf::Vector2f &border_x, std::vector<Robot> &enemy)
        : border_x(border_x), enemy(enemy), texture("statek.png"), sprite(texture) {
        setScale({5, 5});
    }

    float getSize_x() const {
        return (static_cast<float>(texture.getSize().x) * getScale().x);
    }

    void onKeyPressed(const sf::Event::KeyPressed &event) {
        if (event.scancode == sf::Keyboard::Scancode::A) {
            turn = TurnState::Left;
        } else if (event.scancode == sf::Keyboard::Scancode::D) {
            turn = TurnState::Right;
        } else if (event.scancode == sf::Keyboard::Scancode::W) {
            auto &bullet = bullets.emplace_back(sf::Vector2f(getScale().x, getScale().y * 3));
            bullet.setPosition({getPosition().x + (getSize_x() / 2), getPosition().y});
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
            if (float border_position_x = border_x.y - getSize_x();
                getPosition().x > border_position_x) {
                setPosition({border_position_x, getPosition().y});
            }
        }


        for (auto &bullet: bullets) {
            bullet.move({0, elapsed.asSeconds() * bullet_speed * -1});
        }


        std::erase_if(bullets, [&](auto &bullet) {
            auto bullet_rect = bullet.getGlobalBounds();

            bool collided = std::erase_if(enemy, [&](auto &enemy) {
                auto enemy_rect = enemy.getGlobalBounds();
                return enemy_rect.findIntersection(bullet_rect).has_value();
            }) > 0;

            return collided || bullet.getPosition().y < 0;
        });

    }

private:
    TurnState turn = TurnState::None;
    float player_speed = 400;
    float bullet_speed = 600;
    std::vector<sf::RectangleShape> bullets = {};
    std::vector<Robot> &enemy;
    sf::Vector2f border_x;
    sf::Texture texture;
    sf::Sprite sprite;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(sprite, states);
        for (const auto &bullet: bullets) {
            target.draw(bullet);
        }
    };
};

#endif //SPACE_INVADERS_PLAYER_H
