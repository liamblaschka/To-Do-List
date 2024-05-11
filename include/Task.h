#ifndef TASK_H
#define TASK_H

#include "Item.h"
#include <SFML/Graphics.hpp>

class Task : public Item {
    private:
        bool is_completed;
        bool is_added;
        sf::RectangleShape check_box;
        sf::FloatRect check_box_hitbox;
        sf::Texture tick_texture;
        sf::Sprite tick;
        sf::Uint32 style;

        sf::RectangleShape text_bounds;
    public:
        Task(sf::Font& font, std::string str, sf::Vector2f position, bool is_added);
        void set_completed(bool is_completed);
        void toggle_completed();
        bool get_is_completed();
        void set_added(bool is_added);
        void set_is_activated(bool is_activated);
        bool is_point_in_check_box(sf::Vector2f point);
        void set_position(sf::Vector2f position);
        void draw(sf::RenderTarget& window, sf::RenderStates states) const;
};

#endif