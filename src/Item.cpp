#include "Item.h"
#include <SFML/Graphics.hpp>
#include <string>

#include <iostream>

Item::Item(sf::Font& font, std::string str, sf::Vector2f position, int character_size) {
    this->position = position;
    this->character_size = character_size;
    text.setFont(font);
    text.setString(str);
    text.setPosition(position);
    text.setFillColor(sf::Color::Black);
    is_activated = false;
}

void Item::set_string(std::string str) {
    this->str = str;
    text.setString(str);
    
    if (text.getLocalBounds().getSize().y > 0) {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 8), text.getLocalBounds().getSize());
    } else {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 5), sf::Vector2f(150 - position.x, character_size));
    }
}

int Item::get_character_size() {
    return character_size;
}

void Item::set_is_activated(bool is_activated) {
    this->is_activated = is_activated;
}

bool Item::get_is_activated() {
    return is_activated;
}

bool Item::is_point_in_text(sf::Vector2f point) {
    if (text_hitbox.contains(point)) {
        return true;
    }
    return false;
}

std::string Item::get_string() {
    return text.getString();
}

sf::Vector2f Item::get_position() {
    return position;
}

void Item::set_position(sf::Vector2f position) {
    this->position = position;
    text.setPosition(position);
    if (text.getLocalBounds().getSize().y > 0) {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 8), text.getLocalBounds().getSize());
    } else {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 5), sf::Vector2f(150 - position.x, character_size));
    }
}

sf::Vector2f Item::get_size() {
    return text.getLocalBounds().getSize();
}

void Item::set_completed(bool is_completed) {}

bool Item::get_is_completed() { return false; }

void Item::toggle_completed() {}

void Item::set_added(bool is_added) {}

bool Item::is_point_in_check_box(sf::Vector2f point) { return false; }

void Item::draw(sf::RenderTarget& window, sf::RenderStates states) const {
    window.draw(text);
}