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

class Button : public sf::Drawable, public sf::Transformable {
public:
    using Callback = std::function<void()>;

    explicit Button(std::unique_ptr<sf::Drawable> content, Callback cb = [] {
                    })
        : content(std::move(content)), callback(std::move(cb)) {
        background.setSize({120.f, 48.f});
        background.setFillColor(sf::Color(70, 70, 70));
    }

    void onMouseButtonPressed(const sf::Event::MouseButtonPressed &mouseEvent) {
        if (getBounds().contains(static_cast<sf::Vector2f>(mouseEvent.position))) {
            if (callback) callback();
        }
    }

    sf::FloatRect getBounds() const {
        return getTransform().transformRect(background.getGlobalBounds());
    }

    sf::RectangleShape background;
    std::unique_ptr<sf::Drawable> content;
    Callback callback;

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(background, states);
        target.draw(*content, states);
    }
};


#endif //SPACE_INVADERS_BUTTON_H
