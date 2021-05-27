#ifndef STABILIZEDTRAJECTORY_H
#define STABILIZEDTRAJECTORY_H

#include "Trajectory.h"
#include "Position.h"


class StabilizedTrajectory : public Trajectory
{
private:
    std::vector<HalfPosition> _halfPositions;
public:
    StabilizedTrajectory(/* args */);

    virtual void addPosition(float addingPosition[4]);
    virtual void addPosition(HalfPosition addingPosition);

    virtual float *getPosition(int i);
    virtual ~StabilizedTrajectory();
};


#endif
