#pragma once

#include "Screen.h"
#include "../Utils/Button.h"
#include "../Utils/ChartRenderer.h"

namespace ProTrack {

class PerformanceScreen : public Screen {
private:
    UI::Button* backButton;
    UI::ChartRenderer* portfolioChart;
    UI::ChartRenderer* allocationChart;

public:
    PerformanceScreen(sf::Font* font);
    ~PerformanceScreen();

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt, const sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
};

} // namespace ProTrack
