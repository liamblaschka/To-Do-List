#include "Item.h"
#include "Task.h"
#include <SFML/Graphics.hpp>

Task::Task(sf::Font& font, std::string str, sf::Vector2f position, bool is_added):Item(font, str, position, 25) {
    this->is_added = is_added;
    is_completed = false;
    text.setCharacterSize(25);
    check_box = sf::RectangleShape(sf::Vector2f(20, 20));
    check_box.setPosition(position.x - 30, position.y + 7);
    check_box.setFillColor(sf::Color::Transparent);
    if (is_added) {
        check_box.setOutlineColor(sf::Color::Black);
    } else {
        text.setString("+ new");
        text.setFillColor(sf::Color(160, 160, 160));
        check_box.setOutlineColor(sf::Color(160, 160, 160));
    }
    check_box.setOutlineThickness(3);

    check_box_hitbox = sf::FloatRect(position.x - 30, position.y + 7, 20, 20);

    tick_texture.loadFromFile("../../assets/tick.png");
    tick = sf::Sprite(tick_texture);
    tick.setPosition(position.x - 30, position.y + 7);

    style = sf::Text::Regular;

    if (text.getLocalBounds().getSize().y > 0) {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 8), text.getLocalBounds().getSize());
    } else {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 5), sf::Vector2f(150 - position.x, character_size));
    }
}

void Task::set_completed(bool is_completed) {
    this->is_completed = is_completed;
    if (is_completed) {
        text.setFillColor(sf::Color(125, 125, 125));
        style = sf::Text::StrikeThrough;
        check_box.setOutlineColor(sf::Color(125, 125, 125));
    } else {
        text.setFillColor(sf::Color::Black);
        style = sf::Text::Regular;
        check_box.setOutlineColor(sf::Color::Black);
    }
    text.setStyle(style);
}

void Task::toggle_completed() {
    if (is_completed) {
        set_completed(false);
    } else {
        set_completed(true);
    }
}

bool Task::get_is_completed() {
    return is_completed;
}

void Task::set_added(bool is_added) {
    this->is_added = is_added;
    if (is_added) {
        text.setString("");
        text.setFillColor(sf::Color::Black);
        check_box.setOutlineColor(sf::Color::Black);
    } else {
        text.setString("+ new");
        text.setFillColor(sf::Color(160, 160, 160));
        check_box.setOutlineColor(sf::Color(160, 160, 160));
    }
}

void Task::set_is_activated(bool is_activated) {
    this->is_activated = is_activated;

    if (is_activated) {
        text.setStyle(sf::Text::Underlined);
    } else {
        text.setStyle(style);
    }
}

bool Task::is_point_in_check_box(sf::Vector2f point) {
    if (check_box_hitbox.contains(point)) {
        return true;
    }
    return false;
}

void Task::set_position(sf::Vector2f position) {
    this->position = position;
    text.setPosition(position);
    if (text.getLocalBounds().getSize().y > 0) {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 8), text.getLocalBounds().getSize());
    } else {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 5), sf::Vector2f(150 - position.x, character_size));
    }
    check_box.setPosition(position.x - 30, position.y + 7);
    check_box_hitbox = sf::FloatRect(position.x - 30, position.y + 7, 20, 20);
    tick.setPosition(position.x - 30, position.y + 7);
}

void Task::draw(sf::RenderTarget& window, sf::RenderStates states) const {
    window.draw(text);
    window.draw(check_box);
    if (is_completed) {
        window.draw(tick);
    }
}