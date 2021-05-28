# -*- coding: utf-8 -*-
"""
Created on 24/01/2021
classe EndEffector
@author: Georges
"""

class EndEffector:

    def __init__(self, majorAxis, minorAxis):
        self.majorAxis=majorAxis
        self.minorAxis=minorAxis

    def getSize(self):
        return (self.majorAxis, self.minorAxis)

