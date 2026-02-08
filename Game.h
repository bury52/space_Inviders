//
// Created by bogus on 5.01.2026.
//

#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <SFML/Graphics/Drawable.hpp>

#include "Lazy.h"
#include "TomlReader.h"
// gra
class Game : public sf::Drawable {
public:
    Game(const Settings_TOML &settings_toml,
         const Game_TOML &game_toml,
         const std::vector<Level_TOML> &level_toml,
         const std::vector<Wall_TOML> &wall_toml,
         const std::vector<Entity_TOML> &enemy_toml,
         const std::vector<Entity_TOML> &player_toml,
         std::map<std::string, Lazy<Load_Texture> > &textures) : settings_toml_(settings_toml),
                                                                 game_toml_(game_toml),
                                                                 level_toml_(level_toml),
                                                                 wall_toml_(wall_toml),
                                                                 enemy_toml_(enemy_toml),
                                                                 textures_(textures),
                                                                 player_toml_(player_toml),
                                                                 enemy_controller(current_enemy, {
                                                                         10,
                                                                         static_cast<float>(settings_toml.widthWindow) -
                                                                         10
                                                                     }),
                                                                 bullet_controller(settings_toml) {
        if (game_toml_.levels.empty())
            return;
        process_level(game_toml_.levels[level]);
    }
    // następny level
    void next_level() {
        level++;
        if (game_toml_.levels.size() > level)
            process_level(game_toml_.levels[level]);
    }
    // aktualizacja stanu
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
    // klikanie przycisku
    void onKeyPressed(const sf::Event::KeyPressed &event) {
        if (event.scancode == sf::Keyboard::Scancode::Escape) {
            is_pause = true;
        }
        if (!is_pause) {
            player.onKeyPressed(event, buller_helper_player);
        }
    }
    // od klikniecie przycisku
    void onKeyReleased(const sf::Event::KeyReleased &event) {
        if (!is_pause) {
            player.onKeyReleased(event);
        }
    }

    int level = 0;
    std::string end_title = "";
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
    const std::vector<Wall_TOML> &wall_toml_;
    using Bullet_Controller_type = Bullet_Controller<Player, std::vector<std::shared_ptr<Robot> >, std::vector<Wall> >;
    Bullet_Controller_type bullet_controller;
    Bullet_Controller_type::Bullet_Helper buller_helper_robot = bullet_controller.get_helper(
        player, std::nullopt, walls);
    Bullet_Controller_type::Bullet_Helper buller_helper_player = bullet_controller.get_helper(
        std::nullopt, current_enemy, walls);

private:
    // załadowanie, przygotowanie, odpalenie levelu.
    void process_level(const std::string &level_name) {
        auto level = std::ranges::find_if(level_toml_, [&](const auto &e) { return e.name == level_name; });
        if (level == level_toml_.end()) {
            return;
        }
        enemy_controller.set_enemy_y(level->lines);

        end_title = level->end;

        auto player_t = std::ranges::find_if(player_toml_, [&](const auto &e) { return e.name == level->player; });
        if (player_t == player_toml_.end()) {
            return;
        }

        player = Player({10, static_cast<float>(settings_toml_.widthWindow) - 10}, 5,
                        textures_.at(player_t->texture).get(), player_t->health, player_t->damage, level->playerSpeed,
                        player_t->bulletSpeed, player_t->bulletDelay);
        set_y_for_player(player, static_cast<float>(settings_toml_.heightWindow));

        enemy_controller.robot_speed = level->enemySpeed;
        // tworzenie przeciwników
        for (int i = 0; i < level->layout.size(); ++i) {
            for (auto enemy_name: level->layout[i]) {
                auto enemy = std::ranges::find_if(enemy_toml_, [&](const auto &e) { return e.name == enemy_name; });
                if (enemy == enemy_toml_.end()) {
                    continue;
                }
                auto pos = enemy->texture.find(';');
                if (pos == std::string::npos) {
                    continue;
                }
                enemy_controller.add_enemy(i,
                                           textures_.at(enemy->texture.substr(0, pos)).get(),
                                           textures_.at(enemy->texture.substr(pos + 1)).get(), 1, enemy->health,
                                           enemy->damage,
                                           enemy->bulletSpeed, enemy->bulletDelay);
            }
        }
        enemy_controller.set_start_position();

        auto wall = std::ranges::find_if(wall_toml_, [&](const auto &e) { return e.name == level->wall; });
        if (wall == wall_toml_.end()) {
            return;
        }
        // tworzenie ścian
        walls.clear();
        const float wall_size_x = wall->x * 5;
        const float wall_position_x = (settings_toml_.widthWindow - wall_size_x * wall->count) / (wall->count + 1);
        const float wall_position_y = settings_toml_.heightWindow - wall->height;
        for (int i = 1; i <= wall->count; ++i) {
            walls.emplace_back(sf::Vector2f{wall_position_x * i + wall_size_x * (i - 1), wall_position_y},
                               sf::Vector2i{wall->x, wall->y}, 5, wall->cut);
        }
    }

protected:
    // funkcja rysowania z sf::Drawable
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
