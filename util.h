//
// Created by bogus on 20.10.2025.
//

#ifndef SPACE_INVADERS_UTIL_H
#define SPACE_INVADERS_UTIL_H
#include "SFML/Graphics/Transformable.hpp"
// ustawianie wysokość na linij gracza
inline void set_y_for_player(sf::Transformable &target, const float &window_y) {
    target.setPosition({target.getPosition().x, window_y - 180});
};
// pozycja prawej krawedzi
template<typename T>
inline T get_right_bound(const sf::Rect<T> &bounds) {
    return bounds.position.x + bounds.size.x;
}
// pozycja dolnej krawedzi
template<typename T>
inline T get_down_bound(const sf::Rect<T> &bounds) {
    return bounds.position.y + bounds.size.y;
}

#endif //SPACE_INVADERS_UTIL_H
