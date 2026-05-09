#include "SettingsScreen.h"
#include "../Core/Investor.h"
#include "../Core/FundManager.h"
#include "../Data/FilePersistence.h"
#include "../Utils/ColorScheme.h"
#include <sstream>
#include <iomanip>

namespace ProTrack {

SettingsScreen::SettingsScreen(sf::Font* font)
    : Screen(font, "Settings", AppScreen::SETTINGS), selectedSaveIndex(-1) {

    backButton = new UI::Button(20, 15, 80, 30, "< Back", font, BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);
    backButton->setTextSize(12);
    backButton->setOnClick([]() { nextScreen = AppScreen::DASHBOARD; });

    saveNameBox = new UI::TextBox(30, 120, 240, 36, font, "Save file name");

    saveButton = new UI::Button(30, 170, 115, 36, "Save", font, BTN_SUCCESS, BTN_SUCCESS_H, BTN_SUCCESS);
    saveButton->setTextSize(13);
    saveButton->setOnClick([this]() {
        if (!currentInvestor) return;
        std::string name = saveNameBox->getText();
        if (name.empty()) name = "portfolio_save";

        FilePersistence fp;
        if (fp.savePortfolio(*currentInvestor, name)) {
            notification = "Portfolio saved successfully!";
            notificationTimer = 2.0f;
            refreshSaveList();
        } else {
            notification = "Save failed!";
            notificationTimer = 2.0f;
        }
    });

    loadButton = new UI::Button(155, 170, 115, 36, "Load", font, BTN_PRIMARY, BTN_PRIMARY_H, BTN_PRIMARY);
    loadButton->setTextSize(13);
    loadButton->setOnClick([this]() {
        if (!currentInvestor || selectedSaveIndex < 0 || selectedSaveIndex >= static_cast<int>(saveFiles.size())) return;

        FilePersistence fp;
        if (fp.loadPortfolio(*currentInvestor, *fundManager, saveFiles[selectedSaveIndex])) {
            notification = "Portfolio loaded!";
            notificationTimer = 2.0f;
        } else {
            notification = "Load failed!";
            notificationTimer = 2.0f;
        }
    });

    cashBox = new UI::TextBox(400, 120, 200, 36, font, "Amount to add");

    addCashButton = new UI::Button(400, 170, 200, 36, "Add Cash", font, BTN_SUCCESS, BTN_SUCCESS_H, BTN_SUCCESS);
    addCashButton->setTextSize(13);
    addCashButton->setOnClick([this]() {
        if (!currentInvestor) return;
        try {
            double amount = std::stod(cashBox->getText());
            if (amount > 0) {
                currentInvestor->deposit(amount);
                notification = "$" + std::to_string(static_cast<int>(amount)) + " added!";
                notificationTimer = 2.0f;
                cashBox->clear();
            }
        } catch (...) {
            notification = "Invalid amount!";
            notificationTimer = 2.0f;
        }
    });
}

SettingsScreen::~SettingsScreen() {
    delete backButton; delete saveButton; delete loadButton; delete addCashButton;
    delete saveNameBox; delete cashBox;
}

void SettingsScreen::refreshSaveList() {
    FilePersistence fp;
    saveFiles = fp.listSaveFiles();
}

void SettingsScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    backButton->handleEvent(event, window);
    saveButton->handleEvent(event, window);
    loadButton->handleEvent(event, window);
    addCashButton->handleEvent(event, window);
    saveNameBox->handleEvent(event, window);
    cashBox->handleEvent(event, window);

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (size_t i = 0; i < saveFiles.size() && i < 8; ++i) {
            float rowY = 320 + i * 30;
            if (mousePos.x >= 30 && mousePos.x <= 300 && mousePos.y >= rowY && mousePos.y <= rowY + 25) {
                selectedSaveIndex = static_cast<int>(i);
                saveNameBox->setText(saveFiles[i]);
                break;
            }
        }
    }
}

void SettingsScreen::update(float dt, const sf::RenderWindow& window) {
    backButton->update(window);
    saveButton->update(window);
    loadButton->update(window);
    addCashButton->update(window);
    saveNameBox->update(dt);
    cashBox->update(dt);
}

