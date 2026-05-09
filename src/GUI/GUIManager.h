#pragma once

/**
 * GUIManager.h
 * Manages the SFML window and screen transitions.
 * Demonstrates: STATE PATTERN (screen switching), SINGLETON-like access,
 *               GAME LOOP pattern (event -> update -> render).
 */

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
#include "Screen.h"

namespace ProTrack {

class GUIManager {
private:
    sf::RenderWindow window;
    sf::Font mainFont;
    std::map<AppScreen, Screen*> screens;
    AppScreen currentScreen;
    sf::Clock deltaClock;
    sf::Text notificationText;
    sf::RectangleShape notificationBg;
    bool notificationVisible;

public:
    GUIManager();
    ~GUIManager();

    bool initialize();
    sf::Font* getMainFont();
    void registerScreen(Screen* screen);
    void run();

private:
    void handleEvents();
    void update(float dt);
    void render();
    void switchScreen(AppScreen newScreen);
    void showNotification(const std::string& msg);
};

} // namespace ProTrack
