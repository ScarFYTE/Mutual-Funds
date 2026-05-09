#include "PortfolioScreen.h"
#include "../Core/Investor.h"
#include "../Core/FundManager.h"
#include "../Core/MutualFund.h"
#include "../Utils/ColorScheme.h"
#include <sstream>
#include <iomanip>

namespace ProTrack {

PortfolioScreen::PortfolioScreen(sf::Font* font)
    : Screen(font, "Portfolio", AppScreen::PORTFOLIO), selectedFundID(-1), scrollOffset(0) {

    backButton = new UI::Button(20, 15, 80, 30, "< Back", font, BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);
    backButton->setTextSize(12);
    backButton->setOnClick([]() { nextScreen = AppScreen::DASHBOARD; });

    sellButton = new UI::Button(680, 15, 100, 30, "Sell", font, BTN_DANGER, BTN_DANGER_H, BTN_DANGER);
    sellButton->setTextSize(12);
    sellButton->setOnClick([this]() {
        if (!currentInvestor || !fundManager) return;
        if (selectedFundID < 0) {
            notification = "Select a holding to sell!";
            notificationTimer = 2.0f;
            return;
        }
        std::string unitsStr = unitsBox->getText();
        if (unitsStr.empty()) {
            notification = "Enter units to sell!";
            notificationTimer = 2.0f;
            return;
        }
        try {
            double units = std::stod(unitsStr);
            if (currentInvestor->sellFund(selectedFundID, units, *fundManager)) {
                notification = "Sale successful!";
                notificationTimer = 2.0f;
                unitsBox->clear();
                selectedFundID = -1;
            } else {
                notification = "Sale failed! Check holdings.";
                notificationTimer = 2.0f;
            }
        } catch (...) {
            notification = "Invalid units!";
            notificationTimer = 2.0f;
        }
    });

    unitsBox = new UI::TextBox(520, 15, 150, 30, font, "Units to sell");
}

PortfolioScreen::~PortfolioScreen() {
    delete backButton; delete sellButton; delete unitsBox;
}

void PortfolioScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    backButton->handleEvent(event, window);
    sellButton->handleEvent(event, window);
    unitsBox->handleEvent(event, window);

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (mousePos.x >= 20 && mousePos.x <= 780 && currentInvestor) {
            const auto& invs = currentInvestor->getPortfolio().getInvestments();
            for (size_t i = scrollOffset; i < invs.size() && i < scrollOffset + 8; ++i) {
                float rowY = 120 + (i - scrollOffset) * 55;
                if (mousePos.y >= rowY && mousePos.y <= rowY + 50) {
                    selectedFundID = invs[i].getFundID();
                    break;
                }
            }
        }
    }

    if (event.type == sf::Event::MouseWheelScrolled && currentInvestor) {
        const auto& invs = currentInvestor->getPortfolio().getInvestments();
        if (event.mouseWheelScroll.delta > 0 && scrollOffset > 0) scrollOffset--;
        if (event.mouseWheelScroll.delta < 0 && scrollOffset + 8 < invs.size()) scrollOffset++;
    }
}

void PortfolioScreen::update(float dt, const sf::RenderWindow& window) {
    backButton->update(window);
    sellButton->update(window);
    unitsBox->update(dt);
}

