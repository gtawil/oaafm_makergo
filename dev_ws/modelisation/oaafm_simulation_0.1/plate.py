# -*- coding: utf-8 -*-
"""

Created on 24/01/2021
classe Plate
@author: Georges

"""


import numpy as np
import matplotlib.pyplot as plt

class Plate:

    def __init__(self, centerPosition, diameter, height):
        self.centerPosition= centerPosition
        self.diameter=diameter
        self.height=height
        self.rotateAngle=np.pi/4
        

    def split(self, endEffectorSize, pitch):

        """ Create map of the plate """
        self.lineNumber=int(self.diameter/(endEffectorSize[0]*pitch)) 
        self.columnNumber=int(self.diameter/(endEffectorSize[1]*pitch))

        
        print(self.lineNumber)
        print(self.columnNumber)

        #modify pair to impair
        if (self.lineNumber%2==0):
            self.lineNumber=self.lineNumber-1
        if (self.columnNumber%2==0):
            self.columnNumber=self.columnNumber-1

        print("new line and column:")
        print(self.lineNumber)
        print(self.columnNumber)

        self.plateMap=np.ones((self.lineNumber,self.columnNumber))
        for i in range(self.lineNumber):
            for j in range(self.columnNumber):
                if j>=(self.columnNumber/2):
                    self.plateMap[i][j]=2
        print(" plate map :")
        print(self.plateMap)

        """ 
        Picking positions calculation
            We calculate picking positions linked to the map of flag made before in the world frame.
        """

        # Start by create a matrix to store world position (x, y, z)
        platePickingPositions=np.zeros((self.lineNumber,self.columnNumber,3))

        Xpitch=(self.diameter-endEffectorSize[1])/self.columnNumber
        Ypitch=(self.diameter-endEffectorSize[0])/self.lineNumber

        platePickingPositions[int(self.lineNumber/2),int(self.columnNumber/2),0]=0
        platePickingPositions[int(self.lineNumber/2),int(self.columnNumber/2),1]=0
        for i in range(1,int(self.lineNumber/2)+1):
            platePickingPositions[int(self.lineNumber/2)-i,int(self.columnNumber/2),0]=0
            platePickingPositions[int(self.lineNumber/2)-i,int(self.columnNumber/2),1]=i*Ypitch
        for i in range(1,int(self.lineNumber/2)+1):
            platePickingPositions[int(self.lineNumber/2)+i,int(self.columnNumber/2),0]=0
            platePickingPositions[int(self.lineNumber/2)+i,int(self.columnNumber/2),1]=-i*Ypitch
        for j in range(1,int(self.columnNumber/2)+1):
            platePickingPositions[int(self.lineNumber/2),int(self.columnNumber/2)-j,0]=-j*Xpitch
            platePickingPositions[int(self.lineNumber/2),int(self.columnNumber/2)-j,1]=0
        for j in range(1,int(self.columnNumber/2)+1):
            platePickingPositions[int(self.lineNumber/2),int(self.columnNumber/2)+j,0]=j*Xpitch
            platePickingPositions[int(self.lineNumber/2),int(self.columnNumber/2)+j,1]=0

        ellipseTrajectory_majorAxis=self.diameter-endEffectorSize[1]
        ellipseTrajectory_minorAxis=self.diameter-endEffectorSize[0]
        
        for i in range(1,int(self.lineNumber/2)+1):
            for j in range(1,int(self.columnNumber/2)+1):
                new_x=-j*Xpitch
                new_y=i*Ypitch
                normVectPoint=np.linalg.norm([new_x,new_y])
                teta=np.arctan2(new_y, new_x)
                normVectEllipse=np.linalg.norm([(ellipseTrajectory_majorAxis/2) * np.cos(teta),(ellipseTrajectory_minorAxis/2) * np.sin(teta)])
                if (normVectPoint<= normVectEllipse):
                    platePickingPositions[int(self.lineNumber/2)-i,int(self.columnNumber/2)-j,0]=new_x
                    platePickingPositions[int(self.lineNumber/2)-i,int(self.columnNumber/2)-j,1]=new_y
                else:
                    self.plateMap[int(self.lineNumber/2)-i,int(self.columnNumber/2)-j]=0

        for i in range(1,int(self.lineNumber/2)+1):
            for j in range(1,int(self.columnNumber/2)+1):
                new_x=-j*Xpitch
                new_y=-i*Ypitch
                normVectPoint=np.linalg.norm([new_x,new_y])
                teta=np.arctan2(new_y, new_x)
                normVectEllipse=np.linalg.norm([(ellipseTrajectory_majorAxis/2) * np.cos(teta),(ellipseTrajectory_minorAxis/2) * np.sin(teta)])
                if (normVectPoint<= normVectEllipse):
                    platePickingPositions[int(self.lineNumber/2)+i,int(self.columnNumber/2)-j,0]=new_x
                    platePickingPositions[int(self.lineNumber/2)+i,int(self.columnNumber/2)-j,1]=new_y
                else:
                    self.plateMap[int(self.lineNumber/2)+i,int(self.columnNumber/2)-j]=0

        for i in range(1,int(self.lineNumber/2)+1):
            for j in range(1,int(self.columnNumber/2)+1):
                new_x=j*Xpitch
                new_y=-i*Ypitch
                normVectPoint=np.linalg.norm([new_x,new_y])
                teta=np.arctan2(new_y, new_x)
                normVectEllipse=np.linalg.norm([(ellipseTrajectory_majorAxis/2) * np.cos(teta),(ellipseTrajectory_minorAxis/2) * np.sin(teta)])
                if (normVectPoint<= normVectEllipse):
                    platePickingPositions[int(self.lineNumber/2)+i,int(self.columnNumber/2)+j,0]=new_x
                    platePickingPositions[int(self.lineNumber/2)+i,int(self.columnNumber/2)+j,1]=new_y
                else:
                    self.plateMap[int(self.lineNumber/2)-i,int(self.columnNumber/2)+j]=0

        for i in range(1,int(self.lineNumber/2)+1):
            for j in range(1,int(self.columnNumber/2)+1):
                new_x=j*Xpitch
                new_y=i*Ypitch
                normVectPoint=np.linalg.norm([new_x,new_y])
                teta=np.arctan2(new_y, new_x)
                normVectEllipse=np.linalg.norm([(ellipseTrajectory_majorAxis/2) * np.cos(teta),(ellipseTrajectory_minorAxis/2) * np.sin(teta)])
                if (normVectPoint<= normVectEllipse):
                    platePickingPositions[int(self.lineNumber/2)-i,int(self.columnNumber/2)+j,0]=new_x
                    platePickingPositions[int(self.lineNumber/2)-i,int(self.columnNumber/2)+j,1]=new_y
                else:
                    self.plateMap[int(self.lineNumber/2)+i,int(self.columnNumber/2)+j]=0

        print(" plate map :")
        print(self.plateMap)
                

        # Convert in the world space
        self.worldPickingPositions=np.zeros((self.lineNumber,self.columnNumber,3))
        for i in range(self.lineNumber):
            for j in range(self.columnNumber):
                self.worldPickingPositions[i,j,:]=self.plate2world(platePickingPositions[i,j,:])

        # Calculate PrepickingPosition from picking position
        self.worldPrePickingPositions=np.zeros((self.lineNumber,self.columnNumber,3))
        for i in range(self.lineNumber):
            for j in range(self.columnNumber):
                self.worldPrePickingPositions[i][j][0]=self.worldPickingPositions[i][j][0]
                self.worldPrePickingPositions[i][j][1]=self.worldPickingPositions[i][j][1]
                self.worldPrePickingPositions[i][j][2]=self.worldPickingPositions[i][j][2]+self.height

    def calculateFinishTrajectory(self, endEffectorSize, positionPerTrajectory):
        # Calculate the elliptic trajectory for the finished
        
        ellipseTrajectory_majorAxis=self.diameter-endEffectorSize[1]
        ellipseTrajectory_minorAxis=self.diameter-endEffectorSize[0]
        self.positionPerTrajectory=positionPerTrajectory
        
        ellipsePitch=np.pi/(2*(positionPerTrajectory-1))

        self.finishTrajectories=np.zeros((4,positionPerTrajectory,3))
        #Trajectoire coin haut-droit
        counterEllipse=np.pi/2
        for i in range(positionPerTrajectory):
            self.finishTrajectories[0,i,0]=(ellipseTrajectory_majorAxis/2) * np.cos(counterEllipse)
            self.finishTrajectories[0,i,1]=(ellipseTrajectory_minorAxis/2) * np.sin(counterEllipse)
            counterEllipse=counterEllipse-ellipsePitch

        #Trajectoire coin haut-gauche
        counterEllipse=np.pi/2
        for i in range(positionPerTrajectory):
            self.finishTrajectories[1,i,0]=(ellipseTrajectory_majorAxis/2) * np.cos(counterEllipse)
            self.finishTrajectories[1,i,1]=(ellipseTrajectory_minorAxis/2) * np.sin(counterEllipse)
            counterEllipse=counterEllipse+ellipsePitch

        #Trajectoire coin bas-droit
        counterEllipse=-np.pi/2
        for i in range(positionPerTrajectory):
            self.finishTrajectories[2,i,0]=(ellipseTrajectory_majorAxis/2) * np.cos(counterEllipse)
            self.finishTrajectories[2,i,1]=(ellipseTrajectory_minorAxis/2) * np.sin(counterEllipse)
            counterEllipse=counterEllipse+ellipsePitch

        #Trajectoire coin bas-gauche
        counterEllipse=-np.pi/2
        for i in range(positionPerTrajectory):
            self.finishTrajectories[3,i,0]=(ellipseTrajectory_majorAxis/2) * np.cos(counterEllipse)
            self.finishTrajectories[3,i,1]=(ellipseTrajectory_minorAxis/2) * np.sin(counterEllipse)
            counterEllipse=counterEllipse-ellipsePitch

        # Convert in the world space
        for i in range(4):
            for j in range(positionPerTrajectory):
                self.finishTrajectories[i,j,:]=self.plate2world(self.finishTrajectories[i,j,:])


    def plate2world(self, platePosition):

        transformationMatrix_Rplate_to_world=np.array([  [np.cos(self.rotateAngle), np.sin(self.rotateAngle), 0, self.centerPosition[0]],
                                                [-np.sin(self.rotateAngle), np.cos(self.rotateAngle), 0, self.centerPosition[1]],
                                                [0, 0, 1, self.centerPosition[2]],
                                                [0, 0, 0, 1]])
        platePosition=np.hstack((platePosition,1))

        worldPosition=transformationMatrix_Rplate_to_world.dot(platePosition)
        return worldPosition[:3]

    def getMap(self):
        return self.plateMap

    def getColumn(self):
        return self.columnNumber

    def getLine(self):
        return self.lineNumber

    def getFinishTrajecories(self):
        return self.finishTrajectories

    def getPickingPositionsTab(self):
        return self.worldPickingPositions

    def getPrePickingPositionsTab(self):
        return self.worldPrePickingPositions

    def getPickingPosition(self, posX, posY):
        return self.worldPickingPositions[posX][posY]

    def getPrePickingPosition(self, posX, posY):
        return self.worldPrePickingPositions[posX][posY]

    def getFlagInMap(self, i, j):
        return self.plateMap[i][j]

    def showPositionOnPlate(self):
        # Create world point to represent the edge of the plate
        plateX= [ self.centerPosition[0] + (self.diameter/2) * np.cos(t) for t in np.arange(0., 2*np.pi, 0.05)]
        plateY= [ self.centerPosition[1] + (self.diameter/2) * np.sin(t) for t in np.arange(0., 2*np.pi, 0.05)]

        #print picking position
        pickingPositionX=[]
        pickingPositionY=[]
        for i in range(self.lineNumber):
            for j in range(self.columnNumber):
                pickingPositionX.append(self.worldPickingPositions[i,j,0])
                pickingPositionY.append(self.worldPickingPositions[i,j,1])

        #print trajectory haut droit en vert
        finishTrajectoryX_1=[]
        finishTrajectoryY_1=[]
        for j in range(self.positionPerTrajectory):
            finishTrajectoryX_1.append(self.finishTrajectories[0,j,0])
            finishTrajectoryY_1.append(self.finishTrajectories[0,j,1])

        #print trajectory haut gauche en rouge
        finishTrajectoryX_2=[]
        finishTrajectoryY_2=[]
        for j in range(self.positionPerTrajectory):
            finishTrajectoryX_2.append(self.finishTrajectories[1,j,0])
            finishTrajectoryY_2.append(self.finishTrajectories[1,j,1])

        #print trajectory bas droite en orange
        finishTrajectoryX_3=[]
        finishTrajectoryY_3=[]
        for j in range(self.positionPerTrajectory):
            finishTrajectoryX_3.append(self.finishTrajectories[2,j,0])
            finishTrajectoryY_3.append(self.finishTrajectories[2,j,1])

        #print trajectory bas gauche en jaune
        finishTrajectoryX_4=[]
        finishTrajectoryY_4=[]
        for j in range(self.positionPerTrajectory):
            finishTrajectoryX_4.append(self.finishTrajectories[3,j,0])
            finishTrajectoryY_4.append(self.finishTrajectories[3,j,1])

        plt.plot(plateX, plateY, pickingPositionX, pickingPositionY, 'ks', finishTrajectoryX_1, finishTrajectoryY_1, 'gs', finishTrajectoryX_2, finishTrajectoryY_2, 'rs', finishTrajectoryX_3, finishTrajectoryY_3, 'cs', finishTrajectoryX_4, finishTrajectoryY_4, 'ys' )
        plt.axis('equal')
        plt.show()