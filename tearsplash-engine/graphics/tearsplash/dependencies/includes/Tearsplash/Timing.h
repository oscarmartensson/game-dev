// Timing.h

#ifndef TIMING_H
#define TIMING_H

namespace Tearsplash
{
    class FPSLimiter
    {
    public:
        FPSLimiter();
        ~FPSLimiter();

        void init(const float maxFPS);
        void setMaxFPS(const float maxFPS) { mMaxFPS = maxFPS; }
        void begin();

        // Returns the current fps
        float end();

    private:
        // Functions
        void calcFPS();

        // Member variables
        float        mMaxFPS;
        float        mFPS;
        float        mFrameTime;
        unsigned int mStartTicks;
    };
}
#endif // !TIMING_H
