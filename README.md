# 🎮 Campus Esports Arena
### GUI-Based Gaming Tournament Management System Using C++ and Object-Oriented Programming

<p align="center">
  A desktop-based esports tournament management application built using C++, Qt 6, and Object-Oriented Programming concepts.
</p>

---

## 📌 Overview

**Campus Esports Arena** is a GUI-based gaming tournament management system designed to simplify the organization and management of college-level esports tournaments.

The application allows organizers to register players, create teams, select games, generate knockout tournament brackets, manage matches, record scores, track player statistics, and display team leaderboards.

The project demonstrates practical implementation of Object-Oriented Programming concepts through a real-world gaming tournament management scenario.

## ✨ Features

- **Player Management:** Register and maintain player information.
- **Team Management:** Create teams and assign registered players.
- **Multiple Games:** Support for Valorant, BGMI, FIFA, and Chess.
- **Tournament Management:** Configure and create gaming tournaments.
- **Knockout Bracket:** Generate tournament brackets for 4-team and 8-team formats.
- **Match Management:** Record match scores and determine winners.
- **Automatic Progression:** Advance winning teams to subsequent rounds.
- **Player Statistics:** Track matches played, wins, losses, and win rate.
- **Leaderboard:** Display team rankings based on performance.
- **JSON File Storage:** Save and load tournament information locally.
- **Interactive GUI:** Desktop interface developed using Qt 6.
- **Offline Support:** No online gaming server or database required.

## 🛠️ Technology Stack

| Category | Technology |
|---|---|
| Programming Language | C++17 |
| GUI Framework | Qt 6 Widgets |
| Build System | CMake |
| Compiler | GCC / G++ |
| IDE | Visual Studio Code / Qt Creator |
| Data Storage | JSON |
| Operating System | Ubuntu Linux / Windows |
| Data Structures | QVector, Sorting |

## 🏗️ System Architecture

```text
                 USER
                   |
                   v
             +-----------+
             |   Qt GUI  |
             +-----------+
                   |
                   v
          +-------------------+
          | TournamentManager |
          +-------------------+
                   |
          +--------+--------+
          |        |        |
          v        v        v
       Players   Teams   Matches
          |        |        |
          +--------+--------+
                   |
                   v
           +---------------+
           | Tournament    |
           | Management    |
           +---------------+
                   |
          +--------+--------+
          |                 |
          v                 v
       Bracket          Statistics
          |                 |
          +--------+--------+
                   |
                   v
              Leaderboard
                   |
                   v
              JSON Storage
```

## 🧬 Object-Oriented Programming Concepts

The project demonstrates the following OOP concepts:

| OOP Concept | Implementation |
|---|---|
| Class & Object | Player, Team, Match, Game |
| Encapsulation | Private data members and public methods |
| Inheritance | Player inherits from Person |
| Abstraction | Abstract Game base class |
| Polymorphism | Virtual functions in the Game hierarchy |
| Function Overriding | Game-specific implementations |
| Constructors | Object initialization |
| Composition | Managing players and teams through related objects |
| File Handling | JSON save and load operations |
| Exception / Validation Handling | Input and tournament validation |

### Inheritance Hierarchy

```text
              Person
                 |
                 v
               Player


                Game
                  |
       +----------+----------+
       |          |          |
       v          v          v
    Valorant     BGMI       FIFA
                             
                Chess
```

The `Game` class is an abstract base class. Derived game classes implement their own game-specific methods.

## 📂 Project Structure

```text
CampusEsportsArena/
│
├── CMakeLists.txt
│
├── main.cpp
│
├── Models.h
│
├── MainWindow.h
├── MainWindow.cpp
│
├── TournamentManager.h
├── TournamentManager.cpp
│
├── README.md
├── PROJECT_NOTES.md
│
└── build/
    ├── CMakeCache.txt
    ├── Makefile
    └── CampusEsportsArena
```

### File Description

