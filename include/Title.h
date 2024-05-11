#ifndef TITLE_H
#define TITLE_H

#include "Item.h"
#include <SFML/Graphics.hpp>

class Title : public Item {
    public:
        Title(sf::Font& font, std::string str, sf::Vector2f position);
};

#endif