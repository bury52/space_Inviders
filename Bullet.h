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
class Bullet_Controller {
public:
    template<CollisionObjectWith<Bullet> ...Add_Target>
    void add_bullet(const TurnState& turn,const sf::Vector2f& start_position,const float& bullet_speed, std::vector<Add_Target> &... targets) {
        Bullet_Wraperr wraperr;
        wraperr.bullet.turn = turn;
        wraperr.bullet.bullet_speed = bullet_speed;
        wraperr.bullet.setPosition(start_position);
        wraperr.bullet.shape.setSize({5, 15});
        (wraperr.target.insert(wraperr.target.end(), targets.begin(), targets.end()), ...);
        bullets.push_back(std::move(wraperr));
    }

    void update(const sf::Time &elapsed) {



    }

private:
    struct Bullet_Wraperr {
        std::vector<std::variant<Target...> > target = {};
        Bullet bullet;
    };

    std::vector<Bullet_Wraperr> bullets = {};

};

#endif //SPACE_INVADERS_BULLET_H
