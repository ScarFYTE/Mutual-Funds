#include "MutualFund.h"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace ProTrack {

// Constructor with member initialization list
MutualFund::MutualFund(int id, const std::string& name, double initialNav,
                       FundCategory cat, RiskLevel r,
                       const std::string& desc, double expense, double aumMillions)
    : fundID(id)
    , fundName(name)
    , nav(initialNav)
    , previousNav(initialNav)
    , category(cat)
    , risk(r)
    , description(desc)
    , expenseRatio(expense)
    , aum(aumMillions)
    , lastUpdate(std::time(nullptr))
{
    // nav must never go below floor price
    if (nav < 0.01) nav = 0.01;
}

// ACCESSORS
int MutualFund::getFundID() const { return fundID; }
std::string MutualFund::getFundName() const { return fundName; }
double MutualFund::getNAV() const { return nav; }
double MutualFund::getPreviousNAV() const { return previousNav; }
FundCategory MutualFund::getCategory() const { return category; }
RiskLevel MutualFund::getRiskLevel() const { return risk; }
std::string MutualFund::getDescription() const { return description; }
double MutualFund::getExpenseRatio() const { return expenseRatio; }
double MutualFund::getAUM() const { return aum; }
time_t MutualFund::getLastUpdate() const { return lastUpdate; }

// MUTATORS
void MutualFund::setNAV(double newNav) {
    previousNav = nav;
    nav = (newNav < 0.01) ? 0.01 : newNav;
    lastUpdate = std::time(nullptr);
}

void MutualFund::setDescription(const std::string& desc) {
    description = desc;
}

// BUSINESS LOGIC
double MutualFund::getDailyChange() const {
    return nav - previousNav;
}

double MutualFund::getDailyChangePercent() const {
    if (previousNav == 0) return 0.0;
    return ((nav - previousNav) / previousNav) * 100.0;
}

void MutualFund::simulateMarketMovement() {
    // Store current NAV before changing
    previousNav = nav;

    // Simulate realistic market movement based on risk level
    double volatility = 0.0;
    switch (risk) {
        case RiskLevel::LOW:        volatility = 0.005;  break; // 0.5%
        case RiskLevel::MODERATE:   volatility = 0.015;  break; // 1.5%
        case RiskLevel::HIGH:       volatility = 0.030;  break; // 3.0%
        case RiskLevel::VERY_HIGH:  volatility = 0.050;  break; // 5.0%
    }

    // Random movement: -1 to +1 scaled by volatility
    double changePercent = ((rand() % 2001 - 1000) / 1000.0) * volatility;
    nav *= (1.0 + changePercent);

    // Floor price protection
    if (nav < 0.01) nav = 0.01;

    lastUpdate = std::time(nullptr);
}

std::string MutualFund::getInfoString() const {
    std::ostringstream oss;
    oss << "[" << fundID << "] " << fundName
        << " | NAV: $" << std::fixed << std::setprecision(2) << nav
        << " | Change: " << std::setprecision(2) << getDailyChangePercent() << "%"
        << " | Category: " << categoryToString(category)
        << " | Risk: " << riskToString(risk);
    return oss.str();
}

} // namespace ProTrack
