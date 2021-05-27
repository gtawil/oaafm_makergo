#ifndef ARM_H
#define ARM_H

/**
 * @file Arm.h
 * @author Georges Tawil
 * @brief This class provide all the functions relative to the motor control, especially :
 * 
 * @version 0.1
 * @date 2021-05-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
//#include "Config.h"
#include "Joint.h"
#include "DependantJoint.h"
#include "Trajectory.h"
#include "StabilizedTrajectory.h"
#include "Button.h"

//#include <StandardCplusplus.h>
#include <ArduinoSTL.h>
#include <vector>

#define ALL 0
#define THREE 10
#define FOUR 100

using namespace std;

class Arm
{
private:
    unsigned int _maxVelocity;
    float _currentPosition[6];
    float _targetPosition[6];
    std::vector<Joint*> _joints;
    Button *_switch;

    Trajectory _deployTrajectory;
    Trajectory _foldTrajectory;
    bool _deployFlag;

    bool _calibrationFlag;
    float _savedPosition[6];
    unsigned int _savingPositionCounter = 0;
    unsigned int _calibrationStep=0;

    unsigned int _infocounter;
    

public:
    Arm(Joint *joint1, Joint *joint2, DependantJoint *joint3, Joint *joint4, Joint *joint5, Joint *joint6, Button *armSwitch);

    bool isConnected();
    void init(float maxVelocity, const int jointLimit[6][2], const float calibrationTab[6][3]);
    bool isInHomePosition();
    bool isCalibrated();

    bool isEnabled();
    void turnTorqueOn(int jointId);
    void turnTorqueOff(int jointId);

    void deploy();
    void fold();
    bool isDeploy();
    void goToHome();

    bool waitingForReachingTarget(bool stabilization);
    bool isMoving();
    float *getCurrentPosition();
    void setTargetPosition(float position[], int jointId);
    void setTargetPosition(const float *position, int jointId);
    void move(int jointId);
    void moveWithTrajectory(Trajectory *trajectory);
    void moveWithTrajectory(StabilizedTrajectory *trajectory);
    bool isReached(float epsilon);
    void stabiliseSpoon(float offset=0.0);
    void stabiliseSpoonWithTorqueOff();
    
    void printCurrentPosition();

    void initCalibration();
    bool calibrationSequence();
    void calibrationInfo(int stepCounter, int positionCounter);

    void savingPositionSequence();

    ~Arm();

    /* TODO
        
        void setProfilVelocity(float velocity);
        */
};

#endif