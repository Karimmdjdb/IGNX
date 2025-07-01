#include <components/Camera.hpp>

#include <cmath>
#include <SFML/Graphics/RenderWindow.hpp>

Camera::Camera(sf::RenderWindow& window, const sf::Vector2f& size) : Component("camera"), window(window), size(size) {
    view.setSize(size);
}

void Camera::init() {
    view.zoom(0.5f);
}

void Camera::update() {
    view.setCenter({std::floor(getParent().getPosition().x), std::floor(getParent().getPosition().y)});
    window.setView(view);
}