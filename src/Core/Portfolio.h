#pragma once

/**
 * Portfolio.h
 * Manages a collection of investments and provides analytics.
 * Demonstrates: COMPOSITION (has-a Investment), aggregation with FundManager,
 *               ENCAPSULATION, iterator pattern.
 */

#include <vector>
#include <memory>
#include "Investment.h"
#include "Snapshot.h"
#include "Transaction.h"

namespace ProTrack {

class FundManager;  // Forward declaration to avoid circular include

class Portfolio {
private:
    std::vector<Investment> investments;
    std::vector<Snapshot> history;
    std::vector<Transaction> transactions;
    double totalValue;

public:
    Portfolio();
    ~Portfolio() = default;

    // Investment management
    void addInvestment(const Investment& inv);
    bool sellInvestment(int fundID, double unitsToSell);
    double getUnitsForFund(int fundID) const;
    bool hasInvestment(int fundID) const;

    // Analytics
    double calculateValue(const FundManager& fm);
    double getTotalValue() const;
    double calculateTotalProfitLoss(const FundManager& fm);
    double calculateTotalReturnPercent(const FundManager& fm);
    size_t getNumHoldings() const;

    // Snapshot & history
    void recordSnapshot(double totalVal, double cashBalance);
    const std::vector<Snapshot>& getHistory() const;
    void clearHistory();

    // Transactions
    void recordTransaction(const Transaction& txn);
    const std::vector<Transaction>& getTransactions() const;

    // Iteration support
    using iterator = std::vector<Investment>::iterator;
    using const_iterator = std::vector<Investment>::const_iterator;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    // Direct access
    const std::vector<Investment>& getInvestments() const;

    // Reset
    void clear();
};

} // namespace ProTrack
