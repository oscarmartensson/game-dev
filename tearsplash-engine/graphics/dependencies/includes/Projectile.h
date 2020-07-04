// Projectile.h

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <glm/glm.hpp>
#include <Tearsplash/Spritebatch.h>

class Projectile
{
public:
    Projectile(glm::vec2 pos, glm::vec2 dir, float speed, int lifetime);
    ~Projectile();
    
    void draw(Tearsplash::Spritebatch& spriteBatch);

    // Returns true when lifetime is up
    bool update();

    glm::vec2 getPosition() const {
        return mPos;
    }

    glm::vec2 getAABB() const {
        return mAABB;
    }

private:
    float mSpeed;
    int mLifeTime;
    glm::vec2 mUnitDir;;
    glm::vec2 mSize;
    glm::vec2 mPos;
    glm::vec2 mAABB;
};

#endif // !PROJECTILE_H
