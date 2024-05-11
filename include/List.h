#ifndef LIST_H
#define LIST_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Item.h"

class List : public sf::Drawable {
    private:
        int count;
        int line_focus;
        sf::Font font;
        Item** list;
        sf::RectangleShape text_cursor;
        sf::FloatRect item_hitboxes[10];
        sf::RectangleShape item_boxes[10];
        sf::RectangleShape index_lines[10];
        int activated_line;
        bool is_editing;
        sf::Vector2i edited;
        bool text_cursor_on;
        int activated_list;
        bool prompt_add_task;
        sf::Clock clock; 
        sf::Vector2f last_mouse_position;
        bool is_dragging;
        sf::Vector2f task_initial_position;
        sf::Vector2f box_initial_position;
    public:
        List(sf::Font& font);
        void save_edit();
        void mouse_clicked(sf::Vector2f point);
        void mouse_moved(sf::Vector2f point, bool is_mouse_pressed);
        void mouse_released(sf::Vector2f point);
        bool check_point_in_text(sf::Vector2f point);
        void key_pressed(sf::Uint32 unicode);
        void toggle_text_cursor();
        void draw(sf::RenderTarget& window, sf::RenderStates states) const;
        ~List();
};

#endif