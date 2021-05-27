#include "Trajectory.h"

Trajectory::Trajectory(/* args */) : _positions()
{
    _trajectorySize = 0;
}

void Trajectory::addPosition(float addingPosition[6])
{
    Position position;
    for (int i = 0; i < 6; i++)
    {
        position._position[i] = addingPosition[i];
    }
    _positions.push_back(position);
    _trajectorySize++;
}

void Trajectory::addPosition(const float addingPosition[6])
{
    Position position;
    for (int i = 0; i < 6; i++)
    {
        position._position[i] = addingPosition[i];
    }
    _positions.push_back(position);
    _trajectorySize++;
}

void Trajectory::addPosition(Position addingPosition)
{
    _positions.push_back(addingPosition);
    _trajectorySize++;
}

void Trajectory::setEpsilon(float epsilon)
{
    _epsilon=epsilon;
}

int Trajectory::getTrajectorySize()
{
    return _trajectorySize;
}

float Trajectory::getEpsilon()
{
    return _epsilon;
}

float *Trajectory::getPosition(int i)
{
    return _positions[i]._position;
}

Trajectory::~Trajectory()
{
}