// InputManager.h

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <unordered_map>
#include <glm/glm.hpp>

namespace Tearsplash
{
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();

        void update();

        void pressKey(const unsigned int keyID);
        void releaseKey(const unsigned int keyID);
        void setMouseCoords(const float x, const float y);

        // Returns whether keyID is pressed or not, provided that it exists in mKeyMap
        bool isKeyPressed(const unsigned int keyID);
        bool isKeyDown(const unsigned int keyID);

        // Returns mouse coordinates
        glm::vec2 getMouseCoords() const { return mMouseCoords; }

    private:
        bool wasKeyDown(const unsigned int keyID);

        std::unordered_map<unsigned int, bool> mKeyMap;
        std::unordered_map<unsigned int, bool> mPrevKeyMap;
        glm::vec2 mMouseCoords;

    };
}

#endif // !INPUTMANAGER_H