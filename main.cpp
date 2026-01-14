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
#include "Game.h"
#include "Lazy.h"
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
    auto &[settings,textures,enemy_template,player_template,level_template,game_template] = toml.value();

    sf::Font font;
    if (!font.openFromFile("resources/" + settings.font)) {
        return 2;
    }
    font.setSmooth(false);

    sf::RenderWindow window(sf::VideoMode({settings.widthWindow, settings.heightWindow}), "Space Invaders",
                            sf::Style::Close);
    window.setPosition({10, 10});
    window.setKeyRepeatEnabled(false);

    std::optional<Game> game = std::nullopt;

    std::map<std::string, Lazy<Load_Texture> > map_texture = {};
    for (const auto &texture: textures) {
        map_texture.emplace(texture.name, Lazy(Load_Texture(texture.path)));
    }

    const std::function<void(const Game_TOML &)> create_game = [&](const Game_TOML &game_toml) {
        game.emplace(settings, game_toml, level_template, enemy_template, player_template, map_texture);
    };

    StartMenu start_menu = StartMenu(game, game_template, create_game, window.getSize(), font);

    sf::Clock clock;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (game) {
                    game->onKeyPressed(*keyPressed);
                }
            }
            else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                if (game) {
                    game->onKeyReleased(*keyReleased);
                }
            }
            else if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                start_menu.onMouseButtonPressed(*mouseButtonPressed);
            }
        }

        sf::Time restart = clock.restart();

        window.clear();
        if (game) {
            game->update(restart);
            window.draw(*game);
        }
        window.draw(start_menu);
        window.display();


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
