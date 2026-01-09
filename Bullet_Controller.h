//
// Created by bogus on 14.11.2025.
//

#ifndef SPACE_INVADERS_BULLET_CONTROLLER_H
#define SPACE_INVADERS_BULLET_CONTROLLER_H

#include "Bullet.h"
#include "concepts.h"
#include "enum.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/System/Time.hpp"

template<typename... Target>
class Bullet_Controller : public sf::Drawable {
public:
    class Buller_Helper {
    public:
        explicit Buller_Helper(Bullet_Controller<Target...> *const self,
                               std::optional<std::reference_wrapper<Target> >... targets)
            : self(self), targets(targets...) {
        }

        void shoot(const TurnState &turn, const sf::Vector2f &start_position, const float &bullet_speed, const int &damage) {
            self->add_bullet(turn, start_position, bullet_speed, damage, targets);
        }

    private:
        Bullet_Controller<Target...> *const self;
        std::tuple<std::optional<std::reference_wrapper<Target> >...> targets;
    };

    Buller_Helper get_helper(std::optional<std::reference_wrapper<Target> >... targets) {
        return Buller_Helper(this, targets...);
    }

    void add_bullet(const TurnState &turn, const sf::Vector2f &start_position, const float &bullet_speed, const int &damage,
                    std::tuple<std::optional<std::reference_wrapper<Target> >...> targets) {
        Bullet_Wraperr wraperr;
        wraperr.bullet.turn = turn;
        wraperr.bullet.bullet_speed = bullet_speed;
        wraperr.bullet.damage = damage;
        wraperr.bullet.setPosition(start_position);
        wraperr.bullet.shape.setSize({5, 15});
        wraperr.targets = targets;
        bullets.push_back(std::move(wraperr));
    }

    void update(const sf::Time &elapsed) {
        for (Bullet_Wraperr &wraperr: bullets) {
            if (wraperr.bullet.turn == TurnState::Up) {
                wraperr.bullet.move({0, elapsed.asSeconds() * -wraperr.bullet.bullet_speed});
            } else if (wraperr.bullet.turn == TurnState::Down) {
                wraperr.bullet.move({0, elapsed.asSeconds() * wraperr.bullet.bullet_speed});
            }
        }

        erase_if(bullets, [&](Bullet_Wraperr &wraperr) {
            // TODO dostarczyć rozmiat okna zamiast 1000
            if ((wraperr.bullet.turn == TurnState::Up && wraperr.bullet.getBounds().position.y < 0) || (
                    wraperr.bullet.turn == TurnState::Down && wraperr.bullet.getBounds().position.y > 1000))
                return true;

            bool should_erase = false;

            std::apply([&](auto &... targets_) {
                should_erase = (([&]<typename T>(std::optional<T> &target_) {
                    if (!target_.has_value())
                        return false;
                    return if_collided(target_.value().get(), wraperr.bullet);
                }(targets_)) || ...);
            }, wraperr.targets);

            return should_erase && wraperr.bullet.damage <= 0;
        });
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        for (const Bullet_Wraperr &wraperr: bullets) {
            target.draw(wraperr.bullet, states);
        }
    };

private:
    template<typename T>
    static bool if_collided(T &target, Bullet &bullet) {
        return false;
    };

    template<CollisionObjectWith<Bullet> T>
    static bool if_collided(T &target, Bullet &bullet) {
        sf::FloatRect target_bounds = target.getBounds();
        if (bullet.getBounds().findIntersection(target_bounds).has_value()) {
            target.collision(bullet);
            return true;
        }
        return false;
    };

    template<typename T>
    static bool if_collided(std::vector<T> &target, Bullet &bullet) {
        for (auto it = target.begin(); it != target.end(); ++it) {
            if (if_collided(*it, bullet)) {
                if (tryRemove(*it)) {
                    target.erase(it);
                }
                return true;
            }
        }
        return false;
    };

    template<SmartOrRawPointer Ptr>
    static bool if_collided(Ptr &ptr, Bullet &bullet) {
        if (!ptr) return false;
        return if_collided(*ptr, bullet);
    }

    template<typename T>
    static bool tryRemove(const T &element) {
        return false;
    };

    template<Removable T>
    static bool tryRemove(const T &element) {
        return element.shouldRemove();
    };

    template<SmartOrRawPointer Ptr>
    static bool tryRemove(const Ptr &ptr) {
        if (!ptr) return true;
        return tryRemove(*ptr);
    }

    struct Bullet_Wraperr {
        std::tuple<std::optional<std::reference_wrapper<Target> >...> targets = {};
        Bullet bullet;
    };

    std::vector<Bullet_Wraperr> bullets = {};
};

#endif //SPACE_INVADERS_BULLET_CONTROLLER_H
