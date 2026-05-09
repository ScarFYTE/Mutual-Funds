#pragma once

/**
 * MutualFund.h
 * Represents a single mutual fund with its market data.
 * Demonstrates: ENCAPSULATION (private data, public accessors),
 *               CONSTRUCTOR INITIALIZATION, inline methods.
 */

#include <string>
#include <ctime>
#include "Enums.h"

namespace ProTrack {

class MutualFund {
private:
    // Private data members - ENCAPSULATION
    int fundID;
    std::string fundName;
    double nav;                  // Net Asset Value (price per unit)
    double previousNav;          // Yesterday's NAV for change calculation
    FundCategory category;
    RiskLevel risk;
    std::string description;
    double expenseRatio;         // Annual fee percentage
    double aum;                  // Assets Under Management (in millions)
    time_t lastUpdate;           // Timestamp of last NAV update

public:
    // Parameterized constructor with initialization list
    MutualFund(int id, const std::string& name, double initialNav,
               FundCategory cat = FundCategory::EQUITY,
               RiskLevel r = RiskLevel::MODERATE,
               const std::string& desc = "",
               double expense = 1.0, double aumMillions = 100.0);

    // Default destructor (no dynamic allocation in this class)
    ~MutualFund() = default;

    // ACCESSORS (getters) - const-correct
    int getFundID() const;
    std::string getFundName() const;
    double getNAV() const;
    double getPreviousNAV() const;
    FundCategory getCategory() const;
    RiskLevel getRiskLevel() const;
    std::string getDescription() const;
    double getExpenseRatio() const;
    double getAUM() const;
    time_t getLastUpdate() const;

    // MUTATORS (setters)
    void setNAV(double newNav);
    void setDescription(const std::string& desc);

    // Business logic methods
    double getDailyChange() const;           // Absolute change
    double getDailyChangePercent() const;    // Percentage change
    void simulateMarketMovement();           // Random price fluctuation

    // Display helper
    std::string getInfoString() const;
};

} // namespace ProTrack