| File | Description |
|---|---|
| `main.cpp` | Application entry point |
| `Models.h` | Defines Person, Player, Team, Game, derived game classes, and Match |
| `MainWindow.h` | Main GUI class declaration |
| `MainWindow.cpp` | Qt GUI implementation |
| `TournamentManager.h` | Tournament management declarations |
| `TournamentManager.cpp` | Player, team, match, bracket, and JSON management |
| `CMakeLists.txt` | Project configuration and build instructions |

## ⚙️ Installation and Setup

### Prerequisites

Install the required packages on Ubuntu Linux:

```bash
sudo apt update

sudo apt install build-essential cmake qt6-base-dev qt6-base-dev-tools
```

Verify the installations:

```bash
g++ --version
cmake --version
```

### Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/Campus-Esports-Arena.git
```

Navigate into the project:

```bash
cd Campus-Esports-Arena
```

### Build the Project

Create a build directory:

```bash
mkdir build
cd build
```

Configure using CMake:

```bash
cmake ..
```

Compile the project:

```bash
cmake --build . -j$(nproc)
```

### Run the Application

```bash
./CampusEsportsArena
```

The Campus Esports Arena GUI should now launch.

## 🎯 Application Workflow

```text
Open Application
       |
       v
Create Tournament
       |
       v
Select Game
       |
       v
Register Players
       |
       v
Create Teams
       |
       v
Generate Tournament
       |
       v
Generate Bracket
       |
       v
Enter Match Scores
       |
       v
Determine Winners
       |
       v
Update Player Statistics
       |
       v
Update Leaderboard
       |
       v
Declare Champion
```

## 🏆 Tournament Example

Example: VIT Campus Esports Championship

**Game:** Valorant

**Teams:**

| Team ID | Team Name |
|---|---|
| T001 | Phoenix |
| T002 | Titans |
| T003 | Warriors |
| T004 | Legends |

**Semi-Final Results:**

| Match | Score | Winner |
|---|---|---|
| Phoenix vs Titans | 13–9 | Phoenix |
| Warriors vs Legends | 8–13 | Legends |

**Final:**

| Match | Score | Champion |
|---|---|---|
| Phoenix vs Legends | 13–11 | Phoenix |

🏆 **Tournament Champion: Phoenix**

## 💾 Data Storage

The application uses JSON for local data persistence.

Tournament information includes:

- Tournament details
- Player information
- Team information
- Match details
- Match scores
- Tournament champion
- Player statistics

Users can save and load tournament data using the application's JSON controls.

## 🧪 Testing

The following scenarios are considered for application testing:

| Test Case | Expected Result |
|---|---|
| Duplicate player ID | Registration rejected |
| Empty player information | Validation error |
| Duplicate team | Team creation rejected |
| Invalid player ID | Team creation rejected |
| Empty team | Tournament validation error |
| Negative match score | Score rejected |
| Equal match score | Tie-break validation required |
| Completed match | Winner determined |
| Final match completed | Champion declared |
| Missing JSON file | Error message displayed |

## 🚀 Future Enhancements

The following features can be added in future versions:

- Improved gaming-themed GUI and dark mode.
- Advanced visual tournament bracket.
- Separate Tournament, Score, Bracket, and Leaderboard classes.
- Player profile editing and deletion.
- Match scheduling with date and time.
- CSV export and import.
- Match history and undo functionality.
- Advanced player and team analytics.
- Player authentication.
- Online tournament management.
- Cloud database integration.
- Live match updates and notifications.
- AI-based player performance analysis.

## 🎓 Academic Information

**Project Name:** Campus Esports Arena

**Project Type:** Object-Oriented Programming Project

**Programming Language:** C++

**GUI Framework:** Qt 6

**Build System:** CMake

**Domain:** Gaming / Esports Tournament Management

**Platform:** Ubuntu Linux / Windows

## 👨‍💻 Author

**Abhishek Jadhav**

GitHub: [@abhij1401](https://github.com/abhij1401)

LinkedIn: [Abhishek Jadhav](https://www.linkedin.com/in/abhishek-jadhav-a1085a2bb)

---

## 📜 License

This project is developed for academic and educational purposes.

---

<p align="center">
  <b>Campus Esports Arena 🎮</b><br>
  Organize. Compete. Win. 🏆
</p>
