#ifndef POSITION_H
#define POSITION_H

struct HalfPosition
{
    float _position[4];
};

struct Position
{
    float _position[6];
};

struct PickingPosition
{
    int flag;
    float prePosition[4];
    float position[4];
};

#endif