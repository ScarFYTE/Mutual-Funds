#pragma once

/**
 * FundManager.h
 * Central registry for all available mutual funds.
 * Demonstrates: SINGLETON-LIKE pattern (shared instance), AGGREGATION,
 *               CONTAINER MANAGEMENT, SEARCH operations.
 */

#include <vector>
#include <string>
#include <memory>
#include "MutualFund.h"

namespace ProTrack {

class FundManager {
private:
    std::vector<std::unique_ptr<MutualFund>> funds;
    int nextFundID;

public:
    FundManager();
    ~FundManager() = default;

    // Disable copy to maintain unique ownership
    FundManager(const FundManager&) = delete;
    FundManager& operator=(const FundManager&) = delete;

    // Fund management
    void addFund(MutualFund* fund);               // Takes ownership
    void removeFund(int fundID);
    void clearFunds();

    // Queries
    const MutualFund* getFundByID(int fundID) const;
    MutualFund* getFundByID(int fundID);
    size_t getNumFunds() const;
    bool hasFund(int fundID) const;

    // Search and filter
    std::vector<const MutualFund*> searchByName(const std::string& query) const;
    std::vector<const MutualFund*> filterByCategory(FundCategory cat) const;
    std::vector<const MutualFund*> filterByRisk(RiskLevel risk) const;
    std::vector<const MutualFund*> getAllFunds() const;
    std::vector<const MutualFund*> getTopGainers() const;
    std::vector<const MutualFund*> getTopLosers() const;

    // Market operations
    void simulateMarketDay();
    double getTotalMarketCap() const;

    // Iteration
    using iterator = std::vector<std::unique_ptr<MutualFund>>::iterator;
    using const_iterator = std::vector<std::unique_ptr<MutualFund>>::const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

    // Pre-populate with sample funds
    void loadDefaultFunds();
};

} // namespace ProTrack
