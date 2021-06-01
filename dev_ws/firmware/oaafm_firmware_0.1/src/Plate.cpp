#include "Plate.h"

Plate::Plate(float offset, int numberOfPosition, int numberOfLine, int numberOfColumn, PickingPosition pickingPositionList[21], HalfPosition removeExcessPosition[3], HalfPosition finishedPosition[4]) : _positionList()
{
    _offset=offset; _numberOfPosition=numberOfPosition; _numberOfLine=numberOfLine; _numberOfColumn=numberOfColumn;
    for (int i = 0; i < numberOfPosition; i++)
    {
        _positionList.push_back(pickingPositionList[i]);
    }
    _finishedTrajectory = StabilizedTrajectory();
    for (int i = 0; i < 4; i++)
    {
        _finishedTrajectory.addPosition(finishedPosition[i]);
    }
    _removeExcessTrajectory = StabilizedTrajectory();
    for (int i = 0; i < 3; i++)
    {
        _removeExcessTrajectory.addPosition(removeExcessPosition[i]);
    }
}

void Plate::init()
{
    
    this->initSelector();
    // pickingPosition init
    
}

int Plate::initSelector()
{
    _selector2D[0]=int(_numberOfLine/2);
    _selector2D[1]=int(_numberOfColumn/2);
    _selector1D=0;
    return this->convertSelector2Number(_selector2D);
}

int Plate::moveSelector(SelectorDirection direction)
{
    int newSelector[2]={_selector2D[0],_selector2D[1]};
    switch (direction)
    {
        case sUp:
            newSelector[0]--;
            newSelector[1]--;
            break;
        case sDown:
            newSelector[0]++;
            newSelector[1]++;
            break;
        case sLeft:
            newSelector[0]++;
            newSelector[1]--;
            break;
        case sRight:
            newSelector[0]--;
            newSelector[1]++;
            break;
        default:
            break;
    }
    Serial.println();
    Serial.print(F("Address"));Serial.print(newSelector[0]);Serial.print(F(" & "));Serial.println(newSelector[1]);
    Serial.println();
    if (checkIsCorrespondingToPosition(newSelector))
    {     
        _selector2D[0]=newSelector[0]; 
        _selector2D[1]=newSelector[1];
    }
    return this->convertSelector2Number(_selector2D);
}

int Plate::convertSelector2Number(int selector[2])
{
    return (selector[0]*_numberOfColumn)+selector[1];
}

int Plate::getNumberOfPosition()
{
    return _numberOfPosition;
}

int Plate::getNextPosition()
{
    _selector1D++;
    if(_selector1D>=_numberOfPosition)
    {
        _selector1D=0;
    }
    return _selector1D;
}

int Plate::getPreviousPosition()
{
    _selector1D--;
    if(_selector1D<0)
    {
        _selector1D=_numberOfPosition-1;
    }
    return _selector1D;
}

bool Plate::checkIsCorrespondingToPosition(int selector[2])
{
    if (((selector[0] >= 0) && (selector[0] < _numberOfLine) && (selector[1] >= 0) && (selector[1] < _numberOfColumn))==true)
    {
        int posAddress=this->convertSelector2Number(selector);
        if (this->getPickingPositionFlag(posAddress)!=0)
        {
            Serial.println();
            Serial.println(F("flag 0"));
            Serial.println();
            return true;
        }
        else
        {
            return true;
        }
    }
    else
    {
        Serial.println();
        Serial.println(F("Out of the plate"));
        Serial.println();
        return false;
    }
}

float Plate::getOffset()
{
    return _offset;
}

int Plate::getNumberOfLine()
{
    return _numberOfLine;
}

int Plate::getNumberOfColumn()
{
    return _numberOfColumn;
}

int Plate::getPickingPositionFlag(int number)
{
    return _positionList[number].flag;
}

float *Plate::getPosition(int number)
{
    return _positionList[number].position; //_pickingPositions[counter].position;
}

float *Plate::getPrePosition(int number)
{
    return _positionList[number].prePosition; //_pickingPositions[counter].position;
}

StabilizedTrajectory *Plate::getRemoveExcessTrajectory()
{
    return &_removeExcessTrajectory;
}

StabilizedTrajectory *Plate::getFinishedTrajectory()
{
    return &_finishedTrajectory;
}

Plate::~Plate()
{
}