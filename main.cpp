#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

constexpr int window_height = 1000;
constexpr int  window_width = 1000;

class character {

public:
    sf::Sprite sprite;

    explicit character(const sf::Texture &texture,const float &scal = 10)
        : sprite(texture) {
        sprite.setScale({scal,scal});
    }

    unsigned int get_border_x(const unsigned int &border) {
        return border - (sprite.getTexture().getSize().x * static_cast<unsigned int>(sprite.getScale().x));
    }

    unsigned int get_border_y(const unsigned int &border) {
        return border - (sprite.getTexture().getSize().y * static_cast<unsigned int>(sprite.getScale().y));
    }


};

int main() {

    // Create the main window
    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "SFML window");

    // Load a sprite to display
    character player = character(sf::Texture("statek.png"));
    const sf::Texture texture("statek.png");
    sf::Sprite sprite(texture);
    sprite.setScale(sf::Vector2f(10,10));
    sprite.setPosition(sf::Vector2f(0,window_height - (texture.getSize().y *10)));

    // sprite.setPosition(sf::Vector2f(0.0f, 0.0f));
    // Create a graphical text to display
    // const sf::Font font("JetBrainsMono-VariableFont_wght.ttf");
    // sf::Text text(font, "Hello SFML", 50);

    // Load a music to play
    // sf::Music music("nice_music.ogg");

    // Play the music
    // music.play();

    // Start the game loop
    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
            {

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
            {
                // left key is pressed: move our character
                if (sprite.getPosition().x - 10 >= 0) {
                    sprite.move({-10, 0});
                }else {
                    sprite.setPosition({0,sprite.getPosition().y});
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
            {
                // left key is pressed: move our character
                if (sprite.getPosition().x + 10 <= (window_width - (texture.getSize().x * 10))) {
                    sprite.move({10, 0});
                }else {
                    sprite.setPosition({static_cast<float>(window_width - (texture.getSize().x * 10)),sprite.getPosition().y});
                }

            }
        }

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);

        // Draw the string
        // window.draw(text);

        // Update the window
        window.display();
    }
};
