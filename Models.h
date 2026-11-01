#pragma once
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>

class Person {
protected:
    QString id;
    QString name;
public:
    Person() = default;
    Person(const QString& id, const QString& name) : id(id), name(name) {}
    virtual ~Person() = default;

    QString getId() const { return id; }
    QString getName() const { return name; }
};

class Player : public Person {
    int matchesPlayed = 0;
    int wins = 0;
    int losses = 0;
    int mvpCount = 0;
public:
    Player() = default;
    Player(const QString& id, const QString& name)
        : Person(id, name) {}

    void addResult(bool won, bool mvp = false) {
        ++matchesPlayed;
        if (won) ++wins;
        else ++losses;
        if (mvp) ++mvpCount;
    }

    int getMatchesPlayed() const { return matchesPlayed; }
    int getWins() const { return wins; }
    int getLosses() const { return losses; }
    int getMvpCount() const { return mvpCount; }
    double winRate() const {
        return matchesPlayed == 0 ? 0.0 : (100.0 * wins / matchesPlayed);
    }

    QJsonObject toJson() const {
        return {
            {"id", id}, {"name", name},
            {"matchesPlayed", matchesPlayed},
            {"wins", wins}, {"losses", losses},
            {"mvpCount", mvpCount}
        };
    }

    static Player fromJson(const QJsonObject& o) {
        Player p(o["id"].toString(), o["name"].toString());
        p.matchesPlayed = o["matchesPlayed"].toInt();
        p.wins = o["wins"].toInt();
        p.losses = o["losses"].toInt();
        p.mvpCount = o["mvpCount"].toInt();
        return p;
    }
};

class Team {
    QString id;
    QString name;
    QVector<QString> playerIds;
    int wins = 0;
    int losses = 0;
public:
    Team() = default;
    Team(const QString& id, const QString& name) : id(id), name(name) {}

    QString getId() const { return id; }
    QString getName() const { return name; }
    const QVector<QString>& getPlayerIds() const { return playerIds; }
    int getWins() const { return wins; }
    int getLosses() const { return losses; }

    bool addPlayer(const QString& playerId) {
        if (playerIds.size() >= 5 || playerIds.contains(playerId))
            return false;
        playerIds.append(playerId);
        return true;
    }

    void addWin() { ++wins; }
    void addLoss() { ++losses; }

    QJsonObject toJson() const {
        QJsonArray arr;
        for (const auto& id : playerIds) arr.append(id);
        return {
            {"id", id}, {"name", name}, {"players", arr},
            {"wins", wins}, {"losses", losses}
        };
    }

    static Team fromJson(const QJsonObject& o) {
        Team t(o["id"].toString(), o["name"].toString());
        for (const auto& v : o["players"].toArray()) t.playerIds.append(v.toString());
        t.wins = o["wins"].toInt();
        t.losses = o["losses"].toInt();
        return t;
    }
};

// Abstraction + runtime polymorphism.
class Game {
public:
    virtual ~Game() = default;
    virtual QString name() const = 0;
    virtual int maxTeamSize() const = 0;
    virtual QString scoringDescription() const = 0;
};

class Valorant : public Game {
public:
    QString name() const override { return "Valorant"; }
    int maxTeamSize() const override { return 5; }
    QString scoringDescription() const override { return "Round-based score"; }
};

class BGMI : public Game {
public:
    QString name() const override { return "BGMI"; }
    int maxTeamSize() const override { return 4; }
    QString scoringDescription() const override { return "Placement / points"; }
};

class FIFA : public Game {
public:
    QString name() const override { return "FIFA"; }
    int maxTeamSize() const override { return 1; }
    QString scoringDescription() const override { return "Goals"; }
};

class Chess : public Game {
public:
    QString name() const override { return "Chess"; }
    int maxTeamSize() const override { return 1; }
    QString scoringDescription() const override { return "Game points"; }
};

struct Match {
    int id = 0;
    QString round;
    QString teamA;
    QString teamB;
    int scoreA = 0;
    int scoreB = 0;
    bool completed = false;
    QString winner;

    QJsonObject toJson() const {
        return {
            {"id", id}, {"round", round},
            {"teamA", teamA}, {"teamB", teamB},
            {"scoreA", scoreA}, {"scoreB", scoreB},
            {"completed", completed}, {"winner", winner}
        };
    }

    static Match fromJson(const QJsonObject& o) {
        Match m;
        m.id = o["id"].toInt();
        m.round = o["round"].toString();
        m.teamA = o["teamA"].toString();
        m.teamB = o["teamB"].toString();
        m.scoreA = o["scoreA"].toInt();
        m.scoreB = o["scoreB"].toInt();
        m.completed = o["completed"].toBool();
        m.winner = o["winner"].toString();
        return m;
    }
};
