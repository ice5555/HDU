# 拉普拉斯算子
import cv2
import numpy as np


img = cv2.imread(r'image/1.jpg')
img_canny = cv2.Canny(img,64,150)
cv2.imshow('img',img)
cv2.imshow('img_canny',img_canny)
cv2.waitKey(0)
