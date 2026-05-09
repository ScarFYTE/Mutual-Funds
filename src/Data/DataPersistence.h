#pragma once

/**
 * DataPersistence.h
 * Abstract base class for data persistence.
 * Demonstrates: ABSTRACT CLASS, PURE VIRTUAL FUNCTIONS (polymorphism),
 *               INTERFACE SEGREGATION - defines a contract all persistence
 *               mechanisms must fulfill.
 *
 * Inspired by the UML diagram's PaymentMethod abstract class pattern.
 */

#include <string>
#include <vector>
#include "../Core/Investor.h"
#include "../Core/FundManager.h"

namespace ProTrack {

class DataPersistence {
public:
    // Virtual destructor - essential for base classes
    virtual ~DataPersistence() = default;

    // Save/Load investor portfolio data
    virtual bool savePortfolio(const Investor& investor, const std::string& filename) = 0;
    virtual bool loadPortfolio(Investor& investor, FundManager& fm, const std::string& filename) = 0;

    // Check if a save file exists
    virtual bool fileExists(const std::string& filename) = 0;

    // List available save files
    virtual std::vector<std::string> listSaveFiles() = 0;

    // Delete a save file
    virtual bool deleteSaveFile(const std::string& filename) = 0;
};

} // namespace ProTrack
