#include "LoginScreen.h"
#include "../Core/Investor.h"
#include "../Core/FundManager.h"
#include "../Data/FilePersistence.h"
#include "../Utils/ColorScheme.h"

namespace ProTrack {
using namespace UI;

LoginScreen::LoginScreen(sf::Font* font)
    : Screen(font, "Login", AppScreen::LOGIN) {
    // Title
    titleText.setFont(*font);
    titleText.setString("ProTrack");
    titleText.setCharacterSize(48);
    titleText.setFillColor(ACCENT_PRIMARY);
    titleText.setPosition(400 - titleText.getLocalBounds().width / 2, 100);

    subtitleText.setFont(*font);
    subtitleText.setString("Investment & Market Analytics");
    subtitleText.setCharacterSize(18);
    subtitleText.setFillColor(TEXT_SECONDARY);
    subtitleText.setPosition(400 - subtitleText.getLocalBounds().width / 2, 160);

    // Main panel
    mainPanel = new UI::Panel(200, 200, 400, 260, font);

    // Input fields
    nameInput = new UI::TextBox(220, 220, 360, 40, font, "Enter your name");
    balanceInput = new UI::TextBox(220, 280, 360, 40, font, "Initial balance (e.g. 10000)");

    // Buttons
    enterButton = new UI::Button(220, 340, 360, 44, "Enter Market", font,
                                  BTN_SUCCESS, BTN_SUCCESS_H, BTN_SUCCESS);
    enterButton->setOnClick([this]() {
        std::string name = nameInput->getText();
        std::string balStr = balanceInput->getText();

        if (name.empty() || balStr.empty()) {
            notification = "Please enter both name and balance!";
            notificationTimer = 3.0f;
            return;
        }

        try {
            double balance = std::stod(balStr);
            if (balance <= 0) {
                notification = "Balance must be positive!";
                notificationTimer = 3.0f;
                return;
            }

            // Create investor
            delete currentInvestor;
            currentInvestor = new Investor(1001, name, balance);
            nextScreen = AppScreen::DASHBOARD;
        } catch (...) {
            notification = "Invalid balance amount!";
            notificationTimer = 3.0f;
        }
    });

    loadButton = new UI::Button(220, 400, 360, 36, "Load Saved Portfolio", font,
                                 BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);
    loadButton->setTextSize(14);
}

LoginScreen::~LoginScreen() {
    delete nameInput;
    delete balanceInput;
    delete enterButton;
    delete loadButton;
    delete mainPanel;
}

void LoginScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    nameInput->handleEvent(event, window);
    balanceInput->handleEvent(event, window);
    enterButton->handleEvent(event, window);
    loadButton->handleEvent(event, window);
}

void LoginScreen::update(float dt, const sf::RenderWindow& window) {
    nameInput->update(dt);
    balanceInput->update(dt);
    enterButton->update(window);
    loadButton->update(window);
}

void LoginScreen::render(sf::RenderWindow& window) {
    // Background gradient effect
    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(800, 600));
    bg.setFillColor(BG_DARK);
    window.draw(bg);

    mainPanel->render(window);
    window.draw(titleText);
    window.draw(subtitleText);
    nameInput->render(window);
    balanceInput->render(window);
    enterButton->render(window);
    loadButton->render(window);
}

void LoginScreen::onEnter() {
    nameInput->clear();
    balanceInput->clear();
}

} // namespace ProTrack
