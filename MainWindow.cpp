#include "MainWindow.h"

#include <QApplication>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QSplitter>
#include <QStatusBar>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDateTime>
#include <algorithm>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Campus Esports Arena");
    resize(1150, 750);

    tabs = new QTabWidget(this);
    tabs->addTab(createDashboardTab(), "Dashboard");
    tabs->addTab(createPlayersTab(), "Players");
    tabs->addTab(createTeamsTab(), "Teams");
    tabs->addTab(createTournamentTab(), "Tournament");
    tabs->addTab(createBracketTab(), "Bracket");
    tabs->addTab(createLeaderboardTab(), "Leaderboard");
    setCentralWidget(tabs);

    connect(&manager, &TournamentManager::dataChanged, this, &MainWindow::refreshAll);

    auto* saveBtn = new QPushButton("Save JSON");
    auto* loadBtn = new QPushButton("Load JSON");
    statusBar()->addPermanentWidget(saveBtn);
    statusBar()->addPermanentWidget(loadBtn);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveData);
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadData);

    refreshAll();
}

QWidget* MainWindow::createDashboardTab() {
    auto* w = new QWidget;
    auto* layout = new QVBoxLayout(w);

    auto* title = new QLabel("<h1>🎮 Campus Esports Arena</h1>");
    auto* subtitle = new QLabel("GUI-Based Gaming Tournament Management System");
    dashboardInfo = new QLabel;
    championLabel = new QLabel;
    dashboardInfo->setStyleSheet("font-size: 18px; padding: 20px;");
    championLabel->setStyleSheet("font-size: 22px; font-weight: bold; padding: 20px;");

    auto* infoBox = new QGroupBox("Tournament Overview");
    auto* infoLayout = new QVBoxLayout(infoBox);
    infoLayout->addWidget(dashboardInfo);
    infoLayout->addWidget(championLabel);

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addWidget(infoBox);
    layout->addStretch();
    return w;
}

QWidget* MainWindow::createPlayersTab() {
    auto* w = new QWidget;
    auto* layout = new QVBoxLayout(w);

    auto* form = new QHBoxLayout;
    playerIdEdit = new QLineEdit;
    playerIdEdit->setPlaceholderText("P001");
    playerNameEdit = new QLineEdit;
    playerNameEdit->setPlaceholderText("Player name");
    auto* addBtn = new QPushButton("Add Player");

    form->addWidget(new QLabel("ID:"));
    form->addWidget(playerIdEdit);
    form->addWidget(new QLabel("Name:"));
    form->addWidget(playerNameEdit);
    form->addWidget(addBtn);

    playerTable = new QTableWidget;
    playerTable->setColumnCount(6);
    playerTable->setHorizontalHeaderLabels({"ID", "Name", "Matches", "Wins", "Losses", "Win Rate"});
    playerTable->horizontalHeader()->setStretchLastSection(true);

    layout->addLayout(form);
    layout->addWidget(playerTable);
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addPlayer);
    return w;
}

QWidget* MainWindow::createTeamsTab() {
    auto* w = new QWidget;
    auto* layout = new QVBoxLayout(w);

    auto* form = new QFormLayout;
    teamIdEdit = new QLineEdit;
    teamNameEdit = new QLineEdit;
    teamPlayersEdit = new QLineEdit;
    teamPlayersEdit->setPlaceholderText("P001,P002,P003...");
    form->addRow("Team ID:", teamIdEdit);
    form->addRow("Team Name:", teamNameEdit);
    form->addRow("Player IDs:", teamPlayersEdit);

    auto* addBtn = new QPushButton("Create Team");
    layout->addLayout(form);
    layout->addWidget(addBtn);

    teamTable = new QTableWidget;
    teamTable->setColumnCount(5);
    teamTable->setHorizontalHeaderLabels({"ID", "Team", "Players", "Wins", "Losses"});
    teamTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(teamTable);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addTeam);
    return w;
}

QWidget* MainWindow::createTournamentTab() {
    auto* w = new QWidget;
    auto* layout = new QVBoxLayout(w);

    auto* form = new QFormLayout;
    tournamentNameEdit = new QLineEdit("Campus Championship");
    gameCombo = new QComboBox;
    gameCombo->addItems({"Valorant", "BGMI", "FIFA", "Chess"});
    form->addRow("Tournament Name:", tournamentNameEdit);
    form->addRow("Game:", gameCombo);

    auto* generateBtn = new QPushButton("Generate Knockout Tournament");
    generateBtn->setMinimumHeight(45);

    layout->addLayout(form);
    layout->addWidget(generateBtn);
    layout->addWidget(new QLabel(
        "Academic version: create exactly 4 or 8 teams, then generate the knockout bracket."
    ));
    layout->addStretch();

    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::generateTournament);
    return w;
}

