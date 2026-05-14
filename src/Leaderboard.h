#pragma once
#include <string>
#include <vector>

struct ScoreEntry {
    std::string name;
    int shotsFired;
};

class LeaderboardManager {
private:
    std::vector<ScoreEntry> topScores;
    bool isConnected;

public:
    LeaderboardManager();
    ~LeaderboardManager();

    void InitSupabase(const std::string& url, const std::string& apiKey);
    void FetchTopScores();
    void PostScore(const std::string& name, int shotsFired);
    
    const std::vector<ScoreEntry>& GetTopScores() const { return topScores; }
};
