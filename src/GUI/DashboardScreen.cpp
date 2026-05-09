#include "DashboardScreen.h"
#include "../Core/Investor.h"
#include "../Core/FundManager.h"
#include "../Utils/ColorScheme.h"
#include <sstream>
#include <iomanip>

namespace ProTrack {

DashboardScreen::DashboardScreen(sf::Font* font)
    : Screen(font, "Dashboard", AppScreen::DASHBOARD), dayCounter(1) {
    welcomeText.setFont(*font);
    welcomeText.setCharacterSize(20);
    welcomeText.setFillColor(TEXT_PRIMARY);

    dayText.setFont(*font);
    dayText.setString("Market Day: 1");
    dayText.setCharacterSize(14);
    dayText.setFillColor(TEXT_SECONDARY);
    dayText.setPosition(680, 15);

    // Navigation buttons (left sidebar)
    navMarket = new UI::Button(20, 80, 140, 36, "Market", font, BTN_PRIMARY, BTN_PRIMARY_H, BTN_PRIMARY);
    navPortfolio = new UI::Button(20, 126, 140, 36, "Portfolio", font, BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);
    navTrade = new UI::Button(20, 172, 140, 36, "Trade", font, BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);
    navPerformance = new UI::Button(20, 218, 140, 36, "Performance", font, BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);
    navSettings = new UI::Button(20, 264, 140, 36, "Settings", font, BTN_SECONDARY, BTN_SECONDARY_H, BTN_SECONDARY);

    navMarket->setOnClick([]() { nextScreen = AppScreen::MARKET; });
    navPortfolio->setOnClick([]() { nextScreen = AppScreen::PORTFOLIO; });
    navTrade->setOnClick([]() { nextScreen = AppScreen::TRADE; });
    navPerformance->setOnClick([]() { nextScreen = AppScreen::PERFORMANCE; });
    navSettings->setOnClick([]() { nextScreen = AppScreen::SETTINGS; });

    // Simulate market day button
    simulateButton = new UI::Button(20, 320, 140, 36, "Next Day >>", font, BTN_SUCCESS, BTN_SUCCESS_H, BTN_SUCCESS);
    simulateButton->setTextSize(14);
    simulateButton->setOnClick([this]() {
        fundManager->simulateMarketDay();
        dayCounter++;
        dayText.setString("Market Day: " + std::to_string(dayCounter));
        notification = "Market simulated! Prices updated.";
        notificationTimer = 2.0f;
    });

    miniChart = new UI::ChartRenderer(420, 250, 360, 200, font);
}

DashboardScreen::~DashboardScreen() {
    delete navMarket; delete navPortfolio; delete navTrade;
    delete navPerformance; delete navSettings; delete simulateButton;
    delete miniChart;
}

void DashboardScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    navMarket->handleEvent(event, window);
    navPortfolio->handleEvent(event, window);
    navTrade->handleEvent(event, window);
    navPerformance->handleEvent(event, window);
    navSettings->handleEvent(event, window);
    simulateButton->handleEvent(event, window);
}

void DashboardScreen::update(float dt, const sf::RenderWindow& window) {
    navMarket->update(window);
    navPortfolio->update(window);
    navTrade->update(window);
    navPerformance->update(window);
    navSettings->update(window);
    simulateButton->update(window);
}

