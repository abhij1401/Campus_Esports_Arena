#pragma once
#include <QMainWindow>
#include "TournamentManager.h"

class QTabWidget;
class QLabel;
class QLineEdit;
class QComboBox;
class QTableWidget;
class QTextEdit;
class QSpinBox;

class MainWindow : public QMainWindow {
    Q_OBJECT

    TournamentManager manager;

    QTabWidget* tabs = nullptr;
    QLabel* dashboardInfo = nullptr;
    QLabel* championLabel = nullptr;

    QLineEdit* playerIdEdit = nullptr;
    QLineEdit* playerNameEdit = nullptr;
    QTableWidget* playerTable = nullptr;

    QLineEdit* teamIdEdit = nullptr;
    QLineEdit* teamNameEdit = nullptr;
    QLineEdit* teamPlayersEdit = nullptr;
    QTableWidget* teamTable = nullptr;

    QLineEdit* tournamentNameEdit = nullptr;
    QComboBox* gameCombo = nullptr;
    QTableWidget* matchTable = nullptr;
    QTableWidget* leaderboardTable = nullptr;
    QTextEdit* bracketView = nullptr;

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void refreshAll();
    void addPlayer();
    void addTeam();
    void generateTournament();
    void enterScore();
    void saveData();
    void loadData();

private:
    QWidget* createDashboardTab();
    QWidget* createPlayersTab();
    QWidget* createTeamsTab();
    QWidget* createTournamentTab();
    QWidget* createBracketTab();
    QWidget* createLeaderboardTab();
    void refreshPlayers();
    void refreshTeams();
    void refreshMatches();
    void refreshBracket();
    void refreshLeaderboard();
    void refreshDashboard();
};
