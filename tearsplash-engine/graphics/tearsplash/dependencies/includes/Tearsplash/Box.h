#ifndef BOX_H
#define BOX_H

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

namespace Tearsplash {

    class Box {
    public:
        Box();
        ~Box() {
            // Do nothing.
        }

        b2Body* getBody() {
            return mBody;
        }
        b2Fixture* getFixture() {
            return mFixture;
        }

        const glm::vec2& getDimensions() const {
            return mDimensions;
        }

        // Creates a 2D box body and fixture in a Box2D API specific way.
        void init(b2World* physicsWorld, const glm::vec2& position, const glm::vec2& dimensions, const b2BodyType& dynamic = b2_staticBody);

    private:
        b2Body* mBody;
        b2Fixture* mFixture;
        glm::vec2 mDimensions;
    };

}

#endif // BOX_H