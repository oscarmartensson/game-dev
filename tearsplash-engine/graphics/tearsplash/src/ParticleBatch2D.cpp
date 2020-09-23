#include "Tearsplash/ParticleBatch2D.h"

using namespace Tearsplash;

ParticleBatch2D::ParticleBatch2D() :
    mDecayRate(0), mMaxParticles(0), mActiveParticleIndex(0) {
}
ParticleBatch2D::~ParticleBatch2D() {
    delete[] mParticles;
}

void ParticleBatch2D::init(const int numParticles, const float decayRate, Tearsplash::GLTexture& texture) {
    mDecayRate = decayRate;
    mMaxParticles = numParticles;
    mActiveParticleIndex = 0;
    mTexture = texture;
    mParticles = new Particle2D[mMaxParticles];
}

void ParticleBatch2D::addParticle(const glm::vec2& position,
    const glm::vec2& velocity,
    ColorRGBA8& color,
    const float width,
    const float lifeTime) {
    const int freeIndex = findActiveIndex();
    mParticles[freeIndex].position = position;
    mParticles[freeIndex].velocity = velocity;
    mParticles[freeIndex].color = color;
    mParticles[freeIndex].width = width;
    mParticles[freeIndex].lifeTime = lifeTime;
}

void ParticleBatch2D::update(const float deltaTime) {
    for (int i = 0; i < mMaxParticles; i++) {
        if (mParticles[i].lifeTime > 0.0f) {
            mParticles[i].update(deltaTime);
            mParticles[i].lifeTime -= mDecayRate * deltaTime;
        }
    }
}

void ParticleBatch2D::draw(Spritebatch& sb) const {
    glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    for (int i = 0; i < mMaxParticles; i++) {
        if (mParticles[i].lifeTime > 0.0f) {
            glm::vec4 destRect(mParticles[i].position.x, mParticles[i].position.y, mParticles[i].width, mParticles[i].width);
            sb.draw(destRect, uvRect, mTexture.id, 0, mParticles[i].color);
        }
    }
}

int ParticleBatch2D::findActiveIndex() {
    // Loop to the end of the particle array in search of a free particle.
    for (int i = mActiveParticleIndex; i < mMaxParticles; i++) {
        if (mParticles[i].lifeTime <= 0.0f) {
            mActiveParticleIndex = i;
            return mActiveParticleIndex;
        }
    }
    // Wrap around and start looping from the start of the array to mActiveParticleIndex.
    // If the active particle is in the middle of the array but before mActiveParticleIndex,
    // this will be detected now.
    for (int i = 0; i < mActiveParticleIndex; i++) {
        if (mParticles[i].lifeTime <= 0.0f) {
            mActiveParticleIndex = i;
            return mActiveParticleIndex;
        }
    }

    // No particles free. Overwrite the first one.
    return 0;
}