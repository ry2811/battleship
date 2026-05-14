#include "Renderer.h"
#include <string>
#include <cmath>

Renderer::Renderer() : shakeIntensity(0), shakeDuration(0), cameraOffset({0,0}), transitionAlpha(0), timePlayed(0), currentSkin(ThemeSkin::SciFi), nameLength(0) {
    playerName[0] = '\0';
}

Renderer::~Renderer() {
}

void Renderer::LoadResources() {
    InitAudioDevice();

    // Generate procedural audio so we don't need external files
    SetAudioStreamBufferSizeDefault(4096);
    GenerateSounds();
    
    leaderboard.InitSupabase("https://hypothetical-supabase.co", "anon-key-1234");
    leaderboard.FetchTopScores();
}

void Renderer::UnloadResources() {
    UnloadSound(sfxFire);
    UnloadSound(sfxHit);
    UnloadSound(sfxMiss);
    CloseAudioDevice();
}

void Renderer::TriggerShake(float intensity, float duration) {
    shakeIntensity = intensity;
    shakeDuration = duration;
}

void Renderer::Update(float dt, GameState state) {
    // UpdateMusicStream(bgm);
    
    if (shakeDuration > 0) {
        shakeDuration -= dt;
        cameraOffset.x = GetRandomValue(-shakeIntensity, shakeIntensity);
        cameraOffset.y = GetRandomValue(-shakeIntensity, shakeIntensity);
        shakeIntensity *= 0.9f; // dampen
    } else {
        cameraOffset = {0, 0};
        shakeIntensity = 0;
    }

    particleSystem.Update(dt);
    
    if (state == GameState::Menu) {
        timePlayed += dt;
        transitionAlpha = 1.0f; // Reset transition for simplicity
    }
    
    // Smooth transition
    if (transitionAlpha > 0.0f) {
        transitionAlpha -= dt * 0.5f;
        if (transitionAlpha < 0.0f) transitionAlpha = 0.0f;
    }
}

void Renderer::GenerateSounds() {
    int sampleRate = 44100;
    Wave wFire = { 0 };
    wFire.frameCount = sampleRate * 0.2; // 0.2s
    wFire.sampleRate = sampleRate;
    wFire.sampleSize = 16;
    wFire.channels = 1;
    short* dFire = (short*)MemAlloc(wFire.frameCount * sizeof(short));
    for (unsigned int i=0; i<wFire.frameCount; i++) {
        dFire[i] = (short)(((float)GetRandomValue(-100, 100)/100.0f) * 10000.0f * (1.0f - (float)i/wFire.frameCount));
    }
    wFire.data = dFire;
    sfxFire = LoadSoundFromWave(wFire);
    UnloadWave(wFire);

    Wave wHit = { 0 };
    wHit.frameCount = sampleRate * 0.4;
    wHit.sampleRate = sampleRate;
    wHit.sampleSize = 16;
    wHit.channels = 1;
    short* dHit = (short*)MemAlloc(wHit.frameCount * sizeof(short));
    for (unsigned int i=0; i<wHit.frameCount; i++) {
        dHit[i] = (short)(((float)GetRandomValue(-100, 100)/100.0f) * 20000.0f * (1.0f - (float)i/wHit.frameCount));
    }
    wHit.data = dHit;
    sfxHit = LoadSoundFromWave(wHit);
    UnloadWave(wHit);

    Wave wMiss = { 0 };
    wMiss.frameCount = sampleRate * 0.3;
    wMiss.sampleRate = sampleRate;
    wMiss.sampleSize = 16;
    wMiss.channels = 1;
    short* dMiss = (short*)MemAlloc(wMiss.frameCount * sizeof(short));
    for (unsigned int i=0; i<wMiss.frameCount; i++) {
        dMiss[i] = (short)(sinf(i * 0.1f) * 5000.0f * (1.0f - (float)i/wMiss.frameCount));
    }
    wMiss.data = dMiss;
    sfxMiss = LoadSoundFromWave(wMiss);
    UnloadWave(wMiss);
}

void Renderer::PlayFireSound() {
    PlaySound(sfxFire);
}

void Renderer::PlayHitSound() {
    PlaySound(sfxHit);
}

void Renderer::PlayMissSound() {
    PlaySound(sfxMiss);
}

void Renderer::AddExplosion(Vector2 pos) {
    particleSystem.EmitExplosion(pos);
}

void Renderer::AddSplash(Vector2 pos) {
    particleSystem.EmitSplash(pos);
}

