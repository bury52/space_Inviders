#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

constexpr int window_height = 1000;
constexpr int  window_width = 1000;



class Player: public sf::Drawable, public sf::Transformable{

public:

    explicit Player(const sf::Texture &texture)
        : sprite(texture) {
        setScale({5,5});
    }

private:
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

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Space Invaders",sf::Style::Close);
    window.setPosition({10,10});
    // window.setKeyRepeatEnabled(false);

    sf::Texture texture("statek.png");
    Player player = Player(texture);

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
            {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
            {

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
            {
                if (player.getPosition().x - 10 >= 0) {
                    player.move({-10, 0});
                }else {
                    player.setPosition({0,player.getPosition().y});
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
            {
                // if (player.sprite.getPosition().x + 10 <= player.get_border_x(window_width)) {
                    player.move({10, 0});
                // }else {
                    // player.sprite.setPosition({static_cast<float>(player.get_border_x(window_width)),player.sprite.getPosition().y});
                // }

            }
        }

        window.clear();

        window.draw(player);

        window.display();
    }
};


