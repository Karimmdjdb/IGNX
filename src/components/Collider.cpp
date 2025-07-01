#include <components/Collider.hpp>

#include <objects/Group.hpp>

#include <iostream>
#include <functions.hpp>

//static
std::vector<Collider*> Collider::all_colliders;

Collider::Collider(const int& size, bool is_trigger, sf::Vector2f offset) : Component("collider"), size_w(size), size_h(size), offset(offset), is_trigger(is_trigger){
    rect = sf::FloatRect({0,0},{(float)size, (float)size});
    all_colliders.push_back(this);
}

Collider::Collider(const int& width, const int& height, bool is_trigger, sf::Vector2f offset) : Component("collider"), size_w(width), size_h(height), offset(offset), is_trigger(is_trigger){
    rect = sf::FloatRect({0,0},{(float)size_w, (float)size_h});
    all_colliders.push_back(this);
}



void Collider::init() {

}

void Collider::update() {
    rect.position = getParent().getPosition() - sf::Vector2f(size_w/2.0f, size_h/2.0f) + offset;
}

void Collider::setParent(GameObject& parent) {
    Component::setParent(parent);
    rect.position = getParent().getPosition() - sf::Vector2f(size_w/2.0f, size_h/2.0f) + offset;
}

sf::FloatRect& Collider::getRect() {
    return rect;
}

bool Collider::getIsTrigger() {
    return is_trigger;
}


// static
void Collider::checkCollisions() {
    for(const auto& c1 : all_colliders) {
        for(const auto& c2 : all_colliders) {
            if(c1 == c2 || Group::compareGroups(c1->getParent(), c2->getParent())) continue;
            int x1 = c1->getRect().position.x;
            int y1 = c1->getRect().position.y;
            int x2 = c2->getRect().position.x;
            int y2 = c2->getRect().position.y;
            int w1 = c1->getRect().size.x;
            int h1 = c1->getRect().size.y;
            int w2 = c2->getRect().size.x;
            int h2 = c2->getRect().size.y;
            if(x1 < x2 + w2 && x2 < x1 + w1 && y1 < y2 + h2 && y2 < y1 + h1) { // s'il y'a une collision actuellement
                if(c1->current_collisions.find(c2) == c1->current_collisions.end()) { // s'il n y en avait pas alors les deux objets sont rentrés en collision
                    c1->current_collisions.insert(c2);
                    c1->getParent().fireCollisionEnter(*c2);
                }
            }
            else { // s'il n y'a pas de collision actuellement
                if(c1->current_collisions.find(c2) != c1->current_collisions.end()) { // s'il y en avait une alors les deux objets sons sortis de collision
                    c1->current_collisions.erase(c2);
                    c1->getParent().fireCollisionExit(*c2);
                }
            }
        }
    }
}

//static
std::vector<Collider*>& Collider::getAllColliders() {
    return Collider::all_colliders;
}
