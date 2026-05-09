#include "Panel.h"

namespace ProTrack {
namespace UI {

Panel::Panel(float x, float y, float width, float height,
             sf::Font* f, const std::string& title)
    : font(f), hasTitle(!title.empty()) {
    background.setPosition(x, y);
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(BG_PANEL);
    background.setOutlineThickness(1);
    background.setOutlineColor(BORDER_DEFAULT);

    if (font && hasTitle) {
        titleText.setFont(*font);
        titleText.setString(title);
        titleText.setCharacterSize(16);
        titleText.setFillColor(ACCENT_PRIMARY);
        titleText.setPosition(x + 12, y + 8);
    }
}

void Panel::render(sf::RenderWindow& window) {
    window.draw(background);
    if (hasTitle) {
        window.draw(titleText);
    }
}

void Panel::setPosition(float x, float y) {
    background.setPosition(x, y);
    if (hasTitle) {
        titleText.setPosition(x + 12, y + 8);
    }
}

void Panel::setSize(float width, float height) {
    background.setSize(sf::Vector2f(width, height));
}

sf::Vector2f Panel::getPosition() const { return background.getPosition(); }
sf::Vector2f Panel::getSize() const { return background.getSize(); }
sf::FloatRect Panel::getBounds() const { return background.getGlobalBounds(); }

} // namespace UI
} // namespace ProTrack
