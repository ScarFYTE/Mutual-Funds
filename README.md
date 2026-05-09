# ProTrack v2.0 - Investment & Market Analytics System

**Course:** Object Oriented Programming (CS201)  
**Institution:** Institute of Business Administration (IBA), Karachi  
**Platform:** SFML 2.5+ GUI Application  
**Language:** C++11

---

## Project Overview

ProTrack is a full-featured mutual fund investment tracker with a graphical user interface built using **SFML**. It simulates a dynamic market environment where users can buy/sell funds, track portfolio performance, and visualize investment analytics.

This is the **full-scale GUI evolution** of the original console-based prototype (Deliverable 1), demonstrating advanced Object-Oriented Programming principles throughout.

---

## OOP Principles Demonstrated

| Principle | Implementation |
|-----------|---------------|
| **Encapsulation** | All domain classes (`MutualFund`, `Investment`, `Portfolio`, `Investor`) use private data members with public getter/setter interfaces |
| **Inheritance** | `Screen` abstract base class with 7 concrete screen implementations (`LoginScreen`, `DashboardScreen`, etc.) |
| **Polymorphism** | `Screen::render()`, `update()`, `handleEvent()` are pure virtual. `DataPersistence` abstract base with `FilePersistence` concrete implementation |
| **Abstraction** | `DataPersistence` interface hides file I/O complexity. `FundManager` abstracts fund registry operations |
| **Composition** | `Portfolio` has-a collection of `Investment`s; `Investor` has-a `Portfolio`; `GUIManager` has-a collection of `Screen`s |
| **Association** | `Investor` interacts with `FundManager` for buy/sell operations |
| **RAII** | Smart pointers (`unique_ptr`) manage fund memory; fstream RAII in `FilePersistence` |
| **Const-Correctness** | All accessor methods marked `const`; const-reference parameters throughout |
| **Enum Classes** | `FundCategory`, `RiskLevel`, `AppScreen`, `TransactionType` provide type-safe constants |

---

## Project Structure

```
ProTrack/
 CMakeLists.txt           # CMake build configuration
 README.md                # This file
 src/
  main.cpp                # Application entry point
  Core/                   # Domain Model Layer
   Enums.h                # Type-safe enumerations
   MutualFund.h/cpp       # Fund data with NAV simulation
   Investment.h/cpp       # Single holding record
   Snapshot.h/cpp         # Portfolio value snapshot
   Transaction.h/cpp      # Buy/sell transaction record
   Portfolio.h/cpp        # Collection of investments + analytics
   Investor.h/cpp         # User with balance and portfolio
   FundManager.h/cpp      # Registry of available funds
  Data/                   # Persistence Layer
   DataPersistence.h      # Abstract base (interface)
   FilePersistence.h/cpp  # Binary file save/load
  GUI/                    # Presentation Layer
   Screen.h/cpp           # Abstract screen base class
   LoginScreen.h/cpp      # User registration
   DashboardScreen.h/cpp  # Main overview with metrics
   MarketScreen.h/cpp     # Browse/search available funds
   PortfolioScreen.h/cpp  # View and sell holdings
   TradeScreen.h/cpp      # Advanced buy/sell interface
   PerformanceScreen.h/cpp # Charts and transaction history
   SettingsScreen.h/cpp   # Save/load and cash management
   GUIManager.h/cpp       # Window + screen state machine
  Utils/                  # UI Components
   ColorScheme.h          # Centralized color palette
   Button.h/cpp           # Interactive button
   TextBox.h/cpp          # Text input field
   Panel.h/cpp            # Container with title
   ChartRenderer.h/cpp    # Line, bar, donut charts
```

---

## Features

### Market Engine
- 10 diversified mutual funds across 6 categories (Equity, Debt, Hybrid, Index, Sectoral, International)
- Realistic NAV simulation with risk-based volatility
- Risk levels (Low, Moderate, High, Very High) affect price movement magnitude
- Top gainers/losers tracking

### Investment Operations
- **Buy funds** by amount (units auto-calculated)
- **Sell funds** by units (proceeds added to cash)
- **Search** funds by name
- **Tab-switch** between Buy and Sell modes

