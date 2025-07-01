#include <first_game/scripts/PlayerCollision.hpp>
#include <components/Collider.hpp>
#include <iostream>

PlayerCollision::PlayerCollision(const std::string& label) : Script(label) {
}

void PlayerCollision::init() {

}
void PlayerCollision::update() {

}
void PlayerCollision::onCollisionEnter(Collider& target) {
    std::cout << getParent().getTag() << " entered in collision with " << target.getParent().getTag() << std::endl;
}
void PlayerCollision::onCollisionExit(Collider& target) {
    std::cout << getParent().getTag() << " exited from collision with " << target.getParent().getTag() << std::endl;
}