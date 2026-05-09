#include "MarketScreen.h"
#include "../Core/Investor.h"
#include "../Core/FundManager.h"
#include "../Core/MutualFund.h"
#include "../Utils/ColorScheme.h"
#include "../Utils/Panel.h"
#include <sstream>
#include <iomanip>

namespace ProTrack {
using namespace UI;

MarketScreen::MarketScreen(sf::Font* font)
    : Screen(font, "Market", AppScreen::MARKET), selectedFundID(-1), scrollOffset(0) {

    backButton = new UI::Button(20, 15, 80, 30, "< Back", font, BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);
    backButton->setTextSize(12);
    backButton->setOnClick([]() { nextScreen = AppScreen::DASHBOARD; });

    refreshButton = new UI::Button(110, 15, 90, 30, "Refresh", font, BTN_PRIMARY, BTN_PRIMARY_H, BTN_PRIMARY);
    refreshButton->setTextSize(12);
    refreshButton->setOnClick([this]() {
        searchQuery = "";
        searchBox->clear();
    });

    searchBox = new UI::TextBox(210, 15, 280, 30, font, "Search funds...");

    buyButton = new UI::Button(680, 15, 100, 30, "Buy", font, BTN_SUCCESS, BTN_SUCCESS_H, BTN_SUCCESS);
    buyButton->setTextSize(12);
    buyButton->setOnClick([this]() {
        if (!currentInvestor || !fundManager) return;
        if (selectedFundID < 0) {
            notification = "Select a fund first!";
            notificationTimer = 2.0f;
            return;
        }
        std::string amtStr = amountBox->getText();
        if (amtStr.empty()) {
            notification = "Enter an amount!";
            notificationTimer = 2.0f;
            return;
        }
        try {
            double amount = std::stod(amtStr);
            if (currentInvestor->buyFund(selectedFundID, amount, *fundManager)) {
                notification = "Purchase successful!";
                notificationTimer = 2.0f;
                amountBox->clear();
            } else {
                notification = "Purchase failed! Check balance/fund ID.";
                notificationTimer = 2.0f;
            }
        } catch (...) {
            notification = "Invalid amount!";
            notificationTimer = 2.0f;
        }
    });

    amountBox = new UI::TextBox(520, 15, 150, 30, font, "Amount ($)");
}

MarketScreen::~MarketScreen() {
    delete backButton; delete buyButton; delete refreshButton;
    delete searchBox; delete amountBox;
    for (auto* b : filterButtons) delete b;
}

std::vector<const MutualFund*> MarketScreen::getFilteredFunds() {
    if (searchQuery.empty()) {
        return fundManager->getAllFunds();
    }
    return fundManager->searchByName(searchQuery);
}

void MarketScreen::renderFundRow(sf::RenderWindow& window, const MutualFund* fund, float y, bool isSelected) {
    if (!fund) return;

    // Row background
    sf::RectangleShape row;
    row.setPosition(20, y);
    row.setSize(sf::Vector2f(760, 50));
    row.setFillColor(isSelected ? BG_HOVER : BG_CARD);
    row.setOutlineThickness(0.5f);
    row.setOutlineColor(BORDER_DEFAULT);
    window.draw(row);

    // Fund ID
    drawLabel(window, font, std::to_string(fund->getFundID()), 30, y + 5, 12, TEXT_MUTED);

    // Fund Name
    drawLabel(window, font, fund->getFundName(), 80, y + 5, 14, TEXT_PRIMARY);

    // Category
    drawLabel(window, font, categoryToString(fund->getCategory()), 80, y + 28, 10, TEXT_SECONDARY);

    // NAV
    std::ostringstream navStr;
    navStr << "$" << std::fixed << std::setprecision(2) << fund->getNAV();
    drawLabel(window, font, navStr.str(), 380, y + 5, 14, TEXT_PRIMARY);

    // Change %
    double change = fund->getDailyChangePercent();
    std::ostringstream chgStr;
    chgStr << (change >= 0 ? "+" : "") << std::fixed << std::setprecision(2) << change << "%";
    drawLabel(window, font, chgStr.str(), 490, y + 5, 14, getProfitLossColor(change));

    // Risk
    drawLabel(window, font, riskToString(fund->getRiskLevel()), 580, y + 5, 11, TEXT_SECONDARY);

    // AUM
    std::ostringstream aumStr;
    aumStr << "$" << std::fixed << std::setprecision(0) << fund->getAUM() << "M";
    drawLabel(window, font, aumStr.str(), 660, y + 5, 11, TEXT_SECONDARY);
}

void MarketScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    backButton->handleEvent(event, window);
    buyButton->handleEvent(event, window);
    refreshButton->handleEvent(event, window);
    searchBox->handleEvent(event, window);
    amountBox->handleEvent(event, window);

