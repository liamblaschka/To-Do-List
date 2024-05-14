#include "List.h"
#include "Item.h"
#include "Title.h"
#include "Task.h"
#include <fstream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

List::List(sf::Font& font) {
    this->font = font;
    list = new Item*[10];

    text_cursor = sf::RectangleShape(sf::Vector2f(12, 7));
    text_cursor.setFillColor(sf::Color::Black);

    is_editing = false;
    prompt_add_task = false;
    edited = sf::Vector2i(0, 0);
    activated_list = 0;
    is_dragging = false;
    last_mouse_position = sf::Vector2f(0, 0);
    activated_line = -1;

    task_initial_position = sf::Vector2f(0, 0);
    box_initial_position = sf::Vector2f(0, 0);

    std::ifstream file("list.csv");
    count = 0;

    std::string line;
    if (!file.is_open()) {
        std::ofstream new_file("list.csv", std::ios::app);
        new_file << "Title" << ",false\n" << "Task 1" << ",false\n" << "Task 2" << ",false\n" << "Task 3" << ",false\n";
        new_file.close();
        
        file = std::ifstream("list.csv");
    }

    while (std::getline(file, line)) {
        std::stringstream line_stream(line);
        std::string column;
        int i = 0;
        while (std::getline(line_stream, column, ',')) {
            if (i == 1) {
                bool is_completed;
                std::istringstream(column) >> std::boolalpha >> is_completed;
                list[count - 1]->set_completed(is_completed);
            } else if (count == 0) {
                list[count] = new Title(font, column, sf::Vector2f(20, 10));
                item_hitboxes[count] = sf::FloatRect(0, 7, 325, 45);
                item_boxes[count] = sf::RectangleShape(sf::Vector2f(325, 45));
                item_boxes[count].setPosition(0, 7);
                item_boxes[count].setFillColor(sf::Color(247, 226, 148));
                count++;
            } else {
                list[count] = new Task(font, column, sf::Vector2f(52, count * 40 + 15), true);
                count++;
            }
            i++;
        }
    }
    if (count < 10) {
        list[count] = new Task(font, "", sf::Vector2f(52, count * 40 + 15), false);
    }

    file.close();


    for (int i = 0; i < 10; i++) {
        index_lines[i] = sf::RectangleShape(sf::Vector2f(325, 4));
        index_lines[i].setPosition(0, (i + 1) * 40 + 10);
        index_lines[i].setFillColor(sf::Color(237, 182, 119));

        if (i > 0) {
            item_hitboxes[i] = sf::FloatRect(0, i * 40 + 12, 325, 40);
            item_boxes[i] = sf::RectangleShape(sf::Vector2f(325, 40));
            item_boxes[i].setPosition(0, i * 40 + 12);
            item_boxes[i].setFillColor(sf::Color(247, 226, 148));
        }
    }
}

void List::save_edit() {
    if (edited.x) {
        if (edited.y == count) {
            if (list[count]->get_string() == "") {
                list[count]->set_added(false);
            } else if (count < 10) {
                std::ofstream file("list.csv", std::ios::app);
                file << list[count]->get_string() << ",false\n";
                file.close();
                count++;
                list[count] = new Task(font, "", sf::Vector2f(52, count * 40 + 15), false);
            }
        } else {
            std::ifstream old_file("list.csv");
            std::ofstream new_file("listnew.csv");
            std::string line;
            int line_index = 0;
            while (std::getline(old_file, line)) {
                if (line_index == edited.y) {
                    std::string column;
                    std::stringstream line_stream(line);
                    int col_index = 0;
                    while (std::getline(line_stream, column, ',')) {
                        if (col_index == 0) {
                            if (edited.y != 0 && list[edited.y]->get_string() == "") {
                                break;
                            }
                            new_file << list[edited.y]->get_string() << ",";
                        } else {
                            new_file << column << "\n";
                        }
                        col_index++;
                    }
                } else {
                    new_file << line << "\n";
                }
                line_index++;
            }
            old_file.close();
            new_file.close();

            remove("list.csv");
            rename("listnew.csv", "list.csv");

            if (edited.y != 0 && list[edited.y]->get_string() == "") {
                count--;
                for (int i = count; i > edited.y; i--) {
                    list[i]->set_position(list[i - 1]->get_position());
                }
                delete list[edited.y];
                for (int i = edited.y; i < count; i++) {
                    list[i] = list[i + 1];
                }
                sf::Vector2f last_task_position = list[count]->get_position();
                if (count == 9) {
                    list[count] = new Task(font, "", sf::Vector2f(52, count * 40 + 15), false);
                } else {
                    list[count] = list[count + 1];
                    list[count]->set_position(sf::Vector2f(52, count * 40 + 15));
                }
                
            }
        }
        edited = sf::Vector2i(0, 0);
    }
}

