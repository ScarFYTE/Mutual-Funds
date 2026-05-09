#pragma once

/**
 * FilePersistence.h
 * Concrete implementation using binary file I/O.
 * Demonstrates: INHERITANCE, OVERRIDING, FILE I/O with streams,
 *               RAII (fstream manages its own resources).
 */

#include "DataPersistence.h"
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

namespace ProTrack {

class FilePersistence : public DataPersistence {
private:
    static constexpr const char* SAVE_DIRECTORY = "saves/";
    static constexpr const char* FILE_EXTENSION = ".ptk";

    // Helper to ensure directory exists
    void ensureDirectoryExists();

    // Helper to build full path
    std::string buildPath(const std::string& filename);

public:
    FilePersistence();
    ~FilePersistence() override = default;

    // Override all pure virtual methods
    bool savePortfolio(const Investor& investor, const std::string& filename) override;
    bool loadPortfolio(Investor& investor, FundManager& fm, const std::string& filename) override;
    bool fileExists(const std::string& filename) override;
    std::vector<std::string> listSaveFiles() override;
    bool deleteSaveFile(const std::string& filename) override;
};

} // namespace ProTrack
