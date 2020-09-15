#include "Tearsplash/Particle2D.h"

using namespace Tearsplash;

Particle2D::Particle2D() :
    position(glm::vec2(0.0f, 0.0f)), velocity(glm::vec2(0.0f, 0.0f)), lifeTime(0.0)
{
}

void Particle2D::update(const float deltaTime) {
    position += velocity * deltaTime;
}