#include "GUIManager.h"
#include "../Utils/ColorScheme.h"

namespace ProTrack {
using namespace UI;

GUIManager::GUIManager()
    : window(sf::VideoMode(800, 600), "ProTrack - Investment & Market Analytics", sf::Style::Close),
      currentScreen(AppScreen::LOGIN), notificationVisible(false) {
    window.setFramerateLimit(60);
}

GUIManager::~GUIManager() {
    for (auto& pair : screens) {
        delete pair.second;
    }
}

bool GUIManager::initialize() {
    // Load font
    // Try multiple common system font paths
    const char* fontPaths[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/usr/share/fonts/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "C:/Windows/Fonts/arial.ttf",
        "./assets/DejaVuSans.ttf",
        nullptr
    };

    bool fontLoaded = false;
    for (int i = 0; fontPaths[i] != nullptr; ++i) {
        if (mainFont.loadFromFile(fontPaths[i])) {
            fontLoaded = true;
            break;
        }
    }

    if (!fontLoaded) {
        // If no system font found, we can't run - but let's try one more common path
        if (!mainFont.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
            // Last resort - the program will show boxes instead of text
        }
    }

    // Setup notification UI
    notificationText.setFont(mainFont);
    notificationText.setCharacterSize(13);
    notificationText.setFillColor(TEXT_PRIMARY);

    notificationBg.setSize(sf::Vector2f(400, 30));
    notificationBg.setFillColor(sf::Color(0, 0, 0, 180));

    return true;
}

sf::Font* GUIManager::getMainFont() {
    return &mainFont;
}

void GUIManager::registerScreen(Screen* screen) {
    if (screen) {
        screens[screen->getType()] = screen;
    }
}

void GUIManager::run() {
    while (window.isOpen() && !Screen::shouldQuit) {
        float dt = deltaClock.restart().asSeconds();

        handleEvents();
        update(dt);
        render();

        // Check for screen transitions
        if (Screen::nextScreen != currentScreen) {
            switchScreen(Screen::nextScreen);
        }
    }
}

void GUIManager::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        auto it = screens.find(currentScreen);
        if (it != screens.end()) {
            it->second->handleEvent(event, window);
        }
    }
}

void GUIManager::update(float dt) {
    // Update notification timer
    if (Screen::notificationTimer > 0) {
        Screen::notificationTimer -= dt;
        notificationVisible = true;
        notificationText.setString(Screen::notification);
        sf::FloatRect tb = notificationText.getLocalBounds();
        notificationBg.setSize(sf::Vector2f(tb.width + 40, 32));
        notificationBg.setPosition(400 - (tb.width + 40) / 2, 560);
        notificationText.setPosition(400 - tb.width / 2, 565);
    } else {
        notificationVisible = false;
    }

    auto it = screens.find(currentScreen);
    if (it != screens.end()) {
        it->second->update(dt, window);
    }
}

void GUIManager::render() {
    window.clear(BG_DARK);

    auto it = screens.find(currentScreen);
    if (it != screens.end()) {
        it->second->render(window);
    }

    // Notification overlay
    if (notificationVisible) {
        window.draw(notificationBg);
        window.draw(notificationText);
    }

    // Top bar (always visible except on login)
    if (currentScreen != AppScreen::LOGIN) {
        sf::RectangleShape topBar;
        topBar.setPosition(0, 0);
        topBar.setSize(sf::Vector2f(800, 6));
        topBar.setFillColor(ACCENT_PRIMARY);
        window.draw(topBar);
    }

    window.display();
}

void GUIManager::switchScreen(AppScreen newScreen) {
    auto it = screens.find(currentScreen);
    if (it != screens.end()) {
        it->second->onExit();
    }

    currentScreen = newScreen;

    it = screens.find(currentScreen);
    if (it != screens.end()) {
        it->second->onEnter();
    }
}

} // namespace ProTrack
