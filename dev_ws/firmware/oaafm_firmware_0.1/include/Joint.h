#ifndef JOINT_H
#define JOINT_H

#include <Dynamixel2Arduino.h>

using namespace ControlTableItem;

class Joint
{
protected:
    int _id;
    Dynamixel2Arduino *_dxlSerial;
    float _maxVelocity;
    int _encodedVelocity;
    float _currentPosition;
    float _targetPosition;
    int _minPositionLimit;
    int _maxPositionLimit;
    float _offset;
    float _gearRatio;

public:
    Joint(int id, Dynamixel2Arduino *dxlSerial);

    int getId();

    bool isConnected();
    virtual void init(float maxVelocity, int minPositionLimit, int maxPositionLimit, const float gearRatio, const float offset, const float depFactor = 0.0);
    virtual void calibrate(float position1, float position2, float position3 = 0.0);
    void setOffset(float offset);
    void setGearRatio(float gearRatio);
    float getOffset();
    float getGearRatio();

    bool isEnabled();
    void turnTorqueOn();
    void turnTorqueOff();

    bool isMoving();
    virtual float getCurrentPosition();
    virtual void setTargetPosition(float position);
    virtual float getTargetPosition();
    void setProfilVelocity(float velocity);
    virtual void move();
    double compareCurrentTarget();

    float getMinPositionLimit();
    float getMaxPositionLimit();
    
    float getActuatorPosition();

    void printActuatorPosition();
    virtual void printParameters();

    virtual ~Joint();

    /* TODO
void calibrate();

    
    
    
    

        static void calibrationSequence();
        void setProfilVelocity(float velocity);
        */
};

#endif
