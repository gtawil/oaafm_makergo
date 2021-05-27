#ifndef DEPENDANTJOINT_H
#define DEPENDANTJOINT_H

#include "Joint.h"

class DependantJoint : public Joint
{
private:
    Joint *_dependantJoint;
    float _depFactor; //This for the joint depend an other
public:
    DependantJoint(int id, Dynamixel2Arduino *dxlSerial, Joint *dependantJoint);

    virtual void init(float maxVelocity, int minPositionLimit, int maxPositionLimit, const float gearRatio, const float offset, const float depFactor);
    virtual void calibrate(float position1, float position2, float position3);

    virtual void setTargetPosition(float position);
    virtual void move();
    virtual float getCurrentPosition();

    virtual void printParameters();

    virtual ~DependantJoint();
};

#endif
