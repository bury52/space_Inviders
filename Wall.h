//
// Created by bogus on 16.11.2025.
//

#ifndef SPACE_INVADERS_WALL_H
#define SPACE_INVADERS_WALL_H
#include <vector>

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
// ściany
class Wall : public sf::Drawable {
public:
    // tworzenie ściany
    Wall(const sf::Vector2f &position, const sf::Vector2i &segments, const float &scale, const int& cut)
        : bounds(position, static_cast<sf::Vector2f>(segments) * scale) {
        const sf::Vector2f scaleVector(scale, scale);
        // ścinanie rogów ściany.
        for (int i = 0; i < segments.y; ++i) {
            const float segment_y = position.y + static_cast<float>(i) * scale;
            int segments_x_cut = segments.x / cut - (i);
            for (int j = 0; j < segments.x; ++j) {
                if (j < segments_x_cut || (segments.x-j) <= segments_x_cut)
                    continue;
                const float segment_x = position.x + static_cast<float>(j) * scale;
                wall_segment.emplace_back(scaleVector).setPosition({segment_x, segment_y});
            }
        }

    }
    // spełnienie konceptu CollisionObject
    [[nodiscard]] sf::FloatRect getBounds() const {
        return bounds;
    };
    // kolizja z pociskiem i destrukcja
    void collision(Bullet &collider) {
        const float half_damage =  static_cast<float>(collider.damage) / 2.0f;
        auto damage_collider_position = collider.getBounds().getCenter() - sf::Vector2f{
                                            half_damage, half_damage
                                        };
        sf::FloatRect damage_collider = sf::FloatRect(damage_collider_position,
                                                      static_cast<sf::Vector2f>(sf::Vector2i{
                                                          collider.damage, collider.damage
                                                      }));
        collider.damage -= static_cast<int>(std::erase_if(wall_segment, [&](const sf::RectangleShape &segment) -> bool {
            return static_cast<bool>(damage_collider.findIntersection(segment.getGlobalBounds()));
        }));
    }

protected:
    // funkcja rysowania z sf::Drawable
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        for (const auto &segment: wall_segment)
            target.draw(segment, states);
    };

public:
    std::vector<sf::RectangleShape> wall_segment = {};
    sf::FloatRect bounds;
};

#endif //SPACE_INVADERS_WALL_H
