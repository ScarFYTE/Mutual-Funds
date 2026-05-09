#include "PerformanceScreen.h"
#include "../Core/Investor.h"
#include "../Core/FundManager.h"
#include "../Utils/ColorScheme.h"
#include <sstream>
#include <iomanip>

namespace ProTrack {

PerformanceScreen::PerformanceScreen(sf::Font* font)
    : Screen(font, "Performance", AppScreen::PERFORMANCE) {
    backButton = new UI::Button(20, 15, 80, 30, "< Back", font, BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);
    backButton->setTextSize(12);
    backButton->setOnClick([]() { nextScreen = AppScreen::DASHBOARD; });

    portfolioChart = new UI::ChartRenderer(20, 60, 480, 260, font);
    allocationChart = new UI::ChartRenderer(520, 60, 260, 260, font);
}

PerformanceScreen::~PerformanceScreen() {
    delete backButton; delete portfolioChart; delete allocationChart;
}

void PerformanceScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    backButton->handleEvent(event, window);
}

void PerformanceScreen::update(float dt, const sf::RenderWindow& window) {
    backButton->update(window);
}

void PerformanceScreen::render(sf::RenderWindow& window) {
    if (!currentInvestor || !fundManager) return;

    drawLabel(window, font, "Performance Analytics", 20, 60, 20, ACCENT_PRIMARY);

    // Portfolio value line chart
    const auto& history = currentInvestor->getPortfolio().getHistory();
    portfolioChart->renderLineChart(window, history, "Portfolio Value Over Time");

    // Holdings allocation donut chart
    std::vector<std::pair<std::string, double>> allocations;
    double totalVal = 0;
    for (const auto& inv : currentInvestor->getPortfolio().getInvestments()) {
        const MutualFund* fund = fundManager->getFundByID(inv.getFundID());
        if (fund) {
            double val = inv.getCurrentValue(fund->getNAV());
            allocations.push_back({fund->getFundName(), val});
            totalVal += val;
        }
    }

    // Allocation chart panel
    sf::RectangleShape allocBg;
    allocBg.setPosition(520, 60);
    allocBg.setSize(sf::Vector2f(260, 260));
    allocBg.setFillColor(BG_PANEL);
    allocBg.setOutlineThickness(1);
    allocBg.setOutlineColor(BORDER_DEFAULT);
    window.draw(allocBg);

    drawLabel(window, font, "Allocation", 530, 68, 14, ACCENT_PRIMARY);

    if (!allocations.empty()) {
        allocationChart->renderAllocationChart(window, allocations);

        // Legend
        float ly = 330;
        sf::Color colors[] = {
            ACCENT_PRIMARY, ACCENT_SUCCESS, ACCENT_SECOND,
            ACCENT_WARNING, ACCENT_DANGER, lighten(ACCENT_PRIMARY, 40),
            lighten(ACCENT_SUCCESS, 40), sf::Color(139, 92, 246)
        };
        for (size_t i = 0; i < allocations.size() && i < 8; ++i) {
            sf::RectangleShape dot;
            dot.setPosition(530, ly);
            dot.setSize(sf::Vector2f(10, 10));
            dot.setFillColor(colors[i % 8]);
            window.draw(dot);

            std::ostringstream pct;
            pct << std::fixed << std::setprecision(1) << (allocations[i].second / totalVal * 100) << "%";
            drawLabel(window, font, allocations[i].first.substr(0, 20), 545, ly - 2, 10, TEXT_SECONDARY);
            drawLabel(window, font, pct.str(), 720, ly - 2, 10, TEXT_PRIMARY);
            ly += 18;
        }
    } else {
        drawLabel(window, font, "No holdings to display.", 570, 180, 12, TEXT_MUTED);
    }

    // Transaction history table
    sf::RectangleShape txnPanel;
    txnPanel.setPosition(20, 340);
    txnPanel.setSize(sf::Vector2f(760, 240));
    txnPanel.setFillColor(BG_PANEL);
    txnPanel.setOutlineThickness(1);
    txnPanel.setOutlineColor(BORDER_DEFAULT);
    window.draw(txnPanel);

    drawLabel(window, font, "Transaction History", 30, 348, 14, ACCENT_PRIMARY);

    // Column headers
    drawLabel(window, font, "Date/Time", 30, 370, 11, TEXT_MUTED);
    drawLabel(window, font, "Type", 220, 370, 11, TEXT_MUTED);
    drawLabel(window, font, "Fund ID", 280, 370, 11, TEXT_MUTED);
    drawLabel(window, font, "Units", 360, 370, 11, TEXT_MUTED);
    drawLabel(window, font, "Price", 440, 370, 11, TEXT_MUTED);
    drawLabel(window, font, "Total", 540, 370, 11, TEXT_MUTED);

    const auto& txns = currentInvestor->getPortfolio().getTransactions();
    float ty = 390;
    int txnCount = 0;
    for (auto it = txns.rbegin(); it != txns.rend() && txnCount < 8; ++it, ++txnCount) {
        drawLabel(window, font, it->getDateString(), 30, ty, 10, TEXT_SECONDARY);
        drawLabel(window, font, it->getTypeString(), 220, ty, 10,
                  it->getType() == TransactionType::BUY ? ACCENT_SUCCESS : ACCENT_DANGER);
        drawLabel(window, font, std::to_string(it->getFundID()), 280, ty, 10, TEXT_PRIMARY);

        std::ostringstream u; u << std::fixed << std::setprecision(2) << it->getUnits();
        drawLabel(window, font, u.str(), 360, ty, 10, TEXT_PRIMARY);

        std::ostringstream p; p << "$" << std::fixed << std::setprecision(2) << it->getPricePerUnit();
        drawLabel(window, font, p.str(), 440, ty, 10, TEXT_SECONDARY);

        std::ostringstream t; t << "$" << std::fixed << std::setprecision(2) << it->getTotalAmount();
        drawLabel(window, font, t.str(), 540, ty, 10, TEXT_PRIMARY);

        ty += 18;
    }
    if (txns.empty()) {
        drawLabel(window, font, "No transactions recorded.", 300, 450, 12, TEXT_MUTED);
    }

    backButton->render(window);
}

} // namespace ProTrack
