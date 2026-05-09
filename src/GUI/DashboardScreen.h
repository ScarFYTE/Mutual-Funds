#pragma once

#include "Screen.h"
#include "../Utils/Button.h"
#include "../Utils/ChartRenderer.h"
#include <vector>

namespace ProTrack {

class DashboardScreen : public Screen {
private:
    UI::Button* navMarket;
    UI::Button* navPortfolio;
    UI::Button* navTrade;
    UI::Button* navPerformance;
    UI::Button* navSettings;
    UI::Button* simulateButton;
    UI::ChartRenderer* miniChart;
    sf::Text welcomeText;
    sf::Text dayText;
    int dayCounter;

public:
    DashboardScreen(sf::Font* font);
    ~DashboardScreen();

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt, const sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;
};

} // namespace ProTrack
