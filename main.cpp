#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Robot.h"
#include "util.h"


int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Space Invaders", sf::Style::Close);
    window.setPosition({10, 10});
    window.setKeyRepeatEnabled(false);

    std::vector<std::shared_ptr<Robot> > current_enemy = {};
    Enemy_Controller enemy_controller(current_enemy,
                                      {0, 50, 100, 150, 200, 250,300,350,400,450,500},
                                      {10, static_cast<float>(window.getSize().x) - 10});

    enemy_controller
            .add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0)
            .add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1)
            .add_enemy(2).add_enemy(2).add_enemy(2).add_enemy(2)
            .set_start_position();


    Player player = Player({10, static_cast<float>(window.getSize().x) - 10}, current_enemy);
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

        enemy_controller.update(restart);

        for (const auto &enemy: current_enemy) {
            enemy->update(restart);
        }

        window.clear();

        window.draw(player);
        for (const auto &enemy: current_enemy) {
            window.draw(*enemy);
        }
        window.display();
    }
};