void SettingsScreen::render(sf::RenderWindow& window) {
    if (!currentInvestor) return;

    drawLabel(window, font, "Settings", 20, 60, 20, ACCENT_PRIMARY);

    // Save/Load panel
    sf::RectangleShape savePanel;
    savePanel.setPosition(20, 100);
    savePanel.setSize(sf::Vector2f(350, 200));
    savePanel.setFillColor(BG_PANEL);
    savePanel.setOutlineThickness(1);
    savePanel.setOutlineColor(BORDER_DEFAULT);
    window.draw(savePanel);

    drawLabel(window, font, "Save / Load Portfolio", 30, 108, 14, ACCENT_PRIMARY);
    drawLabel(window, font, "File name:", 30, 135, 12, TEXT_SECONDARY);
    saveNameBox->render(window);
    saveButton->render(window);
    loadButton->render(window);

    // Cash management panel
    sf::RectangleShape cashPanel;
    cashPanel.setPosition(390, 100);
    cashPanel.setSize(sf::Vector2f(220, 200));
    cashPanel.setFillColor(BG_PANEL);
    cashPanel.setOutlineThickness(1);
    cashPanel.setOutlineColor(BORDER_DEFAULT);
    window.draw(cashPanel);

    drawLabel(window, font, "Cash Management", 400, 108, 14, ACCENT_PRIMARY);
    std::ostringstream bal;
    bal << "Current: $" << std::fixed << std::setprecision(2) << currentInvestor->getBalance();
    drawLabel(window, font, bal.str(), 400, 135, 12, TEXT_SECONDARY);
    cashBox->render(window);
    addCashButton->render(window);

    // Save files list
    sf::RectangleShape filesPanel;
    filesPanel.setPosition(20, 310);
    filesPanel.setSize(sf::Vector2f(350, 260));
    filesPanel.setFillColor(BG_PANEL);
    filesPanel.setOutlineThickness(1);
    filesPanel.setOutlineColor(BORDER_DEFAULT);
    window.draw(filesPanel);

    drawLabel(window, font, "Saved Files (click to select)", 30, 318, 13, ACCENT_PRIMARY);

    float fy = 345;
    for (size_t i = 0; i < saveFiles.size() && i < 8; ++i) {
        sf::RectangleShape row;
        row.setPosition(30, fy);
        row.setSize(sf::Vector2f(330, 25));
        row.setFillColor(static_cast<int>(i) == selectedSaveIndex ? BG_HOVER : BG_CARD);
        row.setOutlineThickness(0.5f);
        row.setOutlineColor(BORDER_DEFAULT);
        window.draw(row);

        drawLabel(window, font, saveFiles[i], 40, fy + 4, 11, TEXT_PRIMARY);
        fy += 28;
    }
    if (saveFiles.empty()) {
        drawLabel(window, font, "No save files found.", 120, 420, 12, TEXT_MUTED);
    }

    // About panel
    sf::RectangleShape aboutPanel;
    aboutPanel.setPosition(390, 310);
    aboutPanel.setSize(sf::Vector2f(390, 260));
    aboutPanel.setFillColor(BG_PANEL);
    aboutPanel.setOutlineThickness(1);
    aboutPanel.setOutlineColor(BORDER_DEFAULT);
    window.draw(aboutPanel);

    drawLabel(window, font, "About ProTrack", 400, 318, 14, ACCENT_PRIMARY);
    drawLabel(window, font, "ProTrack v2.0 - SFML Edition", 400, 345, 12, TEXT_SECONDARY);
    drawLabel(window, font, "OOP Course Project - IBA Karachi", 400, 365, 11, TEXT_MUTED);
    drawLabel(window, font, "Features:", 400, 395, 11, ACCENT_PRIMARY);
    drawLabel(window, font, "- 10 diversified mutual funds", 400, 415, 10, TEXT_SECONDARY);
    drawLabel(window, font, "- Real-time P&L tracking", 400, 432, 10, TEXT_SECONDARY);
    drawLabel(window, font, "- Market simulation engine", 400, 449, 10, TEXT_SECONDARY);
    drawLabel(window, font, "- Portfolio save/load system", 400, 466, 10, TEXT_SECONDARY);
    drawLabel(window, font, "- Interactive line & donut charts", 400, 483, 10, TEXT_SECONDARY);
    drawLabel(window, font, "- Buy/Sell transaction engine", 400, 500, 10, TEXT_SECONDARY);

    backButton->render(window);
}

void SettingsScreen::onEnter() {
    refreshSaveList();
    selectedSaveIndex = -1;
}

} // namespace ProTrack
