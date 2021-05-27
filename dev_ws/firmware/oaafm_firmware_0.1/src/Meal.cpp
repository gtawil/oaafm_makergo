#include "Meal.h"

#define EPSILON 5.0

#define PICKFOOD 0
#define SERVICE 1

float mouthPosition[4] = {0, 0, 0, 0};
float transitionPosition[4] = {0, -110.0, -30.0, -40.0};

Meal::Meal( Arm *arm, Button *armSwitch, Button *userSwitch1, Button *userSwitch2, Joystick *joystick, Plate *plate1) :  _arm(arm), _armSwitch(armSwitch), _userSwitch1(userSwitch1), _userSwitch2(userSwitch2), _joystick(joystick), _plate(plate1)
{
    _servingTrajectory = StabilizedTrajectory();
}

void Meal::init()
{
    _state = BEGINING;
    _nextAction = PICKFOOD;
    _mouthPositionSavedFlag = false;
    _plate->init();
    _controlMode=SIMPLESWITCH;
    _userSwitch1->init();
    /*
    _velocity=usingProfil->speed;
    _userSwitch1->init(usingProfil->timeShortPushed ,usingProfil->timeLongPushed);
    _userSwitch2->init(usingProfil->timeShortPushed ,usingProfil->timeLongPushed);
    _joystick->init();
    */
    if (_controlMode==JOYSTICKANDSWITCH)
    {
        _pickingAddress = _plate->initSelector();
    }
    else
    {
        _pickingAddress = 0;
    }
}

void Meal::serveToMouth()
{
    _arm->moveWithTrajectory(&_servingTrajectory);
    delay(3000);
}

void Meal::pickFood()
{
    delay(200);
    if (_plate->getPickingPositionFlag(_pickingAddress) == 1)
    {
        this->moveWithFood(_plate->getPrePosition(_pickingAddress), -_plate->getOffset());
        delay(500);
        this->moveWithFood(_plate->getPosition(_pickingAddress), -_plate->getOffset());
        delay(500);
        this->moveWithFood(_plate->getPosition(_pickingAddress), 0.0);
    }
    else if (_plate->getPickingPositionFlag(_pickingAddress) == 2)
    {
        this->moveWithFood(_plate->getPrePosition(_pickingAddress), _plate->getOffset());
        delay(500);
        this->moveWithFood(_plate->getPosition(_pickingAddress), _plate->getOffset());
        delay(500);
        this->moveWithFood(_plate->getPosition(_pickingAddress), 0.0);
    }
    delay(500);
    this->moveWithFood(_plate->getPrePosition(_pickingAddress), 0.0);
    delay(300);
    this->removeExcess();
}

void Meal::moveWithFood(float position[4], float offset)
{
    _arm->setTargetPosition(position, FOUR);
    _arm->move(FOUR);
    _arm->stabiliseSpoon(offset);
    _arm->waitingForReachingTarget(true);
}

void Meal::removeExcess()
{
    _arm->moveWithTrajectory(_plate->getRemoveExcessTrajectory());
}

void Meal::setMealState(MealState state)
{
    _state = state;
}

MealState Meal::getMealState()
{
    return _state;
}

void Meal::simpleSwitchSequence()
{
    int flag1 = _userSwitch1->getFlag();
    if (flag1 == SHORTPUSHED)
    {
        if (_nextAction == SERVICE)
        {
            this->serveToMouth();
            _nextAction = PICKFOOD;
        }
        else
        {
            this->moveWithFood(transitionPosition);
            this->moveWithFood(_plate->getPrePosition(_pickingAddress));
            this->pickFood();
            _pickingAddress=_plate->getNextPosition();
            _nextAction = SERVICE;   
        }
    }
    else if (flag1 == LONGPUSHED)
    {
        if (_nextAction == SERVICE)
        {
            this->moveWithFood(_plate->getPrePosition(_pickingAddress));
            this->pickFood();
            _pickingAddress=_plate->getNextPosition();
            _nextAction = SERVICE;
        }
        else if (_nextAction == PICKFOOD)
        {
            delay(1000);
            _arm->goToHome();
            this->setMealState(FINISHED);
        }
    }
}

void Meal::doubleSwitchSequence()
{
    int flag1 = _userSwitch1->getFlag();
    int flag2 = _userSwitch2->getFlag();
    if (_nextAction == PICKFOOD)
    {
        if (flag1==SHORTPUSHED)
        {
            _pickingAddress=_plate->getNextPosition();
            this->moveWithFood(_plate->getPrePosition(_pickingAddress));
        }
        if (flag2==SHORTPUSHED)
        {
            _pickingAddress=_plate->getPreviousPosition();
            this->moveWithFood(_plate->getPrePosition(_pickingAddress));
        }
        if (flag1==LONGPUSHED)
        {
            this->pickFood();
            this->serveToMouth();
            _nextAction = SERVICE;
        }
        if (flag2==LONGPUSHED)
        {
            _arm->moveWithTrajectory(_plate->getFinishedTrajectory());
            this->serveToMouth();
            _nextAction = SERVICE;
        }
    }
    else
    {
        if (flag1==SHORTPUSHED)
        {
            this->moveWithFood(transitionPosition);
            _pickingAddress=_plate->getNextPosition();
            this->moveWithFood(_plate->getPrePosition(_pickingAddress));
            _nextAction = PICKFOOD;
        }
        if (flag2==LONGPUSHED)
        {
            this->setMealState(FINISHED);
            this->moveWithFood(transitionPosition);
        }
    }
}

