#include "Tearsplash/ParticleEngine2D.h"

using namespace Tearsplash;


ParticleEngine2D::ParticleEngine2D() {
    // Do nothing.
}

ParticleEngine2D::~ParticleEngine2D() {
    // Do nothing.
}

void ParticleEngine2D::addParticleBatch(ParticleBatch2D& pb, Spritebatch& sb) {
    mBatches.push_back(std::pair<ParticleBatch2D&, Spritebatch&>(pb, sb));
}

void ParticleEngine2D::updateBatches(const float deltaTime) {
    for (auto& batch : mBatches) {
        batch.first.update(deltaTime);
    }
}

void ParticleEngine2D::drawBatches() const {
    for (auto& batch : mBatches) {
        // Draw the ParitcleBatch2D with the Spritebatch it was assigned.
        batch.second.begin(Tearsplash::GlyphSortType::TEXTURE);
        batch.first.draw(batch.second);
        batch.second.end();
        batch.second.renderBatch();
    }
}