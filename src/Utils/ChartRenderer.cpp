#include "ChartRenderer.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace ProTrack {
namespace UI {

ChartRenderer::ChartRenderer(float px, float py, float w, float h, sf::Font* f)
    : x(px), y(py), width(w), height(h), font(f) {}

void ChartRenderer::renderLineChart(sf::RenderWindow& window,
                                     const std::vector<Snapshot>& history,
                                     const std::string& title) {
    if (history.size() < 2) {
        drawLabel(window, font, "Not enough data - make some transactions!",
                  x + width / 2 - 120, y + height / 2, 14, TEXT_MUTED);
        return;
    }

    // Draw panel background
    sf::RectangleShape bg;
    bg.setPosition(x, y);
    bg.setSize(sf::Vector2f(width, height));
    bg.setFillColor(BG_PANEL);
    bg.setOutlineThickness(1);
    bg.setOutlineColor(BORDER_DEFAULT);
    window.draw(bg);

    // Title
    drawLabel(window, font, title, x + 12, y + 8, 16, ACCENT_PRIMARY);

    float padding = 50;
    float chartW = width - padding * 2;
    float chartH = height - padding * 2 - 20;
    float chartX = x + padding;
    float chartY = y + padding + 10;

    // Find min/max
    double minVal = history[0].getTotalValue();
    double maxVal = history[0].getTotalValue();
    for (const auto& snap : history) {
        if (snap.getTotalValue() < minVal) minVal = snap.getTotalValue();
        if (snap.getTotalValue() > maxVal) maxVal = snap.getTotalValue();
    }
    double range = maxVal - minVal;
    if (range == 0) range = 1;
    minVal -= range * 0.1;
    maxVal += range * 0.1;

    // Draw grid
    drawGrid(window, minVal, maxVal);

    // Draw the line
    sf::VertexArray line(sf::LinesStrip, history.size());
    for (size_t i = 0; i < history.size(); ++i) {
        float px = mapIndexToX(i, history.size(), chartW) + chartX;
        float py = chartY + chartH - mapValueToY(history[i].getTotalValue(), minVal, maxVal, chartH);
        line[i].position = sf::Vector2f(px, py);
        line[i].color = CHART_LINE;
    }
    window.draw(line);

    // Draw fill area
    sf::VertexArray fill(sf::TriangleFan, history.size() + 2);
    fill[0].position = sf::Vector2f(chartX, chartY + chartH);
    fill[0].color = sf::Color(0, 0, 0, 0);
    for (size_t i = 0; i < history.size(); ++i) {
        float px = mapIndexToX(i, history.size(), chartW) + chartX;
        float py = chartY + chartH - mapValueToY(history[i].getTotalValue(), minVal, maxVal, chartH);
        fill[i + 1].position = sf::Vector2f(px, py);
        fill[i + 1].color = CHART_FILL;
    }
    fill[history.size() + 1].position = sf::Vector2f(chartX + chartW, chartY + chartH);
    fill[history.size() + 1].color = sf::Color(0, 0, 0, 0);
    window.draw(fill);

    // Draw dots at each point
    for (size_t i = 0; i < history.size(); ++i) {
        float px = mapIndexToX(i, history.size(), chartW) + chartX;
        float py = chartY + chartH - mapValueToY(history[i].getTotalValue(), minVal, maxVal, chartH);
        sf::CircleShape dot(3);
        dot.setFillColor(CHART_LINE);
        dot.setOrigin(3, 3);
        dot.setPosition(px, py);
        window.draw(dot);
    }

    // Latest value
    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2) << history.back().getTotalValue();
    drawLabel(window, font, oss.str(), chartX + chartW - 80, chartY - 5, 14, CHART_LINE);
}

