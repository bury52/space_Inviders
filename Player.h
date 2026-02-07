//
// Created by bogus on 20.10.2025.
//

#ifndef SPACE_INVADERS_PLAYER_H
#define SPACE_INVADERS_PLAYER_H
#include "enum.h"
#include "concepts.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"

class Player : public sf::Drawable, public sf::Transformable {
public:
    explicit Player(const sf::Vector2f &border_x, const float &scale, const sf::Texture &texture, const int &health,
                    const int &damage, const float &speed, const float &bulletSpeed, const float &bulletDelay)
        : border_x(border_x), sprite(texture), health_max(health), damage(damage), player_speed(speed),
          bullet_speed(bulletSpeed), bullet_delay(sf::seconds(bulletDelay)) {
        setScale({scale, scale});
    }

    sf::FloatRect getBounds() const {
        return getTransform().transformRect(sprite.getGlobalBounds());
    };
    // kolizja z pociskiem
    void collision(Bullet &collider) {
        health -= collider.damage;
        collider.damage = 0;
        if (health <= 0) {
            health = 0;
            sprite.setColor(sf::Color::Red);
        }
    }
    // straowanie
    void onKeyPressed(const sf::Event::KeyPressed &event, Shooter auto &shooter) {
        if (event.scancode == sf::Keyboard::Scancode::A) {
            turn = TurnState::Left;
        } else if (event.scancode == sf::Keyboard::Scancode::D) {
            turn = TurnState::Right;
        } else if (event.scancode == sf::Keyboard::Scancode::W) {
            if (time_from_shot >= bullet_delay) {
                shooter.shoot(TurnState::Up, sf::Vector2f(getBounds().getCenter().x, getBounds().position.y), bullet_speed,
                              damage);
                time_from_shot = sf::seconds(0);
            }
        }
    }

    void onKeyReleased(const sf::Event::KeyReleased &event) {
        if ((event.scancode == sf::Keyboard::Scancode::A && turn == TurnState::Left) ||
            (event.scancode == sf::Keyboard::Scancode::D && turn == TurnState::Right)) {
            turn = TurnState::None;
        }
    }

    void update(const sf::Time &elapsed) {
        time_from_shot += elapsed;
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

    [[nodiscard]] int getHealth() const {
        return health;
    }

private:
    TurnState turn = TurnState::None;
    float player_speed = 400;
    float bullet_speed = 600;
    sf::Time bullet_delay = sf::seconds(0.2);
    sf::Time time_from_shot = sf::seconds(0);
    int damage = 3;
    int health_max = 3;
    int health = health_max;
    sf::Vector2f border_x;
    sf::Sprite sprite;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(sprite, states);
    };
};

#endif //SPACE_INVADERS_PLAYER_H
