#include "FilePersistence.h"
#include <sys/stat.h>
#include <cstring>
#include <iostream>

namespace ProTrack {

FilePersistence::FilePersistence() {
    ensureDirectoryExists();
}

void FilePersistence::ensureDirectoryExists() {
    #ifdef _WIN32
        _mkdir(SAVE_DIRECTORY);
    #else
        struct stat st;
        if (stat(SAVE_DIRECTORY, &st) != 0) {
            mkdir(SAVE_DIRECTORY, 0755);
        }
    #endif
}

std::string FilePersistence::buildPath(const std::string& filename) {
    std::string path = SAVE_DIRECTORY;
    path += filename;
    // Only append extension if not already present
    if (path.find(FILE_EXTENSION) == std::string::npos) {
        path += FILE_EXTENSION;
    }
    return path;
}

bool FilePersistence::savePortfolio(const Investor& investor, const std::string& filename) {
    std::string path = buildPath(filename);
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    // Write investor basic info
    int id = investor.getInvestorID();
    double balance = investor.getBalance();
    size_t nameLen = investor.getName().length();

    file.write(reinterpret_cast<const char*>(&id), sizeof(id));
    file.write(reinterpret_cast<const char*>(&balance), sizeof(balance));
    file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
    file.write(investor.getName().c_str(), static_cast<long>(nameLen));

    // Write investments
    const auto& investments = investor.getPortfolio().getInvestments();
    size_t numInvestments = investments.size();
    file.write(reinterpret_cast<const char*>(&numInvestments), sizeof(numInvestments));

    for (const auto& inv : investments) {
        int fid = inv.getFundID();
        double units = inv.getUnits();
        double pPrice = inv.getPurchasePrice();
        time_t pDate = inv.getPurchaseDate();

        file.write(reinterpret_cast<const char*>(&fid), sizeof(fid));
        file.write(reinterpret_cast<const char*>(&units), sizeof(units));
        file.write(reinterpret_cast<const char*>(&pPrice), sizeof(pPrice));
        file.write(reinterpret_cast<const char*>(&pDate), sizeof(pDate));
    }

    // Write transaction history
    const auto& txns = investor.getPortfolio().getTransactions();
    size_t numTxns = txns.size();
    file.write(reinterpret_cast<const char*>(&numTxns), sizeof(numTxns));

    for (const auto& txn : txns) {
        int fid = txn.getFundID();
        int tType = (txn.getType() == TransactionType::BUY) ? 0 : 1;
        double units = txn.getUnits();
        double price = txn.getPricePerUnit();
        double total = txn.getTotalAmount();
        time_t tTime = txn.getTransactionTime();

        file.write(reinterpret_cast<const char*>(&fid), sizeof(fid));
        file.write(reinterpret_cast<const char*>(&tType), sizeof(tType));
        file.write(reinterpret_cast<const char*>(&units), sizeof(units));
        file.write(reinterpret_cast<const char*>(&price), sizeof(price));
        file.write(reinterpret_cast<const char*>(&total), sizeof(total));
        file.write(reinterpret_cast<const char*>(&tTime), sizeof(tTime));
    }

    // Write snapshot history
    const auto& snaps = investor.getPortfolio().getHistory();
    size_t numSnaps = snaps.size();
    file.write(reinterpret_cast<const char*>(&numSnaps), sizeof(numSnaps));

    for (const auto& snap : snaps) {
        time_t ts = snap.getTimestamp();
        double val = snap.getTotalValue();
        double cash = snap.getCashBalance();
        size_t h = snap.getNumHoldings();

        file.write(reinterpret_cast<const char*>(&ts), sizeof(ts));
        file.write(reinterpret_cast<const char*>(&val), sizeof(val));
        file.write(reinterpret_cast<const char*>(&cash), sizeof(cash));
        file.write(reinterpret_cast<const char*>(&h), sizeof(h));
    }

    file.close();
    return true;
}

bool FilePersistence::loadPortfolio(Investor& investor, FundManager& fm, const std::string& filename) {
    std::string path = buildPath(filename);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    // Read investor basic info
    int id;
    double balance;
    size_t nameLen;

    file.read(reinterpret_cast<char*>(&id), sizeof(id));
    file.read(reinterpret_cast<char*>(&balance), sizeof(balance));
    file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));

    std::string name(nameLen, '\0');
    file.read(&name[0], static_cast<long>(nameLen));

    // Read investments
    size_t numInvestments;
    file.read(reinterpret_cast<char*>(&numInvestments), sizeof(numInvestments));

    investor.getPortfolio().clear();

    for (size_t i = 0; i < numInvestments; ++i) {
        int fid;
        double units, pPrice;
        time_t pDate;

        file.read(reinterpret_cast<char*>(&fid), sizeof(fid));
        file.read(reinterpret_cast<char*>(&units), sizeof(units));
        file.read(reinterpret_cast<char*>(&pPrice), sizeof(pPrice));
        file.read(reinterpret_cast<char*>(&pDate), sizeof(pDate));

        Investment inv(fid, units, pPrice);
        // Note: purchaseDate is set in constructor; we accept it as-is
        investor.getPortfolio().addInvestment(inv);
    }

    // Read transactions
    size_t numTxns;
    file.read(reinterpret_cast<char*>(&numTxns), sizeof(numTxns));

    for (size_t i = 0; i < numTxns; ++i) {
        int fid, tType;
        double units, price, total;
        time_t tTime;

        file.read(reinterpret_cast<char*>(&fid), sizeof(fid));
        file.read(reinterpret_cast<char*>(&tType), sizeof(tType));
        file.read(reinterpret_cast<char*>(&units), sizeof(units));
        file.read(reinterpret_cast<char*>(&price), sizeof(price));
        file.read(reinterpret_cast<char*>(&total), sizeof(total));
        file.read(reinterpret_cast<char*>(&tTime), sizeof(tTime));

        TransactionType type = (tType == 0) ? TransactionType::BUY : TransactionType::SELL;
        Transaction txn(fid, type, units, price);
        investor.getPortfolio().recordTransaction(txn);
    }

    // Read snapshots
    size_t numSnaps;
    file.read(reinterpret_cast<char*>(&numSnaps), sizeof(numSnaps));

    for (size_t i = 0; i < numSnaps; ++i) {
        time_t ts;
        double val, cash;
        size_t h;

        file.read(reinterpret_cast<char*>(&ts), sizeof(ts));
        file.read(reinterpret_cast<char*>(&val), sizeof(val));
        file.read(reinterpret_cast<char*>(&cash), sizeof(cash));
        file.read(reinterpret_cast<char*>(&h), sizeof(h));

        Snapshot snap(val, cash, h);
        // We add to history but the timestamp was set by constructor to now
        // In a production app, we'd snapshot-copy the timestamp
        investor.getPortfolio().recordSnapshot(snap.getTotalValue(), snap.getCashBalance());
    }

    // Update investor state
    investor.deposit(balance);  // This sets the balance
    file.close();
    return true;
}

bool FilePersistence::fileExists(const std::string& filename) {
    std::string path = buildPath(filename);
    struct stat st;
    return (stat(path.c_str(), &st) == 0);
}

std::vector<std::string> FilePersistence::listSaveFiles() {
    std::vector<std::string> files;
    #ifdef _WIN32
        // Windows implementation would use FindFirstFile
        // Simplified: check known files
        for (int i = 1; i <= 10; ++i) {
            std::string name = "save" + std::to_string(i);
            if (fileExists(name)) {
                files.push_back(name);
            }
        }
    #else
        DIR* dir = opendir(SAVE_DIRECTORY);
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                std::string fname(entry->d_name);
                if (fname.length() > 4 && fname.substr(fname.length() - 4) == ".ptk") {
                    files.push_back(fname.substr(0, fname.length() - 4));
                }
            }
            closedir(dir);
        }
    #endif
    return files;
}

bool FilePersistence::deleteSaveFile(const std::string& filename) {
    std::string path = buildPath(filename);
    return (remove(path.c_str()) == 0);
}

} // namespace ProTrack
