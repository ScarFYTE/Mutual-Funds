/**
 * main.cpp
 * Entry point for ProTrack SFML Application.
 * Demonstrates: OBJECT CREATION, DEPENDENCY INJECTION,
 *               proper initialization order.
 *
 * ProTrack - Automated Investment & Market Analytics System
 * OOP Course Project - IBA Karachi
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "GUI/GUIManager.h"
#include "GUI/LoginScreen.h"
#include "GUI/DashboardScreen.h"
#include "GUI/MarketScreen.h"
#include "GUI/PortfolioScreen.h"
#include "GUI/TradeScreen.h"
#include "GUI/PerformanceScreen.h"
#include "GUI/SettingsScreen.h"
#include "Core/FundManager.h"
#include "Core/Enums.h"
#include "GUI/Screen.h"

using namespace ProTrack;

int main() {
    // Seed random number generator for market simulation
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "========================================" << std::endl;
    std::cout << "  ProTrack - Investment & Analytics" << std::endl;
    std::cout << "  OOP Course Project - IBA Karachi" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Initializing SFML GUI..." << std::endl;

    // Create the fund manager and load default funds
    FundManager fundManager;
    fundManager.loadDefaultFunds();

    std::cout << "Loaded " << fundManager.getNumFunds() << " default funds." << std::endl;

    // Create GUI manager
    GUIManager gui;
    if (!gui.initialize()) {
        std::cerr << "Failed to initialize GUI!" << std::endl;
        return 1;
    }

    // Create font reference for screens
    sf::Font* appFont = Screen::font;

    // Create and register all screens
    LoginScreen* loginScreen = new LoginScreen(appFont);
    DashboardScreen* dashboardScreen = new DashboardScreen(appFont);
    MarketScreen* marketScreen = new MarketScreen(appFont);
    PortfolioScreen* portfolioScreen = new PortfolioScreen(appFont);
    TradeScreen* tradeScreen = new TradeScreen(appFont);
    PerformanceScreen* performanceScreen = new PerformanceScreen(appFont);
    SettingsScreen* settingsScreen = new SettingsScreen(appFont);

    gui.registerScreen(loginScreen);
    gui.registerScreen(dashboardScreen);
    gui.registerScreen(marketScreen);
    gui.registerScreen(portfolioScreen);
    gui.registerScreen(tradeScreen);
    gui.registerScreen(performanceScreen);
    gui.registerScreen(settingsScreen);

    // Link static references
    Screen::fundManager = &fundManager;

    std::cout << "All screens registered. Starting main loop..." << std::endl;

    // Run the application
    gui.run();

    std::cout << "ProTrack exited. Goodbye!" << std::endl;
    return 0;
}
