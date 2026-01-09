//
// Created by bogus on 5.01.2026.
//

#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <SFML/Graphics/Drawable.hpp>

#include "TomlReader.h"

class Game : public sf::Drawable {
public:
    Game(const Settings_TOML &settings_toml,
         const Game_TOML &game_toml,
         const std::vector<Level_TOML> &level_toml,
         const std::vector<Entity_TOML> &enemy_toml,
         const std::vector<Entity_TOML> &player_toml,
         const std::vector<Texture_TOML> &texture_toml) : game_toml_(game_toml), level_toml_(level_toml),
                                                          enemy_toml_(enemy_toml), texture_toml_(texture_toml),
                                                          player_toml_(player_toml) {
        process_game();

        std::vector<std::shared_ptr<Robot> > current_enemy = {};
        Enemy_Controller enemy_controller(current_enemy,
                                          {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},
                                          {10, static_cast<float>(settings_toml.widthWindow) - 10});

        // enemy_controller
        //         .add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0).add_enemy(0)
        //         .add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1).add_enemy(1)
        //         .add_enemy(2).add_enemy(2).add_enemy(2).add_enemy(2)
        //         .set_start_position();

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
    }


    void onKeyPressed(const sf::Event::KeyPressed &event) {
        if (event.scancode == sf::Keyboard::Scancode::Escape) {
            is_pause = true;
        }
    }

    bool is_pause = false;
    const Game_TOML &game_toml_;
    const std::vector<Level_TOML> &level_toml_;
    const std::vector<Entity_TOML> &enemy_toml_;
    const std::vector<Texture_TOML> &texture_toml_;
    const std::vector<Entity_TOML> &player_toml_;

    std::map<std::string, sf::Texture> current_textures;

private:
    void process_game() {
        for (const auto &level: level_toml_ | std::ranges::views::filter([&](auto &e) {
            return std::ranges::contains(game_toml_.levels, e.name);
        })) {
            process_level(level);
        }
    }

    void process_level(const Level_TOML &level) {
    };

    void process_entity(const Entity_TOML &entity) {
    }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    };
};

#endif //SPACE_INVADERS_GAME_H
