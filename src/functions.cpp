#include "functions.hpp"

void centerWindow(sf::RenderWindow& window) {
    int x,y;
    x = (sf::VideoMode::getDesktopMode().size.x / 2) - window.getSize().x / 2;
    y = (sf::VideoMode::getDesktopMode().size.y / 2) - window.getSize().y / 2;
    window.setPosition({x,y});
}

bool isKeyPressed(const int& code) {
    return global::keys_pressed.find(code) == global::keys_pressed.end();
}