#pragma once
#include "Models.h"
#include <QObject>
#include <QJsonDocument>

class TournamentManager : public QObject {
    Q_OBJECT
    QVector<Player> players;
    QVector<Team> teams;
    QVector<Match> matches;
    QString tournamentName = "Campus Championship";
    QString selectedGame = "Valorant";
    QString champion;

public:
    explicit TournamentManager(QObject* parent = nullptr);

    const QVector<Player>& getPlayers() const { return players; }
    const QVector<Team>& getTeams() const { return teams; }
    const QVector<Match>& getMatches() const { return matches; }
    QString getTournamentName() const { return tournamentName; }
    QString getSelectedGame() const { return selectedGame; }
    QString getChampion() const { return champion; }

    bool addPlayer(const QString& id, const QString& name, QString& error);
    bool addTeam(const QString& id, const QString& name,
                 const QVector<QString>& playerIds, QString& error);

    bool generateBracket(QString& error);
    bool recordScore(int matchId, int scoreA, int scoreB, QString& error);

    void setTournamentName(const QString& n) { tournamentName = n; }
    void setSelectedGame(const QString& g) { selectedGame = g; }

    bool save(const QString& fileName, QString& error) const;
    bool load(const QString& fileName, QString& error);

signals:
    void dataChanged();

private:
    Team* findTeam(const QString& name);
    Player* findPlayer(const QString& id);
    void updatePlayerStats(const QString& teamName, bool won);
    void advanceWinner(int matchId, const QString& winner);
};