void Meal::joystickAndSwitchSequence()
{
    int flag1 = _userSwitch1->getFlag();
    int joystickFlag = _joystick->getState();
    if (_nextAction == PICKFOOD)
    {
        if (_joystick->isBackInMiddle())
        {
            switch(joystickFlag)
            {
                case JOYSTCIKDOWN:
                    _pickingAddress=_plate->moveSelector(DOWN);
                    _joystick->needToBackInMiddle();
                    this->moveWithFood(_plate->getPrePosition(_pickingAddress));
                    break;
                case JOYSTCIKUP:
                    _pickingAddress=_plate->moveSelector(UP);
                    _joystick->needToBackInMiddle();
                    this->moveWithFood(_plate->getPrePosition(_pickingAddress));
                    break;
                case JOYSTCIKLEFT:
                    _pickingAddress=_plate->moveSelector(LEFT);
                    _joystick->needToBackInMiddle();
                    this->moveWithFood(_plate->getPrePosition(_pickingAddress));
                    break;
                case JOYSTCIKRIGHT:
                    _pickingAddress=_plate->moveSelector(RIGHT);
                    _joystick->needToBackInMiddle();
                    this->moveWithFood(_plate->getPrePosition(_pickingAddress));
                    break;

                default:
                    break;
            }
        }
        if (flag1==SHORTPUSHED)
        {
            this->pickFood();
            this->serveToMouth();
            _nextAction = SERVICE;
        }
        else if (flag1==LONGPUSHED)
        {
            _arm->moveWithTrajectory(_plate->getFinishedTrajectory());
            this->removeExcess();
            this->serveToMouth();
            _nextAction = SERVICE;
        }
    }
    else
    {
        if (flag1==SHORTPUSHED)
        {
            _pickingAddress=_plate->initSelector();
            this->moveWithFood(transitionPosition);
            this->moveWithFood(_plate->getPrePosition(_pickingAddress));
            _nextAction = PICKFOOD;
        }
        if (flag1==LONGPUSHED)
        {
            this->setMealState(FINISHED);
            this->moveWithFood(transitionPosition);
        }
    }
}

void Meal::eatingSequence()
{
    switch (_controlMode)
    {
        case SIMPLESWITCH:
                this->simpleSwitchSequence();
            break;

        case DOUBLESWITCH:
                this->doubleSwitchSequence();
            break;

        case JOYSTICKANDSWITCH:
                this->joystickAndSwitchSequence();
            break;
        default :
                this->simpleSwitchSequence();
            break;
    }
}

void Meal::savingMouthPositionSequence()
{
    _arm->stabiliseSpoonWithTorqueOff();
    int flag = _armSwitch->getFlag();
    switch (flag)
    {
    case SHORTPUSHED:
        this->setServingTrajectory(_arm->getCurrentPosition());
        _mouthPositionSavedFlag = true;
        Serial.println();
        Serial.println(F("Mouth Position Saved"));
        Serial.println();
        break;

    case LONGPUSHED:
        _mouthPositionSavedFlag = false;
        Serial.println();
        Serial.println(F("Set position again"));
        Serial.println();
        break;

    default:
        if (_armSwitch->isHolded())
        {
            _arm->turnTorqueOff(THREE);
        }
        else
        {
            _arm->turnTorqueOn(THREE);
        }
        break;
    }
    if (_userSwitch1->getFlag() == SHORTPUSHED)
    {
        if (_mouthPositionSavedFlag)
        {
            Serial.println();
            Serial.println(F("Position Validated"));
            Serial.println();
            _arm->setTargetPosition(transitionPosition, FOUR);
            _arm->move(FOUR);
            _arm->waitingForReachingTarget(true);
            this->setMealState(ONGOING);
        }
    }
}

void Meal::setServingTrajectory(float mouthPositon[6])
{
    float position[4] = {0.0, 0.0, 0.0, 0.0};
    for (int i = 0; i < 4; i++)
    {
        position[i] = mouthPositon[i];
    }
    _servingTrajectory = StabilizedTrajectory();
    _servingTrajectory.addPosition(transitionPosition);
    _servingTrajectory.addPosition(position);
}

Meal::~Meal()
{
}