void DashboardScreen::render(sf::RenderWindow& window) {
    if (!currentInvestor || !fundManager) return;

    // Welcome header
    std::string welcome = "Welcome, " + currentInvestor->getName();
    welcomeText.setString(welcome);
    welcomeText.setPosition(180, 15);
    window.draw(welcomeText);
    window.draw(dayText);

    double portfolioVal = currentInvestor->getPortfolio().calculateValue(*fundManager);
    double cash = currentInvestor->getBalance();
    double netWorth = cash + portfolioVal;
    double pnl = currentInvestor->getPortfolio().calculateTotalProfitLoss(*fundManager);
    double pnlPct = currentInvestor->getPortfolio().calculateTotalReturnPercent(*fundManager);
    size_t holdings = currentInvestor->getPortfolio().getNumHoldings();
    size_t snaps = currentInvestor->getPortfolio().getHistory().size();

    // Metric cards (top row)
    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2) << netWorth;
    drawMetricCard(window, font, "Net Worth", oss.str(), 180, 60, 185, 70);

    oss.str(""); oss << "$" << std::fixed << std::setprecision(2) << portfolioVal;
    drawMetricCard(window, font, "Portfolio Value", oss.str(), 375, 60, 185, 70);

    oss.str(""); oss << "$" << std::fixed << std::setprecision(2) << cash;
    drawMetricCard(window, font, "Cash Balance", oss.str(), 570, 60, 185, 70);

    oss.str(""); oss << "$" << std::fixed << std::setprecision(2) << pnl;
    sf::Color pnlColor = getProfitLossColor(pnl);
    drawMetricCard(window, font, "Total P&L", oss.str(), 180, 140, 185, 70, pnlColor);

    oss.str(""); oss << std::fixed << std::setprecision(2) << pnlPct << "%";
    drawMetricCard(window, font, "Return %", oss.str(), 375, 140, 185, 70, pnlColor);

    oss.str(""); oss << holdings << " funds";
    drawMetricCard(window, font, "Holdings", oss.str(), 570, 140, 185, 70);

    // Sidebar panel background
    sf::RectangleShape sidebar;
    sidebar.setPosition(10, 50);
    sidebar.setSize(sf::Vector2f(160, 530));
    sidebar.setFillColor(BG_PANEL);
    sidebar.setOutlineThickness(1);
    sidebar.setOutlineColor(BORDER_DEFAULT);
    window.draw(sidebar);

    // Mini portfolio chart
    if (snaps >= 2) {
        miniChart->renderLineChart(window, currentInvestor->getPortfolio().getHistory(), "Portfolio Trend");
    } else {
        sf::RectangleShape chartBg;
        chartBg.setPosition(420, 250);
        chartBg.setSize(sf::Vector2f(360, 200));
        chartBg.setFillColor(BG_PANEL);
        chartBg.setOutlineThickness(1);
        chartBg.setOutlineColor(BORDER_DEFAULT);
        window.draw(chartBg);
        drawLabel(window, font, "Make transactions to see your portfolio trend!",
                  480, 340, 14, TEXT_MUTED);
    }

    // Quick stats panel
    sf::RectangleShape statsPanel;
    statsPanel.setPosition(180, 250);
    statsPanel.setSize(sf::Vector2f(230, 200));
    statsPanel.setFillColor(BG_PANEL);
    statsPanel.setOutlineThickness(1);
    statsPanel.setOutlineColor(BORDER_DEFAULT);
    window.draw(statsPanel);

    drawLabel(window, font, "Quick Stats", 190, 258, 14, ACCENT_PRIMARY);

    // Market stats
    double totalCap = fundManager->getTotalMarketCap();
    size_t numFunds = fundManager->getNumFunds();
    auto topGainer = fundManager->getTopGainers();
    auto topLoser = fundManager->getTopLosers();

    std::ostringstream stats;
    stats << "Market Funds: " << numFunds << "\n\n";
    stats << "Total Market Cap:\n$" << std::fixed << std::setprecision(0) << totalCap << "M\n\n";
    if (!topGainer.empty()) {
        stats << "Top Gainer:\n" << topGainer[0]->getFundName() << "\n+" << std::setprecision(2) << topGainer[0]->getDailyChangePercent() << "%\n\n";
    }
    if (!topLoser.empty()) {
        stats << "Top Loser:\n" << topLoser[0]->getFundName() << "\n" << std::setprecision(2) << topLoser[0]->getDailyChangePercent() << "%\n";
    }

    sf::Text statsText;
    statsText.setFont(*font);
    statsText.setString(stats.str());
    statsText.setCharacterSize(11);
    statsText.setFillColor(TEXT_SECONDARY);
    statsText.setPosition(190, 285);
    window.draw(statsText);

    // Recent transactions
    sf::RectangleShape txnPanel;
    txnPanel.setPosition(180, 460);
    txnPanel.setSize(sf::Vector2f(600, 120));
    txnPanel.setFillColor(BG_PANEL);
    txnPanel.setOutlineThickness(1);
    txnPanel.setOutlineColor(BORDER_DEFAULT);
    window.draw(txnPanel);

    drawLabel(window, font, "Recent Transactions", 190, 468, 14, ACCENT_PRIMARY);

    const auto& txns = currentInvestor->getPortfolio().getTransactions();
    int yOffset = 490;
    int count = 0;
    for (auto it = txns.rbegin(); it != txns.rend() && count < 4; ++it, ++count) {
        std::ostringstream line;
        line << it->getTypeString() << " | Fund #" << it->getFundID()
             << " | " << std::fixed << std::setprecision(2) << it->getUnits() << " units"
             << " | $" << std::setprecision(2) << it->getTotalAmount();
        sf::Color c = (it->getType() == TransactionType::BUY) ? ACCENT_SUCCESS : ACCENT_DANGER;
        drawLabel(window, font, line.str(), 190, static_cast<float>(yOffset), 11, c);
        yOffset += 18;
    }
    if (txns.empty()) {
        drawLabel(window, font, "No transactions yet.", 190, 510, 12, TEXT_MUTED);
    }

    // Render navigation buttons
    navMarket->render(window);
    navPortfolio->render(window);
    navTrade->render(window);
    navPerformance->render(window);
    navSettings->render(window);
    simulateButton->render(window);
}

void DashboardScreen::onEnter() {
    if (currentInvestor) {
        std::string welcome = "Welcome, " + currentInvestor->getName();
        welcomeText.setString(welcome);
    }
}

} // namespace ProTrack
