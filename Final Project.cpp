#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

struct Player {
    string name;
    int totalPoints = 0;
    int gamesPlayed = 0;

    double averagePoints() const {
        return gamesPlayed ? (double)totalPoints / gamesPlayed : 0.0;
    }
};

struct Team {
    string name;
    vector<Player> players;
    int totalPoints = 0;
    int gamesPlayed = 0;
    int wins = 0, losses = 0;

    void addPlayer(const string& playerName) {
        players.push_back({ playerName });
    }

    void updatePlayerScore(const string& playerName, int points) {
        for (auto& p : players) {
            if (p.name == playerName) {
                p.totalPoints += points;
                p.gamesPlayed++;
                return;
            }
        }
        cout << "Player not found: " << playerName << endl;
    }

    double averagePoints() const {
        return gamesPlayed ? (double)totalPoints / gamesPlayed : 0.0;
    }

    void printStats() const {
        cout << "Team: " << name << " | Wins: " << wins << " | Losses: " << losses
            << " | Avg Points: " << averagePoints() << "\n";
        for (const auto& p : players) {
            cout << "  Player: " << p.name << " | Avg Points: " << p.averagePoints() << "\n";
        }
    }
};

class League {
    string sport;
    vector<Team> teams;

public:
    League(const string& sportName = "") : sport(sportName) {}

    void addTeam(const Team& t) {
        teams.push_back(t);
    }

    int getTeamCount() const {
        return teams.size();
    }

    Team& getTeam(int index) {
        return teams[index];
    }

    void listTeams() const {
        for (int i = 0; i < teams.size(); ++i) {
            cout << i << ": " << teams[i].name << "\n";
        }
    }

    void playGame() {
        if (teams.size() < 2) {
            cout << "Not enough teams.\n";
            return;
        }

        listTeams();
        int t1, t2;
        cout << "Enter index of Team 1: "; cin >> t1;
        cout << "Enter index of Team 2: "; cin >> t2;

        if (t1 < 0 || t2 < 0 || t1 >= teams.size() || t2 >= teams.size() || t1 == t2) {
            cout << "Invalid team indices.\n";
            return;
        }

        int t1Score = 0, t2Score = 0;
        cout << "Enter total score for " << teams[t1].name << ": ";
        cin >> t1Score;
        cout << "Enter total score for " << teams[t2].name << ": ";
        cin >> t2Score;

        Team& team1 = teams[t1];
        Team& team2 = teams[t2];

        team1.totalPoints += t1Score;
        team2.totalPoints += t2Score;
        team1.gamesPlayed++;
        team2.gamesPlayed++;

        if (t1Score > t2Score) {
            team1.wins++;
            team2.losses++;
        }
        else {
            team2.wins++;
            team1.losses++;
        }

        // Individual scoring
        for (Team* team : { &team1, &team2 }) {
            cout << "Enter player scores for " << team->name << ":\n";
            for (auto& p : team->players) {
                int pts;
                cout << "  " << p.name << ": ";
                cin >> pts;
                p.totalPoints += pts;
                p.gamesPlayed++;
            }
        }

        cout << "Game recorded.\n";
    }

    void saveLeague(const string& filename) const {
        ofstream out(filename);
        if (!out) {
            cout << "Error saving to file.\n";
            return;
        }

        out << sport << "\n" << teams.size() << "\n";
        for (const auto& team : teams) {
            out << team.name << "\n" << team.wins << " " << team.losses << " " << team.gamesPlayed << " " << team.totalPoints << "\n";
            out << team.players.size() << "\n";
            for (const auto& player : team.players) {
                out << player.name << "\n" << player.totalPoints << " " << player.gamesPlayed << "\n";
            }
        }

        out.close();
    }

    void loadLeague(const string& filename) {
        ifstream in(filename);
        if (!in) {
            cout << "Error loading file.\n";
            return;
        }

        getline(in, sport);
        int teamCount;
        in >> teamCount;
        in.ignore();

        teams.clear();

        for (int i = 0; i < teamCount; ++i) {
            Team team;
            getline(in, team.name);
            in >> team.wins >> team.losses >> team.gamesPlayed >> team.totalPoints;

            int playerCount;
            in >> playerCount;
            in.ignore();

            for (int j = 0; j < playerCount; ++j) {
                Player p;
                getline(in, p.name);
                in >> p.totalPoints >> p.gamesPlayed;
                in.ignore();
                team.players.push_back(p);
            }

            teams.push_back(team);
        }

        cout << "League loaded successfully.\n";
    }

    void printLeagueStats() const {
        cout << "\n=== League: " << sport << " ===\n";
        for (const auto& t : teams) {
            t.printStats();
        }
    }
};

int main() {
    League league;
    string choice;

    while (true) {
        cout << "\nMenu:\n"
            << "1. Create League\n"
            << "2. Load League from File\n"
            << "3. Add Team\n"
            << "4. Add Player to Team\n"
            << "5. Play Game\n"
            << "6. Show League Stats\n"
            << "7. Save League\n"
            << "8. Exit\n"
            << "Choice: ";
        cin >> choice;

        if (choice == "1") {
            string sport;
            cout << "Enter sport name: ";
            cin.ignore();
            getline(cin, sport);
            league = League(sport);
        }
        else if (choice == "2") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            league.loadLeague(filename);
        }
        else if (choice == "3") {
            string teamName;
            cout << "Enter team name: ";
            cin.ignore();
            getline(cin, teamName);
            league.addTeam({ teamName });
        }
        else if (choice == "4") {
            league.listTeams();
            int teamIndex;
            cout << "Select team index: ";
            cin >> teamIndex;
            string playerName;
            cout << "Enter player name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, playerName);
            if (teamIndex >= 0 && teamIndex < league.getTeamCount()) {
                league.getTeam(teamIndex).addPlayer(playerName);
            }
            else {
                cout << "Invalid team index.\n";
            }
        }
        else if (choice == "5") {
            league.playGame();
        }
        else if (choice == "6") {
            league.printLeagueStats();
        }
        else if (choice == "7") {
            string filename;
            cout << "Enter filename to save: ";
            cin >> filename;
            league.saveLeague(filename);
        }
        else if (choice == "8") {
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}