void ChartRenderer::renderHoldingsBarChart(sf::RenderWindow& window,
                                            const std::vector<std::pair<std::string, double>>& holdings,
                                            const std::string& title) {
    if (holdings.empty()) return;

    sf::RectangleShape bg;
    bg.setPosition(x, y);
    bg.setSize(sf::Vector2f(width, height));
    bg.setFillColor(BG_PANEL);
    window.draw(bg);

    drawLabel(window, font, title, x + 12, y + 8, 16, ACCENT_PRIMARY);

    float barPadding = 20;
    float chartH = height - 60;
    float maxVal = 0;
    for (const auto& h : holdings) {
        if (h.second > maxVal) maxVal = h.second;
    }
    if (maxVal == 0) maxVal = 1;

    float barWidth = (width - barPadding * 2) / holdings.size() - 10;
    for (size_t i = 0; i < holdings.size(); ++i) {
        float barH = (holdings[i].second / maxVal) * chartH * 0.8f;
        float bx = x + barPadding + i * (barWidth + 10);
        float by = y + 50 + chartH * 0.8f - barH;

        sf::RectangleShape bar;
        bar.setPosition(bx, by);
        bar.setSize(sf::Vector2f(barWidth, barH));
        bar.setFillColor(CHART_LINE);
        window.draw(bar);

        // Label
        sf::Text label;
        label.setFont(*font);
        label.setString(holdings[i].first);
        label.setCharacterSize(10);
        label.setFillColor(TEXT_SECONDARY);
        label.setPosition(bx, y + 50 + chartH * 0.8f + 5);
        window.draw(label);
    }
}

void ChartRenderer::renderAllocationChart(sf::RenderWindow& window,
                                           const std::vector<std::pair<std::string, double>>& allocations) {
    if (allocations.empty()) return;

    float centerX = x + width / 2;
    float centerY = y + height / 2;
    float radius = std::min(width, height) * 0.35f;

    double total = 0;
    for (const auto& a : allocations) total += a.second;
    if (total == 0) return;

    sf::Color colors[] = {
        ACCENT_PRIMARY, ACCENT_SUCCESS, ACCENT_SECOND,
        ACCENT_WARNING, ACCENT_DANGER, lighten(ACCENT_PRIMARY, 40),
        lighten(ACCENT_SUCCESS, 40), sf::Color(139, 92, 246)
    };

    float startAngle = 0;
    for (size_t i = 0; i < allocations.size(); ++i) {
        float sweep = (allocations[i].second / total) * 360.0f;

        sf::ConvexShape slice;
        slice.setPointCount(32);
        slice.setPoint(0, sf::Vector2f(centerX, centerY));
        for (int j = 0; j <= 30; ++j) {
            float angle = (startAngle + sweep * j / 30.0f) * 3.14159f / 180.0f;
            slice.setPoint(j + 1, sf::Vector2f(
                centerX + radius * std::cos(angle),
                centerY + radius * std::sin(angle)
            ));
        }
        slice.setFillColor(colors[i % 8]);
        window.draw(slice);

        startAngle += sweep;
    }

    // Center hole (donut)
    sf::CircleShape hole(radius * 0.5f);
    hole.setFillColor(BG_PANEL);
    hole.setOrigin(radius * 0.5f, radius * 0.5f);
    hole.setPosition(centerX, centerY);
    window.draw(hole);
}

float ChartRenderer::mapValueToY(double value, double minVal, double maxVal, float chartH) const {
    return static_cast<float>((value - minVal) / (maxVal - minVal)) * chartH;
}

float ChartRenderer::mapIndexToX(size_t index, size_t total, float chartW) const {
    return static_cast<float>(index) / static_cast<float>(total - 1) * chartW;
}

void ChartRenderer::drawGrid(sf::RenderWindow& window, double minVal, double maxVal) {
    float padding = 50;
    float chartW = width - padding * 2;
    float chartH = height - padding * 2 - 20;
    float chartX = x + padding;
    float chartY = y + padding + 10;

    // Horizontal grid lines
    for (int i = 0; i <= 4; ++i) {
        float gy = chartY + chartH * (i / 4.0f);
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(chartX, gy), CHART_GRID),
            sf::Vertex(sf::Vector2f(chartX + chartW, gy), CHART_GRID)
        };
        window.draw(line, 2, sf::Lines);

        double val = maxVal - (maxVal - minVal) * (i / 4.0f);
        std::ostringstream oss;
        oss << "$" << std::fixed << std::setprecision(0) << val;
        drawLabel(window, font, oss.str(), chartX - 45, gy - 7, 10, TEXT_MUTED);
    }
}

void ChartRenderer::drawAxisLabels(sf::RenderWindow& window, double minVal, double maxVal) {
    // X-axis labels
    drawLabel(window, font, "Time ->", x + width / 2 - 20, y + height - 15, 10, TEXT_MUTED);
}

} // namespace UI
} // namespace ProTrack
