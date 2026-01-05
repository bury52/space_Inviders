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

    Button(const sf::Font& font,
           const sf::String& label,
           Callback cb)
        : text(font,label) ,callback(std::move(cb))
    {
        button.setSize({120.f, 48.f});
        button.setFillColor(sf::Color(70, 70, 70));

        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        centerText();
    }

    void onMouseButtonPressed(const sf::Event::MouseButtonPressed& event) {
        if (getBounds().contains(
            static_cast<sf::Vector2f>(event.position)))
        {
            callback();
        }
    }

    sf::FloatRect getBounds() const {
        return getTransform().transformRect(button.getGlobalBounds());
    }

    // public
    sf::RectangleShape button;
    sf::Text text;

private:
    Callback callback;

    void centerText() {
        text.setOrigin(text.getLocalBounds().getCenter());
        text.setPosition(button.getLocalBounds().getCenter());
    }

protected:
    void draw(sf::RenderTarget& t, sf::RenderStates s) const override {
        s.transform *= getTransform();
        t.draw(button, s);
        t.draw(text, s);
    }
};


#endif //SPACE_INVADERS_BUTTON_H