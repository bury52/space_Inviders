//
// Created by bogus on 26.10.2025.
//
#include "Robot.h"

Robot::Robot(const std::shared_ptr<sf::Texture> &texture, const float &scale) : texture(texture), sprite(*texture) {
    setScale({scale, scale});
}

sf::FloatRect Robot::get_bounds() const {
    return getTransform().transformRect(sprite.getGlobalBounds());
}

float Robot::getSize_x() const {
    return (static_cast<float>(texture->getSize().x) * getScale().x);
}

void Robot::update(const sf::Time &elapsed, const sf::FloatRect &player) {
    if (!can_shoot)
        return;

    sprite.setColor(sf::Color::Red); 
    time_from_shot += elapsed;
    if (time_from_shot >= bullet_delay && get_bounds().getCenter().x - 5 < player.getCenter().x && get_bounds().getCenter().x + 5 > player.getCenter().x ) {
        auto &bullet = bullets.emplace_back(sf::Vector2f(getScale().x, getScale().y * 3));
        bullet.setPosition({get_bounds().getCenter().x, getPosition().y});
        time_from_shot = sf::seconds(0);
    }
    for (auto &bullet: bullets) {
        bullet.move({0, elapsed.asSeconds() * bullet_speed});
    }
}

void Robot::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
    for (const auto &bullet: bullets) {
        target.draw(bullet);
    }
}