void Renderer::Draw(GameLogic& game) {
    BeginDrawing();
    
    // Rich gradient background for modern look based on skin
    Color bg1 = { 10, 15, 25, 255 };
    Color bg2 = { 20, 35, 60, 255 };
    
    if (currentSkin == ThemeSkin::Modern) {
        bg1 = { 20, 20, 20, 255 };
        bg2 = { 40, 40, 50, 255 };
    } else if (currentSkin == ThemeSkin::Retro) {
        bg1 = { 0, 20, 0, 255 };
        bg2 = { 0, 50, 0, 255 };
    }
    
    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), bg1, bg2);

    BeginMode2D({cameraOffset, {0,0}, 0.0f, 1.0f});

    switch (game.state) {
        case GameState::Menu:
            DrawMenu(game);
            break;
        case GameState::Playing:
            DrawPlaying(game);
            break;
        case GameState::GameOver:
            DrawGameOver(game);
            break;
        case GameState::Win:
            DrawWin(game);
            break;
        case GameState::InputName:
            DrawInputName(game);
            break;
        case GameState::Rules:
            DrawRules(game);
            break;
        case GameState::Leaderboard:
            DrawLeaderboard(game);
            break;
    }

    EndMode2D();
    EndDrawing();
}

bool Renderer::DrawButton(const char* text, float x, float y, float width, float height) {
    Rectangle rect = { x - width/2, y, width, height };
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, rect);
    
    Color bgColor = isHovered ? (Color){ 50, 80, 120, 255 } : (Color){ 30, 40, 60, 255 };
    Color outlineColor = isHovered ? (Color){ 0, 200, 255, 255 } : (Color){ 100, 100, 100, 255 };
    
    if (currentSkin == ThemeSkin::Modern) {
        bgColor = isHovered ? (Color){ 80, 80, 80, 255 } : (Color){ 40, 40, 40, 255 };
        outlineColor = isHovered ? (Color){ 200, 200, 200, 255 } : (Color){ 100, 100, 100, 255 };
    } else if (currentSkin == ThemeSkin::Retro) {
        bgColor = isHovered ? (Color){ 0, 100, 0, 255 } : (Color){ 0, 40, 0, 255 };
        outlineColor = isHovered ? (Color){ 0, 255, 0, 255 } : (Color){ 0, 150, 0, 255 };
    }
    
    DrawRectangleRec(rect, bgColor);
    DrawRectangleLinesEx(rect, 2.0f, outlineColor);
    
    int textWidth = MeasureText(text, 20);
    DrawText(text, x - textWidth/2, y + height/2 - 10, 20, RAYWHITE);
    
    return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void Renderer::DrawMenu(GameLogic& game) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    
    // Title with modern glow / pulse effect
    float scale = 1.0f + 0.05f * sinf(timePlayed * 3.0f);
    int titleWidth = MeasureText("BATTLESHIP", 60);
    DrawTextEx(GetFontDefault(), "BATTLESHIP", { sw/2 - (titleWidth/2.0f)*scale, sh/4 - 30.0f*scale }, 60 * scale, 2.0f, { 0, 200, 255, 255 });
    
    if (DrawButton("START GAME", sw/2, sh/2, 250, 50)) {
        game.state = GameState::Playing;
        transitionAlpha = 1.0f;
    }
    if (DrawButton("BANG XEP HANG", sw/2, sh/2 + 70, 250, 50)) {
        game.state = GameState::Leaderboard;
        transitionAlpha = 1.0f;
    }
    if (DrawButton("LUAT CHOI", sw/2, sh/2 + 140, 250, 50)) {
        game.state = GameState::Rules;
        transitionAlpha = 1.0f;
    }
    
    // Toggle Skin
    std::string themeName = "Sci-Fi";
    if (currentSkin == ThemeSkin::Modern) themeName = "Modern";
    else if (currentSkin == ThemeSkin::Retro) themeName = "Retro";
    
    if (DrawButton(("Theme: " + themeName).c_str(), sw/2, sh/2 + 210, 250, 50)) {
        if (currentSkin == ThemeSkin::SciFi) currentSkin = ThemeSkin::Modern;
        else if (currentSkin == ThemeSkin::Modern) currentSkin = ThemeSkin::Retro;
        else currentSkin = ThemeSkin::SciFi;
    }
}

