#include "StabilizedTrajectory.h"


StabilizedTrajectory::StabilizedTrajectory(/* args */)
{
    _trajectorySize=0;
}


void StabilizedTrajectory::addPosition(float addingPosition[4])
{
    HalfPosition position;
    for (int i = 0; i < 4; i++)
    {
        position._position[i] = addingPosition[i];
    }
    _halfPositions.push_back(position);
    _trajectorySize++;
}

void StabilizedTrajectory::addPosition(HalfPosition addingPosition)
{
    _halfPositions.push_back(addingPosition);
    _trajectorySize++;
}

float *StabilizedTrajectory::getPosition(int i)
{
    return _halfPositions[i]._position;
}

StabilizedTrajectory::~StabilizedTrajectory()
{
}