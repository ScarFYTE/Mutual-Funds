#pragma once

#include "Screen.h"
#include "../Utils/Button.h"
#include "../Utils/TextBox.h"
#include <vector>

namespace ProTrack {

class MutualFund;

class MarketScreen : public Screen {
private:
    UI::Button* backButton;
    UI::Button* buyButton;
    UI::Button* refreshButton;
    UI::TextBox* searchBox;
    UI::TextBox* amountBox;
    std::vector<UI::Button*> filterButtons;
    int selectedFundID;
    int scrollOffset;
    std::string searchQuery;

public:
    MarketScreen(sf::Font* font);
    ~MarketScreen();

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt, const sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;

private:
    std::vector<const MutualFund*> getFilteredFunds();
    void renderFundRow(sf::RenderWindow& window, const MutualFund* fund, float y, bool isSelected);
};

} // namespace ProTrack
