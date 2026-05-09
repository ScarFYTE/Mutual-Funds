#pragma once

#include "Screen.h"
#include "../Utils/Button.h"
#include "../Utils/TextBox.h"

namespace ProTrack {

class PortfolioScreen : public Screen {
private:
    UI::Button* backButton;
    UI::Button* sellButton;
    UI::TextBox* unitsBox;
    int selectedFundID;
    int scrollOffset;

public:
    PortfolioScreen(sf::Font* font);
    ~PortfolioScreen();

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt, const sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;
};

} // namespace ProTrack
