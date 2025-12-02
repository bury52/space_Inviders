//
// Created by bogus on 23.11.2025.
//

#ifndef SPACE_INVADERS_BUTTON_H
#define SPACE_INVADERS_BUTTON_H
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Window/Event.hpp"

template<typename T>
requires std::invocable<T>
class Button : public sf::Drawable,public sf::Transformable {
public:
    explicit Button(const T &invocable)
        : invocable(invocable) {
    }

    sf::FloatRect get_bounds() const {
        return getTransform().transformRect(button.getGlobalBounds());
    };

    void onMouseButtonPressed(const sf::Event::MouseButtonPressed &event) {
        if (get_bounds().contains(static_cast<sf::Vector2f>(event.position))) {
            invocable();
        }
    }

    T invocable;
protected:

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(button,states);
    };
public:
    sf::RectangleShape button = sf::RectangleShape({100,50});
};

#endif //SPACE_INVADERS_BUTTON_H