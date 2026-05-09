#pragma once

/**
 * Snapshot.h
 * Records portfolio value at a point in time.
 * Demonstrates: simple DATA CLASS with full encapsulation.
 */

#include <ctime>
#include <string>

namespace ProTrack {

class Snapshot {
private:
    time_t timestamp;
    double totalValue;
    double cashBalance;
    size_t numHoldings;

public:
    Snapshot(double val, double cash, size_t holdings);

    // ACCESSORS
    time_t getTimestamp() const;
    double getTotalValue() const;
    double getCashBalance() const;
    size_t getNumHoldings() const;

    // Display helpers
    std::string getDateString() const;
    std::string getTimeString() const;
};

} // namespace ProTrack