### Real-time Analytics
- Portfolio value calculation
- Profit/Loss per holding and aggregate
- Return percentage tracking
- Net worth (cash + portfolio)
- Performance snapshots recorded on every transaction

### Performance Visualization
- **Line chart:** Portfolio value over time
- **Donut chart:** Holdings allocation by value
- **Transaction history table:** Complete audit trail

### State Persistence
- Save portfolio to binary `.ptk` files
- Load previously saved portfolios
- List available save files
- Add cash to balance

---

## Compilation Instructions

### Prerequisites
- **SFML 2.5+** installed on your system
- **CMake 3.10+**
- **C++11 compatible compiler** (GCC, Clang, MSVC)

### Installing SFML

**Ubuntu/Debian:**
```bash
sudo apt-get install libsfml-dev
```

**Fedora:**
```bash
sudo dnf install SFML-devel
```

**macOS (Homebrew):**
```bash
brew install sfml
```

**Windows:**
Download SFML from https://www.sfml-dev.org/download.php and follow their setup guide.

### Building with CMake

```bash
cd ProTrack
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Building Manually (without CMake)

**Linux/GCC:**
```bash
g++ -std=c++11 -O2 -o protrack \
    src/main.cpp \
    src/GUI/Screen.cpp \
    src/GUI/LoginScreen.cpp \
    src/GUI/DashboardScreen.cpp \
    src/GUI/MarketScreen.cpp \
    src/GUI/PortfolioScreen.cpp \
    src/GUI/TradeScreen.cpp \
    src/GUI/PerformanceScreen.cpp \
    src/GUI/SettingsScreen.cpp \
    src/GUI/GUIManager.cpp \
    src/Core/MutualFund.cpp \
    src/Core/Investment.cpp \
    src/Core/Snapshot.cpp \
    src/Core/Transaction.cpp \
    src/Core/Portfolio.cpp \
    src/Core/Investor.cpp \
    src/Core/FundManager.cpp \
    src/Data/FilePersistence.cpp \
    src/Utils/Button.cpp \
    src/Utils/TextBox.cpp \
    src/Utils/Panel.cpp \
    src/Utils/ChartRenderer.cpp \
    -I src -lsfml-graphics -lsfml-window -lsfml-system
```

**macOS:**
```bash
# Same source list, but link with:
-framework sfml-graphics -framework sfml-window -framework sfml-system
```

**Windows (MinGW):**
```bash
# Same source list, but link with:
-lsfml-graphics -lsfml-window -lsfml-system
```

### Running

```bash
./protrack
```

> **Note:** If fonts don't load (you see boxes instead of text), install DejaVu fonts:
> ```bash
> sudo apt-get install fonts-dejavu
> ```

---

## User Guide

1. **Login Screen:** Enter your name and starting balance, then click "Enter Market"
2. **Dashboard:** See your net worth, portfolio value, P&L, and quick stats. Use the sidebar to navigate
3. **Market:** Browse 10 available funds. Click a row to select, enter an amount, and click "Buy"
4. **Portfolio:** View your holdings with live P&L. Select a holding, enter units, and click "Sell"
5. **Trade:** Advanced trading desk with tab-switch between Buy (by amount) and Sell (by units)
6. **Performance:** View portfolio trend chart, allocation donut chart, and full transaction history
7. **Settings:** Save your portfolio, load previous saves, or add cash to your balance

Use **"Next Day >>"** on the Dashboard to simulate market movements and watch prices change!

---

## Test Case Coverage (from Deliverable 1)

| Test Case | Feature | Status |
|-----------|---------|--------|
| TC-01 | Buy Fund (Invest in Fund) | Implemented |
| TC-02 | Market Update (updateAll) | Implemented via "Next Day >>" |
| TC-03 | Snapshot Recording | Implemented - auto on every transaction |
| TC-04 | Portfolio View | Implemented with live P&L |
| TC-05 | Insufficient Balance Error | Implemented with notification system |

---

## Screenshots

*(Run the application to see the full GUI with professional dark-mode styling)*

---

## Group Members

- Raman Kumar (Computer Science, Freshman)
- Group Member 2
- Group Member 3

**Institution:** Institute of Business Administration (IBA), Karachi

---

## License

Academic project for educational purposes.
