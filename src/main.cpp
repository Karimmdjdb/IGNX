#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <objects/GameObject.hpp>
#include <objects/Group.hpp>
#include <components/Renderer.hpp>
#include <components/Collider.hpp>
#include <components/Body.hpp>
#include <components/TilesRenderer.hpp>
#include <components/Camera.hpp>
#include <functions.hpp>
#include <first_game/scripts/PlayerCollision.hpp>
#include <first_game/scripts/PlayerMovement.hpp>
#include <first_game/scripts/PlayerSpawn.hpp>
#include <first_game/scripts/PlayerFeetCollision.hpp>
#include <first_game/scripts/CameraMovement.hpp>
#include <first_game/scripts/PlayerSidesCollision.hpp>

#define SHOW_HINTS 0
#define SHOW_FPS 0

sf::RenderWindow window;
unsigned int width(800), height(600);

#pragma region vars
// player 1
GameObject player("p1", 50, 750);
Renderer player_renderer("assets/sprites/virtual guy", window);
Collider player_collider(16, 26, true, {0,3});
Body player1_body(1.f, true);
PlayerCollision player1_collision_script("col-p1");
PlayerMovement player1_movement_script("mov-p1", 5);
PlayerSpawn player1_spawn_script;

// player 1 feet
GameObject player1_feet("p1-feet",0,0);
Collider player1_feet_collider(16, 2, true, {0,17});
PlayerFeetCollision player1_feet_collision_script("p1-fcs");

// player 1 sides
GameObject player1_sides("p1-side", 0,0);
Collider player1_sides_collider(25, 1, true, {0,0});
PlayerSidesCollision player1_sides_collisionscript("p1scs");

// player 2
GameObject player2("p2", 500, 912);
Renderer player2_renderer("assets/sprites/frog", window);
Collider player2_collider(32);

// tilesmap
GameObject tilesmap("tilesmap");
TilesRenderer tiles_renderer(window);

// camera
GameObject camera("camera");
Camera camera_controller(window, {width,height});
CameraMovement camera_mvt_script;

// others
sf::CircleShape point(1);
#pragma endregion

int main()
{
    sf::Clock clk;
    #pragma region vars initialization
    // point
    point.setFillColor(sf::Color::Red);

    // joueur1 composants
    player.addComponent(player_renderer);
    player.getComponent<Renderer>("renderer").addAnimation("Idle");
    player.getComponent<Renderer>("renderer").addAnimation("Run");
    player.getComponent<Renderer>("renderer").addAnimation("Jump");
    player.getComponent<Renderer>("renderer").addAnimation("Double Jump");
    player.addComponent(player_collider);
    player.addComponent(player1_body);
    player.addComponent(player1_movement_script);
    player.addComponent(player1_spawn_script);

    player1_feet.addComponent(player1_feet_collider);
    player1_feet.addComponent(player1_feet_collision_script);

    player1_sides.addComponent(player1_sides_collider);
    player1_sides.addComponent(player1_sides_collisionscript);

    Group& p1group = player.createGroup();
    p1group.addObject(player1_feet);
    p1group.addObject(player1_sides);

    // joueur1 scripts
    player.addComponent(player1_collision_script);

    // joueur2 composants
    player2.addComponent(player2_renderer);
    player2.getComponent<Renderer>("renderer").addAnimation("Idle");
    player2.addComponent(player2_collider);

    // tilesmap
    tilesmap.addComponent(tiles_renderer);

    // camera
    camera.addComponent(camera_controller);
    camera.addComponent(camera_mvt_script);
    #pragma endregion

    #pragma region window initialization
    // Création d'une fenêtre de 800x600 pixels
    window.create(sf::VideoMode({width, height}), "Game");
    window.setFramerateLimit(120);
    // centerWindow(window);
    #pragma endregion

    // game objects initilization
    GameObject::initAllObjects();

    #pragma region game loop
    // Boucle principale de l'application
    while(window.isOpen()) {

        #pragma region event management
        while(const std::optional event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) window.close(); // si l'event corréspond à closed on ferme la fenêtre
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) { // si l'event corréspond à un appui
                switch(keyPressed->scancode) {
                    case sf::Keyboard::Scancode::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::Scancode::Left:
                    case sf::Keyboard::Scancode::Up:
                    case sf::Keyboard::Scancode::Right:
                    case sf::Keyboard::Scancode::Down:
                        global::keys_pressed.insert((int)keyPressed->scancode);
                        break;
                    default:
                        break;
                }
            }
            else if(const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                switch(keyReleased->scancode) {
                    case sf::Keyboard::Scancode::Left:
                    case sf::Keyboard::Scancode::Up:
                    case sf::Keyboard::Scancode::Right:
                    case sf::Keyboard::Scancode::Down:
                        global::keys_pressed.erase((int)keyReleased->scancode);
                        break;
                    default:
                        break;
                }
            }
        }
        #pragma endregion

        #pragma region calculations
        Collider::checkCollisions(); // checks for collisions
        #pragma endregion

        #pragma region update / rendering
        GameObject::updateAllObjects();

        #if SHOW_HINTS == 1
        for(auto& collider : Collider::getAllColliders()) {
            sf::FloatRect rect = collider->getRect();
            sf::RectangleShape shape(rect.size);
            shape.setPosition(rect.position);shape.setFillColor(sf::Color::Transparent);shape.setOutlineThickness(1);shape.setOutlineColor(collider->getIsTrigger() ? sf::Color::Blue : sf::Color::Red);
            window.draw(shape);
        }

        point.setPosition({48,224});
        window.draw(point);
        #endif
        #if SHOW_FPS == 1
        std::cout << 1.0f / clk.restart().asSeconds() << std::endl;
        #endif
        #pragma endregion

        window.display();
        window.clear(sf::Color::White);

        // std::cout << player.getPosition() << " | " << (player_collider.getRect().position + sf::Vector2f(16,16)) << std::endl;
    }
    #pragma endregion

    return 0;
}
