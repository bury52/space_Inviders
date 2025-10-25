//
// Created by bogus on 20.10.2025.
//

#ifndef SPACE_INVADERS_UTIL_H
#define SPACE_INVADERS_UTIL_H
#include "SFML/Graphics/Transformable.hpp"

inline void set_y_for_player(sf::Transformable &target, const float &window_y) {
    target.setPosition({target.getPosition().x, window_y - 80});
};

#endif //SPACE_INVADERS_UTIL_H
