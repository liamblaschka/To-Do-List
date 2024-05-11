#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include <string>

class Item {
    protected:
        sf::Text text;
        sf::Font font;
        bool is_activated;
        sf::FloatRect text_hitbox;
        sf::Vector2f position;
        std::string str;
        int character_size;

        // sf::RectangleShape hitbox_display;
    public:
        Item(sf::Font& font, std::string str, sf::Vector2f position, int character_size);
        void set_string(std::string str);
        std::string get_string();
        int get_character_size();
        sf::Vector2f get_position();
        sf::Vector2f get_size();
        virtual void set_is_activated(bool is_activated);
        bool get_is_activated();
        bool is_point_in_text(sf::Vector2f point);
        virtual void set_position(sf::Vector2f position);
        virtual void set_completed(bool is_completed);
        virtual void toggle_completed();
        virtual bool get_is_completed();
        virtual void set_added(bool is_added);
        virtual bool is_point_in_check_box(sf::Vector2f point);
        virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const;
};

#endif