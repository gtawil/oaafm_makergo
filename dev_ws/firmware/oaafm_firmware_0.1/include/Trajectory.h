#ifndef TRAJECTORY_H
#define TRAJECTORY_H

//#include <StandardCplusplus.h>
#include <ArduinoSTL.h>
#include <vector>
#include "Position.h"

using namespace std;



class Trajectory
{
protected:
    int _trajectorySize;
    std::vector<Position> _positions;
    float _epsilon = 12.0;

public:
    Trajectory(/* args */);

    virtual void addPosition(float addingPosition[6]);
    virtual void addPosition(const float addingPosition[6]);
    virtual void addPosition(Position addingPosition);
    void setEpsilon(float epsilon);

    int getTrajectorySize();
    float getEpsilon();
    virtual float *getPosition(int i);

    virtual ~Trajectory();
};

#endif