void Renderer::DrawRules(GameLogic& game) {
    float sw = GetScreenWidth();
    DrawText("LUAT CHOI", sw/2 - MeasureText("LUAT CHOI", 40)/2, 80, 40, { 0, 200, 255, 255 });
    
    int startX = sw/2 - 350;
    DrawText("- Ban co 20 luot ban (Shots) de tim va tieu diet toan bo 4 tau dich.", startX, 180, 20, RAYWHITE);
    DrawText("- Cac tau duoc an ngau nhien duoi man suong mu tren ban co 10x10.", startX, 220, 20, RAYWHITE);
    DrawText("- Kich thuoc cac tau lan luot la: 1, 2, 3 va 4 o.", startX, 260, 20, RAYWHITE);
    DrawText("- Dung CHUOT TRAI de chon o ban. Mau do la TRUNG, mau xanh la TRUOT.", startX, 300, 20, RAYWHITE);
    DrawText("- Neu ban tieu diet toan bo 4 tau truoc khi het luot, ban se THANG.", startX, 340, 20, GREEN);
    DrawText("- Neu het 20 luot ma chua tieu diet het tau, ban se THUA.", startX, 380, 20, RED);
    
    if (DrawButton("BACK TO MENU", sw/2, 500, 250, 50)) {
        game.state = GameState::Menu;
    }
}

void Renderer::DrawLeaderboard(GameLogic& game) {
    float sw = GetScreenWidth();
    DrawText("BANG XEP HANG", sw/2 - MeasureText("BANG XEP HANG", 40)/2, 80, 40, GOLD);
    
    int y = 160;
    int rank = 1;
    for (const auto& entry : leaderboard.GetTopScores()) {
        std::string txt = std::to_string(rank) + ". " + entry.name + " - " + std::to_string(entry.shotsFired) + " shots";
        DrawText(txt.c_str(), sw/2 - MeasureText(txt.c_str(), 25)/2, y, 25, RAYWHITE);
        y += 40;
        rank++;
    }
    
    if (DrawButton("BACK TO MENU", sw/2, 500, 250, 50)) {
        game.state = GameState::Menu;
    }
}

void Renderer::DrawPlaying(GameLogic& game) {
    int cellSize = 40;
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    int offsetX = sw/2 - (10 * cellSize)/2;
    int offsetY = sh/2 - (10 * cellSize)/2;
    
    // Draw UI
    std::string shotsTxt = "Shots Left: " + std::to_string(game.shotsLeft);
    DrawText(shotsTxt.c_str(), sw/2 - 200, 20, 30, GREEN);
    
    std::string sunkTxt = "Ships Sunk: " + std::to_string(game.shipsSunk) + "/" + std::to_string(game.totalShips);
    DrawText(sunkTxt.c_str(), sw/2 + 50, 20, 30, YELLOW);

    // Fog of war effect - unrevealed cells look opaque
    DrawGrid(game, offsetX, offsetY, cellSize);
    particleSystem.Draw();

    // Interaction
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        mousePos.x -= cameraOffset.x;
        mousePos.y -= cameraOffset.y;
        
        int gridX = (mousePos.x - offsetX) / cellSize;
        int gridY = (mousePos.y - offsetY) / cellSize;
        
        if (gridX >= 0 && gridX < 10 && gridY >= 0 && gridY < 10) {
            PlayFireSound();
            
            GridNode* node = game.grid.GetNode(gridX, gridY);
            if (node && (node->state == GridState::Empty || node->state == GridState::Ship)) {
                bool hit = game.Fire(gridX, gridY);
                Vector2 center = { (float)offsetX + gridX * cellSize + cellSize / 2, 
                                   (float)offsetY + gridY * cellSize + cellSize / 2 };
                if (hit) {
                    PlayHitSound();
                    TriggerShake(15.0f, 0.4f);
                    AddExplosion(center);
                } else {
                    PlayMissSound();
                    AddSplash(center);
                    TriggerShake(3.0f, 0.2f);
                }
            }
        }
    }
}

void Renderer::DrawGameOver(GameLogic& game) {
    float sw = GetScreenWidth();
    DrawText("GAME OVER", sw/2 - MeasureText("GAME OVER", 60)/2, 200, 60, RED);
    DrawText("Out of shots!", sw/2 - MeasureText("Out of shots!", 30)/2, 300, 30, RAYWHITE);
    if (DrawButton("BACK TO MENU", sw/2, 400, 250, 50)) {
        game.ResetGame();
    }
}

