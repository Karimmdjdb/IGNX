#include <first_game/scripts/CameraMovement.hpp>

CameraMovement::CameraMovement() : Script("camera mvt") {
}

void CameraMovement::init() {

}

void CameraMovement::update() {
    getParent().changePosition(GameObject::withTag("p1").getPosition());
}