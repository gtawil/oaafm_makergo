# -*- coding: utf-8 -*-

"""
OAAFM Simulation with Pybullet

Created on 25/01/2021
Version : 0.01
Last update : 25/01/2021

purpose : main program to simulate a meal

@author: Georges
"""

import robot
import plate
import endEffector
import numpy as np
import time


# Main

# DEFINITION ET INITIALISATION DES VARIABLES

# Define Plate
WORLD_CENTER_PLATE_POSITION=[0.125, 0.125, 0.051]
PLATE_DIAMETER=0.175
PLATE_HEIGHT=0.035   # hauteurPrePosition=0.04 utilisé avant

ENDEFFECTOR_MAJORAXIS=0.05
ENDEFFECTOR_MINORAXIS=0.03

# Define pitch for spliting the plate
PITCH=0.5

# Position take on prototype
JOINT_CENTER_PLATE_POSITION=[-36.19, -81.41, -26.33, 0.130, 0.0, 0.0, -68.0]
HOME_POSITION=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
PRE_USING_POSITION1=[0.0, -90.0, -90.0, 0.0, 0.0, 0.0, 0.0]
PRE_USING_POSITION2=[0.0, -90.0, -90.0, 0.130, 0.0, 0.0, 0.0]
UP_JOINT_CENTER_PLATE_POSITION=[-48.32, -93.93, -21.74, 0.130, 0.0, 0.0, -62.74]
BPlateCornerPosition=[-48.20, -74.66, -32.28, 0.130, 0.0, 0.0, -71.44]

def degre2rad(angle):
    return (angle*np.pi)/180.0

def radian2degree(angle):
    return (angle*180)/np.pi

def real2model(realPosition):
        command=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        Factor=[-1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0]
        for i in np.arange(7):
            if i!=3:
                command[i]=degre2rad(realPosition[i])*Factor[i]
            else:
                command[i]=realPosition[i]*Factor[i]
        return command

def world2Joint(robotId, worldPositionTab):
    n,m,l=np.shape(worldPositionTab)
    jointPositionTab=np.zeros((n,m,7))
    for i in range(n):
        for j in range(m):
            tempojointposition=robotId.MGI(worldPositionTab[i][j])
            for k in range(len(tempojointposition)):
                tempojointposition[k]=radian2degree(tempojointposition[k])
            jointPositionTab[i][j][0]=tempojointposition[0]
            jointPositionTab[i][j][1]=tempojointposition[1]
            jointPositionTab[i][j][2]=tempojointposition[2]
            jointPositionTab[i][j][3]=0.130
            jointPositionTab[i][j][6]=tempojointposition[3]
    return jointPositionTab

def main():
    #create robot
    OAAFM_P1= robot.Robot("P1_URDF_25012020/6axis_P1_07112020.urdf")
    
    OAAFM_P1.startSimulation()
    #Start
    time.sleep(1.)
    OAAFM_P1.moveTo(real2model(PRE_USING_POSITION1))
    time.sleep(1.)
    OAAFM_P1.moveTo(real2model(PRE_USING_POSITION2))
    time.sleep(1.)
    OAAFM_P1.moveTo(real2model(JOINT_CENTER_PLATE_POSITION))

    world_center_plate_position=OAAFM_P1.getWorldPosition()
    print(world_center_plate_position)
    time.sleep(10.)

    OAAFM_P1.endSimulation()

    #create plate
    currentPlate = plate.Plate(world_center_plate_position, PLATE_DIAMETER, PLATE_HEIGHT)

    #create endeffector
    currentEndEffector = endEffector.EndEffector(ENDEFFECTOR_MAJORAXIS, ENDEFFECTOR_MINORAXIS)

    #create commmand
    #pad=tkinter.Tk()

    #Split the plate based on the size of the end effector and chosing pitch
    currentPlate.split2(currentEndEffector.getSize(), 0.5)

    world_picking_positions_tab=currentPlate.getPickingPositionsTab()
    print(" Position dans le monde pour chaque positions de picking ::: ")
    print(world_picking_positions_tab)

    joint_picking_positions_tab=world2Joint(OAAFM_P1, world_picking_positions_tab)
    print(" Position angulaire pour chaque positions de picking ::: ")
    print(joint_picking_positions_tab)

    world_pre_picking_positions_tab=currentPlate.getPrePickingPositionsTab()
    joint_pre_picking_positions_tab=world2Joint(OAAFM_P1, world_pre_picking_positions_tab)


    #Start simulation
    OAAFM_P1.startSimulation()
    #Start
    time.sleep(1.)
    OAAFM_P1.moveTo(real2model(PRE_USING_POSITION1))
    time.sleep(1.)
    OAAFM_P1.moveTo(real2model(PRE_USING_POSITION2))
    time.sleep(1.)
    OAAFM_P1.moveTo(real2model(UP_JOINT_CENTER_PLATE_POSITION))

    offset=30.0
    v,h,w=np.shape(joint_picking_positions_tab)
    for i in range(v):
        for j in range(h):
            flag=currentPlate.getFlagInMap(i,j)
            if flag==0:
                pass
            elif flag==1 :
                OAAFM_P1.moveTo(real2model(joint_pre_picking_positions_tab[i][j]))
                joint_picking_positions_tab[i][j][6]=joint_picking_positions_tab[i][j][6]+offset
                OAAFM_P1.moveTo(real2model(joint_picking_positions_tab[i][j]))
                joint_picking_positions_tab[i][j][6]=joint_picking_positions_tab[i][j][6]-offset
                OAAFM_P1.moveTo(real2model(joint_picking_positions_tab[i][j]))
            elif flag==2:
                OAAFM_P1.moveTo(real2model(joint_pre_picking_positions_tab[i][j]))
                joint_picking_positions_tab[i][j][6]=joint_picking_positions_tab[i][j][6]-offset
                OAAFM_P1.moveTo(real2model(joint_picking_positions_tab[i][j]))
                joint_picking_positions_tab[i][j][6]=joint_picking_positions_tab[i][j][6]+offset
                OAAFM_P1.moveTo(real2model(joint_picking_positions_tab[i][j]))


    plateMap=plate.getMap()
    endEffectorWorldPosition=robot.pybullet.getLinkState(OAAFM_P1.robotId, 7)
    v=list(endEffectorWorldPosition[0])

    OAAFM_P1.moveTo(real2model(BPlateCornerPosition))
    endEffectorWorldPosition2=robot.pybullet.getLinkState(OAAFM_P1.robotId, 7)
    u=list(endEffectorWorldPosition2[0])

main()