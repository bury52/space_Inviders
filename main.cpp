#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

constexpr int window_height = 1000;
constexpr int window_width = 1000;

void set_y_for_player(sf::Transformable &target) {
    target.setPosition({target.getPosition().x, window_height - 80});
};


class Player : public sf::Drawable, public sf::Transformable {
public:
    explicit Player()
        : texture("statek.png"), sprite(texture) {
        setScale({5, 5});
    }

    void onKeyPressed(const sf::Event::KeyPressed &event) {
        if (event.scancode == sf::Keyboard::Scancode::A) {
            if (getPosition().x - 10 >= 0) {
                move({-10, 0});
            } else {
                setPosition({0, getPosition().y});
            }
        }

        if (event.scancode == sf::Keyboard::Scancode::D) {
            if (sprite.getPosition().x + 10 <= get_border_x(window_width)) {
                move({10, 0});
            } else {
                sprite.setPosition({static_cast<float>(get_border_x(window_width)), sprite.getPosition().y});
            }
        }
    }

private:
    sf::Texture texture;
    sf::Sprite sprite;

    unsigned int get_border_x(const unsigned int &border) const {
        return border - (sprite.getTexture().getSize().x * static_cast<unsigned int>(sprite.getScale().x));
    }

    unsigned int get_border_y(const unsigned int &border) const {
        return border - (sprite.getTexture().getSize().y * static_cast<unsigned int>(sprite.getScale().y));
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(sprite, states);
    };
};


int main() {
    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Space Invaders", sf::Style::Close);
    window.setPosition({10, 10});
    // window.setKeyRepeatEnabled(false);

    Player player = Player();
    set_y_for_player(player);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                player.onKeyPressed(*keyPressed);

                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }


        }

        window.clear();

        window.draw(player);

        window.display();
    }
};
