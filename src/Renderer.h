#pragma once
#include <raylib.h>
#include "GameLogic.h"
#include "ParticleSystem.h"
#include "Leaderboard.h"

enum class ThemeSkin { Modern, Retro, SciFi };

class Renderer {
private:
    ParticleSystem particleSystem;
    Sound sfxFire;
    Sound sfxHit;
    Sound sfxMiss;
    Music bgm;
    
    float shakeIntensity;
    float shakeDuration;
    Vector2 cameraOffset;

    // UI state
    float transitionAlpha;
    float timePlayed;
    ThemeSkin currentSkin;
    
    LeaderboardManager leaderboard;

public:
    Renderer();
    ~Renderer();

    void LoadResources();
    void UnloadResources();
    
    void TriggerShake(float intensity, float duration);
    void Update(float dt, GameState state);
    void Draw(GameLogic& game);
    void PlayFireSound();
    void PlayHitSound();
    void PlayMissSound();
    void AddExplosion(Vector2 pos);
    void AddSplash(Vector2 pos);

private:
    void GenerateSounds();
    bool DrawButton(const char* text, float x, float y, float width, float height);
    void DrawMenu(GameLogic& game);
    void DrawPlaying(GameLogic& game);
    void DrawGameOver(GameLogic& game);
    void DrawWin(GameLogic& game);
    void DrawInputName(GameLogic& game);
    void DrawRules(GameLogic& game);
    void DrawLeaderboard(GameLogic& game);
    void DrawGrid(GameLogic& game, int offsetX, int offsetY, int cellSize);
    
    char playerName[32];
    int nameLength;
};