void List::mouse_clicked(sf::Vector2f point) {
    is_editing = false;
    list[activated_list]->set_is_activated(false);
    bool new_task_added = false;
    for (int i = 0; i <= count; i++) {
        if (i != count || count < 10) {
            if (i != count && list[i]->is_point_in_check_box(point)) {

                list[i]->toggle_completed();

                std::ifstream old_file("list.csv");
                std::ofstream new_file("listnew.csv");
                std::string line;
                int line_index = 0;
                while (std::getline(old_file, line)) {
                    if (line_index == i) {
                        std::string column;
                        std::stringstream line_stream(line);
                        int col_index = 0;
                        while (std::getline(line_stream, column, ',')) {
                            if (col_index == 1) {
                                new_file << std::boolalpha << list[i]->get_is_completed() << "\n";
                            } else {
                                new_file << column << ",";
                            }
                            col_index++;
                        }
                    } else {
                        new_file << line << "\n";
                    }
                    line_index++;
                }
                old_file.close();
                new_file.close();

                remove("list.csv");
                rename("listnew.csv", "list.csv");

            } else if (list[i]->is_point_in_text(point)) {
                if (i == count) {
                    list[count]->set_added(true);
                    new_task_added = true;
                }
                sf::Vector2f item_position = list[i]->get_position();
                sf::Vector2f item_size = list[i]->get_size();
                text_cursor.setPosition(item_position.x + item_size.x + 4, item_position.y + list[i]->get_character_size() - 5);
                list[i]->set_is_activated(true);
                activated_list = i;
                is_editing = true;
            }
        }
        
    }
    save_edit();
    if (count < 10 && !new_task_added) {
        list[count]->set_added(false);
    }
    last_mouse_position = point;
}

void List::mouse_moved(sf::Vector2f point, bool is_mouse_pressed) {
    if (is_mouse_pressed && !is_dragging) {
        for (int i = 1; i < count; i++) {
            if ((abs(point.x - last_mouse_position.x) > 3 || abs(point.y - last_mouse_position.y) > 3)
                    && (item_hitboxes[i].contains(last_mouse_position) && !list[i]->is_point_in_check_box(last_mouse_position)))
            {
                task_initial_position = list[i]->get_position();
                box_initial_position = item_boxes[i].getPosition();
                list[i]->set_is_activated(false);
                is_editing = false;
                activated_list = i;
                is_dragging = true;
            }
        }
    } else if (!is_mouse_pressed && is_dragging) {
        is_dragging = false;
    }

    if (is_dragging) {
        list[activated_list]->set_position(sf::Vector2f(task_initial_position.x + point.x - last_mouse_position.x, task_initial_position.y + point.y - last_mouse_position.y));
        item_boxes[activated_list].setPosition(box_initial_position.x + point.x - last_mouse_position.x, box_initial_position.y + point.y - last_mouse_position.y);

        activated_line = -1;
        for (int i = 0; i < count; i++) {
            float line_y = index_lines[i].getPosition().y;
            float box_y = item_boxes[activated_list].getPosition().y;
            if ((line_y + 4 >= box_y && line_y - box_y <= 40) && (i != activated_list && i + 1 != activated_list)) {
                activated_line = i;
            }
        }

    } else if (count < 10 && (!is_editing || activated_list == count) && item_hitboxes[count].contains(point)) {
        prompt_add_task = true;
    } else if (prompt_add_task) {
        prompt_add_task = false;
    }
}

