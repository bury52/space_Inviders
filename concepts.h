//
// Created by bogus on 5.11.2025.
//

#ifndef SPACE_INVADERS_CONCEPTS_H
#define SPACE_INVADERS_CONCEPTS_H
#include "SFML/Graphics/Rect.hpp"
#include "type_traits"
#include "enum.h"
#include <memory>

template<typename T>
concept CollisionObject = requires(const T &obj)
{
    { obj.get_bounds() } -> std::same_as<sf::FloatRect>;
};

template<typename T, typename R>
concept CollisionWith = requires(T &obj, R &collider)
{
    { obj.collision(collider) } -> std::same_as<void>;
};

template<typename T, typename R>
concept CollisionObjectWith = CollisionObject<T> && CollisionWith<T, R>;

template<typename T>
concept Shooter = requires(T &obj, const TurnState &turn, const sf::Vector2f &start_position, const float &bullet_speed)
{
    { obj.shoot(turn, start_position, bullet_speed) } -> std::same_as<void>;
};

template<typename T>
concept SmartOrRawPointer =
        std::is_pointer_v<T> ||
        (requires { typename T::element_type; } &&
         (std::same_as<T, std::unique_ptr<typename T::element_type> > ||
          std::same_as<T, std::shared_ptr<typename T::element_type> >));

#endif //SPACE_INVADERS_CONCEPTS_H
