#include "Arm.h"

// Define starting position
const float FOLDED_HOME_POSITION[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
const float DEPLOYED_TRANSITION_POSITION[6] = {0.0, -90.0, 0.0, 0.0, 0.0, 0.0};
const float DEPLOYED_HOME_POSITION[6] = {0.0, -90.0, -90.0, 0.0, 0.0, 0.0};

Arm::Arm(Joint *joint1, Joint *joint2, DependantJoint *joint3, Joint *joint4, Joint *joint5, Joint *joint6, Button *armSwitch) : _joints(), _switch(armSwitch)
{
    _joints.push_back(joint1);
    _joints.push_back(joint2);
    _joints.push_back(joint3);
    _joints.push_back(joint4);
    _joints.push_back(joint5);
    _joints.push_back(joint6);
    _deployTrajectory = Trajectory();
    _deployTrajectory.addPosition(DEPLOYED_TRANSITION_POSITION);
    _deployTrajectory.addPosition(DEPLOYED_HOME_POSITION);
    _deployTrajectory.setEpsilon(8);
    _foldTrajectory = Trajectory();
    _foldTrajectory.addPosition(DEPLOYED_HOME_POSITION);
    _foldTrajectory.addPosition(FOLDED_HOME_POSITION);
    _foldTrajectory.setEpsilon(8);

    _calibrationFlag = false;
    _deployFlag = false;
}

bool Arm::isConnected()
{
    for (int i = 0; i < 6; i++)
    {
        if (_joints[i]->isConnected() == false)
        {
            return false;
        }
        delay(200);
    }
    return true;
}

void Arm::init(float maxVelocity, const int jointLimit[6][2], const float calibrationTab[6][3])
{
    _switch->init();
    for (int i = 0; i < 6; i++)
    {
        _joints[i]->init(maxVelocity, jointLimit[i][1], jointLimit[i][0], calibrationTab[i][0], calibrationTab[i][2], calibrationTab[i][1]);
    }
}

bool Arm::isInHomePosition()
{
    //Check if the position and calibration are ok
    this->getCurrentPosition();
    for (int i = 0; i < 3; i++)
    {
        if (((_currentPosition[i] > 2.5) || (_currentPosition[i] < -2.5)) == true)
        {
            _calibrationFlag = false;
            return false;
        }
    }
    _calibrationFlag = true;
    _deployFlag = false;
    return true;
}

bool Arm::isCalibrated()
{
    return _calibrationFlag;
}

bool Arm::isEnabled()
{
    bool enabled = true;
    for (int i = 0; i < 6; i++)
    {
        enabled = _joints[i]->isEnabled() && enabled;
    }
    return enabled;
}

void Arm::turnTorqueOn(int jointId = ALL)
{
    switch (jointId)
    {
    case ALL:
        for (int i = 0; i < 6; i++)
        {
            _joints[i]->turnTorqueOn();
        }
        break;

    case THREE:
        for (int i = 0; i < 3; i++)
        {
            _joints[i]->turnTorqueOn();
        }
        break;

    default:
        _joints[jointId - 1]->turnTorqueOn();
        break;
    }
}

void Arm::turnTorqueOff(int jointId = ALL)
{
    switch (jointId)
    {
    case ALL:
        for (int i = 0; i < 6; i++)
        {
            _joints[i]->turnTorqueOff();
        }
        break;

    case THREE:
        for (int i = 0; i < 3; i++)
        {
            _joints[i]->turnTorqueOff();
        }
        break;

    default:
        _joints[jointId - 1]->turnTorqueOff();
        break;
    }
}

void Arm::deploy()
{
    this->turnTorqueOn(ALL);
    this->moveWithTrajectory(&_deployTrajectory);
    _deployFlag = true;
}

void Arm::fold()
{
    this->moveWithTrajectory(&_foldTrajectory);
    this->turnTorqueOff(ALL);
    _deployFlag = false;
}

bool Arm::isDeploy()
{
    return _deployFlag;
}

void Arm::goToHome()
{
    this->setTargetPosition(DEPLOYED_HOME_POSITION, ALL);
    this->move(ALL);
    this->waitingForReachingTarget(false);
}

/* MOVING

*/

bool Arm::isMoving()
{
    bool moving = false;
    for (int i = 0; i < 6; i++)
    {
        moving = _joints[i]->isMoving() || moving;
    }
    return moving;
}

float *Arm::getCurrentPosition()
{
    for (int i = 0; i < 6; i++)
    {
        _currentPosition[i] = _joints[i]->getCurrentPosition();
    }
    return _currentPosition;
}

void Arm::setTargetPosition(float *position, int jointId)
{
    switch (jointId)
    {
    case ALL:
        for (int i = 0; i < 6; i++)
        {
            _joints[i]->setTargetPosition(position[i]);
        }
        break;

    case FOUR:
        for (int i = 0; i < 4; i++)
        {
            _joints[i]->setTargetPosition(position[i]);
        }
        break;

    default:
        _joints[jointId - 1]->setTargetPosition(position[0]);
        break;
    }
}

void Arm::setTargetPosition(const float *position, int jointId)
{
    switch (jointId)
    {
    case ALL:
        for (int i = 0; i < 6; i++)
        {
            _joints[i]->setTargetPosition(position[i]);
        }
        break;

    case FOUR:
        for (int i = 0; i < 4; i++)
        {
            _joints[i]->setTargetPosition(position[i]);
        }
        break;

    default:
        _joints[jointId - 1]->setTargetPosition(position[0]);
        break;
    }
}


void Arm::move(int jointId)
{
    switch (jointId)
    {
    case ALL:
        for (int i = 0; i < 6; i++)
        {
            _joints[i]->move();
        }
        break;

    case FOUR:
        for (int i = 0; i < 4; i++)
        {
            _joints[i]->move();
        }
        break;

    default:
        _joints[jointId - 1]->move();
        break;
    }
}

void Arm::moveWithTrajectory(Trajectory *trajectory)
{
    if (trajectory != 0)
    {
        int trajectorySize = trajectory->getTrajectorySize();
        for (int i = 0; i < trajectorySize; i++)
        {
            this->setTargetPosition(trajectory->getPosition(i), ALL);
            this->move(ALL);
            while (this->isReached(trajectory->getEpsilon()) == false)
            {
                this->move(ALL);
            }
        }
    }
}

void Arm::moveWithTrajectory(StabilizedTrajectory *trajectory)
{
    if (trajectory != 0)
    {
        int trajectorySize = trajectory->getTrajectorySize();
        for (int i = 0; i < trajectorySize; i++)
        {
            this->setTargetPosition(trajectory->getPosition(i), FOUR);
            this->move(FOUR);
            while (this->isReached(trajectory->getEpsilon()) == false)
            {
                this->move(FOUR);
                this->stabiliseSpoon();
            }
        }
    }
}

bool Arm::waitingForReachingTarget(bool stabilization)
{
    delay(500);
    while (this->isReached(8) == false)
    {
        this->move(ALL);
        if (this->isMoving())
        {
            if (stabilization)
            {
                this->stabiliseSpoon();
            }
        }
        else
        {
            this->stabiliseSpoon();
            return false;
        }
    }
    return true;
}

bool Arm::isReached(float epsilon)
{
    float difference = 0.0;
    for (int i = 0; i < 6; i++)
    {
        difference = difference + _joints[i]->compareCurrentTarget();
    }
    if (difference < epsilon)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Arm::stabiliseSpoon(float offset)
{
    this->getCurrentPosition();
    if (_currentPosition[0] > 0)
    {
        _joints[4]->setTargetPosition(-_currentPosition[0]);
    }
    else
    {
        _joints[4]->setTargetPosition(0.0);
    }
    _joints[5]->setTargetPosition((-180 - _currentPosition[1] - _currentPosition[2] - _currentPosition[3]) + offset);
    for (int i = 0; i < 2; i++)
    {
        _joints[i + 4]->move();
    }
}

void Arm::stabiliseSpoonWithTorqueOff()
{
    this->getCurrentPosition();
    _joints[3]->setTargetPosition(-180 - _currentPosition[1] - _currentPosition[2]);
    _joints[4]->setTargetPosition(-_currentPosition[0]);
    _joints[5]->setTargetPosition(0.0);
    for (int i = 0; i < 3; i++)
    {
        _joints[i + 3]->move();
    }
}

/**
 * ------------------------------
 * Fonction for Debugging
 * ------------------------------
 */
void Arm::printCurrentPosition()
{
    this->getCurrentPosition();
    Serial.println();
    Serial.println(F("Position : "));
    for (int i = 0; i < 6; i++)
    {
        Serial.print(F("Joint : ")); Serial.print(_joints[i]->getId()); Serial.print(F(" : ")); Serial.println(_currentPosition[i]);
    }
    Serial.println();
    delay(200);
}

void Arm::initCalibration()
{
    _calibrationStep = 0;
    _savingPositionCounter = 0;
    _calibrationFlag=false;
}

bool Arm::calibrationSequence()
{
    if (_calibrationStep < 3)
    {
        int flag = _switch->getFlag();
        if(flag==SHORTPUSHED)
        {
            switch(_calibrationStep)
            {
                //Calibration Joint 1
                case 0:
                    if (_savingPositionCounter < 2)
                    {
                        Serial.println();
                        Serial.println(F("Position Saved"));
                        delay(200);
                        _joints[0]->printActuatorPosition();
                        Serial.println();
                        delay(200);
                        _savedPosition[_savingPositionCounter] = _joints[0]->getActuatorPosition();
                        _savingPositionCounter++;
                    }
                    if (_savingPositionCounter == 2)
                    {
                        _joints[0]->calibrate(_savedPosition[0], _savedPosition[1]);
                        delay(200);
                        _joints[0]->printParameters();
                        delay(200);
                        _calibrationStep++;
                        _savingPositionCounter = 0;
                    }
                    break;

                // Calibration Joint 2&3
                case 1:
                    if (_savingPositionCounter < 3)
                    {
                        Serial.println();
                        Serial.println(F("Position Saved"));
                        delay(200);
                        _joints[1]->printActuatorPosition();
                        _joints[2]->printActuatorPosition();
                        delay(200);
                        _savedPosition[_savingPositionCounter] = _joints[1]->getActuatorPosition();
                        _savedPosition[_savingPositionCounter+3] = _joints[2]->getActuatorPosition();
                        _savingPositionCounter++;
                    }
                    if (_savingPositionCounter == 3)
                    {
                        _joints[1]->calibrate(_savedPosition[0], _savedPosition[1]);
                        _joints[2]->calibrate(_savedPosition[3], _savedPosition[4], _savedPosition[5]);
                        delay(200);
                        _joints[1]->printParameters();
                        _joints[2]->printParameters();
                        delay(200);
                        _calibrationStep++;
                        _savingPositionCounter = 0;
                    }
                    break;

                // Calibration 4, 5 & 6
                case 2:
                    Serial.println();
                    Serial.println(F("Position Saved"));
                    delay(200);
                    _joints[3]->printActuatorPosition();
                    _joints[4]->printActuatorPosition();
                    _joints[5]->printActuatorPosition();
                    Serial.println();
                    delay(200);
                    _savedPosition[0] = _joints[3]->getActuatorPosition();
                    _savedPosition[1] = _joints[4]->getActuatorPosition();
                    _savedPosition[2] = _joints[5]->getActuatorPosition();
                    _joints[3]->setOffset(_savedPosition[0]);
                    _joints[4]->setOffset(_savedPosition[1]);
                    _joints[5]->setOffset(_savedPosition[2]);

                    delay(200);
                    Serial.println();
                    Serial.print(F("Calibration parameters"));
                    Serial.println();
                    _joints[3]->printParameters();
                    _joints[4]->printParameters();
                    _joints[5]->printParameters();
                    delay(200);
                    _calibrationStep++;
                    _savingPositionCounter = 0;
                    break;

                default:
                    Serial.println(F("Finish"));
                    break;
            }
            this->calibrationInfo(_calibrationStep, _savingPositionCounter);
        }
    }
    else
    {
        Serial.println();
        Serial.print(F("Calibration parameters"));
        for (int i = 0; i < 6; i++)
        {
            _joints[i]->printParameters();
        }
        _calibrationFlag = true;
        Serial.println();
    }
    return _calibrationFlag;
}

void Arm::calibrationInfo(int stepCounter, int positionCounter)
{
    switch (stepCounter)
    {
        case 0:
            Serial.println();
            Serial.print(F("Put joint 1 on :"));
            if (positionCounter == 0)
            {
                Serial.println(_joints[stepCounter]->getMaxPositionLimit());
                
            }
            else if (positionCounter == 1)
            {
                Serial.println(_joints[stepCounter]->getMinPositionLimit());
            }
            Serial.println();
            break;

        case 1:
            Serial.println();
            Serial.print(F("Put joints 2&3 on :"));
            if (positionCounter == 0)
            {
                Serial.print(_joints[1]->getMaxPositionLimit()); Serial.print(F(" , ")); Serial.print(_joints[2]->getMaxPositionLimit());
            }
            else if (positionCounter == 1)
            {
                Serial.print(_joints[1]->getMinPositionLimit()); Serial.print(F(" , ")); Serial.print(_joints[2]->getMaxPositionLimit());
            }
            else if (positionCounter == 2)
            {
                Serial.print(_joints[1]->getMaxPositionLimit()); Serial.print(F(" , ")); Serial.print(_joints[2]->getMinPositionLimit());
            }
            Serial.println();
            break;

        case 2:
            Serial.println();
            Serial.print(F("Put joints 4,5&6 on : 0"));
            Serial.println();
            break;
        
        default:
            Serial.println(F("Finish"));
            break;
    }
}

void Arm::savingPositionSequence()
{
    if (_switch->getFlag() == SHORTPUSHED)
    {
        this->printCurrentPosition();
    }
}

Arm::~Arm()
{
    
}