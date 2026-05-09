#include "Button.h"

namespace ProTrack {
namespace UI {

Button::Button(float x, float y, float width, float height,
               const std::string& text, sf::Font* f,
               sf::Color idle, sf::Color hover, sf::Color pressed)
    : font(f), idleColor(idle), hoverColor(hover), pressedColor(pressed),
      isHovered(false), isPressed(false), isVisible(true) {
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(idleColor);
    shape.setOutlineThickness(0);

    if (font) {
        label.setFont(*font);
        label.setString(text);
        label.setCharacterSize(static_cast<unsigned int>(height * 0.4f));
        label.setFillColor(TEXT_PRIMARY);
        // Center text
        sf::FloatRect textBounds = label.getLocalBounds();
        label.setOrigin(textBounds.left + textBounds.width / 2.0f,
                        textBounds.top + textBounds.height / 2.0f);
        label.setPosition(x + width / 2.0f, y + height / 2.0f);
    }
}

void Button::setPosition(float x, float y) {
    shape.setPosition(x, y);
    sf::Vector2f size = shape.getSize();
    label.setPosition(x + size.x / 2.0f, y + size.y / 2.0f);
}

void Button::setSize(float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
    label.setCharacterSize(static_cast<unsigned int>(height * 0.4f));
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(textBounds.left + textBounds.width / 2.0f,
                    textBounds.top + textBounds.height / 2.0f);
    sf::Vector2f pos = shape.getPosition();
    label.setPosition(pos.x + width / 2.0f, pos.y + height / 2.0f);
}

void Button::setColors(sf::Color idle, sf::Color hover, sf::Color pressed) {
    idleColor = idle;
    hoverColor = hover;
    pressedColor = pressed;
    shape.setFillColor(idleColor);
}

void Button::setText(const std::string& text) {
    label.setString(text);
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(textBounds.left + textBounds.width / 2.0f,
                    textBounds.top + textBounds.height / 2.0f);
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f size = shape.getSize();
    label.setPosition(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
}

void Button::setTextSize(unsigned int size) {
    label.setCharacterSize(size);
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(textBounds.left + textBounds.width / 2.0f,
                    textBounds.top + textBounds.height / 2.0f);
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f sz = shape.getSize();
    label.setPosition(pos.x + sz.x / 2.0f, pos.y + sz.y / 2.0f);
}

void Button::setOnClick(std::function<void()> callback) { onClick = callback; }
void Button::setVisible(bool visible) { isVisible = visible; }

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!isVisible) return;

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseMoved) {
        isHovered = shape.getGlobalBounds().contains(mousePos);
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            isPressed = true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (isPressed && shape.getGlobalBounds().contains(mousePos)) {
            if (onClick) onClick();
        }
        isPressed = false;
    }
}

void Button::update(const sf::RenderWindow& window) {
    if (!isVisible) return;
    if (isPressed) {
        shape.setFillColor(pressedColor);
    } else if (isHovered) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(idleColor);
    }
}

void Button::render(sf::RenderWindow& window) {
    if (!isVisible) return;
    window.draw(shape);
    window.draw(label);
}

bool Button::contains(float x, float y) const {
    return shape.getGlobalBounds().contains(x, y);
}

sf::Vector2f Button::getPosition() const { return shape.getPosition(); }
sf::Vector2f Button::getSize() const { return shape.getSize(); }

} // namespace UI
} // namespace ProTrack
