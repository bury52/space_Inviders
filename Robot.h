//
// Created by bogus on 20.10.2025.
//

#ifndef SPACE_INVADERS_ROBOT_H
#define SPACE_INVADERS_ROBOT_H
#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

#include "concepts.h"
#include "enum.h"
#include "structs.h"
#include "util.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Time.hpp"

class Enemy_Controller;

class Robot : public sf::Drawable, public sf::Transformable {
public:
    explicit Robot(const Res &res, const float &scale) : sprite(res.robot) {
        setScale({scale, scale});
    };

    sf::FloatRect get_bounds() const {
        return getTransform().transformRect(sprite.getGlobalBounds());
    }



    void update(const sf::Time &elapsed,CollisionObjectWith<sf::RectangleShape> auto &player) {
        if (!can_shoot)
            return;

        const sf::FloatRect player_bounds = player.get_bounds();

        sprite.setColor(sf::Color::Red);
        time_from_shot += elapsed;
        if (time_from_shot >= bullet_delay && get_bounds().getCenter().x - 5 < player_bounds.getCenter().x && get_bounds().
            getCenter().x + 5 > player_bounds.getCenter().x) {
            auto &bullet = bullets.emplace_back(sf::Vector2f(getScale().x, getScale().y * 3));
            bullet.setPosition({get_bounds().getCenter().x, getPosition().y});
            time_from_shot = sf::seconds(0);
        }
        for (auto &bullet: bullets) {
            bullet.move({0, elapsed.asSeconds() * bullet_speed});
            if (player_bounds.findIntersection(bullet.getGlobalBounds())) {
                player.collision(bullet);
            }
        }
    };

    bool can_shoot = false;

private:
    sf::Time bullet_delay = sf::seconds(3);
    sf::Time time_from_shot = sf::seconds(0);
    float bullet_speed = 400;
    std::vector<sf::RectangleShape> bullets = {};
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


class Enemy_Controller {
public:
    explicit Enemy_Controller(std::vector<std::shared_ptr<Robot> > &current_enemy, const std::vector<float> &enemy_y,
                              const sf::Vector2f &border_x, const Res &res)
        : current_enemy(current_enemy), enemy_y(enemy_y),
          enemy_line(enemy_y.size(), std::vector<std::weak_ptr<Robot> >()), border_x(border_x), res(res) {
    }

    void update(const sf::Time &elapsed) {
        if (turn == TurnState::Left) {
            for (const auto &robot: current_enemy) {
                robot->move({elapsed.asSeconds() * -robot_speed, 0});
            }
            if (std::ranges::any_of(current_enemy, [&](const auto &enemy_) {
                return enemy_->getPosition().x < border_x.x;
            })) {
                update_y();
                turn = TurnState::Right;
            }
        } else if (turn == TurnState::Right) {
            for (const auto &robot: current_enemy) {
                robot->move({elapsed.asSeconds() * robot_speed, 0});
            }

            if (std::ranges::any_of(current_enemy, [&](const auto &enemy_) {
                return get_right_bound(enemy_->get_bounds()) > border_x.y;
            })) {
                update_y();
                turn = TurnState::Left;
            }
        }
        update_can_shoot();
    };

    Enemy_Controller &add_enemy(const int &line) {
        auto robot = std::make_shared<Robot>(res, robot_scale);
        erase_if(enemy_line[line], [](const std::weak_ptr<Robot> &enemy) { return enemy.expired(); });
        enemy_line[line].push_back(robot);
        current_enemy.push_back(std::move(robot));
        return *this;
    }

    void set_start_position() {
        for (int i = 0; i < enemy_line.size(); ++i) {
            set_position_in_line(enemy_line[i], i);
        }
        turn = TurnState::Right;
    }

    TurnState turn = TurnState::Right;
    float robot_speed = 200;
    const Res &res;
    float robot_scale = 5;
    std::vector<std::shared_ptr<Robot> > &current_enemy;
    std::vector<float> enemy_y;
    std::vector<std::vector<std::weak_ptr<Robot> > > enemy_line;
    sf::Vector2f border_x;

private:
    static void delete_expired(std::vector<std::weak_ptr<Robot> > &current_line) {
        erase_if(current_line, [](const std::weak_ptr<Robot> &enemy) { return enemy.expired(); });
    }

    void set_position_in_line(std::vector<std::weak_ptr<Robot> > &current_line, const int &line) {
        delete_expired(current_line);
        float size_x = static_cast<float>(res.robot.getSize().x) * robot_scale;
        float x_position = border_x.x;
        for (auto &enemy: current_line) {
            if (const std::shared_ptr<Robot> lock_enemy = enemy.lock()) {
                lock_enemy->setPosition({x_position, enemy_y[line]});
                x_position += size_x + 10;
            }
        }
    }

    void update_y() {
        delete_expired(enemy_line.back());
        if (enemy_line.back().empty()) {
            std::ranges::rotate(enemy_line, enemy_line.end() - 1);
        }
        for (int i = 0; i < enemy_line.size(); ++i) {
            for (auto &enemy: enemy_line[i]) {
                if (const std::shared_ptr<Robot> lock_enemy = enemy.lock()) {
                    lock_enemy->setPosition({lock_enemy->getPosition().x, enemy_y[i]});
                }
            }
        }
    }

    void update_can_shoot() {
        std::vector<float> occupied_lines;
        for (const auto &line: enemy_line | std::ranges::views::reverse) {
            for (const auto &enemy_: line) {
                if (const std::shared_ptr<Robot> lock_enemy = enemy_.lock()) {
                    if (std::ranges::none_of(occupied_lines, [&](const float &occupied_x) {
                        return occupied_x == lock_enemy->getPosition().x;
                    })) {
                        lock_enemy->can_shoot = true;
                        occupied_lines.push_back(lock_enemy->getPosition().x);
                    }
                }
            }
        }
    }
};

#endif //SPACE_INVADERS_ROBOT_H
