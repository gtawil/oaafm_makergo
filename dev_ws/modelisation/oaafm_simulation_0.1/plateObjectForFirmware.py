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
import codecs


""" define and initialize variables  """

#Mode 
MODE="EXPORT"  # MODE ="SIMULATION" ou MODE = "EXPORT"

# Parameter you need to change
RECORDED_CENTER_PLATE_POSITION=[-40.96, -81.96, -23.89, 0.130, 0.0, 0.0, -62.9] #26/02 proto rose
RECORDED_EDGE_PLATE_POSITION=[-39.8, -76.76, -39.86, 0.130, 0.0, 0.0, -58.28]

# Define pitch for spliting the plate
PITCH=0.8
OFFSET=40.0
POSITION_PER_TRAJECTORY=4  #for fnished Trajectory

# Define Plate
FILE_NAME = "plate.txt"
PLATE_NAME = "plate1"
PLATE_TYPE = "circle"
PLATE_DIAMETER=0.175
PLATE_HEIGHT=0.04

# Define Spoon (endEffector)
ENDEFFECTOR_TYPE = "ellipsis"
ENDEFFECTOR_MAJORAXIS=0.05
ENDEFFECTOR_MINORAXIS=0.03


""" Main programm  """

# Position for simulation
# Define position to deploy the arm
HOME_POSITION=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
PRE_USING_POSITION1=[0.0, -90.0, 0.0, 0.0, 0.0, 0.0, 0.0]
PRE_USING_POSITION2=[0.0, -90.0, -90.0, 0.0, 0.0, 0.0, 0.0]
PRE_USING_POSITION3=[0.0, -90.0, -90.0, 0.130, 0.0, 0.0, 0.0]

#Define position to simulate, in real the mouth is saved before starting the meal
TRANSITION_POSITION=[0.0, -110.0, -30.0, 0.130, -40.0, 0.0, 0.0]
MOUTH_POSITION=[45.0, -60.0, -30.0, 0.130, 0.0, 0.0, 0.0]


