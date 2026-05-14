#pragma once
#include <vector>
#include <raylib.h>

struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float life;
    float maxLife;
    float size;
};

class ParticleSystem {
private:
    std::vector<Particle> particles;

public:
    ParticleSystem();
    ~ParticleSystem();

    void EmitExplosion(Vector2 pos);
    void EmitSplash(Vector2 pos);
    void Update(float dt);
    void Draw();
    void Clear();
};
