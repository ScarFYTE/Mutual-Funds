#pragma once

/**
 * Investment.h
 * Represents a single investment holding.
 * Demonstrates: ENCAPSULATION, const-correctness.
 */

#include <ctime>
#include <string>

namespace ProTrack {

class Investment {
private:
    int fundID;           // Which fund this investment is in
    double units;         // Number of units owned
    double purchasePrice; // NAV at time of purchase
    time_t purchaseDate;  // When the investment was made

public:
    Investment(int id, double u, double p);

    // ACCESSORS
    int getFundID() const;
    double getUnits() const;
    double getPurchasePrice() const;
    time_t getPurchaseDate() const;

    // MUTATORS
    void setUnits(double u);

    // Business logic
    double getCurrentValue(double currentNAV) const;
    double getProfitLoss(double currentNAV) const;
    double getProfitLossPercent(double currentNAV) const;

    // For display
    std::string getPurchaseDateString() const;
};

} // namespace ProTrack
