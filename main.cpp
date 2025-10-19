#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "util.h"


int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Space Invaders", sf::Style::Close);
    window.setPosition({10, 10});
    window.setKeyRepeatEnabled(false);

    Player player = Player(static_cast<float>(window.getSize().x));
    set_y_for_player(player, window.getSize().y);

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                player.onKeyPressed(*keyPressed);

                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            } else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                player.onKeyReleased(*keyReleased);
            }
        }
        sf::Time restart = clock.restart();

        player.update(restart);

        window.clear();

        window.draw(player);

        window.display();
    }
};
