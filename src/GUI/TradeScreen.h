#pragma once

#include "Screen.h"
#include "../Utils/Button.h"
#include "../Utils/TextBox.h"

namespace ProTrack {

class TradeScreen : public Screen {
private:
    UI::Button* backButton;
    UI::Button* buyButton;
    UI::Button* sellButton;
    UI::TextBox* fundIdBox;
    UI::TextBox* amountBox;
    UI::TextBox* unitsBox;
    bool isBuyMode;

public:
    TradeScreen(sf::Font* font);
    ~TradeScreen();

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt, const sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;
};

} // namespace ProTrack
