#include <components/Component.hpp>

Component::Component(const std::string& label) : label(label) {}
Component::Component(const std::string& label, GameObject& parent) : label(label), parent(&parent) {}

std::string Component::getLabel() const {
    return label;
}

void Component::setLabel(const std::string& new_label) {
    label = new_label;
}

GameObject& Component::getParent() const {
    return *parent;
}

void Component::setParent(GameObject& parent) {
    this->parent = &parent;
}