void List::mouse_released(sf::Vector2f point) {
    if (is_dragging) {
        if (activated_line > -1) {
            Item* item = list[activated_list];
            if (activated_line + 1 > activated_list) { // shift up
                for (int i = activated_list; i < activated_line; i++) {
                    list[i] = list[i + 1];
                    list[i]->set_position(sf::Vector2f(52, i * 40 + 15));
                }
                list[activated_line] = item;
                list[activated_line]->set_position(sf::Vector2f(52, (activated_line) * 40 + 15));
            } else { // shift down
                for (int i = activated_list; i > activated_line + 1; i--) {
                    list[i] = list[i - 1];
                    list[i]->set_position(sf::Vector2f(52, i * 40 + 15));
                }
                list[activated_line + 1] = item;
                list[activated_line + 1]->set_position(sf::Vector2f(52, (activated_line + 1) * 40 + 15));
            }

            std::ofstream new_file("newlist.csv");
            new_file << list[0]->get_string() << ",false\n";
            for (int i = 1; i < count; i++) {
                new_file << list[i]->get_string() << "," << std::boolalpha << list[i]->get_is_completed() << "\n";
            }
            new_file.close();
            remove("list.csv");
            rename("newlist.csv", "list.csv");

        } else {
            list[activated_list]->set_position(task_initial_position);
        }
        item_boxes[activated_list].setPosition(box_initial_position);
        is_dragging = false;
    }
}

bool List::check_point_in_text(sf::Vector2f point) {
    if (!is_dragging) {
        for (int i = 0; i < count; i++) {
            if (list[i]->is_point_in_text(point)) {
                return true;
            }
        }
        if (count < 10 && list[count]->is_point_in_text(point)) {
            return true;
        }
    }
    return false;
}

void List::key_pressed(sf::Uint32 unicode) {
    if (is_editing) {
        std::string str = list[activated_list]->get_string();
        if (unicode > 32 && unicode < 127) {
            list[activated_list]->set_string(str + static_cast<char>(unicode));
        } else if (unicode == 8 && str.length() > 0) {
            str.pop_back();
            list[activated_list]->set_string(str);
        } else if (unicode == 32) {
            list[activated_list]->set_string(str + " ");
        }
        sf::Vector2f text_cursor_position = text_cursor.getPosition();
        sf::Vector2f item_position = list[activated_list]->get_position();
        sf::Vector2f item_size = list[activated_list]->get_size();
        text_cursor.setPosition(item_position.x + item_size.x + 4, text_cursor_position.y);
        edited = sf::Vector2i(1, activated_list);
    }
}

void List::toggle_text_cursor() {
    if (text_cursor_on) {
        text_cursor_on = false;
    } else {
        text_cursor_on = true;
    }
}

void List::draw(sf::RenderTarget& window, sf::RenderStates states) const {
    if (prompt_add_task) {
        window.draw(item_boxes[count]);
    }

    if (is_editing) {
        window.draw(item_boxes[activated_list]);
        if (is_editing && text_cursor_on) {
            window.draw(text_cursor);
        }
    }

    for (int i = 0; i < count; i++) {
        if (!is_dragging || (is_dragging && i != activated_list)) {
            list[i]->draw(window, states);
        }
    }
    if (prompt_add_task || (is_editing && activated_list == count)) {
        list[count]->draw(window, states);
    }

    if (is_dragging) {
        window.draw(item_boxes[activated_list]);
        if (activated_line > -1) {
            window.draw(index_lines[activated_line]);
        }
        list[activated_list]->draw(window, states);
    }
}

List::~List() {
    for (int i = 0; i < count; i++) {
        delete list[i];
    }
    delete[] list;
}