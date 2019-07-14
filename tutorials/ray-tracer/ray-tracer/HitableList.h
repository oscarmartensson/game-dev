#ifndef HITABLELIST_H
#define HITABLELIST_H

#include "Hitable.h"

class HitableList : public Hitable
{
public:
    HitableList() : mList(nullptr), mListSize(0) {};
    HitableList(Hitable** l, int n) : mList(l), mListSize(n) {};
    virtual ~HitableList()
    {
        for (int i = 0; i < mListSize; i++)
        {
            if (mList[i] != nullptr)
            {
                delete(mList[i]);
            }
        }
        if (mList != nullptr)
        {
            delete mList;
        }
    }

    virtual bool hit(const Ray& r, const float tMin, const float tMax, HitRecord& record) const
    {
        HitRecord tempRecord;
        bool objectHit = false;
        float closestHitYet = tMax;

        for (int i = 0; i < mListSize; i++)
        {
            // Iterate through all hitable objects and see if any object in list is hit
            // by ray r
            if (mList[i]->hit(r, tMin, closestHitYet, tempRecord))
            {
                objectHit = true;
                closestHitYet = tempRecord.t;
                record = tempRecord;
            }
        }
        return objectHit;
    }
    int getSize() const { return mListSize; }
    Hitable** getList() { return mList; }

private:
    Hitable** mList;
    int mListSize;
};

#endif // !HITABLELIST_H
