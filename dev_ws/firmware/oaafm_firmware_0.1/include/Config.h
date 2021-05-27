#ifndef CONFIG_H
#define CONFIG_H


#include "Position.h"
#include <Arduino.h>

/*
******************************************************************************
                Set up Debug mode
******************************************************************************
*/

#define DEBUG

/*

-------------------------------
PROTO ROSE CONFIG
-------------------------------
*/

// Define DXL Motors communication
#define DXL_SERIAL Serial
const uint8_t DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN
const float DXL_PROTOCOL_VERSION = 2.0;

// Define pin for 3 switch : 1 in the arm and 2 for jack3.5 mm connector
#define ARM_SWITCH_PIN 18
#define USER_SWITCH1_PIN 19
#define USER_SWITCH2_PIN 20

// Define pin used by SUB D 9 
#define JOYSTICK_SPEED_PIN 8
#define JOYSTICK_DIRECTION_PIN 9
#define JOYSTICK_REF_PIN 10
#define JOYSTICK_THRESHOLD 40

//  Define pin used by RGB Led
#define RED_LED_PIN 25
#define GREEN_LED_PIN 27
#define BLUE_LED_PIN 29

//  Define Serial port used by bluetooth module
#define BT_SERIAL Serial2

//  Define actuators ID
#define M_XC430_W240_T 1
#define M_XM540_W270_T 2
#define M_XM430_W350_T 3
#define M_2XL430_W250_T_A 5
#define M_2XL430_W250_T_B 4
#define M_XL_320 6

#define TIME_AFTER_DEBUG_MESSAGE 300


// VARIBALES MECANIQUES (à modifier si le prototype change d'un point de vue méca)
const int JOINT_LIMIT[6][2] =
{
    {150, -150},
    {0, -180},
    {0, -180},
    {90, -90},
    {90, -90},
    {150, -150}
};

const float CALIBRATION_TAB[6][3] =
{
    {3.72, 0.0, 121.26},
    {2, 0.0, 311.7},
    {-4.0, -2.0, 88.26},
    {-1.0, 0.0, 179},
    {-1.0, 0.0, 179.12},
    {1, 0.0, 147}
};


float OFFSET = 40.0;

int NUMBER_OF_POSITION = 21;

int NUMBER_OF_LINE = 3;

int NUMBER_OF_COLUMN = 7;

PickingPosition PICKING_POSITION_LIST[21] =
{
    {0, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}},
    {1, {-54.538, -87.355, -26.167, 0.0}, {-54.538, -79.551, -26.065, 0.0}},
    {1, {-55.539, -90.438, -22.964, 0.0}, {-55.539, -81.759, -22.852, 0.0}},
    {1, {-56.787, -93.944, -19.726, 0.0}, {-56.787, -84.149, -19.6, 0.0}},
    {2, {-58.39, -98.099, -16.425, 0.0}, {-58.39, -86.825, -16.28, 0.0}},
    {2, {-60.54, -103.343, -13.013, 0.0}, {-60.54, -89.989, -12.84, 0.0}},
    {0, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}},
    {1, {-44.056, -82.808, -31.482, 0.0}, {-44.056, -76.116, -31.394, 0.0}},
    {1, {-43.972, -85.303, -28.48, 0.0}, {-43.972, -78.027, -28.385, 0.0}},
    {1, {-43.872, -87.968, -25.503, 0.0}, {-43.872, -79.998, -25.4, 0.0}},
    {1, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}},
    {2, {-43.601, -94.069, -19.618, 0.0}, {-43.601, -84.233, -19.492, 0.0}},
    {2, {-43.409, -97.713, -16.708, 0.0}, {-43.409, -86.584, -16.565, 0.0}},
    {2, {-43.159, -101.99, -13.819, 0.0}, {-43.159, -89.193, -13.654, 0.0}},
    {0, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}},
    {1, {-34.249, -82.661, -31.665, 0.0}, {-34.249, -76.002, -31.578, 0.0}},
    {1, {-33.311, -84.907, -28.942, 0.0}, {-33.311, -77.728, -28.849, 0.0}},
    {1, {-32.206, -87.24, -26.292, 0.0}, {-32.206, -79.467, -26.191, 0.0}},
    {2, {-30.889, -89.676, -23.724, 0.0}, {-30.889, -81.223, -23.615, 0.0}},
    {2, {-29.3, -92.228, -21.257, 0.0}, {-29.3, -82.995, -21.139, 0.0}},
    {0, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}}
};

HalfPosition FINISHED_TRAJECTORY_POSITION[4] =
{
    {-58.56, -85.35, -19.66, 0.0},
    {-54.05, -80.78, -25.74, 0.0},
    {-47.78, -77.29, -30.89, 0.0},
    {-40.97, -75.19, -34.18, 0.0}
};

HalfPosition REMOVE_EXCESS_POSITION[3] = 
{
    {-40.97, -78.33, -34.2, 0.0},
    {-40.97, -81.37, -34.33, 0.0},
    {-41.37, -78.1, -38.67, 0.0}
};

