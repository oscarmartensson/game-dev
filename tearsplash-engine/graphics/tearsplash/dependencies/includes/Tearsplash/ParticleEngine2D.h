#ifndef PARTICLEENGINE2D_H
#define PARTICLEENGINE2D_H

#include <utility>

#include "Tearsplash/ParticleBatch2D.h"

namespace Tearsplash {
    class ParticleEngine2D {
    public:
        ParticleEngine2D();
        ~ParticleEngine2D();

        void addParticleBatch(Tearsplash::ParticleBatch2D& pb, Spritebatch& sb);
        void updateBatches(const float deltaTime);
        void drawBatches() const;


    private:
        std::vector<std::pair<Tearsplash::ParticleBatch2D&, Tearsplash::Spritebatch&>> mBatches;
    };
}

#endif // !PARTICLEENGINE2D_H
