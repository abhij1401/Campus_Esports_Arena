# Campus Esports Arena

GUI-based gaming tournament management system for an OOP project.

## Stack
- C++17
- Qt 6 Widgets
- CMake
- JSON local storage
- Ubuntu Linux / Windows

## Features in this starter version
- Player registration
- Team creation
- Game selection: Valorant, BGMI, FIFA, Chess
- 4-team or 8-team knockout tournament
- Automatic bracket progression
- Match score entry
- Winner/champion calculation
- Player statistics
- Team leaderboard
- JSON save/load
- OOP inheritance hierarchy for games

## Build on Ubuntu

Install Qt 6 and CMake first. Then:

```bash
mkdir build
cd build
cmake ..
cmake --build . -j$(nproc)
./CampusEsportsArena
```

If your Qt installation is not found, configure CMake with the Qt installation path, for example:

```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
```

## Demo flow

1. Open Players.
2. Add at least 8 players.
3. Open Teams.
4. Create 4 or 8 teams using player IDs such as `P001,P002,P003`.
5. Open Tournament.
6. Choose a game and click Generate Knockout Tournament.
7. Open Bracket.
8. Enter match scores.
9. Winners advance automatically.
10. View the final champion and Leaderboard.
11. Use Save JSON to persist the tournament.

## OOP mapping

- `Person` -> base class
- `Player` -> derived class
- `Game` -> abstract class
- `Valorant`, `BGMI`, `FIFA`, `Chess` -> derived classes
- `Team`, `Match`, `TournamentManager` -> core domain/management classes
- Encapsulation -> private data members
- Inheritance -> game hierarchy
- Abstraction -> abstract `Game`
- Polymorphism -> virtual game methods
- Composition -> tournament data contains teams/matches
- STL -> QVector and sorting
- File handling -> JSON
- Exception-like validation -> error-return validation in this starter version
