#include "FundManager.h"
#include <algorithm>
#include <cctype>

namespace ProTrack {

FundManager::FundManager() : nextFundID(1000) {}

void FundManager::addFund(MutualFund* fund) {
    if (fund) {
        funds.emplace_back(fund);
    }
}

void FundManager::removeFund(int fundID) {
    funds.erase(
        std::remove_if(funds.begin(), funds.end(),
            [fundID](const std::unique_ptr<MutualFund>& f) {
                return f->getFundID() == fundID;
            }),
        funds.end()
    );
}

void FundManager::clearFunds() {
    funds.clear();
}

const MutualFund* FundManager::getFundByID(int fundID) const {
    for (const auto& f : funds) {
        if (f->getFundID() == fundID) return f.get();
    }
    return nullptr;
}

MutualFund* FundManager::getFundByID(int fundID) {
    for (auto& f : funds) {
        if (f->getFundID() == fundID) return f.get();
    }
    return nullptr;
}

size_t FundManager::getNumFunds() const { return funds.size(); }

bool FundManager::hasFund(int fundID) const {
    return getFundByID(fundID) != nullptr;
}

std::vector<const MutualFund*> FundManager::searchByName(const std::string& query) const {
    std::vector<const MutualFund*> results;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    for (const auto& f : funds) {
        std::string lowerName = f->getFundName();
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        if (lowerName.find(lowerQuery) != std::string::npos) {
            results.push_back(f.get());
        }
    }
    return results;
}

std::vector<const MutualFund*> FundManager::filterByCategory(FundCategory cat) const {
    std::vector<const MutualFund*> results;
    for (const auto& f : funds) {
        if (f->getCategory() == cat) results.push_back(f.get());
    }
    return results;
}

std::vector<const MutualFund*> FundManager::filterByRisk(RiskLevel risk) const {
    std::vector<const MutualFund*> results;
    for (const auto& f : funds) {
        if (f->getRiskLevel() == risk) results.push_back(f.get());
    }
    return results;
}

std::vector<const MutualFund*> FundManager::getAllFunds() const {
    std::vector<const MutualFund*> result;
    for (const auto& f : funds) result.push_back(f.get());
    return result;
}

std::vector<const MutualFund*> FundManager::getTopGainers() const {
    auto result = getAllFunds();
    std::sort(result.begin(), result.end(),
        [](const MutualFund* a, const MutualFund* b) {
            return a->getDailyChangePercent() > b->getDailyChangePercent();
        });
    return result;
}

std::vector<const MutualFund*> FundManager::getTopLosers() const {
    auto result = getAllFunds();
    std::sort(result.begin(), result.end(),
        [](const MutualFund* a, const MutualFund* b) {
            return a->getDailyChangePercent() < b->getDailyChangePercent();
        });
    return result;
}

void FundManager::simulateMarketDay() {
    for (auto& f : funds) {
        f->simulateMarketMovement();
    }
}

double FundManager::getTotalMarketCap() const {
    double total = 0.0;
    for (const auto& f : funds) {
        total += f->getNAV() * f->getAUM();
    }
    return total;
}

FundManager::const_iterator FundManager::begin() const { return funds.begin(); }
FundManager::const_iterator FundManager::end() const { return funds.end(); }

void FundManager::loadDefaultFunds() {
    // Clear any existing funds first
    funds.clear();

    funds.emplace_back(std::make_unique<MutualFund>(
        101, "IBA Alpha Growth Fund", 125.50,
        FundCategory::EQUITY, RiskLevel::HIGH,
        "Large-cap equity fund focused on high-growth technology and healthcare sectors",
        1.25, 850.0));

    funds.emplace_back(std::make_unique<MutualFund>(
        102, "Karachi Tech ETF", 48.75,
        FundCategory::INDEX, RiskLevel::MODERATE,
        "Passively tracks the KSE-100 Technology Index",
        0.50, 420.0));

    funds.emplace_back(std::make_unique<MutualFund>(
        103, "Pakistan Sovereign Bond Fund", 15.20,
        FundCategory::DEBT, RiskLevel::LOW,
        "Invests in government bonds and treasury bills for stable returns",
        0.75, 1200.0));

    funds.emplace_back(std::make_unique<MutualFund>(
        104, "Global Emerging Markets Fund", 82.40,
        FundCategory::INTERNATIONAL, RiskLevel::VERY_HIGH,
        "Exposure to BRIC economies and frontier markets",
        2.00, 350.0));

    funds.emplace_back(std::make_unique<MutualFund>(
        105, "Balanced Allocation Fund", 35.90,
        FundCategory::HYBRID, RiskLevel::MODERATE,
        "60% equity and 40% debt mix for balanced risk-return profile",
        1.50, 680.0));

    funds.emplace_back(std::make_unique<MutualFund>(
        106, "Energy Sector Fund", 58.30,
        FundCategory::SECTORAL, RiskLevel::HIGH,
        "Concentrated exposure to oil, gas, and renewable energy companies",
        1.80, 290.0));

    funds.emplace_back(std::make_unique<MutualFund>(
        107, "Dividend Aristocrats Fund", 72.15,
        FundCategory::EQUITY, RiskLevel::MODERATE,
        "Invests in companies with 10+ years of consistent dividend growth",
        1.10, 560.0));

    funds.emplace_back(std::make_unique<MutualFund>(
        108, "Short Term Income Fund", 10.05,
        FundCategory::DEBT, RiskLevel::LOW,
        "Low-duration debt instruments for capital preservation",
        0.40, 2100.0));

    funds.emplace_back(std::make_unique<MutualFund>(
        109, "Asia Pacific Ex-Japan Fund", 95.60,
        FundCategory::INTERNATIONAL, RiskLevel::HIGH,
        "Exposure to China, India, South Korea, and ASEAN markets",
        1.90, 180.0));

    funds.emplace_back(std::make_unique<MutualFund>(
        110, "Smart Beta Fund", 42.80,
        FundCategory::HYBRID, RiskLevel::MODERATE,
        "Factor-based investing combining value, momentum, and quality signals",
        1.35, 310.0));
}

} // namespace ProTrack
