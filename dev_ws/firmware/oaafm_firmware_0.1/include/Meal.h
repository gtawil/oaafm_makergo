#ifndef MEAL_H
#define MEAL_H

#include "Arm.h"
#include "Button.h"
#include "Joystick.h"
#include "Plate.h"

enum ControlMode
{
    simpleSwitch,
    doubleSwitch,
    joystickAndSwitch
};

enum MealState
{
    begining,
    mouthPositionSaving,
    ongoing,
    finished
};

class Meal
{
    private:
        Arm *_arm;
        Button *_armSwitch;
        Button *_userSwitch1;
        Button *_userSwitch2;
        Joystick *_joystick;
        Plate *_plate;

        ControlMode _controlMode;

        bool _mouthPositionSavedFlag;
        StabilizedTrajectory _servingTrajectory;

        int _nextAction;
        int _pickingAddress;
        MealState _state;

    public:
        Meal(Arm *arm, Button *armSwitch, Button *userSwitch1, Button *userSwitch2, Joystick *joystick, Plate *plate1);

        void init();

        void moveWithFood(float position[4], float offset = 0.0);
        void removeExcess();

        void serveToMouth();
        void pickFood();

        void setMealState(MealState state);
        MealState getMealState();

        void simpleSwitchSequence();
        void doubleSwitchSequence();
        void joystickAndSwitchSequence();
        void eatingSequence();
        void savingMouthPositionSequence();

        void setServingTrajectory(float mouthPositon[6]);
        void setMouthPositionSavedFlag(int flag);
        bool mouthPositionIsSaved();

        ~Meal();
};

#endif