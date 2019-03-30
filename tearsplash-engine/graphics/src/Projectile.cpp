/**********************************************************************/
// File:    Projectile.cpp
// Summary: Class that mimics a projectile with speed an velocity that can
//          also have a sprite image.
//
// Version: Version 1.0 - 2019-03-30
// Author:  Oscar Mårtensson
// -------------------------------------------
// Log:     2019-03-30 File created
/**********************************************************************/

#include "Projectile.h"

#include <Tearsplash/ResourceManager.h>

Projectile::Projectile(glm::vec2 pos, glm::vec2 dir, float speed, int lifetime) : mPos(pos), mUnitDir(dir), mSpeed(speed), mLifeTime(lifetime) {}
Projectile::~Projectile() {}


void Projectile::draw(Tearsplash::Spritebatch& spriteBatch)
{
    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static Tearsplash::GLTexture texture = Tearsplash::ResourceManager::getTexture("textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
    Tearsplash::Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;

    spriteBatch.draw(glm::vec4(mPos, 30.0f, 30.0f), uv, texture.id, 0.0f, color);
}

bool Projectile::update()
{
    mPos += mUnitDir * mSpeed;
    if (--mLifeTime <= 0)
    {
        mLifeTime = 0;
        return true;
    }

    return false;
}