#!/usr/bin/python

# a simple implementation of marked AR
# author: Noni
# date: Feb 12

# reference:
# https://rdmilligan.wordpress.com/2016/12/18/augmented-reality-using-opencv-opengl-and-blender-mark-ii/
# opencv sample code: houghlines.py

import cv2
import numpy as np
import sys
import math


class Detection:
    def __init__(self):
        self.cap = cv2.VideoCapture(0)
        self.frame = None
        self.copy = None

    def cameraView(self):
        while True:
            ret, self.frame = self.cap.read()
            self.copy = self.frame

            # call processing functions
            self.houghLines()
            # end of processing

            cv2.imshow("gray", self.copy)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        self.cap.release()
        cv2.destroyAllWindows()

    def houghLines(self):
        gray = cv2.cvtColor(self.copy, cv2.COLOR_BGR2GRAY)
        canny = cv2.Canny(gray, 100, 200)
        lines = cv2.HoughLinesP(canny, 1, math.pi/180.0, 40, np.array([]), 50, 10)
        if not lines == None:
            a, b, c = lines.shape
            for i in range(a):
                cv2.line(self.copy, (lines[i][0][0], lines[i][0][1]),
                                    (lines[i][0][2], lines[i][0][3]),
                                    (0, 0, 200), 2, cv2.LINE_AA)


if __name__ == "__main__":
    dect = Detection()
    dect.cameraView()
