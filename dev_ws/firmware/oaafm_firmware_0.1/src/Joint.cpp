#include "Joint.h"

Joint::Joint(int id, Dynamixel2Arduino *dxlSerial) : _id(id), _dxlSerial(dxlSerial)
{
}

int Joint::getId()
{
    return _id;
}

bool Joint::isConnected()
{
    if (_dxlSerial->ping(_id))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Joint::init(float maxVelocity, int minPositionLimit, int maxPositionLimit, float gearRatio, float offset, float depFactor)
{
    _maxVelocity = maxVelocity;
    _minPositionLimit = minPositionLimit;
    _maxPositionLimit = maxPositionLimit;
    _offset = offset;
    _gearRatio = gearRatio;
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

//Attention joint init
void Joint::calibrate(float position1, float position2, float position3)
{
    _gearRatio = (position1 - position2) / (_maxPositionLimit - _minPositionLimit);
    _offset = position1 - _gearRatio * _maxPositionLimit;
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

void Joint::setOffset(float offsetPosition)
{
    _offset=offsetPosition;
}

void Joint::setGearRatio(float gearRatio)
{
    _gearRatio=gearRatio;
}

float Joint::getOffset()
{
    return _offset;
}

float Joint::getGearRatio()
{
    return _gearRatio;
}

bool Joint::isEnabled()
{
    return _dxlSerial->readControlTableItem(TORQUE_ENABLE, _id);
}

void Joint::turnTorqueOn()
{
    _dxlSerial->torqueOn(_id);
}

void Joint::turnTorqueOff()
{
    _dxlSerial->torqueOff(_id);
}

bool Joint::isMoving()
{
    return _dxlSerial->readControlTableItem(MOVING, _id);
}

float Joint::getCurrentPosition()
{
    _currentPosition = ((_dxlSerial->getPresentPosition(_id, UNIT_DEGREE) - _offset) / _gearRatio);
    return _currentPosition;
}

void Joint::setTargetPosition(float position)
{
    if ((position>=_minPositionLimit && position<=_maxPositionLimit)==true)
    {
        _targetPosition=position;
    }
    else
    {
        Serial.println();
        Serial.println(F("Out of range"));
        Serial.println();
    }
}

float Joint::getTargetPosition()
{
    return _targetPosition;
}

void Joint::setProfilVelocity(float velocity)
{
    _encodedVelocity = (abs(velocity * _gearRatio) / 0.229);
    if (_encodedVelocity < 2)
    {
        _encodedVelocity = 2;
    }
    _dxlSerial->writeControlTableItem(PROFILE_VELOCITY, _id, _encodedVelocity);
    _dxlSerial->writeControlTableItem(PROFILE_ACCELERATION, _id, _encodedVelocity / 2);
}

void Joint::move()
{
    _dxlSerial->setGoalPosition(_id, _targetPosition * _gearRatio + _offset, UNIT_DEGREE);
}

double Joint::compareCurrentTarget()
{
    this->getCurrentPosition();
    return abs(_currentPosition - _targetPosition);
}

float Joint::getActuatorPosition()
{
    return _dxlSerial->getPresentPosition(_id, UNIT_DEGREE);
}

float Joint::getMinPositionLimit()
{
    return _minPositionLimit;
}

float Joint::getMaxPositionLimit()
{
    return _maxPositionLimit;
}

void Joint::printActuatorPosition()
{
    Serial.println();
    Serial.print(F("Joint :")); Serial.print(_id); delay(200); Serial.println(this->getActuatorPosition()); delay(200);
    Serial.println();
}

void Joint::printParameters()
{
    Serial.println();
    Serial.print(F("Joint :")); Serial.print(_id); Serial.print(F("GearRatio :")); Serial.print(_gearRatio); Serial.print(F("Offset :")); Serial.print(_offset);
    Serial.println();
}

Joint::~Joint()
{
}