def main():

    """ 
        STEP 1 : Calculate the world center position of the plate
    """

    #create robot
    OAAFM_P1= robot.Robot("P1_URDF_25012020/6axis_P1_07112020.urdf")
    OAAFM_P1.startSimulation()

    #Starting sequence
    OAAFM_P1.moveTo(PRE_USING_POSITION1)
    OAAFM_P1.moveTo(PRE_USING_POSITION2)
    OAAFM_P1.moveTo(PRE_USING_POSITION3)


    # Move to center plate postion with values recorded on the prototype and calculate the world position of the center plate
    OAAFM_P1.moveTo(RECORDED_CENTER_PLATE_POSITION)
    world_center_plate_position=OAAFM_P1.getWorldPosition()
    print("World center plate :")
    print(world_center_plate_position)
    robot.printFrameWorld(world_center_plate_position)
    time.sleep(2.)

    # Move to center plate postion with values recorded on the prototype and calculate the world position of the center plate
    OAAFM_P1.moveTo(RECORDED_EDGE_PLATE_POSITION)
    world_edge_plate_position=OAAFM_P1.getWorldPosition()
    print("World edge plate :")
    print(world_edge_plate_position)
    robot.printFrameWorld(world_edge_plate_position)
    time.sleep(2.)
    # Stop the simulation
    OAAFM_P1.endSimulation()


    """ 
        STEP 2 : Split the plate and calculate :
         - picking position and pre picking position in the World space
         - finish trajectory
         - remove excess trajectory
    """
    # Calculate PLATE_HEIGHT par rapport au position enregistrer
    PLATE_HEIGHT=np.round(abs(world_center_plate_position[2]-world_edge_plate_position[2]),4)+0.01
    print("Plate height :")
    print(PLATE_HEIGHT)
    PLATE_HEIGHT=0.03

    # Create the plate
    currentPlate = plate.Plate(world_center_plate_position, PLATE_DIAMETER, PLATE_HEIGHT)

    # Create the Spoon (endeffector)
    currentEndEffector = endEffector.EndEffector(ENDEFFECTOR_MAJORAXIS, ENDEFFECTOR_MINORAXIS)

    # Split the plate based on the size of the end effector and chosing pitch
    currentPlate.split(currentEndEffector.getSize(), PITCH)


    # Calculate finished trajectories
    currentPlate.calculateFinishTrajectory( currentEndEffector.getSize(), POSITION_PER_TRAJECTORY)


    #info plot
    currentPlate.showPositionOnPlate()

    if MODE=="EXPORT":
        """ 
            STEP 4 : Edit the File to push in the firmware
        """

        fichier = open("temp",'wt')
        fichier.write("// the new plate is :  ")
        fichier.write(PLATE_NAME)
        fichier.write("\n")

        fichier.write("float OFFSET = ")
        fichier.write(str(OFFSET))
        fichier.write(";\n")
        fichier.write("\n")

        fichier.write("int NUMBER_OF_POSITION = ")
        fichier.write(str(currentPlate.getColumn()*currentPlate.getLine()))
        fichier.write(";\n")
        fichier.write("\n")

        fichier.write("int NUMBER_OF_LINE = ")
        fichier.write(str(currentPlate.getLine()))
        fichier.write(";\n")
        fichier.write("\n")
        
        fichier.write("int NUMBER_OF_COLUMN = ")
        fichier.write(str(currentPlate.getColumn()))
        fichier.write(";\n")
        fichier.write("\n")

        fichier.write("PickingPosition PICKING_POSITION_LIST[")
        fichier.write(str(currentPlate.getColumn()*currentPlate.getLine()))
        fichier.write("] = \n{\n")
        for i in range(currentPlate.getLine()):
            for j in range(currentPlate.getColumn()):
                fichier.write("{")
                fichier.write(str(int(currentPlate.getFlagInMap(i,j))))
                fichier.write(", ")
                fichier.write(str(tuple(robot.world2JointForFirmware(OAAFM_P1, currentPlate.getPrePickingPosition(i,j)))))
                fichier.write(", ")
                fichier.write(str(tuple(robot.world2JointForFirmware(OAAFM_P1, currentPlate.getPickingPosition(i,j)))))
                fichier.write("},\n")
        fichier.write("};\n")
        fichier.write("\n")

        #todo
        fichier.write("HalfPosition FINISHED_TRAJECTORY_POSITION[4] = \n{\n")

        fichier.write("};\n")
        fichier.write("\n")

        fichier.write("HalfPosition REMOVE_EXCESS_POSITION[3] = \n{\n")

        fichier.write("};\n")
        fichier.write("\n")


        fichier.close()

        
        contents = codecs.open("temp", encoding='utf-8').read()
        print(contents)
        newcontents = contents.replace('(','{')
        finalcontents = newcontents.replace(')', '}')
        fichier2 = open(FILE_NAME,'wt')
        fichier2.write(finalcontents)
        fichier2.close()

    else:
        """
            Simulation vérification
        """
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
        for i in range(currentPlate.getLine()):
            for j in range(currentPlate.getColumn()):
                flag=currentPlate.getFlagInMap(i,j)
                if flag==0:
                    pass
                elif flag==1 :
                    OAAFM_P1.moveTo(robot.world2JointForModel(OAAFM_P1, currentPlate.getPrePickingPosition(i,j)))
                    pickingPosition=robot.world2JointForModel(OAAFM_P1, currentPlate.getPickingPosition(i,j))
                    pickingPosition[6]=pickingPosition[6]+OFFSET
                    OAAFM_P1.moveTo(pickingPosition)
                    pickingPosition[6]=pickingPosition[6]-OFFSET
                    OAAFM_P1.moveTo(pickingPosition)
                elif flag==2:
                    OAAFM_P1.moveTo(robot.world2JointForModel(OAAFM_P1, currentPlate.getPrePickingPosition(i,j)))
                    pickingPosition=robot.world2JointForModel(OAAFM_P1, currentPlate.getPickingPosition(i,j))
                    pickingPosition[6]=pickingPosition[6]-OFFSET
                    OAAFM_P1.moveTo(pickingPosition)
                    pickingPosition[6]=pickingPosition[6]+OFFSET
                    OAAFM_P1.moveTo(pickingPosition)
                OAAFM_P1.moveTo(TRANSITION_POSITION)



main()