#pragma once

/**
 * Investor.h
 * Represents an investor/user with a portfolio and cash balance.
 * Demonstrates: COMPOSITION (has-a Portfolio), ENCAPSULATION,
 *               high cohesion - investor manages their own transactions.
 */

#include <string>
#include "Portfolio.h"

namespace ProTrack {

class FundManager;

class Investor {
private:
    int investorID;
    std::string name;
    double balance;           // Cash balance available for investing
    Portfolio portfolio;
    std::string password;     // Simple authentication
    time_t registrationDate;

public:
    Investor(int id, const std::string& n, double b, const std::string& pwd = "");

    // ACCESSORS
    int getInvestorID() const;
    std::string getName() const;
    double getBalance() const;
    const Portfolio& getPortfolio() const;
    Portfolio& getPortfolio();
    time_t getRegistrationDate() const;

    // MUTATORS
    void setName(const std::string& n);
    void deposit(double amount);

    // Business operations
    bool buyFund(int fundID, double amount, FundManager& fm);
    bool sellFund(int fundID, double units, FundManager& fm);
    double getTotalNetWorth(FundManager& fm);

    // Authentication
    bool verifyPassword(const std::string& pwd) const;
    void setPassword(const std::string& pwd);

    // Summary
    std::string getSummary(FundManager& fm);
};

} // namespace ProTrack
