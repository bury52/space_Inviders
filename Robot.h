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

class Robot : public sf::Drawable, public sf::Transformable {
public:
    explicit Robot(const sf::Vector2f &border_x,const float& start_x, const std::vector<float>::iterator &start,
                   const std::vector<float>::iterator &end, const std::shared_ptr<sf::Texture> &texture)
        : border_x(border_x), start(start), end(end), texture(texture), sprite(*texture) {
        setScale({5, 5});
        setPosition({start_x, *start});
        turn = TurnState::Right;
    }

    sf::FloatRect getGlobalBounds() const {
        return getTransform().transformRect(sprite.getGlobalBounds());
    }

    float getSize_x() const {
        return (static_cast<float>(texture->getSize().x) * getScale().x);
    }

    void update(const sf::Time &elapsed) {
        if (turn == TurnState::Left) {
            move({elapsed.asSeconds() * -robot_speed, 0});
        } else if (turn == TurnState::Right) {
            move({elapsed.asSeconds() * robot_speed, 0});
        }

        if (getPosition().x < border_x.x) {
            setPosition({border_x.x, getPosition().y});
            turn = TurnState::Right;
            update_y();
        } else {
            if (float border_position_x = border_x.y - getSize_x();
                getPosition().x > border_position_x) {
                setPosition({border_position_x, getPosition().y});
                turn = TurnState::Left;
                update_y();
            }
        }

        for (auto &bullet: bullets) {
            bullet.move({0, elapsed.asSeconds() * bullet_speed * -1});
        }
        std::erase_if(bullets, [](const auto &bullet) { return bullet.getPosition().y < 0; });
    }

private:
    void update_y() {
        if (const auto next = start + 1; next != end) {
            start = next;
            setPosition({getPosition().x, *start});
        }
    }

    TurnState turn = TurnState::None;
    float robot_speed = 200;
    float bullet_speed = 400;
    std::vector<sf::RectangleShape> bullets = {};
    sf::Vector2f border_x;
    std::vector<float>::iterator start;
    std::vector<float>::iterator end;
    std::shared_ptr<sf::Texture> texture;
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


#endif //SPACE_INVADERS_ROBOT_H
