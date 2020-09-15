#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

#include "Tearsplash/Vertex.h"

namespace Tearsplash {

    class Particle2D {
        friend class ParticleBatch2D;

    public:
        Particle2D();
        ~Particle2D() {};
        void update(const float deltaTime);

    private:
        glm::vec2 position = glm::vec2(0.0f, 0.0f);
        glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
        ColorRGBA8 color;
        float lifeTime = 0.0f;
        float width; // Particle is assumed to be square.
    };

}

#endif // !PARTICLE_H
