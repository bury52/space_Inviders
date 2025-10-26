//
// Created by bogus on 20.10.2025.
//

#ifndef SPACE_INVADERS_ROBOT_H
#define SPACE_INVADERS_ROBOT_H
#include <vector>

#include "enum.h"
#include "util.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Time.hpp"


class Robot;

class Enemy_Controller {
public:
    explicit Enemy_Controller(const std::vector<float> &enemy_y)
        : enemy_y(enemy_y), enemy_line(enemy_y.size(), std::vector<std::weak_ptr<Robot> >()) {
    }

    std::vector<float> enemy_y;
    std::vector<std::vector<std::weak_ptr<Robot> > > enemy_line;
};

class Robot : public sf::Drawable, public sf::Transformable {
public:
    explicit Robot(const sf::Vector2f &border_x, Enemy_Controller &controller,
                   const std::shared_ptr<sf::Texture> &texture)
        : border_x(border_x), controller_(controller), texture(texture), sprite(*texture) {
        setScale({5, 5});
        turn = TurnState::Right;
    }

    sf::FloatRect getGlobalBounds() const {
        return getTransform().transformRect(sprite.getGlobalBounds());
    }

    float getSize_x() const {
        return (static_cast<float>(texture->getSize().x) * getScale().x);
    }

    void update(const sf::Time &elapsed) {
        if (const auto line = std::ranges::find_if(controller_.enemy_line, [&](auto &line_) {
                auto find_if = std::ranges::find_if(line_, [&](const std::weak_ptr<Robot> &enemy) {
                    if (const auto lock_enemy = enemy.lock()) {
                        if (&*lock_enemy == this) {
                            return true;
                        }
                    }
                    return false;
                });
                return find_if != line_.end();
            });
            line != controller_.enemy_line.end()) {
            if (turn == TurnState::Left) {
                move({elapsed.asSeconds() * -robot_speed, 0});
            } else if (turn == TurnState::Right) {
                move({elapsed.asSeconds() * robot_speed, 0});
            }

            if (getPosition().x < border_x.x) {
                setPosition({border_x.x, getPosition().y});
                turn = TurnState::Right;
            } else {
                if (float border_position_x = border_x.y - getSize_x();
                    getPosition().x > border_position_x) {
                    setPosition({border_position_x, getPosition().y});
                    turn = TurnState::Left;
                }
            }

            for (auto &bullet: bullets) {
                bullet.move({0, elapsed.asSeconds() * bullet_speed * -1});
            }
            std::erase_if(bullets, [](const auto &bullet) { return bullet.getPosition().y < 0; });
        }
    }

private:
    TurnState turn = TurnState::None;
    float robot_speed = 200;
    float bullet_speed = 400;
    std::vector<sf::RectangleShape> bullets = {};
    sf::Vector2f border_x;
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    Enemy_Controller &controller_;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(sprite, states);
        for (const auto &bullet: bullets) {
            target.draw(bullet);
        }
    };
};


class Enemy_Factory {
public:
    Enemy_Factory(std::vector<std::shared_ptr<Robot> > &current_enemy, Enemy_Controller &enemy_controller,
                  const sf::Vector2f &border_x)
        : current_enemy(current_enemy),
          enemy_controller(enemy_controller),
          border_x(border_x) {
    }


    Enemy_Factory &add_enemy(const int &line) {
        auto robot = std::make_shared<Robot>(border_x, enemy_controller, robot_texture);
        current_enemy.push_back(robot);
        std::vector<std::weak_ptr<Robot> > &current_line = enemy_controller.enemy_line[line];

        erase_if(current_line, [](const std::weak_ptr<Robot> &enemy) { return enemy.expired(); });
        current_line.push_back(robot);
        float size_x = static_cast<float>(robot_texture->getSize().x) * robot->getScale().x;
        float x_position = border_x.x;
        for (auto &enemy: current_line) {
            if (const std::shared_ptr<Robot> lock_enemy = enemy.lock()) {
                lock_enemy->setPosition({x_position, enemy_controller.enemy_y[line]});
                x_position += size_x + 20;
            }
        }

        return *this;
    }

private:
    std::shared_ptr<sf::Texture> robot_texture = std::make_shared<sf::Texture>("robak.png");
    std::vector<std::shared_ptr<Robot> > &current_enemy;
    Enemy_Controller &enemy_controller;
    sf::Vector2f border_x;
};


#endif //SPACE_INVADERS_ROBOT_H
