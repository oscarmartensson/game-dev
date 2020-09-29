#ifndef TILESHEET_H
#define TILESHEET_H

#include "Tearsplash/GLTexture.h"

#include <glm/glm.hpp>

namespace Tearsplash {

    class Tilesheet {
    public:

        void init(const Tearsplash::GLTexture& texture, glm::ivec3& dims) {
            mTexture = texture;
            mDimensions = dims;
        }

        // A tile sheet has it's indices calculated as following:
        // ^   8 9 ...
        // y   4 5 6 7
        // x>  0 1 2 3
        glm::vec4 getUV(const int tileIndex) {
            const int xTile = tileIndex % mDimensions.x;
            const int yTile = tileIndex / mDimensions.x; // Rounding down, just like flooring.

            glm::vec4 result;
            result.x = static_cast<float>(xTile) / mDimensions.x;
            result.y = static_cast<float>(yTile) / mDimensions.y;
            result.z = 1.0f / mDimensions.x; // width
            result.w = 1.0f / mDimensions.y; // height

            return result;
        }

        glm::ivec3 mDimensions;
        Tearsplash::GLTexture mTexture;
    };

}

#endif // !TILESHEET_H
