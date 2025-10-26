//
// Created by bogus on 20.10.2025.
//

#ifndef SPACE_INVADERS_ROBOT_H
#define SPACE_INVADERS_ROBOT_H
#include <algorithm>
#include <vector>

#include "enum.h"
#include "util.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Time.hpp"

class Enemy_Controller;

class Robot : public sf::Drawable, public sf::Transformable {
public:
    explicit Robot(const std::shared_ptr<sf::Texture> &texture,const float& scale);

    sf::FloatRect getGlobalBounds() const;

    float getSize_x() const;

    void update(const sf::Time &elapsed);

private:
    float bullet_speed = 400;
    std::vector<sf::RectangleShape> bullets = {};
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;;
};


class Enemy_Controller {
public:
    explicit Enemy_Controller(std::vector<std::shared_ptr<Robot> > &current_enemy, const std::vector<float> &enemy_y,
                              const sf::Vector2f &border_x)
        : current_enemy(current_enemy), enemy_y(enemy_y),
          enemy_line(enemy_y.size(), std::vector<std::weak_ptr<Robot> >()), border_x(border_x) {
    }

    void update(const sf::Time &elapsed) {
        if (turn == TurnState::Left) {
            for (const auto &robot: current_enemy) {
                robot->move({elapsed.asSeconds() * -robot_speed, 0});
            }
            if (std::ranges::any_of(current_enemy, [&](const auto &enemy_) {return enemy_->getPosition().x < border_x.x;})) {
                update_y();
                turn = TurnState::Right;
            }
        } else if (turn == TurnState::Right) {
            for (const auto &robot: current_enemy) {
                robot->move({elapsed.asSeconds() * robot_speed, 0});
            }
            if (std::ranges::any_of(current_enemy, [&](const auto &enemy_) {return enemy_->getPosition().x + enemy_->getSize_x() > border_x.y;})) {
                update_y();
                turn = TurnState::Left;
            }
        }
    };

    Enemy_Controller &add_enemy(const int &line) {
        auto robot = std::make_shared<Robot>(robot_texture,robot_scale);
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
    std::shared_ptr<sf::Texture> robot_texture = std::make_shared<sf::Texture>("robak.png");
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
        float size_x = static_cast<float>(robot_texture->getSize().x) * robot_scale;
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
            std::ranges::rotate(enemy_line,enemy_line.end()-1);
        }
        for (int i = 0; i < enemy_line.size(); ++i) {
            for (auto &enemy: enemy_line[i]) {
                if (const std::shared_ptr<Robot> lock_enemy = enemy.lock()) {
                    lock_enemy->setPosition({lock_enemy->getPosition().x, enemy_y[i]});
                }
            }
        }
    }
};

#endif //SPACE_INVADERS_ROBOT_H
