#include <objects/GameObject.hpp>
#include <objects/Group.hpp>

#include <components/Renderer.hpp>
#include <components/Collider.hpp>
#include <components/Script.hpp>
#include <components/Body.hpp>

#include <functional>

std::map<std::string, GameObject*> GameObject::all_objects;

// static
void GameObject::initAllObjects() {
    if(all_objects.empty()) return;
    for(auto& obj : all_objects) {
        obj.second->init();
    }
}
void GameObject::updateAllObjects() {
    if(all_objects.empty()) return;
    for(auto& obj : all_objects) {
        obj.second->update();
    }
}

GameObject& GameObject::withTag(const std::string& tag) {
    return *all_objects[tag];
}

GameObject::GameObject(std::string tag) : tag(tag), position(0,0) {
    all_objects[tag] = this;
}

GameObject::GameObject(std::string tag, int x, int y) : tag(tag), position(x,y) {
    all_objects[tag] = this;
}

GameObject::GameObject(sf::Vector2f position) : position(position) {
    all_objects[tag] = this;
}

GameObject::~GameObject() {
    if(group == nullptr || group->main != this || group->objects.empty()) return; // the group is deleted only if the instance is the main object of it
    delete group;
    group = nullptr;
}

void GameObject::addComponent(Component& component) {
    components[component.getLabel()] = &component;
    component.setParent(*this);
}

void GameObject::init() {
    for(auto& [_, component] : components) {
        component->init();
    }
    if(group == nullptr || group->main != this || group->objects.empty()) return; // the group is initialized only if the instance is the main object of it
    for(auto& obj : group->objects) {
        obj->init();
    }
}

void GameObject::update() {
    for(auto& [_, component] : components) {
        component->update();
    }
    if(group == nullptr || group->main != this || group->objects.empty()) return; // the group is updated only if the instance is the main object of it
    for(auto& obj : group->objects) {
        obj->update();
    }
}

sf::Vector2f& GameObject::getPosition() {
    return this->position;
}

void GameObject::changePosition(const sf::Vector2f& new_position) {
    position = new_position;
    transmitPositionToGroup();
}

const std::string& GameObject::getTag() const {
    return tag;
}

bool GameObject::compareTag(std::string target_tag) const {
    return tag.compare(target_tag);
}


void GameObject::fireCollisionEnter(Collider& target) {
    for(auto& [_, component] : components)
    {
        if(Script* script = dynamic_cast<Script*>(component)) {
            script->onCollisionEnter(target);
        }
    }
}

void GameObject::fireCollisionExit(Collider& target) {
    for(auto& [_, component] : components)
    {
        if(Script* script = dynamic_cast<Script*>(component)) {
            script->onCollisionExit(target);
        }
    }
}

Group& GameObject::createGroup() {
    Group* grp = new Group();
    grp->setMain(*this);
    return *grp;
}

const Group& GameObject::getGroup() const {
    return *group;
}

void GameObject::transmitPositionToGroup() {
    if(group == nullptr || group->main != this || group->objects.empty()) return; // the object transmit its position to group memebers only if he's the main object
    for(auto& obj : group->objects) {
        obj->position = position;
    }
}


