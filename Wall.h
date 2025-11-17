//
// Created by bogus on 16.11.2025.
//

#ifndef SPACE_INVADERS_WALL_H
#define SPACE_INVADERS_WALL_H
#include <vector>

#include "cmake-build-debug/_deps/sfml-src/extlibs/headers/glad/include/glad/gl.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Vertex.hpp"

class Wall : public sf::Drawable {
public:
    Wall(const sf::Vector2f &position,const sf::Vector2i &segments, const float& scale)
        : bounds(position, static_cast<sf::Vector2f>(segments) * scale) ,
          scale(scale) {
        for (int i = 0; i < segments.y; ++i) {
            const float segment_y = position.y + static_cast<float>(i) * scale;
            for (int j = 0; j < segments.x; ++j) {
                const float segment_x = position.x + static_cast<float>(j) * scale;
                wall_segment.emplace_back(sf::Vector2f{segment_x, segment_y});
            }
        }
    }

    sf::FloatRect get_bounds() const {
        return bounds;
    };

    void collision(Bullet& collider) {
        collider.damage -= std::ranges::remove_if(wall_segment, [&](const sf::Vertex& segment) {
           return collider.get_bounds().contains(segment.position);
        }).size();
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        target.pushGLStates();
        glPointSize(scale);
        target.draw(wall_segment.data(), wall_segment.size(), sf::PrimitiveType::Points,states);
        target.popGLStates();
    };

public:
    std::vector<sf::Vertex> wall_segment = {};
    sf::FloatRect bounds;
    const float scale;

};

#endif //SPACE_INVADERS_WALL_H