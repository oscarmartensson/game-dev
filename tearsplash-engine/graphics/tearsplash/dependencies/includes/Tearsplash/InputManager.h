// InputManager.h

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <unordered_map>

namespace Tearsplash
{
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();

        void pressKey(const unsigned int keyID);
        void releaseKey(const unsigned int keyID);

        // Returns whether keyID is pressed or not, provided that it exists in mKeyMap
        bool isKeyPressed(unsigned int keyID);

    private:
        std::unordered_map<unsigned int, bool> mKeyMap;

    };
}

#endif // !INPUTMANAGER_H