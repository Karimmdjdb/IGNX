#include <first_game/scripts/PlayerFeetCollision.hpp>

#include <first_game/scripts/PlayerMovement.hpp>

#include <objects/Group.hpp>
#include <components/Collider.hpp>
#include <iostream>

PlayerFeetCollision::PlayerFeetCollision(const std::string& label) : Script(label) {
}

void PlayerFeetCollision::init() {

}
void PlayerFeetCollision::update() {

}
void PlayerFeetCollision::onCollisionEnter(Collider& target) {
    if(target.getIsTrigger() == false) {
        PlayerMovement& mvt = getParent().getGroup().main->getComponent<PlayerMovement>("mov-p1");
        mvt.jumps_count = 2;
        mvt.is_jumping = false;
    }

}
void PlayerFeetCollision::onCollisionExit(Collider& target) {
}