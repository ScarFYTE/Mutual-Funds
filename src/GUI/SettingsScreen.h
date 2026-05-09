#pragma once

#include "Screen.h"
#include "../Utils/Button.h"
#include "../Utils/TextBox.h"

namespace ProTrack {

class SettingsScreen : public Screen {
private:
    UI::Button* backButton;
    UI::Button* saveButton;
    UI::Button* loadButton;
    UI::Button* addCashButton;
    UI::TextBox* saveNameBox;
    UI::TextBox* cashBox;
    std::vector<std::string> saveFiles;
    int selectedSaveIndex;

public:
    SettingsScreen(sf::Font* font);
    ~SettingsScreen();

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt, const sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;

private:
    void refreshSaveList();
};

} // namespace ProTrack
