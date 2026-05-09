#include "Investment.h"
#include <sstream>
#include <iomanip>

namespace ProTrack {

Investment::Investment(int id, double u, double p)
    : fundID(id), units(u), purchasePrice(p), purchaseDate(std::time(nullptr)) {}

int Investment::getFundID() const { return fundID; }
double Investment::getUnits() const { return units; }
double Investment::getPurchasePrice() const { return purchasePrice; }
time_t Investment::getPurchaseDate() const { return purchaseDate; }

void Investment::setUnits(double u) { units = u; }

double Investment::getCurrentValue(double currentNAV) const {
    return units * currentNAV;
}

double Investment::getProfitLoss(double currentNAV) const {
    return (currentNAV - purchasePrice) * units;
}

double Investment::getProfitLossPercent(double currentNAV) const {
    if (purchasePrice == 0) return 0.0;
    return ((currentNAV - purchasePrice) / purchasePrice) * 100.0;
}

std::string Investment::getPurchaseDateString() const {
    struct tm* timeinfo = localtime(&purchaseDate);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return std::string(buffer);
}

} // namespace ProTrack
