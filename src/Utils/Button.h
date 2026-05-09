#pragma once

/**
 * Button.h
 * Interactive SFML button component.
 * Demonstrates: SFML rendering, event handling, state management (idle/hover/pressed).
 */

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "ColorScheme.h"

namespace ProTrack {
namespace UI {

class Button {
private:
    sf::RectangleShape shape;
    sf::Text label;
    sf::Font* font;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color pressedColor;
    bool isHovered;
    bool isPressed;
    bool isVisible;
    std::function<void()> onClick;

public:
    Button(float x, float y, float width, float height,
           const std::string& text, sf::Font* f,
           sf::Color idle = BTN_PRIMARY,
           sf::Color hover = BTN_PRIMARY_H,
           sf::Color pressed = BTN_PRIMARY);

    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setColors(sf::Color idle, sf::Color hover, sf::Color pressed);
    void setText(const std::string& text);
    void setTextSize(unsigned int size);
    void setOnClick(std::function<void()> callback);
    void setVisible(bool visible);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    bool contains(float x, float y) const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
};

} // namespace UI
} // namespace ProTrack
