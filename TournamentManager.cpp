#include "TournamentManager.h"
#include <QFile>
#include <QJsonArray>
#include <QSet>

TournamentManager::TournamentManager(QObject* parent) : QObject(parent) {}

Player* TournamentManager::findPlayer(const QString& id) {
    for (auto& p : players)
        if (p.getId() == id) return &p;
    return nullptr;
}

Team* TournamentManager::findTeam(const QString& name) {
    for (auto& t : teams)
        if (t.getName() == name) return &t;
    return nullptr;
}

bool TournamentManager::addPlayer(const QString& id, const QString& name, QString& error) {
    if (id.trimmed().isEmpty() || name.trimmed().isEmpty()) {
        error = "Player ID and name are required.";
        return false;
    }
    if (findPlayer(id.trimmed())) {
        error = "Duplicate player ID.";
        return false;
    }
    players.append(Player(id.trimmed(), name.trimmed()));
    emit dataChanged();
    return true;
}

bool TournamentManager::addTeam(const QString& id, const QString& name,
                                const QVector<QString>& playerIds, QString& error) {
    if (id.trimmed().isEmpty() || name.trimmed().isEmpty()) {
        error = "Team ID and name are required.";
        return false;
    }
    for (const auto& t : teams) {
        if (t.getId() == id.trimmed() || t.getName().compare(name.trimmed(), Qt::CaseInsensitive) == 0) {
            error = "Duplicate team ID or team name.";
            return false;
        }
    }
    if (playerIds.isEmpty()) {
        error = "A team must contain at least one player.";
        return false;
    }
    if (playerIds.size() > 5) {
        error = "A team can contain at most 5 players in this academic version.";
        return false;
    }

    Team t(id.trimmed(), name.trimmed());
    QSet<QString> unique;
    for (const auto& pidRaw : playerIds) {
        QString pid = pidRaw.trimmed();
        if (pid.isEmpty() || unique.contains(pid)) continue;
        if (!findPlayer(pid)) {
            error = "Player ID not found: " + pid;
            return false;
        }
        unique.insert(pid);
        t.addPlayer(pid);
    }

    if (t.getPlayerIds().isEmpty()) {
        error = "No valid players were supplied.";
        return false;
    }

    teams.append(t);
    emit dataChanged();
    return true;
}

bool TournamentManager::generateBracket(QString& error) {
    if (teams.size() != 4 && teams.size() != 8) {
        error = "For this version, create exactly 4 or 8 teams.";
        return false;
    }

    matches.clear();
    champion.clear();

    const QString round = teams.size() == 8 ? "Quarter-Final" : "Semi-Final";
    int id = 1;
    for (int i = 0; i < teams.size(); i += 2) {
        Match m;
        m.id = id++;
        m.round = round;
        m.teamA = teams[i].getName();
        m.teamB = teams[i + 1].getName();
        matches.append(m);
    }

    if (teams.size() == 8) {
        for (int i = 0; i < 2; ++i) {
            Match m;
            m.id = id++;
            m.round = "Semi-Final";
            matches.append(m);
        }
        Match finalMatch;
        finalMatch.id = id++;
        finalMatch.round = "Final";
        matches.append(finalMatch);
    } else {
        Match finalMatch;
        finalMatch.id = id++;
        finalMatch.round = "Final";
        matches.append(finalMatch);
    }

    emit dataChanged();
    return true;
}

void TournamentManager::updatePlayerStats(const QString& teamName, bool won) {
    Team* team = findTeam(teamName);
    if (!team) return;
    if (won) team->addWin();
    else team->addLoss();

    for (const auto& pid : team->getPlayerIds()) {
        if (auto* p = findPlayer(pid))
            p->addResult(won);
    }
}

