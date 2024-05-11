#include "Item.h"
#include "Title.h"
#include <SFML/Graphics.hpp>

Title::Title(sf::Font& font, std::string str, sf::Vector2f position):Item(font, str, position, 30) {
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    if (text.getLocalBounds().getSize().y > 0) {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 8), text.getLocalBounds().getSize());
    } else {
        text_hitbox = sf::FloatRect(sf::Vector2f(position.x, position.y + 5), sf::Vector2f(150 - position.x, character_size));
    }
}