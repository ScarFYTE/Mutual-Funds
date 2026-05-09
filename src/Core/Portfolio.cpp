#include "Portfolio.h"
#include "FundManager.h"
#include <algorithm>

namespace ProTrack {

Portfolio::Portfolio() : totalValue(0.0) {}

void Portfolio::addInvestment(const Investment& inv) {
    // Check if we already hold this fund - if so, merge
    for (auto& existing : investments) {
        if (existing.getFundID() == inv.getFundID()) {
            double newUnits = existing.getUnits() + inv.getUnits();
            double newCost = (existing.getPurchasePrice() * existing.getUnits()
                           + inv.getPurchasePrice() * inv.getUnits()) / newUnits;
            existing.setUnits(newUnits);
            // Weighted average purchase price would require modifying Investment
            // For simplicity, we keep original cost basis on new purchases
            return;
        }
    }
    investments.push_back(inv);
}

bool Portfolio::sellInvestment(int fundID, double unitsToSell) {
    for (auto it = investments.begin(); it != investments.end(); ++it) {
        if (it->getFundID() == fundID) {
            if (it->getUnits() < unitsToSell) return false;
            it->setUnits(it->getUnits() - unitsToSell);
            // Remove if fully sold
            if (it->getUnits() <= 0.0001) {
                investments.erase(it);
            }
            return true;
        }
    }
    return false; // Fund not found
}

double Portfolio::getUnitsForFund(int fundID) const {
    for (const auto& inv : investments) {
        if (inv.getFundID() == fundID) return inv.getUnits();
    }
    return 0.0;
}

bool Portfolio::hasInvestment(int fundID) const {
    for (const auto& inv : investments) {
        if (inv.getFundID() == fundID) return true;
    }
    return false;
}

double Portfolio::calculateValue(const FundManager& fm) {
    totalValue = 0.0;
    for (const auto& inv : investments) {
        const MutualFund* fund = fm.getFundByID(inv.getFundID());
        if (fund) {
            totalValue += inv.getCurrentValue(fund->getNAV());
        }
    }
    return totalValue;
}

double Portfolio::getTotalValue() const { return totalValue; }

double Portfolio::calculateTotalProfitLoss(const FundManager& fm) {
    double totalPL = 0.0;
    for (const auto& inv : investments) {
        const MutualFund* fund = fm.getFundByID(inv.getFundID());
        if (fund) {
            totalPL += inv.getProfitLoss(fund->getNAV());
        }
    }
    return totalPL;
}

double Portfolio::calculateTotalReturnPercent(const FundManager& fm) {
    double totalCost = 0.0;
    double totalCurrent = 0.0;
    for (const auto& inv : investments) {
        const MutualFund* fund = fm.getFundByID(inv.getFundID());
        if (fund) {
            totalCost += inv.getPurchasePrice() * inv.getUnits();
            totalCurrent += inv.getCurrentValue(fund->getNAV());
        }
    }
    if (totalCost == 0) return 0.0;
    return ((totalCurrent - totalCost) / totalCost) * 100.0;
}

size_t Portfolio::getNumHoldings() const { return investments.size(); }

void Portfolio::recordSnapshot(double totalVal, double cashBalance) {
    history.emplace_back(totalVal, cashBalance, investments.size());
}

const std::vector<Snapshot>& Portfolio::getHistory() const { return history; }

void Portfolio::clearHistory() { history.clear(); }

void Portfolio::recordTransaction(const Transaction& txn) {
    transactions.push_back(txn);
}

const std::vector<Transaction>& Portfolio::getTransactions() const { return transactions; }

Portfolio::iterator Portfolio::begin() { return investments.begin(); }
Portfolio::iterator Portfolio::end() { return investments.end(); }
Portfolio::const_iterator Portfolio::begin() const { return investments.begin(); }
Portfolio::const_iterator Portfolio::end() const { return investments.end(); }

const std::vector<Investment>& Portfolio::getInvestments() const { return investments; }

void Portfolio::clear() {
    investments.clear();
    history.clear();
    transactions.clear();
    totalValue = 0.0;
}

} // namespace ProTrack
