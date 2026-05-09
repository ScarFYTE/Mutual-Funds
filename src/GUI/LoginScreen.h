#pragma once

#include "Screen.h"
#include "../Utils/Button.h"
#include "../Utils/TextBox.h"
#include "../Utils/Panel.h"

namespace ProTrack {

class LoginScreen : public Screen {
private:
    UI::TextBox* nameInput;
    UI::TextBox* balanceInput;
    UI::Button* enterButton;
    UI::Button* loadButton;
    UI::Panel* mainPanel;
    sf::Text titleText;
    sf::Text subtitleText;

public:
    LoginScreen(sf::Font* font);
    ~LoginScreen();

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;
};

} // namespace ProTrack
