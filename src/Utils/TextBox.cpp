#include "TextBox.h"

namespace ProTrack {
namespace UI {

TextBox::TextBox(float x, float y, float width, float height,
                 sf::Font* f, const std::string& placeholderText)
    : font(f), isFocused(false), isVisible(true), isPassword(false),
      cursorBlinkTimer(0), cursorVisible(true), maxChars(100), placeholder(placeholderText) {
    background.setPosition(x, y);
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(BG_INPUT);
    background.setOutlineThickness(1);
    background.setOutlineColor(BORDER_DEFAULT);

    if (font) {
        text.setFont(*font);
        text.setCharacterSize(static_cast<unsigned int>(height * 0.4f));
        text.setFillColor(TEXT_PRIMARY);
        text.setPosition(x + 12, y + height * 0.3f);

        // Placeholder text styling
        if (!placeholderText.empty()) {
            text.setFillColor(TEXT_MUTED);
            text.setString(placeholderText);
        }
    }

    cursor.setSize(sf::Vector2f(2, height * 0.5f));
    cursor.setFillColor(ACCENT_PRIMARY);
    cursor.setPosition(x + 12, y + height * 0.25f);
}

void TextBox::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!isVisible) return;

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        isFocused = background.getGlobalBounds().contains(mousePos);
        background.setOutlineColor(isFocused ? BORDER_FOCUSED : BORDER_DEFAULT);
    }

    if (!isFocused) return;

    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) { // Backspace
            if (!content.empty()) {
                content.pop_back();
            }
        } else if (event.text.unicode == 13) { // Enter
            // Do nothing - let parent handle enter
        } else if (event.text.unicode < 128 && content.length() < maxChars) {
            content += static_cast<char>(event.text.unicode);
        }
    }
}

void TextBox::update(float dt) {
    if (!isFocused) {
        cursorVisible = false;
        return;
    }

    cursorBlinkTimer += dt;
    if (cursorBlinkTimer >= 0.5f) {
        cursorBlinkTimer = 0;
        cursorVisible = !cursorVisible;
    }

    // Update text display
    if (content.empty() && !isFocused) {
        text.setFillColor(TEXT_MUTED);
        text.setString(placeholder);
    } else {
        text.setFillColor(TEXT_PRIMARY);
        if (isPassword) {
            text.setString(std::string(content.length(), '*'));
        } else {
            text.setString(content);
        }
    }

    // Update cursor position
    float textWidth = text.getGlobalBounds().width;
    sf::Vector2f bgPos = background.getPosition();
    cursor.setPosition(bgPos.x + 12 + textWidth + 2, bgPos.y + background.getSize().y * 0.25f);
}

void TextBox::render(sf::RenderWindow& window) {
    if (!isVisible) return;
    window.draw(background);
    window.draw(text);
    if (isFocused && cursorVisible) {
        window.draw(cursor);
    }
}

void TextBox::setPosition(float x, float y) {
    background.setPosition(x, y);
    text.setPosition(x + 12, y + background.getSize().y * 0.3f);
}

void TextBox::setSize(float width, float height) {
    background.setSize(sf::Vector2f(width, height));
    text.setCharacterSize(static_cast<unsigned int>(height * 0.4f));
}

void TextBox::setText(const std::string& str) {
    content = str;
    if (content.empty() && !isFocused) {
        text.setFillColor(TEXT_MUTED);
        text.setString(placeholder);
    } else {
        text.setFillColor(TEXT_PRIMARY);
        text.setString(isPassword ? std::string(content.length(), '*') : content);
    }
}

std::string TextBox::getText() const { return content; }

void TextBox::setPasswordMode(bool enabled) { isPassword = enabled; }
void TextBox::setMaxChars(unsigned int max) { maxChars = max; }

void TextBox::setFocused(bool focused) {
    isFocused = focused;
    background.setOutlineColor(isFocused ? BORDER_FOCUSED : BORDER_DEFAULT);
}

void TextBox::setPlaceholder(const std::string& str) { placeholder = str; }

void TextBox::clear() {
    content.clear();
    text.setString(placeholder);
    text.setFillColor(TEXT_MUTED);
}

void TextBox::setVisible(bool visible) { isVisible = visible; }

bool TextBox::isMouseOver(const sf::RenderWindow& window) const {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return background.getGlobalBounds().contains(mousePos);
}

sf::Vector2f TextBox::getPosition() const { return background.getPosition(); }
sf::Vector2f TextBox::getSize() const { return background.getSize(); }

} // namespace UI
} // namespace ProTrack
