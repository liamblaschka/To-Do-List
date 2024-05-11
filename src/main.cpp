#include "List.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

#include <iostream>
#include <fstream>
#include <string>

int main() {
    sf::RenderWindow window(sf::VideoMode(325, 450), "To Do List", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    sf::Cursor cursor;

    sf::Font font;
    font.loadFromFile("../../assets/calibri-regular.ttf");

    List list(font);

    sf::Clock clock;
    bool has_focus = true;
    bool is_mouse_pressed = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                list.save_edit();
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                list.mouse_clicked(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                is_mouse_pressed = true;
            } else if (event.type == sf::Event::MouseButtonReleased) {
                is_mouse_pressed = false;
                list.mouse_released(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
            } else if (event.type == sf::Event::MouseMoved) {
                if (has_focus) {
                    list.mouse_moved(sf::Vector2f(event.mouseMove.x, event.mouseMove.y), is_mouse_pressed);
                    if (list.check_point_in_text(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
                        cursor.loadFromSystem(sf::Cursor::Text);
                        window.setMouseCursor(cursor);
                    } else {
                        cursor.loadFromSystem(sf::Cursor::Arrow);
                        window.setMouseCursor(cursor);
                    }
                }
            } else if (event.type == sf::Event::TextEntered && event.text.unicode < 128) {
                list.key_pressed(event.text.unicode);
            } else if (event.type == sf::Event::LostFocus) {
                list.mouse_clicked(sf::Vector2f(0, 0));
                has_focus = false;
            } else if (event.type == sf::Event::GainedFocus) {
                has_focus = true;
            } else if (event.type == sf::Event::MouseLeft) {
                list.mouse_released(sf::Vector2f(0, 0));
                list.mouse_moved(sf::Vector2f(0, 0), false);
		        is_mouse_pressed = false;
            }
        }

        if (clock.getElapsedTime() >= sf::seconds(0.7)) {
            list.toggle_text_cursor();
            clock.restart();
        }

        window.clear(sf::Color(250, 234, 195));
        window.draw(list);
        window.display();
    }
    return 0;
}