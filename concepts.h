//
// Created by bogus on 5.11.2025.
//

#ifndef SPACE_INVADERS_CONCEPTS_H
#define SPACE_INVADERS_CONCEPTS_H
#include "SFML/Graphics/Rect.hpp"
#include "type_traits"

template <typename T>
concept CollisionObject = requires(const T& obj)
{
    { obj.get_bounds() } -> std::same_as<sf::FloatRect>;
};

template <typename T,typename R>
concept CollisionWith = requires(T& obj,R& collider)
{
    obj.collision(collider);
};

template<typename T,typename R>
concept CollisionObjectWith = CollisionObject<T> && CollisionWith<T,R>;


#endif //SPACE_INVADERS_CONCEPTS_H