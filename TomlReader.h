//
// Created by bogus on 2.12.2025.
//

#ifndef SPACE_INVADERS_TOMLREADER_H
#define SPACE_INVADERS_TOMLREADER_H
#include <fstream>
#include <list>
#include <toml++/toml.hpp>
#include "SFML/Graphics/Texture.hpp"

// ładowanie obiektów z pliku toml.
template<std::invocable<toml::table>... Fs>
std::optional<std::tuple<std::invoke_result_t<Fs, toml::table>...> > create_form_toml(
    const std::string &filename, Fs... fs) {
    toml::table tbl;
    try {
        tbl = toml::parse_file(filename);
    } catch (const toml::parse_error &err) {
        return std::nullopt;
    }
    return std::make_tuple(fs(tbl)...);
}

// --- loaders ---

struct Settings_TOML {
    unsigned int widthWindow;
    unsigned int heightWindow;
    std::string font;
};

inline Settings_TOML load_settings(const toml::table &tbl) {
    Settings_TOML main_{
        tbl["Settings"]["widthWindow"].value_or(1000U),
        tbl["Settings"]["heightWindow"].value_or(1000U),
        tbl["Settings"]["font"].value_or("")
    };
    return main_;
}

struct Texture_TOML {
    std::string name;
    std::string path;
};

inline std::vector<Texture_TOML> load_textures(const toml::table &tbl) {
    std::vector<Texture_TOML> textures;

    if (auto texture_array = tbl["Resources"]["texture"].as_array()) {
        texture_array->for_each([&](auto &&el) {
            if constexpr (toml::is_table<decltype(el)>) {
                textures.emplace_back(el["name"].value_or(""), el["path"].value_or(""));
            }
        });
    }

    return textures;
}

struct Entity_TOML {
    std::string name;
    std::string texture;
    int health;
    int damage;
    float bulletSpeed;
    float bulletDelay;
};

inline std::vector<Entity_TOML> load_enemy(const toml::table &tbl) {
    std::vector<Entity_TOML> entity;

    if (auto entity_array = tbl["Entity"]["enemy"].as_array()) {
        entity_array->for_each([&](auto &&el) {
            if constexpr (toml::is_table<decltype(el)>) {
                entity.emplace_back(el["name"].value_or(""), el["texture"].value_or(""), el["health"].value_or(0),
                                    el["damage"].value_or(0), el["bulletSpeed"].value_or(400.0),
                                    el["bulletDelay"].value_or(3.0));
            }
        });
    }

    return entity;
}

inline std::vector<Entity_TOML> load_player(const toml::table &tbl) {
    std::vector<Entity_TOML> entity;

    if (auto entity_array = tbl["Entity"]["player"].as_array()) {
        entity_array->for_each([&](auto &&el) {
            if constexpr (toml::is_table<decltype(el)>) {
                entity.emplace_back(el["name"].value_or(""), el["texture"].value_or(""), el["health"].value_or(0),
                                    el["damage"].value_or(0), el["bulletSpeed"].value_or(600.0),
                                    el["bulletDelay"].value_or(1.0));
            }
        });
    }

    return entity;
}

struct Wall_TOML {
    std::string name;
    int count;
    int x;
    int y;
    int height;
    int cut;
};

inline std::vector<Wall_TOML> load_wall(const toml::table &tbl) {
    std::vector<Wall_TOML> entity;

    if (auto wall_array = tbl["Layout"]["wall"].as_array()) {
        wall_array->for_each([&](auto &&el) {
            if constexpr (toml::is_table<decltype(el)>) {
                entity.emplace_back(el["name"].value_or(""), el["count"].value_or(0), el["x"].value_or(0),
                                    el["y"].value_or(0), el["height"].value_or(0), el["cut"].value_or(4));
            }
        });
    }

    return entity;
}


struct Level_TOML {
    std::string name;
    std::string player;
    std::string end;
    float playerSpeed;
    float enemySpeed;
    std::vector<std::vector<std::string> > layout;
    std::vector<float> lines;
    std::string wall;
};

inline std::vector<Level_TOML> load_level(const toml::table &tbl) {
    std::vector<Level_TOML> levels;

    if (auto level_array = tbl["Layout"]["level"].as_array()) {
        level_array->for_each([&](auto &&el) {
            if constexpr (toml::is_table<decltype(el)>) {
                Level_TOML lvl;
                lvl.name = el["name"].value_or("");
                lvl.player = el["player"].value_or("");
                lvl.end = el["end"].value_or("");
                lvl.wall = el["wall"].value_or("");
                lvl.playerSpeed = el["playerSpeed"].value_or(400.0);
                lvl.enemySpeed = el["enemySpeed"].value_or(400.0);

                if (auto layout_array = el["layout"].as_array()) {
                    for (auto &&row: *layout_array) {
                        std::vector<std::string> row_vec;
                        if (auto row_array = row.as_array()) {
                            for (auto &&cell: *row_array) {
                                if (cell.is_string())
                                    row_vec.push_back(cell.value_or(""));
                            }
                        }
                        lvl.layout.push_back(std::move(row_vec));
                    }
                }

                if (auto lines_array = el["lines"].as_array()) {
                    for (auto &&line: *lines_array) {
                        if (std::optional<float> current = line.value<float>()) {
                            lvl.lines.push_back(*current);
                        }
                    }
                }

                levels.push_back(std::move(lvl));
            }
        });
    }

    return levels;
}

struct Game_TOML {
    std::string name;
    std::vector<std::string> levels;
};

inline std::vector<Game_TOML> load_game(const toml::table &tbl) {
    std::vector<Game_TOML> levels;

    if (auto level_array = tbl["Layout"]["game"].as_array()) {
        level_array->for_each([&](auto &&el) {
            if constexpr (toml::is_table<decltype(el)>) {
                Game_TOML game;
                game.name = el["name"].value_or("");

                if (auto levels_array = el["levels"].as_array()) {
                    for (auto &&levels_: *levels_array) {
                        game.levels.push_back(levels_.value_or(""));
                    }
                }

                levels.push_back(std::move(game));
            }
        });
    }

    return levels;
}

#endif //SPACE_INVADERS_TOMLREADER_H
