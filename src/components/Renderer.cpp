#include <components/Renderer.hpp>
#include <cmath>

Renderer::Renderer(const std::string& path, sf::RenderWindow& window) : Component("renderer"), texture_path(path), window(window) {
    textures = std::map<std::string, sf::Texture*>();
}

Renderer::~Renderer() {
    delete sprite;
    sprite = nullptr;
    for(std::map<std::string, sf::Texture*>::iterator it = textures.begin(); it != textures.end(); it++) {
        delete it->second;
        it->second = nullptr;
        textures.erase(it->first);
    }
}

void Renderer::init() {
    playAnimation("Idle");
}

void Renderer::update() {
    if(clock.getElapsedTime().asMilliseconds() >= 50) {
        frame++;
        if(frame % (texture->getSize().x / size) == 0) {
            if(play_animation_once) {
                animation_endend = true;
                playAnimation(last_animation_name);
            }
            frame = 0;
        }
        clock.restart();
    }
    sprite->setTextureRect(sf::IntRect({frame * size, 0}, {size, size}));
    sprite->setPosition({std::floor(getParent().getPosition().x), std::floor(getParent().getPosition().y)});
    window.draw(*sprite);
}

void Renderer::addAnimation(std::string name) {
    sf::Texture *text = new sf::Texture();
    if(!text->loadFromFile(texture_path + "/" + name + ".png")) {
        std::cout << "Erreur lors du chargement de la texture " << texture_path + "/" + name << "." << std::endl;
    } else {
        textures[name] = text;
    }
}

void Renderer::playAnimation(std::string name, const Frequency& freq) {
    if(!animation_endend) return;
    if(textures.find(name) != textures.end()) {
        if(texture == textures[name]) return;
        texture = textures[name];
        if(sprite == nullptr) {
            sprite = new sf::Sprite(*texture);
            sprite->setOrigin({(float)size/2, (float)size/2});
        }
        sprite->setTexture(*texture);
        frame = 0;
        if(freq == Frequency::Infinite) {
            last_animation_name = name;
            play_animation_once = false;
        }
        else if(freq == Frequency::Once) {
            play_animation_once = true;
            animation_endend = false;
        }
    }
    else {
        throw std::exception();
    }
}

void Renderer::flipX(bool flip) {
    if(flip) {
        sprite->setScale({-1.0f, 1.0f});
    }
    else {
        sprite->setScale({1.0f, 1.0f});
    }
}