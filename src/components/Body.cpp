#include <components/Body.hpp>

#include <iostream>
#include <functions.hpp>

#include <components/Collider.hpp>

const sf::Vector2f Body::GRAVITY(0,0.3);

Body::Body(float mass, bool is_subject_to_gravity) : Component("body"), mass(mass), is_subject_to_gravity(is_subject_to_gravity) {
}

void Body::init() {
    if(is_subject_to_gravity) forces.push_back(GRAVITY * mass);
}

void Body::update() {
    // calcul de la somme des forces ΣF
    sf::Vector2f forces_sum(0, 0);
    for(auto& force : forces) {
        forces_sum += force;
    }
    if(!impulsions.empty())
    for(int i = 0; i < impulsions.size(); i++) {
        forces_sum += *(impulsions.begin()+i);
        impulsions.erase(impulsions.begin()+i);
    }
    // calcul de l'accélération en appliquant la 2nd loi de Newton a = ΣF / m
    sf::Vector2f accel(forces_sum);
    accel /= mass;
    velocity += accel;
    movePosition();
    // std::cout << velocity << std::endl;
}

void Body::movePosition() {
    // check if this movement will engenders a forbidden collision
    sf::FloatRect rect = getParent().getComponent<Collider>("collider").getRect();
    const auto& [dx, dy] = velocity;
    const auto& [x1, y1] = rect.position;
    const auto& [w1, h1] = rect.size;
    const bool& self_is_trigger = getParent().getComponent<Collider>("collider").getIsTrigger();
    for(const auto& collider : Collider::getAllColliders()) {
        if(self_is_trigger && collider->getIsTrigger()) continue; // if the two colliders are triggerable allow movement
        const auto& [x2, y2] = collider->getRect().position;
        const auto& [w2, h2] = collider->getRect().size;
        // check in x
        if(x1+dx < x2+w2 && x2 < x1+dx+w1 && y1 < y2+h2 && y2 < y1+h1) {
            velocity.x = 0;
        }
        // check in y
        if(x1 < x2+w2 && x2 < x1+w1 && y1+dy < y2+h2 && y2 < y1+dy+h1) {
            velocity.y = 0;
        }
    }
    // (x1 <= x2 + w2 && x2 <= x1 + w1 && y1 <= y2 + h2 && y2 <= y1 + h1)
    getParent().changePosition(getParent().getPosition() + velocity);
}

void Body::addForce(const sf::Vector2f& force, const Force& type) {
    if(type == Force::Constant) forces.push_back(force);
    else if(type == Force::Impulsion) impulsions.push_back(force);
}

