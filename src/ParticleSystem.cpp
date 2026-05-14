#include "ParticleSystem.h"
#include <cstdlib>
#include <cmath>

ParticleSystem::ParticleSystem() {}
ParticleSystem::~ParticleSystem() {}

void ParticleSystem::EmitExplosion(Vector2 pos) {
    for (int i = 0; i < 30; i++) {
        Particle p;
        p.position = pos;
        float angle = (GetRandomValue(0, 360) * DEG2RAD);
        float speed = GetRandomValue(50, 150);
        p.velocity = Vector2{ cosf(angle) * speed, sinf(angle) * speed };
        p.color = { 255, (unsigned char)GetRandomValue(50, 150), 0, 255 }; // Orange-ish
        p.maxLife = GetRandomValue(5, 10) / 10.0f;
        p.life = p.maxLife;
        p.size = GetRandomValue(2, 6);
        particles.push_back(p);
    }
}

void ParticleSystem::EmitSplash(Vector2 pos) {
    for (int i = 0; i < 20; i++) {
        Particle p;
        p.position = pos;
        float angle = (GetRandomValue(0, 360) * DEG2RAD);
        float speed = GetRandomValue(30, 100);
        p.velocity = Vector2{ cosf(angle) * speed, sinf(angle) * speed };
        p.color = { (unsigned char)GetRandomValue(100, 200), 200, 255, 255 }; // Blue-ish
        p.maxLife = GetRandomValue(4, 8) / 10.0f;
        p.life = p.maxLife;
        p.size = GetRandomValue(2, 5);
        particles.push_back(p);
    }
}

void ParticleSystem::Update(float dt) {
    for (auto it = particles.begin(); it != particles.end(); ) {
        it->position.x += it->velocity.x * dt;
        it->position.y += it->velocity.y * dt;
        it->life -= dt;
        it->color.a = (unsigned char)((it->life / it->maxLife) * 255.0f);
        
        if (it->life <= 0) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }
}

void ParticleSystem::Draw() {
    for (const auto& p : particles) {
        DrawCircleV(p.position, p.size, p.color);
    }
}

void ParticleSystem::Clear() {
    particles.clear();
}
