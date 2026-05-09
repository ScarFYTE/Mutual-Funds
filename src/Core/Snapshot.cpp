#include "Snapshot.h"
#include <sstream>
#include <iomanip>

namespace ProTrack {

Snapshot::Snapshot(double val, double cash, size_t holdings)
    : timestamp(std::time(nullptr)), totalValue(val), cashBalance(cash), numHoldings(holdings) {}

time_t Snapshot::getTimestamp() const { return timestamp; }
double Snapshot::getTotalValue() const { return totalValue; }
double Snapshot::getCashBalance() const { return cashBalance; }
size_t Snapshot::getNumHoldings() const { return numHoldings; }

std::string Snapshot::getDateString() const {
    struct tm* timeinfo = localtime(&timestamp);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return std::string(buffer);
}

std::string Snapshot::getTimeString() const {
    struct tm* timeinfo = localtime(&timestamp);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
    return std::string(buffer);
}

} // namespace ProTrack
