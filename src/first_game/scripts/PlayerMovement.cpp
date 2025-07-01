
#include <first_game/scripts/PlayerMovement.hpp>

#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <components/Body.hpp>
#include <components/Renderer.hpp>
#include <functions.hpp>

PlayerMovement::PlayerMovement(const std::string& label, const float& speed) : Script(label), speed(speed) {
}

void PlayerMovement::init() {
}

void PlayerMovement::update() {
    if(!isKeyPressed(sf::Keyboard::Scancode::Space)) can_type_jump = true;

    Renderer& rd = getParent().getComponent<Renderer>("renderer");
    Body& body = getParent().getComponent<Body>("body");

    bool is_moving = false;

    // vertical movement
    float horizontal_movement = 0.0f;
    if(isKeyPressed(sf::Keyboard::Scancode::Left)) horizontal_movement += -0.5f * speed;
    if(isKeyPressed(sf::Keyboard::Scancode::Right)) horizontal_movement += 0.5f * speed;
    if(std::abs(horizontal_movement) > 0) is_moving = true;
    if(horizontal_movement > 0) rd.flipX(false);
    else if(horizontal_movement < 0) rd.flipX(true);
    body.velocity.x = horizontal_movement;
    std::cout << (can_cling && std::abs(horizontal_movement) > 0) << std::endl;
    if(can_cling && std::abs(horizontal_movement) > 0) {
        body.velocity.y = 0;
        body.addForce(Body::GRAVITY*-2.0f*body.mass, Body::Force::Impulsion);
    }

    // jump
    if(isKeyPressed(sf::Keyboard::Scancode::Space)) {
        if(can_type_jump) {
            if(jumps_count >= 1) {
                body.velocity = {0,0};
                body.addForce({0.f, Body::GRAVITY.y * jump_force * -6}, Body::Force::Impulsion);
                if(jumps_count == 1) rd.playAnimation("Double Jump", Renderer::Frequency::Once);
                jumps_count--;
                is_jumping = true;
                can_type_jump = false;
            }
        }
    }

    if(is_jumping) rd.playAnimation("Jump");
    else if(is_moving) rd.playAnimation("Run");
    else rd.playAnimation("Idle");

    // air resistance
    if(body.velocity.y >= 10) {
        body.addForce({0, -Body::GRAVITY.y}, Body::Force::Impulsion);
    }
}
