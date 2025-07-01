#include <objects/Group.hpp>
#include <objects/GameObject.hpp>

#include <iostream>


void Group::setMain(GameObject& main) {
    this->main = &main;
    main.group = this;
}

GameObject& Group::getMain() {
    return *main;
}

void Group::addObject(GameObject& obj) {
    objects.push_back(&obj);
    obj.group = this;
}

bool Group::compareGroups(const GameObject& obj1, const GameObject& obj2) {
    if(obj1.group == nullptr || obj2.group == nullptr) return false;
    return obj1.group->main == obj2.group->main;
}