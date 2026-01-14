//
// Created by bogus on 5.01.2026.
//

#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <SFML/Graphics/Drawable.hpp>

#include "Lazy.h"
#include "TomlReader.h"

class Game : public sf::Drawable {
public:
    Game(const Settings_TOML &settings_toml,
         const Game_TOML &game_toml,
         const std::vector<Level_TOML> &level_toml,
         const std::vector<Entity_TOML> &enemy_toml,
         const std::vector<Entity_TOML> &player_toml,
         std::map<std::string, Lazy<Load_Texture> > &textures) : settings_toml_(settings_toml),
                                                                 game_toml_(game_toml),
                                                                 level_toml_(level_toml),
                                                                 enemy_toml_(enemy_toml), textures_(textures),
                                                                 player_toml_(player_toml),
                                                                 enemy_controller(current_enemy, {
                                                                     10,
                                                                     static_cast<float>(settings_toml.widthWindow) -
                                                                     10
                                                                 }) {
        if (game_toml_.levels.empty())
            return;
        process_level(game_toml_.levels[level]);
    }

    void next_level() {
        level++;
        if (game_toml_.levels.size() > level)
            process_level(game_toml_.levels[level]);
    }

    void update(const sf::Time &elapsed) {
        if (is_pause)
            return;

        player.update(elapsed);
        enemy_controller.update(elapsed);
        for (const auto &enemy: current_enemy) {
            enemy->update(elapsed, player, buller_helper_robot);
        }
        bullet_controller.update(elapsed);
    }

    void onKeyPressed(const sf::Event::KeyPressed &event) {
        if (event.scancode == sf::Keyboard::Scancode::Escape) {
            is_pause = true;
        }
        if (!is_pause) {
            player.onKeyPressed(event, buller_helper_player);
        }
    }

    void onKeyReleased(const sf::Event::KeyReleased &event) {
        if (!is_pause) {
            player.onKeyReleased(event);
        }
    }

    int level = 0;
    bool is_pause = false;
    const Game_TOML &game_toml_;
    const std::vector<Level_TOML> &level_toml_;
    const std::vector<Entity_TOML> &enemy_toml_;
    std::map<std::string, Lazy<Load_Texture> > &textures_;
    const std::vector<Entity_TOML> &player_toml_;

    std::vector<std::shared_ptr<Robot> > current_enemy = {};
    Enemy_Controller enemy_controller;
    const Settings_TOML &settings_toml_;
    Player player = Player({0, 0}, 0, sf::Texture(sf::Image({8, 8}, sf::Color::Red)), 0, 0, 0, 0, 0);
    std::vector<Wall> walls = {};
    using Bullet_Controller_type = Bullet_Controller<Player, std::vector<std::shared_ptr<Robot> >, std::vector<Wall> >;
    Bullet_Controller_type bullet_controller = {};
    Bullet_Controller_type::Buller_Helper buller_helper_robot = bullet_controller.get_helper(
        player, std::nullopt, walls);
    Bullet_Controller_type::Buller_Helper buller_helper_player = bullet_controller.get_helper(
        std::nullopt, current_enemy, walls);

private:
    void process_level(const std::string &level_name) {
        auto level = std::ranges::find_if(level_toml_, [&](const auto &e) { return e.name == level_name; });
        if (level == level_toml_.end()) {
            return;
        }
        enemy_controller.set_enemy_y(level->lines);

        auto player_t = std::ranges::find_if(player_toml_, [&](const auto &e) { return e.name == level->player; });
        if (player_t == player_toml_.end()) {
            return;
        }

        player = Player({10, static_cast<float>(settings_toml_.widthWindow) - 10}, 5,
                        textures_.at(player_t->texture).get(), player_t->health, player_t->damage, level->playerSpeed,
                        player_t->bulletSpeed, player_t->bulletDelay);
        set_y_for_player(player, static_cast<float>(settings_toml_.heightWindow));

        enemy_controller.robot_speed = level->enemySpeed;

        for (int i = 0; i < level->layout.size(); ++i) {
            for (auto enemy_name: level->layout[i]) {
                auto enemy = std::ranges::find_if(enemy_toml_, [&](const auto &e) { return e.name == enemy_name; });
                if (enemy == enemy_toml_.end()) {
                    continue;
                }
                enemy_controller.add_enemy(i, textures_.at(enemy->texture).get(), enemy->health, enemy->damage,
                                           enemy->bulletSpeed,enemy->bulletDelay);
            }
        }
        enemy_controller.set_start_position();
    }

    // std::vector<Wall> walls = {
    //     {{50.0f, 800.0f}, {10, 10}, 5.0f},
    //     {{350.0f, 800.0f}, {10, 10}, 5.0f},
    //     {{650.0f, 800.0f}, {10, 10}, 5.0f},
    //     {{950.0f, 800.0f}, {10, 10}, 5.0f},
    // };


protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        if (is_pause)
            return;

        for (const auto &enemy: current_enemy) {
            target.draw(*enemy, states);
        }
        target.draw(player, states);
        target.draw(bullet_controller, states);
        for (const auto &wall: walls) {
            target.draw(wall, states);
        }
    };
};

#endif //SPACE_INVADERS_GAME_H
