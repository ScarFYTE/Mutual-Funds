#pragma once

/**
 * ChartRenderer.h
 * Renders line/bar charts from snapshot data.
 * Demonstrates: SFML VertexArray/Line drawing, data visualization,
 *               ALGORITHM (min/max finding, coordinate mapping).
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../Core/Snapshot.h"
#include "ColorScheme.h"

namespace ProTrack {
namespace UI {

class ChartRenderer {
private:
    float x, y, width, height;
    sf::Font* font;

public:
    ChartRenderer(float px, float py, float w, float h, sf::Font* f);

    // Line chart from snapshot history
    void renderLineChart(sf::RenderWindow& window,
                         const std::vector<Snapshot>& history,
                         const std::string& title = "Portfolio Value");

    // Bar chart for fund holdings
    void renderHoldingsBarChart(sf::RenderWindow& window,
                                const std::vector<std::pair<std::string, double>>& holdings,
                                const std::string& title = "Holdings");

    // Pie/donut chart placeholder (shows allocation)
    void renderAllocationChart(sf::RenderWindow& window,
                               const std::vector<std::pair<std::string, double>>& allocations);

private:
    float mapValueToY(double value, double minVal, double maxVal, float chartH) const;
    float mapIndexToX(size_t index, size_t total, float chartW) const;
    void drawGrid(sf::RenderWindow& window, double minVal, double maxVal);
    void drawAxisLabels(sf::RenderWindow& window, double minVal, double maxVal);
};

} // namespace UI
} // namespace ProTrack
