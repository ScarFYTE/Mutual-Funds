#pragma once

/**
 * Screen.h
 * Abstract base class for all application screens.
 * Demonstrates: ABSTRACT CLASS, PURE VIRTUAL FUNCTIONS (polymorphism),
 *               TEMPLATE METHOD pattern - all screens follow same lifecycle:
 *               handleEvent -> update -> render.
 *
 * This is the foundation of the STATE PATTERN for screen management.
 */

#include <SFML/Graphics.hpp>
#include <string>
#include "../Core/Enums.h"

namespace ProTrack {

class Investor;
class FundManager;

class Screen {
protected:
    sf::Font* font;
    std::string name;
    AppScreen type;

public:
    Screen(sf::Font* f, const std::string& n, AppScreen t)
        : font(f), name(n), type(t) {}

    virtual ~Screen() = default;

    // Pure virtual - all screens MUST implement these
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window) = 0;
    virtual void update(float dt, const sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    // Lifecycle hooks
    virtual void onEnter() {}  // Called when screen becomes active
    virtual void onExit() {}   // Called when screen becomes inactive

    // Accessors
    AppScreen getType() const { return type; }
    std::string getName() const { return name; }

    // Shared reference to application state (declared here, defined in Screen.cpp)
    static Investor* currentInvestor;
    static FundManager* fundManager;
    static AppScreen nextScreen;
    static bool shouldQuit;
    static std::string notification;
    static float notificationTimer;
};

} // namespace ProTrack
