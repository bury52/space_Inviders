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
#include "SFML/System/Time.hpp"

class Bullet : public sf::Drawable, public sf::Transformable {
public:
    TurnState turn = TurnState::Up;
    sf::RectangleShape shape = sf::RectangleShape();
    float bullet_speed = 600;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(shape, states);
    };
};

template<CollisionObjectWith<Bullet> ...Target>
class Bullet_Controller : public sf::Drawable {
public:
    template<CollisionObjectWith<Bullet> ...Add_Target>
    class Buller_Helper {
    public:
        void shoot(const TurnState &turn, const sf::Vector2f &start_position, const float &bullet_speed) {
            std::apply([&](auto &... targets_) {
                self->add_bullet(turn, start_position, bullet_speed, targets_...);
            }, targets);
        }

    private:
        explicit Buller_Helper(Bullet_Controller<Target...> *const self, std::vector<Add_Target> &... targets)
            : self(self), targets(targets...) {
        }

        Bullet_Controller<Target...> *const self;
        std::tuple<std::vector<Add_Target>&...> targets;
    };

    template<CollisionObjectWith<Bullet> ...Add_Target>
    void add_bullet(const TurnState &turn, const sf::Vector2f &start_position, const float &bullet_speed,
                    std::vector<Add_Target> &... targets) {
        Bullet_Wraperr wraperr;
        wraperr.bullet.turn = turn;
        wraperr.bullet.bullet_speed = bullet_speed;
        wraperr.bullet.setPosition(start_position);
        wraperr.bullet.shape.setSize({5, 15});

        ([]<typename Vec>(std::tuple<std::vector<Target>&... > &tuple, Vec &vec) {
            std::apply([&]<typename... Ts>(Ts &... tupleVec) {
                           (([&]<typename T>(T &tvec) {
                               if constexpr (std::is_same_v<T, Vec>) {
                                   tvec.insert(tvec.end(), vec.begin(), vec.end());
                               }
                           }(tupleVec)), ...);
                       }
                       , tuple);
        }(wraperr.target, targets), ...);

        bullets.push_back(std::move(wraperr));
    }

    void update(const sf::Time &elapsed) {
        for (Bullet_Wraperr &wraperr: bullets) {
            if (wraperr.bullet.turn == TurnState::Up) {
                wraperr.bullet.move({0, elapsed.asSeconds() * -wraperr.bullet_speed});
            } else if (wraperr.bullet.turn == TurnState::Down) {
                wraperr.bullet.move({0, elapsed.asSeconds() * wraperr.bullet_speed});
            }

            std::apply([&]<typename... Ts>(Ts &... targets_) {
                (([&]<CollisionObjectWith<Bullet> T>(std::vector<T> &target_) {

                    for (T& element : target_) {
                        sf::FloatRect target_bounds = element.get_bounds();

                    }

                }(targets_)), ...);
            }, wraperr.target);
        }
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        for (Bullet_Wraperr &wraperr: bullets) {
            target.draw(wraperr.bullet, states);
        }
    };

private:
    struct Bullet_Wraperr {
        std::tuple<std::vector<Target>&... > target = {};
        Bullet bullet;
    };

    std::vector<Bullet_Wraperr> bullets = {};
};

#endif //SPACE_INVADERS_BULLET_H
