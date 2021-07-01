#ifndef PLATE_H
#define PLATE_H

//#include <StandardCplusplus.h>
#include <ArduinoSTL.h>
#include <vector>
#include "Arduino.h"
#include "Trajectory.h"
#include "StabilizedTrajectory.h"
#include "Position.h"
#include "Joystick.h"

using namespace std;

class Plate
{
private:
    int _numberOfPosition;
    float _offset;
    int _numberOfLine;
    int _numberOfColumn;
    int _selector2D[2];
    int _selector1D;
    std::vector<PickingPosition> _positionList;
    StabilizedTrajectory _finishedTrajectory;
    StabilizedTrajectory _removeExcessTrajectory;

public:
    Plate(float offset, int numberOfPosition, int numberOfLine, int numberOfColumn, PickingPosition pickingPositionList[21], HalfPosition removeExcessPosition[3], HalfPosition finishedPosition[4]);
    void init();

    int initSelector();
    int moveSelector(Direction direction);
    int convertSelector2Number(int selector[2]);
    int getNumberOfPosition();

    int getNextPosition();
    int getPreviousPosition();
    bool checkIsCorrespondingToPosition(int selector[2]);

    float getOffset();
    int getNumberOfLine();
    int getNumberOfColumn();
    int getPickingPositionFlag(int number);
    float *getPosition(int number);
    float *getPrePosition(int number);
    StabilizedTrajectory *getRemoveExcessTrajectory();
    StabilizedTrajectory *getFinishedTrajectory();


    ~Plate();
};

#endif
