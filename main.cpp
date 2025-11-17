#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Bullet.h"
#include "Bullet_Controller.h"
#include "Player.h"
#include "Robot.h"
#include "util.h"
#include "Wall.h"

int main() {
    const auto res = Res();
    std::vector<std::shared_ptr<Robot> > current_enemy = {};

    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Space Invaders", sf::Style::Close);
    window.setPosition({10, 10});
    window.setKeyRepeatEnabled(false);

    Enemy_Controller enemy_controller(current_enemy,
                                      {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},
                                      {10, static_cast<float>(window.getSize().x) - 10},
                                      res);

    enemy_controller
            .add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0)
            .add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1)
            .add_enemy(2).add_enemy(2).add_enemy(2).add_enemy(2)
            .set_start_position();

    Wall wall = Wall({100.0f,800.0f},{5,1},25.0f);

    Player player = Player({10, static_cast<float>(window.getSize().x) - 10}, 5, res);
    set_y_for_player(player, window.getSize().y);

    Bullet_Controller<Player, std::vector<std::shared_ptr<Robot> >,Wall > bullet_controller = {};
    auto buller_helper_robot = bullet_controller.get_helper(player, std::nullopt, std::nullopt);
    auto buller_helper_player = bullet_controller.get_helper(std::nullopt, current_enemy, wall);

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                player.onKeyPressed(*keyPressed,buller_helper_player);

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
            enemy->update(restart, player, buller_helper_robot);
        }

        bullet_controller.update(restart);

        window.clear();

        window.draw(player);

        for (const auto &enemy: current_enemy) {
            window.draw(*enemy);
        }

        window.draw(bullet_controller);

        window.draw(wall);

        window.display();
    }
};
