#ifndef PARTICLEBATCH2D_H
#define PARTICLEBATCH2D_H

#include "Tearsplash/Particle2D.h"
#include "Tearsplash/Spritebatch.h"
#include "Tearsplash/GlTexture.h"

namespace Tearsplash {

    class ParticleBatch2D {
    public:
        ParticleBatch2D();
        ~ParticleBatch2D();

        void init(const int numParticles, const float decayRate, Tearsplash::GLTexture& texture);
        void addParticle(const glm::vec2& position,
                         const glm::vec2& velocity,
                         ColorRGBA8& color,
                         const float width,
                         const float lifeTime = 1.0f);
        void update(const float deltaTime);
        void draw(Tearsplash::Spritebatch& sb) const;

    private:
        Tearsplash::Particle2D* mParticles;
        float mDecayRate;
        int mMaxParticles;
        int mActiveParticleIndex;
        Tearsplash::GLTexture mTexture;

        int findActiveIndex();
    };

}

#endif // !PARTICLEBATCH2D_H
