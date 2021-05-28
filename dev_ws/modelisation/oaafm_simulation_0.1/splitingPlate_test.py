# -*- coding: utf-8 -*-

"""
Created on 24/01/2021
classe Plate
@author: Georges
"""


import plate
import endEffector

def main():
    currentPlate = plate.Plate([0.125, 0.125, 0.050], 0.175, 0.035)
    currentEndEffector = endEffector.EndEffector(0.050, 0.030)
    currentPlate.split2(currentEndEffector.getSize(), 0.5)
    #currentPlate.split(currentEndEffector.getSize(), 0.6)


main()