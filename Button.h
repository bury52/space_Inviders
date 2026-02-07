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
// element UI przycisk
template<UIElement T>
class Button : public sf::Drawable, public sf::Transformable {
public:
    using Callback = std::function<void()>;

    explicit Button(std::unique_ptr<T> content, Callback cb = [] {
                    })
        : content_(std::move(content)), callback(std::move(cb)) {
    }

    void onMouseButtonPressed(const sf::Event::MouseButtonPressed &mouseEvent) {
        if (getBounds().contains(static_cast<sf::Vector2f>(mouseEvent.position))) {
            if (callback) callback();
        }
    }

    sf::FloatRect getBounds() const {
        return getTransform().transformRect(UIElement_getBounds(*content_));
    }

    std::unique_ptr<T> content_;
    Callback callback;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(*content_, states);
    }
};

class UIButton : public sf::Drawable, public sf::Transformable {
public:
    UIButton(const sf::Text &text)
        : text(text), shape(text.getLocalBounds().size + sf::Vector2f{20.f,20.f}){
        shape.setOrigin(shape.getGeometricCenter());
        shape.setPosition(text.getGlobalBounds().getCenter());
        shape.setFillColor(sf::Color(30, 30, 30));
        shape.setOutlineColor(sf::Color(200, 200, 200));
        shape.setOutlineThickness(1.f);
    }

    sf::FloatRect getBounds() const {
        return getTransform().transformRect(shape.getGlobalBounds());
    }

    sf::Text text;
    sf::RectangleShape shape;



protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(shape, states);
        target.draw(text, states);
    }
};

#endif //SPACE_INVADERS_BUTTON_H
