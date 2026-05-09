#include "Transaction.h"

namespace ProTrack {

Transaction::Transaction(int fid, TransactionType t, double u, double price)
    : fundID(fid), type(t), units(u), pricePerUnit(price),
      totalAmount(u * price), transactionTime(std::time(nullptr)) {}

int Transaction::getFundID() const { return fundID; }
TransactionType Transaction::getType() const { return type; }
double Transaction::getUnits() const { return units; }
double Transaction::getPricePerUnit() const { return pricePerUnit; }
double Transaction::getTotalAmount() const { return totalAmount; }
time_t Transaction::getTransactionTime() const { return transactionTime; }

std::string Transaction::getTypeString() const {
    return (type == TransactionType::BUY) ? "BUY" : "SELL";
}

std::string Transaction::getDateString() const {
    struct tm* timeinfo = localtime(&transactionTime);
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

} // namespace ProTrack
