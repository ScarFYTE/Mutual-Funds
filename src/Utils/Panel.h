#pragma once

/**
 * Panel.h
 * Container UI element with background and optional title.
 * Demonstrates: COMPOSITION, reusable UI building block.
 */

#include <SFML/Graphics.hpp>
#include <string>
#include "ColorScheme.h"

namespace ProTrack {
namespace UI {

class Panel {
private:
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Font* font;
    bool hasTitle;

public:
    Panel(float x, float y, float width, float height,
          sf::Font* f, const std::string& title = "");

    void render(sf::RenderWindow& window);
    void setPosition(float x, float y);
    void setSize(float width, float height);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::FloatRect getBounds() const;
};

// Helper to draw a text label anywhere
inline void drawLabel(sf::RenderWindow& window, sf::Font* font,
                      const std::string& text, float x, float y,
                      unsigned int size = 18, sf::Color color = TEXT_PRIMARY) {
    sf::Text label;
    label.setFont(*font);
    label.setString(text);
    label.setCharacterSize(size);
    label.setFillColor(color);
    label.setPosition(x, y);
    window.draw(label);
}

// Helper to draw a colored metric card
inline void drawMetricCard(sf::RenderWindow& window, sf::Font* font,
                           const std::string& label, const std::string& value,
                           float x, float y, float w, float h,
                           sf::Color valueColor = TEXT_PRIMARY) {
    sf::RectangleShape card;
    card.setPosition(x, y);
    card.setSize(sf::Vector2f(w, h));
    card.setFillColor(BG_CARD);
    card.setOutlineThickness(1);
    card.setOutlineColor(BORDER_DEFAULT);
    window.draw(card);

    sf::Text labelText;
    labelText.setFont(*font);
    labelText.setString(label);
    labelText.setCharacterSize(14);
    labelText.setFillColor(TEXT_SECONDARY);
    labelText.setPosition(x + 12, y + 8);
    window.draw(labelText);

    sf::Text valueText;
    valueText.setFont(*font);
    valueText.setString(value);
    valueText.setCharacterSize(24);
    valueText.setFillColor(valueColor);
    valueText.setPosition(x + 12, y + 30);
    window.draw(valueText);
}

} // namespace UI
} // namespace ProTrack
