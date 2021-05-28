# -*- coding: utf-8 -*-

"""
OAAFM Simulation with Pybullet

Created on 25/01/2021
Version : 0.01
Last update : 25/01/2021

purpose : main program to simulate a meal

@author: Georges
"""

# import libraries
import robot
import plate
import endEffector
import numpy as np
import time

# Main

# define and initialize variables

# Define Plate
RECORDED_CENTER_PLATE_POSITION=[-44.0, -81.96, -22.25, 0.130, 0.0, 0.0, -75.37]
RECORDED_EDGE_PLATE_POSITION=[-43.48, -77.52, -37.93, 0.130, 0.0, 0.0, -64.06]

PLATE_TYPE = "circle"
PLATE_DIAMETER=0.175
PLATE_HEIGHT=0.04   # real height is 25mm because of the default you need to ajust higher

# Define Spoon (endEffector)
ENDEFFECTOR_TYPE = "ellipsis"
ENDEFFECTOR_MAJORAXIS=0.05
ENDEFFECTOR_MINORAXIS=0.03


# Define position for control
HOME_POSITION=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
PRE_USING_POSITION1=[0.0, -90.0, 0.0, 0.0, 0.0, 0.0, 0.0]
PRE_USING_POSITION2=[0.0, -90.0, -90.0, 0.0, 0.0, 0.0, 0.0]
PRE_USING_POSITION3=[0.0, -90.0, -90.0, 0.130, 0.0, 0.0, 0.0]

TRANSITION_POSITION=[0.0, -110.0, -30.0, 0.130, -40.0, 0.0, 0.0]
MOUTH_POSITION=[45.0, -60.0, -30.0, 0.130, 0.0, 0.0, 0.0]

# Parameter you need to change

# Define pitch for spliting the plate
PITCH=0.5

OFFSET=40.0

def main():
    # STEP 1 : Calculate the world center position of the plate

    #create robot
    OAAFM_P1= robot.Robot("P1_URDF_25012020/6axis_P1_07112020.urdf")
    OAAFM_P1.startSimulation()
    #Starting sequence
    time.sleep(1.)
    OAAFM_P1.moveTo(PRE_USING_POSITION1)
    time.sleep(1.)
    OAAFM_P1.moveTo(PRE_USING_POSITION2)
    time.sleep(1.)
    OAAFM_P1.moveTo(PRE_USING_POSITION3)
    time.sleep(1.)
    # Move to center plate postion with values recorded on the prototype and calculate the world position of the center plate
    OAAFM_P1.moveTo(RECORDED_CENTER_PLATE_POSITION)
    time.sleep(1.)
    world_center_plate_position=OAAFM_P1.getWorldPosition()
    print("World center plate :")
    print(world_center_plate_position)
    robot.printFrameWorld(world_center_plate_position)
    time.sleep(2.)

    OAAFM_P1.moveTo(RECORDED_EDGE_PLATE_POSITION)
    time.sleep(1.)
    world_edge_plate_position=OAAFM_P1.getWorldPosition()
    print("World edge plate :")
    print(world_edge_plate_position)
    robot.printFrameWorld(world_edge_plate_position)
    time.sleep(2.)
    # Stop the simulation
    OAAFM_P1.endSimulation()

    # calculate from the model in Fusion 360
    #WORLD_CENTER_PLATE_POSITION=[0.125, 0.125, 0.051]  
    #world_center_plate_position=WORLD_CENTER_PLATE_POSITION

    # STEP 2 : Split the plate and calculate picking position in the World space

    # Create the plate
    currentPlate = plate.Plate(world_center_plate_position, PLATE_DIAMETER, PLATE_HEIGHT)
    # Create the Spoon (endeffector)
    currentEndEffector = endEffector.EndEffector(ENDEFFECTOR_MAJORAXIS, ENDEFFECTOR_MINORAXIS)
    # Split the plate based on the size of the end effector and chosing pitch
    currentPlate.split2(currentEndEffector.getSize(), 0.5)
    # Get picking position of the plate in the world space
    world_picking_positions_tab=currentPlate.getPickingPositionsTab()
    print(" Position dans le monde pour chaque positions de picking ::: ")
    print(world_picking_positions_tab)
    # Same process for the PrePicking Positions  which are just beyond the other one
    world_pre_picking_positions_tab=currentPlate.getPrePickingPositionsTab()


    # STEP 3 : Convert picking position in the world space to the joint space
    joint_picking_positions_tab=robot.world2Joint(OAAFM_P1, world_picking_positions_tab)
    print(" Position angulaire pour chaque positions de picking ::: ")
    print(joint_picking_positions_tab)
    joint_pre_picking_positions_tab=robot.world2Joint(OAAFM_P1, world_pre_picking_positions_tab)

    print(" Results ::: ")
    print(joint_picking_positions_tab)
    print(joint_pre_picking_positions_tab)

    # STEP 4 : Validate the picking position with simulation

    # Start simulation 
    OAAFM_P1.startSimulation()
    # Starting sequence
    time.sleep(1.)
    OAAFM_P1.moveTo(PRE_USING_POSITION1)
    time.sleep(1.)
    OAAFM_P1.moveTo(PRE_USING_POSITION2)
    time.sleep(1.)
    OAAFM_P1.moveTo(PRE_USING_POSITION3)
    time.sleep(1.)
    # Eating sequence
    v,h,w=np.shape(joint_picking_positions_tab)
    for i in range(v):
        for j in range(h):
            flag=currentPlate.getFlagInMap(i,j)
            if flag==1:
                pass
            elif flag==2 :
                OAAFM_P1.moveTo(joint_pre_picking_positions_tab[i][j])
                joint_picking_positions_tab[i][j][6]=joint_picking_positions_tab[i][j][6]+OFFSET
                OAAFM_P1.moveTo(joint_picking_positions_tab[i][j])
                joint_picking_positions_tab[i][j][6]=joint_picking_positions_tab[i][j][6]-OFFSET
                OAAFM_P1.moveTo(joint_picking_positions_tab[i][j])
            elif flag==3:
                OAAFM_P1.moveTo(joint_pre_picking_positions_tab[i][j])
                joint_picking_positions_tab[i][j][6]=joint_picking_positions_tab[i][j][6]-OFFSET
                OAAFM_P1.moveTo(joint_picking_positions_tab[i][j])
                joint_picking_positions_tab[i][j][6]=joint_picking_positions_tab[i][j][6]+OFFSET
                OAAFM_P1.moveTo(joint_picking_positions_tab[i][j])
            OAAFM_P1.moveTo(TRANSITION_POSITION)

    """
    plateMap=plate.getMap()
    endEffectorWorldPosition=robot.pybullet.getLinkState(OAAFM_P1.robotId, 7)
    v=list(endEffectorWorldPosition[0])

    OAAFM_P1.moveTo(real2model(BPlateCornerPosition))
    endEffectorWorldPosition2=robot.pybullet.getLinkState(OAAFM_P1.robotId, 7)
    u=list(endEffectorWorldPosition2[0])
    """


main()