QWidget* MainWindow::createBracketTab() {
    auto* w = new QWidget;
    auto* layout = new QVBoxLayout(w);

    bracketView = new QTextEdit;
    bracketView->setReadOnly(true);
    bracketView->setStyleSheet("font-family: monospace; font-size: 15px;");

    auto* scoreBtn = new QPushButton("Enter / Update Match Score");
    layout->addWidget(new QLabel("<h2>Tournament Bracket</h2>"));
    layout->addWidget(bracketView);
    layout->addWidget(scoreBtn);

    connect(scoreBtn, &QPushButton::clicked, this, &MainWindow::enterScore);
    return w;
}

QWidget* MainWindow::createLeaderboardTab() {
    auto* w = new QWidget;
    auto* layout = new QVBoxLayout(w);

    leaderboardTable = new QTableWidget;
    leaderboardTable->setColumnCount(5);
    leaderboardTable->setHorizontalHeaderLabels({"Rank", "Team", "Wins", "Losses", "Win %"});
    leaderboardTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(new QLabel("<h2>Team Leaderboard</h2>"));
    layout->addWidget(leaderboardTable);
    return w;
}

void MainWindow::addPlayer() {
    QString error;
    if (!manager.addPlayer(playerIdEdit->text(), playerNameEdit->text(), error)) {
        QMessageBox::warning(this, "Registration Error", error);
        return;
    }
    playerIdEdit->clear();
    playerNameEdit->clear();
}

void MainWindow::addTeam() {
    QVector<QString> ids = teamPlayersEdit->text().split(",", Qt::SkipEmptyParts);
    QString error;
    if (!manager.addTeam(teamIdEdit->text(), teamNameEdit->text(), ids, error)) {
        QMessageBox::warning(this, "Team Error", error);
        return;
    }
    teamIdEdit->clear();
    teamNameEdit->clear();
    teamPlayersEdit->clear();
}

void MainWindow::generateTournament() {
    manager.setTournamentName(tournamentNameEdit->text());
    manager.setSelectedGame(gameCombo->currentText());

    QString error;
    if (!manager.generateBracket(error)) {
        QMessageBox::warning(this, "Tournament Error", error);
        return;
    }
    QMessageBox::information(this, "Tournament Created",
                             "Knockout bracket generated successfully.");
    tabs->setCurrentIndex(4);
}

void MainWindow::enterScore() {
    if (manager.getMatches().isEmpty()) {
        QMessageBox::information(this, "No Tournament", "Generate a tournament first.");
        return;
    }

    QStringList available;
    for (const auto& m : manager.getMatches()) {
        if (!m.completed && !m.teamA.isEmpty() && !m.teamB.isEmpty())
            available << QString("#%1 | %2 | %3 vs %4")
                         .arg(m.id).arg(m.round).arg(m.teamA).arg(m.teamB);
    }

    if (available.isEmpty()) {
        QMessageBox::information(this, "No Match",
                                 "There is no currently playable match.");
        return;
    }

    bool ok = false;
    QString selected = QInputDialog::getItem(this, "Select Match",
                                             "Match:", available, 0, false, &ok);
    if (!ok) return;

    int id = selected.section("|", 0, 0).remove("#").trimmed().toInt();
    int a = QInputDialog::getInt(this, "Score", "Team A score:", 0, 0, 999, 1, &ok);
    if (!ok) return;
    int b = QInputDialog::getInt(this, "Score", "Team B score:", 0, 0, 999, 1, &ok);
    if (!ok) return;

    QString error;
    if (!manager.recordScore(id, a, b, error)) {
        QMessageBox::warning(this, "Score Error", error);
        return;
    }

    QMessageBox::information(this, "Match Completed",
                             "Winner recorded and tournament progression updated.");
}