    if (event.type == sf::Event::TextEntered) {
        searchQuery = searchBox->getText();
        scrollOffset = 0;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (mousePos.x >= 20 && mousePos.x <= 780) {
            auto funds = getFilteredFunds();
            for (size_t i = scrollOffset; i < funds.size() && i < scrollOffset + 8; ++i) {
                float rowY = 120 + (i - scrollOffset) * 55;
                if (mousePos.y >= rowY && mousePos.y <= rowY + 50) {
                    selectedFundID = funds[i]->getFundID();
                    break;
                }
            }
        }
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        auto funds = getFilteredFunds();
        if (event.mouseWheelScroll.delta > 0 && scrollOffset > 0) scrollOffset--;
        if (event.mouseWheelScroll.delta < 0 && scrollOffset + 8 < funds.size()) scrollOffset++;
    }
}

void MarketScreen::update(float dt, const sf::RenderWindow& window) {
    backButton->update(window);
    buyButton->update(window);
    refreshButton->update(window);
    searchBox->update(dt);
    amountBox->update(dt);
}

void MarketScreen::render(sf::RenderWindow& window) {
    if (!fundManager) return;

    // Title
    drawLabel(window, font, "Market Funds", 20, 60, 20, ACCENT_PRIMARY);

    // Column headers
    sf::RectangleShape header;
    header.setPosition(20, 95);
    header.setSize(sf::Vector2f(760, 22));
    header.setFillColor(BG_PANEL);
    window.draw(header);

    drawLabel(window, font, "ID", 30, 97, 11, TEXT_MUTED);
    drawLabel(window, font, "Fund Name / Category", 80, 97, 11, TEXT_MUTED);
    drawLabel(window, font, "NAV", 380, 97, 11, TEXT_MUTED);
    drawLabel(window, font, "Change", 490, 97, 11, TEXT_MUTED);
    drawLabel(window, font, "Risk", 580, 97, 11, TEXT_MUTED);
    drawLabel(window, font, "AUM", 660, 97, 11, TEXT_MUTED);

    // Fund rows
    auto funds = getFilteredFunds();
    for (size_t i = scrollOffset; i < funds.size() && i < scrollOffset + 8; ++i) {
        bool selected = (funds[i]->getFundID() == selectedFundID);
        renderFundRow(window, funds[i], 120 + (i - scrollOffset) * 55, selected);
    }

    if (funds.empty()) {
        drawLabel(window, font, "No funds found matching your search.", 280, 300, 14, TEXT_MUTED);
    }

    // Scroll indicator
    if (funds.size() > 8) {
        std::ostringstream scrollInfo;
        scrollInfo << "Showing " << scrollOffset + 1 << "-"
                   << std::min(scrollOffset + 8, static_cast<int>(funds.size()))
                   << " of " << funds.size();
        drawLabel(window, font, scrollInfo.str(), 640, 570, 11, TEXT_MUTED);
    }

    backButton->render(window);
    refreshButton->render(window);
    searchBox->render(window);
    buyButton->render(window);
    amountBox->render(window);
}

void MarketScreen::onEnter() {
    selectedFundID = -1;
    scrollOffset = 0;
}

} // namespace ProTrack
