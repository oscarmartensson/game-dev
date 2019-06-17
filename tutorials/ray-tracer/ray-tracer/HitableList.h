#ifndef HITABLELIST_H
#define HITABLELIST_H

#include "Hitable.h"

class HitableList : public Hitable
{
public:
    HitableList() : list(nullptr), listSize(0) {};
    HitableList(Hitable** l, int n) : list(l), listSize(n) {};
    virtual bool hit(const Ray& r, const float tMin, const float tMax, HitRecord& record) const
    {
        HitRecord tempRecord;
        bool objectHit = false;
        float closestHitYet = tMax;

        for (int i = 0; i < listSize; i++)
        {
            // Iterate through all hitable objects and see if any object in list is hit
            // by ray r
            if (list[i]->hit(r, tMin, closestHitYet, tempRecord))
            {
                objectHit = true;
                closestHitYet = tempRecord.t;
                record = tempRecord;
            }
        }
        return objectHit;
    }

private:
    Hitable** list;
    int listSize;
};

#endif // !HITABLELIST_H