void Renderer::DrawInputName(GameLogic& game) {
    float sw = GetScreenWidth();
    DrawText("YOU WIN!", sw/2 - MeasureText("YOU WIN!", 60)/2, 100, 60, GREEN);
    
    std::string stats = "Shots Fired: " + std::to_string(game.maxShots - game.shotsLeft);
    DrawText(stats.c_str(), sw/2 - MeasureText(stats.c_str(), 30)/2, 180, 30, RAYWHITE);
    
    DrawText("Nhap ten cua ban (Luu vao BXH):", sw/2 - MeasureText("Nhap ten cua ban (Luu vao BXH):", 20)/2, 260, 20, GRAY);
    
    Rectangle textBox = { sw/2 - 150, 300, 300, 50 };
    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
    
    DrawText(playerName, (int)textBox.x + 10, (int)textBox.y + 10, 30, BLACK);
    
    // Blinking cursor
    if (((int)(timePlayed * 2)) % 2 == 0) {
        DrawText("_", (int)textBox.x + 10 + MeasureText(playerName, 30), (int)textBox.y + 10, 30, RED);
    }
    
    // Handle input
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (nameLength < 31)) {
            playerName[nameLength] = (char)key;
            playerName[nameLength+1] = '\0';
            nameLength++;
        }
        key = GetCharPressed();
    }
    
    if (IsKeyPressed(KEY_BACKSPACE)) {
        nameLength--;
        if (nameLength < 0) nameLength = 0;
        playerName[nameLength] = '\0';
    }
    
    if (DrawButton("SAVE & CONTINUE", sw/2, 400, 250, 50) || IsKeyPressed(KEY_ENTER)) {
        std::string n = (nameLength > 0) ? playerName : "Anonymous";
        leaderboard.PostScore(n, game.maxShots - game.shotsLeft);
        playerName[0] = '\0';
        nameLength = 0;
        game.state = GameState::Win;
    }
}

void Renderer::DrawWin(GameLogic& game) {
    float sw = GetScreenWidth();
    DrawText("SCORE SAVED!", sw/2 - MeasureText("SCORE SAVED!", 60)/2, 200, 60, GREEN);
    if (DrawButton("BACK TO MENU", sw/2, 350, 250, 50)) {
        game.ResetGame();
    }
}

void Renderer::DrawGrid(GameLogic& game, int offsetX, int offsetY, int cellSize) {
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            GridNode* node = game.grid.GetNode(x, y);
            Rectangle rect = { (float)offsetX + x * cellSize, (float)offsetY + y * cellSize, (float)cellSize, (float)cellSize };
            
            Color cellColor = { 30, 40, 60, 255 }; // Fog of war / Unknown
            Color outlineColor = { 50, 70, 90, 255 };
            
            if (currentSkin == ThemeSkin::Modern) {
                cellColor = { 50, 50, 50, 255 };
                outlineColor = { 100, 100, 100, 255 };
            } else if (currentSkin == ThemeSkin::Retro) {
                cellColor = { 0, 40, 0, 255 };
                outlineColor = { 0, 150, 0, 255 };
            }
            
            if (node) {
                if (node->state == GridState::Hit) {
                    cellColor = { 200, 40, 40, 255 }; // Bright Red
                    outlineColor = { 255, 100, 100, 255 };
                } else if (node->state == GridState::Miss) {
                    cellColor = { 40, 100, 200, 255 }; // Bright Blue
                    outlineColor = { 100, 150, 255, 255 };
                } else if (node->state == GridState::Empty || node->state == GridState::Ship) {
                    // Hover effect
                    Vector2 mousePos = GetMousePosition();
                    mousePos.x -= cameraOffset.x;
                    mousePos.y -= cameraOffset.y;
                    if (CheckCollisionPointRec(mousePos, rect)) {
                        cellColor = { 50, 70, 100, 255 };
                        outlineColor = { 0, 200, 255, 255 }; // Cyan glow
                        
                        if (currentSkin == ThemeSkin::Modern) {
                            cellColor = { 80, 80, 80, 255 };
                            outlineColor = { 200, 200, 200, 255 };
                        } else if (currentSkin == ThemeSkin::Retro) {
                            cellColor = { 0, 100, 0, 255 };
                            outlineColor = { 0, 255, 0, 255 };
                        }
                    }
                }
            }
            
            // Subtle glassmorphism / inner gradient simulation could be done, 
            // but for simplicity we'll just draw the colored rect with a nice outline
            DrawRectangleRec(rect, cellColor);
            DrawRectangleLinesEx(rect, 2.0f, outlineColor);
        }
    }
}
