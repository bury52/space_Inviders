//
// Created by bogus on 26.10.2025.
//
#include "Robot.h"

Robot::Robot(const std::shared_ptr<sf::Texture> &texture,const float& scale) : texture(texture), sprite(*texture) {
    setScale({scale, scale});
}

sf::FloatRect Robot::getGlobalBounds() const {
    return getTransform().transformRect(sprite.getGlobalBounds());
}

float Robot::getSize_x() const {
    return (static_cast<float>(texture->getSize().x) * getScale().x);
}

void Robot::update(const sf::Time &elapsed) {

}

void Robot::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
    for (const auto &bullet: bullets) {
        target.draw(bullet);
    }
}
