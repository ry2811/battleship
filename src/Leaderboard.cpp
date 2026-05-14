#include "Leaderboard.h"
#include <iostream>
#include <algorithm>

LeaderboardManager::LeaderboardManager() : isConnected(false) {
    // Placeholder dummy data
    topScores.push_back({"Player1", 12});
    topScores.push_back({"Player2", 15});
    topScores.push_back({"Player3", 18});
}

LeaderboardManager::~LeaderboardManager() {}

void LeaderboardManager::InitSupabase(const std::string& url, const std::string& apiKey) {
    isConnected = true;
    std::cout << "[Supabase] Initialized with URL: " << url << "\n";
}

void LeaderboardManager::FetchTopScores() {
    if (!isConnected) return;
    std::cout << "[Supabase] Fetching top scores...\n";
    // In a real app, do HTTP GET here using Emscripten fetch API or libcurl
}

void LeaderboardManager::PostScore(const std::string& name, int shotsFired) {
    std::cout << "[Supabase] Posting score for " << name << ": " << shotsFired << " shots\n";
    topScores.push_back({name, shotsFired});
    
    // Sort ascending (least shots is better)
    std::sort(topScores.begin(), topScores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.shotsFired < b.shotsFired;
    });

    if (topScores.size() > 10) {
        topScores.pop_back();
    }
}
