#pragma once

/**
 * Enums.h
 * Demonstrates: Strong typing with enumerations
 * These enums replace magic numbers with readable, type-safe constants.
 */

namespace ProTrack {

// Fund categories for classification and filtering
enum class FundCategory {
    EQUITY,      // Stock-based funds
    DEBT,        // Bond/fixed-income funds
    HYBRID,      // Mixed equity+debt
    INDEX,       // Passively track indices
    SECTORAL,    // Industry-specific
    INTERNATIONAL // Foreign market exposure
};

// Risk assessment levels
enum class RiskLevel {
    LOW,         // Conservative, stable returns
    MODERATE,    // Balanced risk-return
    HIGH,        // Aggressive growth potential
    VERY_HIGH    // Maximum volatility
};

// Screen states for the GUI state machine
enum class AppScreen {
    LOGIN,
    DASHBOARD,
    MARKET,
    PORTFOLIO,
    TRADE,
    PERFORMANCE,
    SETTINGS
};

// Transaction types
enum class TransactionType {
    BUY,
    SELL
};

// Helper to convert FundCategory to display string
inline const char* categoryToString(FundCategory cat) {
    switch (cat) {
        case FundCategory::EQUITY:        return "Equity";
        case FundCategory::DEBT:          return "Debt";
        case FundCategory::HYBRID:        return "Hybrid";
        case FundCategory::INDEX:         return "Index";
        case FundCategory::SECTORAL:      return "Sectoral";
        case FundCategory::INTERNATIONAL: return "International";
    }
    return "Unknown";
}

// Helper to convert RiskLevel to display string
inline const char* riskToString(RiskLevel risk) {
    switch (risk) {
        case RiskLevel::LOW:       return "Low";
        case RiskLevel::MODERATE:  return "Moderate";
        case RiskLevel::HIGH:      return "High";
        case RiskLevel::VERY_HIGH: return "Very High";
    }
    return "Unknown";
}

} // namespace ProTrack
