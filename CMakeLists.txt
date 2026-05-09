cmake_minimum_required(VERSION 3.10)
project(ProTrack VERSION 2.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Collect all source files
set(SOURCES
    src/main.cpp
    src/GUI/Screen.cpp
    src/GUI/LoginScreen.cpp
    src/GUI/DashboardScreen.cpp
    src/GUI/MarketScreen.cpp
    src/GUI/PortfolioScreen.cpp
    src/GUI/TradeScreen.cpp
    src/GUI/PerformanceScreen.cpp
    src/GUI/SettingsScreen.cpp
    src/GUI/GUIManager.cpp
    src/Core/MutualFund.cpp
    src/Core/Investment.cpp
    src/Core/Snapshot.cpp
    src/Core/Transaction.cpp
    src/Core/Portfolio.cpp
    src/Core/Investor.cpp
    src/Core/FundManager.cpp
    src/Data/FilePersistence.cpp
    src/Utils/Button.cpp
    src/Utils/TextBox.cpp
    src/Utils/Panel.cpp
    src/Utils/ChartRenderer.cpp
)

# Create executable
add_executable(protrack ${SOURCES})

# Include directories
target_include_directories(protrack PRIVATE src)

# Link SFML
target_link_libraries(protrack PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# Platform-specific settings
if(WIN32)
    # Copy SFML DLLs on Windows (if needed)
    set_target_properties(protrack PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

# Install target
install(TARGETS protrack DESTINATION bin)