void MainWindow::saveData() {
    QString fileName = QFileDialog::getSaveFileName(
        this, "Save Tournament", "campus_esports_arena.json", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QString error;
    if (!manager.save(fileName, error))
        QMessageBox::critical(this, "Save Error", error);
    else
        statusBar()->showMessage("Tournament saved.", 3000);
}

void MainWindow::loadData() {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Load Tournament", QString(), "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QString error;
    if (!manager.load(fileName, error))
        QMessageBox::critical(this, "Load Error", error);
    else
        statusBar()->showMessage("Tournament loaded.", 3000);
}

void MainWindow::refreshPlayers() {
    playerTable->setRowCount(manager.getPlayers().size());
    for (int r = 0; r < manager.getPlayers().size(); ++r) {
        const auto& p = manager.getPlayers()[r];
        playerTable->setItem(r, 0, new QTableWidgetItem(p.getId()));
        playerTable->setItem(r, 1, new QTableWidgetItem(p.getName()));
        playerTable->setItem(r, 2, new QTableWidgetItem(QString::number(p.getMatchesPlayed())));
        playerTable->setItem(r, 3, new QTableWidgetItem(QString::number(p.getWins())));
        playerTable->setItem(r, 4, new QTableWidgetItem(QString::number(p.getLosses())));
        playerTable->setItem(r, 5, new QTableWidgetItem(QString::number(p.winRate(), 'f', 1) + "%"));
    }
}

void MainWindow::refreshTeams() {
    teamTable->setRowCount(manager.getTeams().size());
    for (int r = 0; r < manager.getTeams().size(); ++r) {
        const auto& t = manager.getTeams()[r];
        teamTable->setItem(r, 0, new QTableWidgetItem(t.getId()));
        teamTable->setItem(r, 1, new QTableWidgetItem(t.getName()));
        teamTable->setItem(r, 2, new QTableWidgetItem(QString::number(t.getPlayerIds().size())));
        teamTable->setItem(r, 3, new QTableWidgetItem(QString::number(t.getWins())));
        teamTable->setItem(r, 4, new QTableWidgetItem(QString::number(t.getLosses())));
    }
}

void MainWindow::refreshMatches() {
    if (!matchTable) return;
    matchTable->setRowCount(manager.getMatches().size());
    for (int r = 0; r < manager.getMatches().size(); ++r) {
        const auto& m = manager.getMatches()[r];
        matchTable->setItem(r, 0, new QTableWidgetItem(QString::number(m.id)));
        matchTable->setItem(r, 1, new QTableWidgetItem(m.round));
        matchTable->setItem(r, 2, new QTableWidgetItem(m.teamA));
        matchTable->setItem(r, 3, new QTableWidgetItem(m.teamB));
        matchTable->setItem(r, 4, new QTableWidgetItem(QString("%1 - %2").arg(m.scoreA).arg(m.scoreB)));
        matchTable->setItem(r, 5, new QTableWidgetItem(m.completed ? m.winner : "Pending"));
    }
}

void MainWindow::refreshBracket() {
    QString text;
    text += manager.getTournamentName() + " | " + manager.getSelectedGame() + "\n\n";

    for (const QString& round : {"Quarter-Final", "Semi-Final", "Final"}) {
        bool found = false;
        text += "========== " + round + " ==========\n";
        for (const auto& m : manager.getMatches()) {
            if (m.round != round) continue;
            found = true;
            QString status = m.completed
                ? QString("%1 - %2  -> Winner: %3").arg(m.scoreA).arg(m.scoreB).arg(m.winner)
                : "PENDING";
            text += QString("Match #%1: %2  vs  %3  [%4]\n")
                        .arg(m.id)
                        .arg(m.teamA.isEmpty() ? "TBD" : m.teamA)
                        .arg(m.teamB.isEmpty() ? "TBD" : m.teamB)
                        .arg(status);
        }
        if (!found) text += "No matches.\n";
        text += "\n";
    }

    if (!manager.getChampion().isEmpty())
        text += "🏆 CHAMPION: " + manager.getChampion() + "\n";

    bracketView->setPlainText(text);
}

void MainWindow::refreshLeaderboard() {
    struct Row { QString name; int wins; int losses; };
    QVector<Row> rows;
    for (const auto& t : manager.getTeams())
        rows.append({t.getName(), t.getWins(), t.getLosses()});

    std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
        if (a.wins != b.wins) return a.wins > b.wins;
        return a.losses < b.losses;
    });

    leaderboardTable->setRowCount(rows.size());
    for (int r = 0; r < rows.size(); ++r) {
        const auto& x = rows[r];
        int games = x.wins + x.losses;
        double rate = games ? 100.0 * x.wins / games : 0.0;
        leaderboardTable->setItem(r, 0, new QTableWidgetItem(QString::number(r + 1)));
        leaderboardTable->setItem(r, 1, new QTableWidgetItem(x.name));
        leaderboardTable->setItem(r, 2, new QTableWidgetItem(QString::number(x.wins)));
        leaderboardTable->setItem(r, 3, new QTableWidgetItem(QString::number(x.losses)));
        leaderboardTable->setItem(r, 4, new QTableWidgetItem(QString::number(rate, 'f', 1) + "%"));
    }
}

void MainWindow::refreshDashboard() {
    dashboardInfo->setText(
        QString("Tournament: <b>%1</b><br>"
                "Game: <b>%2</b><br>"
                "Players: <b>%3</b><br>"
                "Teams: <b>%4</b><br>"
                "Matches: <b>%5</b>")
        .arg(manager.getTournamentName())
        .arg(manager.getSelectedGame())
        .arg(manager.getPlayers().size())
        .arg(manager.getTeams().size())
        .arg(manager.getMatches().size()));

    championLabel->setText(manager.getChampion().isEmpty()
                            ? "🏆 Champion: Tournament in progress"
                            : "🏆 Champion: " + manager.getChampion());
}

void MainWindow::refreshAll() {
    refreshPlayers();
    refreshTeams();
    refreshMatches();
    refreshBracket();
    refreshLeaderboard();
    refreshDashboard();
}
