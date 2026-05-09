#pragma once

/**
 * ColorScheme.h
 * Central color palette for the application.
 * Demonstrates: NAMESPACE utility, centralized design constants.
 * Professional dark-mode financial dashboard aesthetic.
 */

#include <SFML/Graphics.hpp>

namespace ProTrack {
namespace UI {

// Background colors
constexpr sf::Color BG_DARK        (15, 23, 42);     // Deep navy background
constexpr sf::Color BG_PANEL       (30, 41, 59);     // Slightly lighter panel
constexpr sf::Color BG_CARD        (51, 65, 85);     // Card/elevated surface
constexpr sf::Color BG_HOVER       (71, 85, 105);    // Hover state
constexpr sf::Color BG_INPUT       (15, 23, 42);     // Input field background

// Text colors
constexpr sf::Color TEXT_PRIMARY   (241, 245, 249);  // White-ish primary text
constexpr sf::Color TEXT_SECONDARY (148, 163, 184);  // Muted gray text
constexpr sf::Color TEXT_MUTED     (100, 116, 139);  // Very muted labels

// Accent colors
constexpr sf::Color ACCENT_PRIMARY (56, 189, 248);   // Cyan/blue accent
constexpr sf::Color ACCENT_SECOND  (99, 102, 241);   // Indigo accent
constexpr sf::Color ACCENT_SUCCESS (34, 197, 94);    // Green for profits
constexpr sf::Color ACCENT_DANGER  (239, 68, 68);    // Red for losses
constexpr sf::Color ACCENT_WARNING (251, 191, 36);   // Yellow for warnings

// Chart colors
constexpr sf::Color CHART_LINE     (56, 189, 248);   // Primary line
constexpr sf::Color CHART_FILL     (56, 189, 248, 40); // Faded fill
constexpr sf::Color CHART_GRID     (51, 65, 85);     // Grid lines
constexpr sf::Color CHART_POSITIVE (34, 197, 94);    // Positive change
constexpr sf::Color CHART_NEGATIVE (239, 68, 68);    // Negative change

// Border colors
constexpr sf::Color BORDER_DEFAULT (51, 65, 85);     // Default borders
constexpr sf::Color BORDER_FOCUSED (56, 189, 248);   // Focused input border

// Button colors
constexpr sf::Color BTN_PRIMARY    (37, 99, 235);    // Blue primary button
constexpr sf::Color BTN_PRIMARY_H  (59, 130, 246);   // Primary button hover
constexpr sf::Color BTN_SECONDARY  (51, 65, 85);     // Secondary button
constexpr sf::Color BTN_SECONDARY_H(71, 85, 105);    // Secondary hover
constexpr sf::Color BTN_SUCCESS    (22, 163, 74);    // Green action button
constexpr sf::Color BTN_SUCCESS_H  (34, 197, 94);    // Green hover
constexpr sf::Color BTN_DANGER     (220, 38, 38);    // Red action button
constexpr sf::Color BTN_DANGER_H   (239, 68, 68);    // Red hover

// Helper to get profit/loss color
inline sf::Color getProfitLossColor(double value) {
    return (value >= 0) ? ACCENT_SUCCESS : ACCENT_DANGER;
}

// Helper to lighten a color
inline sf::Color lighten(sf::Color c, int amount) {
    int r = std::min(255, static_cast<int>(c.r) + amount);
    int g = std::min(255, static_cast<int>(c.g) + amount);
    int b = std::min(255, static_cast<int>(c.b) + amount);
    return sf::Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
}

} // namespace UI
} // namespace ProTrack
