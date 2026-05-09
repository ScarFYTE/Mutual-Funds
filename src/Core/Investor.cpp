#include "Investor.h"
#include "FundManager.h"
#include <sstream>
#include <iomanip>

namespace ProTrack {

Investor::Investor(int id, const std::string& n, double b, const std::string& pwd)
    : investorID(id), name(n), balance(b), password(pwd), registrationDate(std::time(nullptr)) {}

int Investor::getInvestorID() const { return investorID; }
std::string Investor::getName() const { return name; }
double Investor::getBalance() const { return balance; }
const Portfolio& Investor::getPortfolio() const { return portfolio; }
Portfolio& Investor::getPortfolio() { return portfolio; }
time_t Investor::getRegistrationDate() const { return registrationDate; }

void Investor::setName(const std::string& n) { name = n; }

void Investor::deposit(double amount) {
    if (amount > 0) balance += amount;
}

bool Investor::buyFund(int fundID, double amount, FundManager& fm) {
    if (amount <= 0 || amount > balance) return false;

    const MutualFund* fund = fm.getFundByID(fundID);
    if (!fund) return false;

    double units = amount / fund->getNAV();
    Investment inv(fundID, units, fund->getNAV());
    portfolio.addInvestment(inv);
    balance -= amount;

    // Record transaction
    portfolio.recordTransaction(Transaction(fundID, TransactionType::BUY, units, fund->getNAV()));

    // Record snapshot
    double totalVal = portfolio.calculateValue(fm);
    portfolio.recordSnapshot(totalVal, balance);

    return true;
}

bool Investor::sellFund(int fundID, double units, FundManager& fm) {
    if (units <= 0) return false;
    if (!portfolio.hasInvestment(fundID)) return false;
    if (portfolio.getUnitsForFund(fundID) < units) return false;

    const MutualFund* fund = fm.getFundByID(fundID);
    if (!fund) return false;

    double proceeds = units * fund->getNAV();
    if (portfolio.sellInvestment(fundID, units)) {
        balance += proceeds;

        // Record transaction
        portfolio.recordTransaction(Transaction(fundID, TransactionType::SELL, units, fund->getNAV()));

        // Record snapshot
        double totalVal = portfolio.calculateValue(fm);
        portfolio.recordSnapshot(totalVal, balance);

        return true;
    }
    return false;
}

double Investor::getTotalNetWorth(FundManager& fm) {
    return balance + portfolio.calculateValue(fm);
}

bool Investor::verifyPassword(const std::string& pwd) const {
    return password == pwd;
}

void Investor::setPassword(const std::string& pwd) {
    password = pwd;
}

std::string Investor::getSummary(FundManager& fm) {
    std::ostringstream oss;
    double portfolioValue = portfolio.calculateValue(fm);
    double totalNetWorth = balance + portfolioValue;
    double totalPL = portfolio.calculateTotalProfitLoss(fm);

    oss << "=== " << name << "'s Portfolio Summary ===" << "\n"
        << "Cash Balance:  $" << std::fixed << std::setprecision(2) << balance << "\n"
        << "Portfolio Value: $" << portfolioValue << "\n"
        << "Total Net Worth: $" << totalNetWorth << "\n"
        << "Total P&L:     $" << totalPL << "\n"
        << "Return %:      " << std::setprecision(2) << portfolio.calculateTotalReturnPercent(fm) << "%" << "\n"
        << "Holdings:      " << portfolio.getNumHoldings() << " funds" << "\n"
        << "Snapshots:     " << portfolio.getHistory().size() << " records";
    return oss.str();
}

} // namespace ProTrack
