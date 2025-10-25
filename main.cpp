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

    std::shared_ptr<sf::Texture> robot_texture = std::make_shared<sf::Texture>("robak.png");

    std::vector<float> enemy_y = {0,50,100,150,200,250};
    std::vector<Robot> enemy = {
        Robot({10, static_cast<float>(window.getSize().x) - 10},10.0, enemy_y.begin(), enemy_y.end()-1, robot_texture),
        Robot({10, static_cast<float>(window.getSize().x) - 10},80.0, enemy_y.begin(), enemy_y.end()-1, robot_texture),
        Robot({10, static_cast<float>(window.getSize().x) - 10},10.0, enemy_y.begin()+1, enemy_y.end(), robot_texture)
    };


    Player player = Player({10, static_cast<float>(window.getSize().x) - 10}, enemy);
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
        for (auto &robot: enemy) {
            robot.update(restart);
        }


        window.clear();

        window.draw(player);
        for (auto &robot: enemy) {
            window.draw(robot);
        }
        window.display();
    }
};
