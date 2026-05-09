#include "TradeScreen.h"
#include "../Core/Investor.h"
#include "../Core/FundManager.h"
#include "../Utils/ColorScheme.h"
#include "../Utils/Panel.h"
#include <sstream>
#include <iomanip>

namespace ProTrack {
using namespace UI;

TradeScreen::TradeScreen(sf::Font* font)
    : Screen(font, "Trade", AppScreen::TRADE), isBuyMode(true) {

    backButton = new UI::Button(20, 15, 80, 30, "< Back", font, BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);
    backButton->setTextSize(12);
    backButton->setOnClick([]() { nextScreen = AppScreen::DASHBOARD; });

    fundIdBox = new UI::TextBox(280, 150, 240, 36, font, "Enter Fund ID");
    amountBox = new UI::TextBox(280, 200, 240, 36, font, "Enter Amount ($)");
    unitsBox = new UI::TextBox(280, 200, 240, 36, font, "Enter Units");
    unitsBox->setVisible(false);

    buyButton = new UI::Button(280, 260, 115, 36, "Buy (Amount)", font, BTN_SUCCESS, BTN_SUCCESS_H, BTN_SUCCESS);
    buyButton->setTextSize(12);
    buyButton->setOnClick([this]() {
        if (!currentInvestor || !fundManager) return;
        try {
            int fid = std::stoi(fundIdBox->getText());
            if (isBuyMode) {
                double amt = std::stod(amountBox->getText());
                if (currentInvestor->buyFund(fid, amt, *fundManager)) {
                    notification = "Buy order executed!";
                    notificationTimer = 2.0f;
                    fundIdBox->clear(); amountBox->clear();
                } else {
                    notification = "Buy failed! Check balance/fund ID.";
                    notificationTimer = 2.0f;
                }
            } else {
                double units = std::stod(unitsBox->getText());
                if (currentInvestor->sellFund(fid, units, *fundManager)) {
                    notification = "Sell order executed!";
                    notificationTimer = 2.0f;
                    fundIdBox->clear(); unitsBox->clear();
                } else {
                    notification = "Sell failed! Check holdings.";
                    notificationTimer = 2.0f;
                }
            }
        } catch (...) {
            notification = "Invalid input!";
            notificationTimer = 2.0f;
        }
    });
}

TradeScreen::~TradeScreen() {
    delete backButton; delete buyButton;
    delete fundIdBox; delete amountBox; delete unitsBox;
}

void TradeScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    backButton->handleEvent(event, window);
    buyButton->handleEvent(event, window);
    fundIdBox->handleEvent(event, window);
    amountBox->handleEvent(event, window);
    unitsBox->handleEvent(event, window);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
        isBuyMode = !isBuyMode;
        amountBox->setVisible(isBuyMode);
        unitsBox->setVisible(!isBuyMode);
        buyButton->setText(isBuyMode ? "Buy (Amount)" : "Sell (Units)");
        buyButton->setColors(isBuyMode ? BTN_SUCCESS : BTN_DANGER,
                             isBuyMode ? BTN_SUCCESS_H : BTN_DANGER_H,
                             isBuyMode ? BTN_SUCCESS : BTN_DANGER);
    }
}

void TradeScreen::update(float dt, const sf::RenderWindow& window) {
    backButton->update(window);
    buyButton->update(window);
    fundIdBox->update(dt);
    amountBox->update(dt);
    unitsBox->update(dt);
}

void TradeScreen::render(sf::RenderWindow& window) {
    if (!currentInvestor || !fundManager) return;

    drawLabel(window, font, "Trade Desk", 20, 60, 20, ACCENT_PRIMARY);

    // Mode indicator
    std::string modeStr = isBuyMode ? "BUY MODE (Press Tab to switch to Sell)" : "SELL MODE (Press Tab to switch to Buy)";
    drawLabel(window, font, modeStr, 280, 120, 12, isBuyMode ? ACCENT_SUCCESS : ACCENT_DANGER);

    // Instructions
    drawLabel(window, font, "Fund ID:", 180, 157, 13, TEXT_SECONDARY);
    drawLabel(window, font, isBuyMode ? "Amount $:" : "Units:", 180, 207, 13, TEXT_SECONDARY);

    fundIdBox->render(window);
    amountBox->render(window);
    unitsBox->render(window);

    // Quick fund reference
    sf::RectangleShape refPanel;
    refPanel.setPosition(540, 120);
    refPanel.setSize(sf::Vector2f(240, 280));
    refPanel.setFillColor(BG_PANEL);
    refPanel.setOutlineThickness(1);
    refPanel.setOutlineColor(BORDER_DEFAULT);
    window.draw(refPanel);

    drawLabel(window, font, "Available Funds", 550, 128, 12, ACCENT_PRIMARY);

    float fy = 150;
    int count = 0;
    for (const auto& fundPtr : *fundManager) {
        if (count >= 8) break;
        std::ostringstream line;
        line << fundPtr->getFundID() << ": " << fundPtr->getFundName()
             << " ($" << std::fixed << std::setprecision(2) << fundPtr->getNAV() << ")";
        drawLabel(window, font, line.str(), 550, fy, 10, TEXT_SECONDARY);
        fy += 18;
        count++;
    }

    // Cash balance display
    std::ostringstream bal;
    bal << "Available Cash: $" << std::fixed << std::setprecision(2) << currentInvestor->getBalance();
    drawLabel(window, font, bal.str(), 280, 310, 13, TEXT_SECONDARY);

    // Current holdings
    std::ostringstream holds;
    holds << "Holdings: " << currentInvestor->getPortfolio().getNumHoldings() << " funds";
    drawLabel(window, font, holds.str(), 280, 330, 13, TEXT_SECONDARY);

    buyButton->render(window);
    backButton->render(window);
}

void TradeScreen::onEnter() {
    isBuyMode = true;
    amountBox->setVisible(true);
    unitsBox->setVisible(false);
    buyButton->setText("Buy (Amount)");
    buyButton->setColors(BTN_SUCCESS, BTN_SUCCESS_H, BTN_SUCCESS);
}

} // namespace ProTrack
