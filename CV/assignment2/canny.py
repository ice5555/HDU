# 拉普拉斯算子
import cv2
import numpy as np


img = cv2.imread(r'image/1.jpg')
img_canny = cv2.Canny(img,64,150)
cv2.imshow('img1',img)
cv2.imshow('img2',img_canny)
cv2.waitKey(0)