/*

-------------------------------
PROTO BLEU CONFIG
-------------------------------

// Define DXL Motors communication
#define DXL_SERIAL Serial
const uint8_t DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN
const float DXL_PROTOCOL_VERSION = 2.0;

// Define pin for 3 switch : 1 in the arm and 2 for jack3.5 mm connector
#define ARM_SWITCH_PIN 18
#define USER_SWITCH1_PIN 19
#define USER_SWITCH2_PIN 20

// Define pin used by SUB D 9 
#define JOYSTICK_SPEED_PIN 8
#define JOYSTICK_DIRECTION_PIN 9
#define JOYSTICK_REF_PIN 10
#define JOYSTICK_THRESHOLD 40

//  Define pin used by RGB Led
#define RED_LED_PIN 25
#define GREEN_LED_PIN 27
#define BLUE_LED_PIN 29

//  Define Serial port used by bluetooth module
#define BT_SERIAL Serial2

//  Define actuators ID
#define M_XC430_W240_T 1
#define M_XM540_W270_T 2
#define M_XM430_W350_T 3
#define M_2XL430_W250_T_A 5
#define M_2XL430_W250_T_B 4
#define M_XL_320 6

#define TIME_AFTER_DEBUG_MESSAGE 300




// VARIBALES MECANIQUES (à modifier si le prototype change d'un point de vue méca)
const int JOINT_LIMIT[6][2] =
{
    {150, -150},
    {0, -180},
    {0, -180},
    {90, -90},
    {90, -90},
    {150, -150}
};

const float CALIBRATION_TAB[6][3] =
{
    {3.72, 0.0, 121.26},
    {2, 0.0, 311.7},
    {-4.0, -2.0, 88.26},
    {-1.0, 0.0, 179},
    {-1.0, 0.0, 179.12},
    {1, 0.0, 147}
};

float OFFSET = 40.0;

int NUMBER_OF_POSITION = 21;

int NUMBER_OF_LINE = 3;

int NUMBER_OF_COLUMN = 7;

PickingPosition PICKING_POSITION_LIST[21] =
{
    {0, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}},
    {1, {-54.538, -87.355, -26.167, 0.0}, {-54.538, -79.551, -26.065, 0.0}},
    {1, {-55.539, -90.438, -22.964, 0.0}, {-55.539, -81.759, -22.852, 0.0}},
    {1, {-56.787, -93.944, -19.726, 0.0}, {-56.787, -84.149, -19.6, 0.0}},
    {2, {-58.39, -98.099, -16.425, 0.0}, {-58.39, -86.825, -16.28, 0.0}},
    {2, {-60.54, -103.343, -13.013, 0.0}, {-60.54, -89.989, -12.84, 0.0}},
    {0, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}},
    {1, {-44.056, -82.808, -31.482, 0.0}, {-44.056, -76.116, -31.394, 0.0}},
    {1, {-43.972, -85.303, -28.48, 0.0}, {-43.972, -78.027, -28.385, 0.0}},
    {1, {-43.872, -87.968, -25.503, 0.0}, {-43.872, -79.998, -25.4, 0.0}},
    {1, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}},
    {2, {-43.601, -94.069, -19.618, 0.0}, {-43.601, -84.233, -19.492, 0.0}},
    {2, {-43.409, -97.713, -16.708, 0.0}, {-43.409, -86.584, -16.565, 0.0}},
    {2, {-43.159, -101.99, -13.819, 0.0}, {-43.159, -89.193, -13.654, 0.0}},
    {0, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}},
    {1, {-34.249, -82.661, -31.665, 0.0}, {-34.249, -76.002, -31.578, 0.0}},
    {1, {-33.311, -84.907, -28.942, 0.0}, {-33.311, -77.728, -28.849, 0.0}},
    {1, {-32.206, -87.24, -26.292, 0.0}, {-32.206, -79.467, -26.191, 0.0}},
    {2, {-30.889, -89.676, -23.724, 0.0}, {-30.889, -81.223, -23.615, 0.0}},
    {2, {-29.3, -92.228, -21.257, 0.0}, {-29.3, -82.995, -21.139, 0.0}},
    {0, {-43.751, -90.863, -22.55, 0.0}, {-43.751, -82.055, -22.436, 0.0}}
};

HalfPosition FINISHED_TRAJECTORY_POSITION[4] =
{
    {-58.56, -85.35, -19.66, 0.0},
    {-54.05, -80.78, -25.74, 0.0},
    {-47.78, -77.29, -30.89, 0.0},
    {-40.97, -75.19, -34.18, 0.0}
};

HalfPosition REMOVE_EXCESS_POSITION[3] = 
{
    {-40.97, -78.33, -34.2, 0.0},
    {-40.97, -81.37, -34.33, 0.0},
    {-41.37, -78.1, -38.67, 0.0}
};

*/

#endif