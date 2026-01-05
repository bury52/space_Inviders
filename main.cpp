#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Bullet.h"
#include "Bullet_Controller.h"
#include "Player.h"
#include "Robot.h"
#include "util.h"
#include "Wall.h"

#include <optional>

#include "Button.h"
#include "StartMenu.h"
#include "TomlReader.h"

int main() {
    auto toml = create_form_toml(
        "resources/config.toml",
        load_settings,
        load_textures,
        load_enemy,
        load_player,
        load_level,
        load_game);
    if (!toml) return 1;
    auto &[settings,textures,enemy,player,level,game] = toml.value();

    sf::Font font;
    if (!font.openFromFile("resources/" + settings.font)) {
        return 2;
    }

    sf::RenderWindow window(sf::VideoMode({settings.widthWindow, settings.heightWindow}), "Space Invaders",
                            sf::Style::Close);
    window.setPosition({10, 10});
    window.setKeyRepeatEnabled(false);

    StartMenu start_menu = StartMenu(window.getSize(),font);

    //
    // Button start_game_button = Button([&]() {
    //     is_game_pause = false;
    // });
    //
    // start_game_button.setPosition({100,100});
    //
    // std::vector<std::shared_ptr<Robot> > current_enemy = {};
    // Enemy_Controller enemy_controller(current_enemy,
    //                                   {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},
    //                                   {10, static_cast<float>(window.getSize().x) - 10},
    //                                   res);
    //
    // enemy_controller
    //         .add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0)
    //         .add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1)
    //         .add_enemy(2).add_enemy(2).add_enemy(2).add_enemy(2)
    //         .set_start_position();
    //
    // std::vector<Wall> walls = {
    //     {{50.0f, 800.0f}, {10, 10}, 5.0f},
    //     {{350.0f, 800.0f}, {10, 10}, 5.0f},
    //     {{650.0f, 800.0f}, {10, 10}, 5.0f},
    //     {{950.0f, 800.0f}, {10, 10}, 5.0f},
    // };
    //
    // Player player = Player({10, static_cast<float>(window.getSize().x) - 10}, 5, res);
    // set_y_for_player(player, window.getSize().y);
    //
    // Bullet_Controller<Player, std::vector<std::shared_ptr<Robot> >, std::vector<Wall> > bullet_controller = {};
    // auto buller_helper_robot = bullet_controller.get_helper(player, std::nullopt, walls);
    // auto buller_helper_player = bullet_controller.get_helper(std::nullopt, current_enemy, walls);
    //
    //
    //
    // sf::Clock clock;
    //
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            //     if (!is_game_pause)
            //     player.onKeyPressed(*keyPressed, buller_helper_player);
            //
            //     if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            //         is_game_pause = true;
            //     }
            // } else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            //     if (!is_game_pause)
            //     player.onKeyReleased(*keyReleased);
            // }
            else if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                start_menu.onMouseButtonPressed(*mouseButtonPressed);
            }
        }

        window.clear();
        window.draw(start_menu);
        window.display();

        //     sf::Time restart = clock.restart();
        //     if (!is_game_pause) {
        //
        //         player.update(restart);
        //
        //         enemy_controller.update(restart);
        //
        //         for (const auto &enemy: current_enemy) {
        //             enemy->update(restart, player, buller_helper_robot);
        //         }
        //
        //         bullet_controller.update(restart);
        //
        //         window.clear();
        //
        //         window.draw(player);
        //
        //         for (const auto &enemy: current_enemy) {
        //             window.draw(*enemy);
        //         }
        //
        //         window.draw(bullet_controller);
        //
        //         for (const auto &wall: walls) {
        //             window.draw(wall);
        //         }
        //         window.display();
        //
        //     }else {
        //         window.clear();
        //
        //         window.draw(start_game_button);
        //
        //         window.display();
        //     }
        //
    }
};
