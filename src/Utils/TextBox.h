#pragma once

/**
 * TextBox.h
 * Editable text input component.
 * Demonstrates: Keyboard event handling, text manipulation, SFML text rendering.
 */

#include <SFML/Graphics.hpp>
#include <string>
#include "ColorScheme.h"

namespace ProTrack {
namespace UI {

class TextBox {
private:
    sf::RectangleShape background;
    sf::Text text;
    sf::Font* font;
    sf::RectangleShape cursor;
    std::string content;
    bool isFocused;
    bool isVisible;
    bool isPassword;
    float cursorBlinkTimer;
    bool cursorVisible;
    unsigned int maxChars;
    std::string placeholder;

public:
    TextBox(float x, float y, float width, float height,
            sf::Font* f, const std::string& placeholderText = "");

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(float dt);
    void render(sf::RenderWindow& window);

    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setText(const std::string& str);
    std::string getText() const;
    void setPasswordMode(bool enabled);
    void setMaxChars(unsigned int max);
    void setFocused(bool focused);
    void setPlaceholder(const std::string& str);
    void clear();
    void setVisible(bool visible);

    bool isMouseOver(const sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
};

} // namespace UI
} // namespace ProTrack
