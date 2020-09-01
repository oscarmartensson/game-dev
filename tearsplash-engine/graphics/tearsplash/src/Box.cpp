#include "Tearsplash/Box.h"

using namespace Tearsplash;

Box::Box() : mBody(nullptr), mFixture(nullptr)
{
    // Do nothing.
}

void Box::init(b2World* physicsWorld, const glm::vec2& position, const glm::vec2& dimensions, const b2BodyType& dynamics /*= b2_staticBody*/)
{
    mDimensions.x = dimensions.x;
    mDimensions.y = dimensions.y;

    // Make a physics body.
    b2BodyDef bodyDef;
    bodyDef.type = dynamics;
    bodyDef.position.Set(position.x, position.y);

    mBody = physicsWorld->CreateBody(&bodyDef);

    // Create a fixture and a shape.
    b2PolygonShape boxShape;
    boxShape.SetAsBox(dimensions.x * 0.5f, dimensions.y * 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    mFixture = mBody->CreateFixture(&fixtureDef);
}