void TournamentManager::advanceWinner(int matchId, const QString& winner) {
    Match* current = nullptr;
    for (auto& m : matches)
        if (m.id == matchId) { current = &m; break; }
    if (!current) return;

    if (current->round == "Quarter-Final") {
        int completedQF = 0;
        for (const auto& m : matches)
            if (m.round == "Quarter-Final" && m.completed) ++completedQF;

        if (completedQF >= 2) {
            // First two QF winners -> first SF; next two -> second SF.
            QVector<QString> qfWinners;
            for (const auto& m : matches)
                if (m.round == "Quarter-Final" && m.completed)
                    qfWinners.append(m.winner);

            for (auto& m : matches) {
                if (m.round == "Semi-Final" && m.teamA.isEmpty() && qfWinners.size() >= 2) {
                    m.teamA = qfWinners[0];
                    m.teamB = qfWinners[1];
                    qfWinners.remove(0, 2);
                    break;
                }
            }
            for (auto& m : matches) {
                if (m.round == "Semi-Final" && m.teamA.isEmpty() && qfWinners.size() >= 2) {
                    m.teamA = qfWinners[0];
                    m.teamB = qfWinners[1];
                    break;
                }
            }
        }
    }

    // Fill the final once both semifinals are complete.
    int completedSF = 0;
    QVector<QString> sfWinners;
    for (const auto& m : matches) {
        if (m.round == "Semi-Final" && m.completed) {
            ++completedSF;
            sfWinners.append(m.winner);
        }
    }

    if (completedSF == 2) {
        for (auto& m : matches) {
            if (m.round == "Final") {
                m.teamA = sfWinners.value(0);
                m.teamB = sfWinners.value(1);
                break;
            }
        }
    }
}

bool TournamentManager::recordScore(int matchId, int scoreA, int scoreB, QString& error) {
    if (scoreA < 0 || scoreB < 0) {
        error = "Score cannot be negative.";
        return false;
    }
    if (scoreA == scoreB) {
        error = "Equal scores are not accepted. Apply the game's tie-break rule first.";
        return false;
    }

    for (auto& m : matches) {
        if (m.id != matchId) continue;
        if (m.completed) {
            error = "This match is already completed.";
            return false;
        }
        if (m.teamA.isEmpty() || m.teamB.isEmpty()) {
            error = "Both teams are not yet determined.";
            return false;
        }

        m.scoreA = scoreA;
        m.scoreB = scoreB;
        m.completed = true;
        m.winner = scoreA > scoreB ? m.teamA : m.teamB;

        updatePlayerStats(m.teamA, m.winner == m.teamA);
        updatePlayerStats(m.teamB, m.winner == m.teamB);

        if (m.round == "Final") {
            champion = m.winner;
        } else {
            advanceWinner(m.id, m.winner);
        }

        emit dataChanged();
        return true;
    }

    error = "Match ID not found.";
    return false;
}

bool TournamentManager::save(const QString& fileName, QString& error) const {
    QJsonObject root;
    root["tournamentName"] = tournamentName;
    root["selectedGame"] = selectedGame;
    root["champion"] = champion;

    QJsonArray pa;
    for (const auto& p : players) pa.append(p.toJson());
    root["players"] = pa;

    QJsonArray ta;
    for (const auto& t : teams) ta.append(t.toJson());
    root["teams"] = ta;

    QJsonArray ma;
    for (const auto& m : matches) ma.append(m.toJson());
    root["matches"] = ma;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        error = file.errorString();
        return false;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool TournamentManager::load(const QString& fileName, QString& error) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        error = file.errorString();
        return false;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        error = "Invalid JSON file.";
        return false;
    }

    QJsonObject root = doc.object();
    tournamentName = root["tournamentName"].toString("Campus Championship");
    selectedGame = root["selectedGame"].toString("Valorant");
    champion = root["champion"].toString();

    players.clear();
    teams.clear();
    matches.clear();

    for (const auto& v : root["players"].toArray())
        players.append(Player::fromJson(v.toObject()));
    for (const auto& v : root["teams"].toArray())
        teams.append(Team::fromJson(v.toObject()));
    for (const auto& v : root["matches"].toArray())
        matches.append(Match::fromJson(v.toObject()));

    emit dataChanged();
    return true;
}
