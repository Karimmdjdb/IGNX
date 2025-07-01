#include <first_game/scripts/PlayerSidesCollision.hpp>

#include <first_game/scripts/PlayerMovement.hpp>

#include <objects/Group.hpp>
#include <components/Collider.hpp>
#include <iostream>

PlayerSidesCollision::PlayerSidesCollision(const std::string& label) : Script(label) {
}

void PlayerSidesCollision::init() {

}
void PlayerSidesCollision::update() {

}
void PlayerSidesCollision::onCollisionEnter(Collider& target) {
    if(target.getIsTrigger() == false) {
        PlayerMovement& mvt = getParent().getGroup().main->getComponent<PlayerMovement>("mov-p1");
        mvt.can_cling = true;
    }

}
void PlayerSidesCollision::onCollisionExit(Collider& target) {
    if(target.getIsTrigger() == false) {
        PlayerMovement& mvt = getParent().getGroup().main->getComponent<PlayerMovement>("mov-p1");
        mvt.can_cling = false;
    }
}