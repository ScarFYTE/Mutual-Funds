#pragma once

/**
 * Transaction.h
 * Records a buy or sell transaction.
 * Demonstrates: ENCAPSULATION, association with Enums.
 */

#include <ctime>
#include <string>
#include "Enums.h"

namespace ProTrack {

class Transaction {
private:
    int fundID;
    TransactionType type;
    double units;
    double pricePerUnit;
    double totalAmount;
    time_t transactionTime;

public:
    Transaction(int fid, TransactionType t, double u, double price);

    // ACCESSORS
    int getFundID() const;
    TransactionType getType() const;
    double getUnits() const;
    double getPricePerUnit() const;
    double getTotalAmount() const;
    time_t getTransactionTime() const;

    // Display helpers
    std::string getTypeString() const;
    std::string getDateString() const;
};

} // namespace ProTrack
