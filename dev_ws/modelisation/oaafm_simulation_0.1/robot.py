import time
import pybullet
import pybullet_data
import numpy as np

def printFrameWorld(Origin):
    X=[Origin[0]+0.1,Origin[1],Origin[2]]
    Y=[Origin[0],Origin[1]+0.1,Origin[2]]
    Z=[Origin[0],Origin[1],Origin[2]+0.1]
    pybullet.addUserDebugLine(Origin,X,[1.0, 0.0, 0.0],2.0,0.0)
    pybullet.addUserDebugLine(Origin,Y,[0.0, 1.0, 0.0],2.0,0.0)
    pybullet.addUserDebugLine(Origin,Z,[0.0, 0.0, 1.0],2.0,0.0)

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

def world2JointForModel(robotId, worldPosition):
    jointPosition=[]
    tempojointposition=robotId.MGI(worldPosition)
    for k in range(len(tempojointposition)):
        tempojointposition[k]=radian2degree(tempojointposition[k])
    jointPosition.append(tempojointposition[0])
    jointPosition.append(tempojointposition[1])
    jointPosition.append(tempojointposition[2])
    jointPosition.append(0.130)
    jointPosition.append(0.0)
    jointPosition.append(0.0)
    jointPosition.append(tempojointposition[3])
    return jointPosition

# pour exporter ver fichier
def world2JointForFirmware(robotId, worldPosition):
    jointPosition=[]
    tempojointposition=robotId.MGI(worldPosition)
    for k in range(len(tempojointposition)):
        tempojointposition[k]=radian2degree(tempojointposition[k])
    jointPosition.append(np.round(tempojointposition[0],3))
    jointPosition.append(np.round(tempojointposition[1],3))
    jointPosition.append(np.round(tempojointposition[2],3))
    jointPosition.append(0.0)
    return jointPosition

class Robot:
    def __init__(self, urdfPath):
        self.urdfPath=urdfPath


    def startSimulation(self):
        #Simulation environement initialisation
        physicsClient = pybullet.connect(pybullet.GUI)
        pybullet.resetSimulation()
        pybullet.setRealTimeSimulation(0)
        pybullet.setGravity(0,0,-9.81)
        pybullet.setAdditionalSearchPath(pybullet_data.getDataPath())

        #Adding ground and robot
        #planeId = pybullet.loadURDF("plane.urdf")
        self.robotId = pybullet.loadURDF(self.urdfPath,[0, 0, 0], useFixedBase = 1)

    def endSimulation(self):
        pybullet.disconnect()

    def MGI(self, world_pos):
        #changement de repere
        R0_pos=[0.332, 0.075, 0.0]
        world_pos[0]=(world_pos[0]-R0_pos[0])
        world_pos[1]=(world_pos[1]-R0_pos[1])
        world_pos[2]=world_pos[2]-R0_pos[2]
        
        L1=0.128
        L2=0.398
        L3=0.032
        L4=0.4257
        L5=0.103
        L6=0.05065
        teta=np.arcsin(L3/L4)
        Q=[0.0, 0.0, 0.0, 0.0]
        R=np.sqrt((world_pos[0]**2)+ (world_pos[1]**2)-L5**2)
        T=np.linalg.norm([L3, L4])
        Z=world_pos[2]+L6-L1
        Q[0]=np.arctan2(world_pos[1],-world_pos[0])+np.arcsin(L5/np.linalg.norm([world_pos[0], world_pos[1]]))
        Q[2]=((np.pi)-teta-np.arccos((Z**2+R**2-L2**2-T**2)/(2*L2*T)))
        beta=-np.pi+teta+Q[2]
        V=L2+T*np.cos(beta)
        U=T*np.sin(beta)
        Q[1]=np.arctan2(V*Z-R*U,V*R+U*Z)
        Q[0]=-Q[0]
        Q[2]=-Q[2]
        Q[1]=-Q[1]
        Q[3]=-np.pi-Q[2]-Q[1]
        return Q

    # comparer deux listes
    def listCompare(self, L1, L2, epsilon):
        equal=True
        if len(L1)==len(L2):
            c=0
            while c<len(L1) and equal==True:
                if c!=3:
                    if abs(L1[c]-L2[c])>epsilon:
                        equal=False
                else:
                    if abs(L1[c]-L2[c])>0.002:
                        equal=False
                c=c+1
        else:
            equal=False
        return equal

    

    def moveTo(self, targetPosition):
        targetPosition=real2model(targetPosition)
        pybullet.setJointMotorControlArray(self.robotId, range(7), pybullet.POSITION_CONTROL, targetPosition)
        currentPosition=[j[0] for j in pybullet.getJointStates(self.robotId, range(7))]
        while self.listCompare(targetPosition, currentPosition, 0.005)==False:
            pybullet.stepSimulation()
            time.sleep(1./10.)
            currentPosition=[j[0] for j in pybullet.getJointStates(self.robotId, range(7))]


    def printFrameOnLink(self, Origin, Link):
        X=[Origin[0]+0.1,Origin[1],Origin[2]]
        Y=[Origin[0],Origin[1]+0.1,Origin[2]]
        Z=[Origin[0],Origin[1],Origin[2]+0.1]
        pybullet.addUserDebugLine(Origin,X,[1.0, 0.0, 0.0],2.0,0.0,self.robotId,Link)
        pybullet.addUserDebugLine(Origin,Y,[0.0, 1.0, 0.0],2.0,0.0,self.robotId,Link)
        pybullet.addUserDebugLine(Origin,Z,[0.0, 0.0, 1.0],2.0,0.0,self.robotId,Link)

    def getWorldPosition(self):
        endEffectorWorldPosition=pybullet.getLinkState(self.robotId, 7)
        return list(endEffectorWorldPosition[0])