void PortfolioScreen::render(sf::RenderWindow& window) {
    if (!currentInvestor || !fundManager) return;

    drawLabel(window, font, "My Portfolio", 20, 60, 20, ACCENT_PRIMARY);

    // Summary bar
    double portfolioVal = currentInvestor->getPortfolio().calculateValue(*fundManager);
    double cash = currentInvestor->getBalance();
    double totalPL = currentInvestor->getPortfolio().calculateTotalProfitLoss(*fundManager);
    double totalRet = currentInvestor->getPortfolio().calculateTotalReturnPercent(*fundManager);

    std::ostringstream summary;
    summary << "Portfolio Value: $" << std::fixed << std::setprecision(2) << portfolioVal
            << "  |  Cash: $" << cash
            << "  |  Total P&L: $" << totalPL
            << "  |  Return: " << std::setprecision(2) << totalRet << "%";
    drawLabel(window, font, summary.str(), 20, 95, 12, TEXT_SECONDARY);

    // Column headers
    sf::RectangleShape header;
    header.setPosition(20, 115);
    header.setSize(sf::Vector2f(760, 22));
    header.setFillColor(BG_PANEL);
    window.draw(header);

    drawLabel(window, font, "Fund ID", 30, 117, 11, TEXT_MUTED);
    drawLabel(window, font, "Fund Name", 100, 117, 11, TEXT_MUTED);
    drawLabel(window, font, "Units", 350, 117, 11, TEXT_MUTED);
    drawLabel(window, font, "Purchase Price", 440, 117, 11, TEXT_MUTED);
    drawLabel(window, font, "Current NAV", 550, 117, 11, TEXT_MUTED);
    drawLabel(window, font, "P&L", 650, 117, 11, TEXT_MUTED);
    drawLabel(window, font, "P&L %", 730, 117, 11, TEXT_MUTED);

    const auto& investments = currentInvestor->getPortfolio().getInvestments();

    if (investments.empty()) {
        drawLabel(window, font, "No holdings yet. Go to Market to buy funds!",
                  250, 300, 14, TEXT_MUTED);
    } else {
        for (size_t i = scrollOffset; i < investments.size() && i < scrollOffset + 8; ++i) {
            const auto& inv = investments[i];
            const MutualFund* fund = fundManager->getFundByID(inv.getFundID());
            if (!fund) continue;

            float rowY = 140 + (i - scrollOffset) * 55;
            bool isSelected = (inv.getFundID() == selectedFundID);

            sf::RectangleShape row;
            row.setPosition(20, rowY);
            row.setSize(sf::Vector2f(760, 50));
            row.setFillColor(isSelected ? BG_HOVER : BG_CARD);
            row.setOutlineThickness(0.5f);
            row.setOutlineColor(BORDER_DEFAULT);
            window.draw(row);

            drawLabel(window, font, std::to_string(inv.getFundID()), 30, rowY + 5, 12, TEXT_MUTED);
            drawLabel(window, font, fund->getFundName(), 100, rowY + 5, 13, TEXT_PRIMARY);

            std::ostringstream u; u << std::fixed << std::setprecision(2) << inv.getUnits();
            drawLabel(window, font, u.str(), 350, rowY + 5, 12, TEXT_PRIMARY);

            std::ostringstream pp; pp << "$" << std::fixed << std::setprecision(2) << inv.getPurchasePrice();
            drawLabel(window, font, pp.str(), 440, rowY + 5, 12, TEXT_SECONDARY);

            std::ostringstream cn; cn << "$" << std::fixed << std::setprecision(2) << fund->getNAV();
            drawLabel(window, font, cn.str(), 550, rowY + 5, 12, TEXT_PRIMARY);

            double pl = inv.getProfitLoss(fund->getNAV());
            std::ostringstream plStr; plStr << "$" << std::fixed << std::setprecision(2) << pl;
            drawLabel(window, font, plStr.str(), 650, rowY + 5, 12, getProfitLossColor(pl));

            double plPct = inv.getProfitLossPercent(fund->getNAV());
            std::ostringstream pctStr; pctStr << std::fixed << std::setprecision(1) << plPct << "%";
            drawLabel(window, font, pctStr.str(), 730, rowY + 5, 12, getProfitLossColor(plPct));
        }
    }

    if (investments.size() > 8) {
        std::ostringstream si;
        si << "Showing " << scrollOffset + 1 << "-"
           << std::min(scrollOffset + 8, static_cast<int>(investments.size()))
           << " of " << investments.size();
        drawLabel(window, font, si.str(), 640, 580, 11, TEXT_MUTED);
    }

    backButton->render(window);
    sellButton->render(window);
    unitsBox->render(window);
}

void PortfolioScreen::onEnter() {
    selectedFundID = -1;
    scrollOffset = 0;
}

} // namespace ProTrack
