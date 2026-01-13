//
// Created by bogus on 13.01.2026.
//

#ifndef SPACE_INVADERS_LAZY_H
#define SPACE_INVADERS_LAZY_H

#include <concepts>
#include <optional>

class Load_Textuer

template<std::invocable F>
class Lazy{
    using T = std::invoke_result_t<F>;
    std::optional<T> instance = std::nullopt;

    operator T&(){
        if (instance)
            return *instance;
        instance = F();
        return *instance;
    }
};

#endif //SPACE_INVADERS_LAZY_H