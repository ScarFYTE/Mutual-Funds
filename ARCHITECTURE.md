# ProTrack Architecture Overview

## System Design

ProTrack follows a layered architecture inspired by MVC (Model-View separation):

```
+---------------------------------------------------+
|                   PRESENTATION                     |
|  +----------+ +----------+ +----------+          |
|  |  Login   | | Dashboard| |  Market  |          |
|  |  Screen  | |  Screen  | |  Screen  |          |
|  +----------+ +----------+ +----------+          |
|  +----------+ +----------+ +----------+          |
|  | Portfolio| |  Trade   | | Settings |          |
|  |  Screen  | |  Screen  | |  Screen  |          |
|  +----------+ +----------+ +----------+          |
|  +----------+ +----------+                        |
|  |Performance| | GUI Mgr |                        |
|  |  Screen   | |         |                        |
|  +----------+ +----------+                        |
+---------------------------------------------------+
|                    UI UTILITIES                    |
|  Button | TextBox | Panel | ChartRenderer | Colors |
+---------------------------------------------------+
|                   DATA / PERSISTENCE               |
|  DataPersistence (abstract) | FilePersistence      |
+---------------------------------------------------+
|                    DOMAIN MODEL                    |
|  MutualFund | Investment | Snapshot | Transaction  |
|  Portfolio  | Investor   | FundManager             |
+---------------------------------------------------+
```

## Class Relationships

### Inheritance Hierarchy
```
Screen (abstract)
  +-- LoginScreen
  +-- DashboardScreen
  +-- MarketScreen
  +-- PortfolioScreen
  +-- TradeScreen
  +-- PerformanceScreen
  +-- SettingsScreen

DataPersistence (abstract)
  +-- FilePersistence
```

### Composition (Has-A) Relationships
```
GUIManager ----> 7 Screen objects (composition)
Investor -----> Portfolio (composition)
Portfolio ----> Investment[] (composition)
Portfolio ----> Snapshot[] (composition)
Portfolio ----> Transaction[] (composition)
FundManager ---> MutualFund[] (composition via unique_ptr)
Dashboard ---> ChartRenderer (composition)
Performance ---> ChartRenderer[2] (composition)
```

### Association (Uses-A) Relationships
```
Investor --------> FundManager (association: buy/sell need fund lookup)
Portfolio --------> FundManager (association: calculate values)
FilePersistence --> Investor (association: save/load)
FilePersistence --> FundManager (association: validate fund IDs)
```

## Screen State Machine

```
                    +-----------+
                    |   LOGIN   |
                    +-----+-----+
                          |
                          v
                    +-----------+
         +---------| DASHBOARD |---------+
         |         +-----------+         |
         |             |   |             |
         v             v   v             v
    +---------+   +---------+   +----------+   +-----------+
    |  MARKET |   |PORTFOLIO|   |  TRADE   |   |SETTINGS   |
    +---------+   +---------+   +----------+   +-----------+
                                         |          |
                                         v          v
                                   +-----------+   |
                                   |PERFORMANCE|   |
                                   +-----------+   |
                                        |          |
                                        +----------+
                                               |
                                               v
                                         +-----------+
                                         | DASHBOARD |
                                         +-----------+
```

## Key Design Decisions

1. **Static Screen State:** `Screen` static members allow screens to communicate without tight coupling
2. **Unique_ptr for Funds:** `FundManager` owns funds exclusively via `unique_ptr` (no memory leaks)
3. **Abstract DataPersistence:** Enables future database backend without changing business logic
4. **ChartRenderer Abstraction:** Reusable charting component for different visualization needs
5. **SFML Native Drawing:** All UI rendered with SFML primitives (no external UI library dependency)

## File Count: 43 files | Total Lines: ~4000 lines of C++
