//
// Created by bogus on 13.01.2026.
//

#ifndef SPACE_INVADERS_LAZY_H
#define SPACE_INVADERS_LAZY_H

#include <concepts>
#include <optional>

#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Texture.hpp"

class Load_Texture {
    std::string path;

public:
    explicit Load_Texture(const std::string &p) : path(p) {
    }

    sf::Texture operator()() const {
        sf::Texture texture;
        if (!texture.loadFromFile("resources/" +path)) {
            return sf::Texture(sf::Image({8, 8}, sf::Color::Red));
        }
        return texture;
    }
};

template<typename F>
    requires std::invocable<F>
class Lazy {
public:
    using T = std::invoke_result_t<F>;

private:
    std::optional<T> instance{};
    F function{};

public:
    Lazy() = default;

    explicit Lazy(const F &fn) : function(fn) {
    }

    explicit Lazy(F &&fn) : function(std::move(fn)) {
    }

    T &get() {
        if (!instance) {
            instance = std::invoke(function);
        }
        return *instance;
    }

    operator T &() { return get(); }
};

#endif //SPACE_INVADERS_LAZY_H
