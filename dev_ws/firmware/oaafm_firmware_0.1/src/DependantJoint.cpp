#include "DependantJoint.h"

DependantJoint::DependantJoint(int id, Dynamixel2Arduino *dxlSerial, Joint *dependantJoint) : Joint(id, dxlSerial), _dependantJoint(dependantJoint)
{
}

void DependantJoint::init(float maxVelocity, int minPositionLimit, int maxPositionLimit, const float gearRatio, const float offset, const float depFactor)
{
    _maxVelocity = maxVelocity;
    _minPositionLimit = minPositionLimit;
    _maxPositionLimit = maxPositionLimit;
    _offset = offset;
    _gearRatio = gearRatio;
    _depFactor = depFactor;
    _targetPosition = 0.0;
    _encodedVelocity = abs(_maxVelocity * _gearRatio) / 0.229;
    if (_encodedVelocity < 2)
    {
        _encodedVelocity = 2;
    }
    this->getCurrentPosition();
    _dxlSerial->torqueOff(_id);
    _dxlSerial->setOperatingMode(_id, OP_EXTENDED_POSITION);
    _dxlSerial->writeControlTableItem(PROFILE_VELOCITY, _id, _encodedVelocity);
    _dxlSerial->writeControlTableItem(PROFILE_ACCELERATION, _id, _encodedVelocity / 2);
}

void DependantJoint::calibrate(float position1, float position2, float position3)
{
    _offset = position1;
    _depFactor = (position2 - _offset) / _dependantJoint->getMinPositionLimit();
    _gearRatio = (position3 - _offset) / _minPositionLimit;
    _targetPosition = 0.0;
    _encodedVelocity = abs(_maxVelocity * _gearRatio) / 0.229;
    if (_encodedVelocity < 2)
    {
        _encodedVelocity = 2;
    }
    this->getCurrentPosition();
    _dxlSerial->torqueOff(_id);
    _dxlSerial->setOperatingMode(_id, OP_EXTENDED_POSITION);
    _dxlSerial->writeControlTableItem(PROFILE_VELOCITY, _id, _encodedVelocity);
    _dxlSerial->writeControlTableItem(PROFILE_ACCELERATION, _id, _encodedVelocity / 2);
}

void DependantJoint::setTargetPosition(float position)
{
    if ((position>=_minPositionLimit && position<=_maxPositionLimit)==true)
    {
        _targetPosition=position;
    }
    else
    {
        Serial.println();
        Serial.println(F("Out joint range"));
        Serial.println();
    }
}

float DependantJoint::getCurrentPosition()
{
    _currentPosition = ((_dxlSerial->getPresentPosition(_id, UNIT_DEGREE) - ((_dependantJoint->getActuatorPosition()-_dependantJoint->getOffset())/_dependantJoint->getGearRatio() * _depFactor) - _offset) / _gearRatio);
    return _currentPosition;
}

void DependantJoint::move()
{
    _dxlSerial->setGoalPosition(_id, _targetPosition * _gearRatio + _dependantJoint->getTargetPosition() * _depFactor + _offset, UNIT_DEGREE);
}

void DependantJoint::printParameters()
{
    Serial.println();
    Serial.print(F("Joint :")); Serial.print(_id); Serial.print(F("GearRatio :")); Serial.print(_gearRatio); Serial.print(F("Offset :")); Serial.print(_offset); Serial.print(F("DepFactor :")); Serial.println(_depFactor);
    Serial.println();
}

DependantJoint::~DependantJoint()
{
}
