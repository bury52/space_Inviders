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

    sf::FloatRect get_bounds() const {
        return getTransform().transformRect(sprite.getGlobalBounds());
    };

    void collision(Bullet& collider) {
        sprite.setColor(sf::Color::Red);
    }

    void onKeyPressed(const sf::Event::KeyPressed &event) {
        if (event.scancode == sf::Keyboard::Scancode::A) {
            turn = TurnState::Left;
        } else if (event.scancode == sf::Keyboard::Scancode::D) {
            turn = TurnState::Right;
        } else if (event.scancode == sf::Keyboard::Scancode::W) {
            auto &bullet = bullets.emplace_back(sf::Vector2f(getScale().x, getScale().y * 3));
            bullet.setPosition({getPosition().x + (get_bounds().size.x / 2), getPosition().y});
        }
    }

    void onKeyReleased(const sf::Event::KeyReleased &event) {
        if ((event.scancode == sf::Keyboard::Scancode::A && turn == TurnState::Left) ||
            (event.scancode == sf::Keyboard::Scancode::D && turn == TurnState::Right)) {
            turn = TurnState::None;
        }
    }

    template<CollisionObject Enemy>
    void update(const sf::Time &elapsed,std::vector<std::shared_ptr<Enemy> > &enemy) {
        if (turn == TurnState::Left) {
            move({elapsed.asSeconds() * -player_speed, 0});
        } else if (turn == TurnState::Right) {
            move({elapsed.asSeconds() * player_speed, 0});
        }

        if (getPosition().x < border_x.x) {
            setPosition({border_x.x, getPosition().y});
        } else {
            if (float border_position_x = border_x.y - get_bounds().size.x;
                getPosition().x > border_position_x) {
                setPosition({border_position_x, getPosition().y});
            }
        }

        for (auto &bullet: bullets) {
            bullet.move({0, elapsed.asSeconds() * -bullet_speed});
        }

        std::erase_if(bullets, [&](auto &bullet) {
            auto bullet_rect = bullet.getGlobalBounds();

            bool collided = std::erase_if(enemy, [&](auto &enemy_) {
                auto enemy_rect = enemy_->get_bounds();
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
    sf::Vector2f border_x;
    const Res& res;
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
