# 拉普拉斯算子
import cv2
import numpy as np
from matplotlib.pyplot import imshow

img = cv2.imread(r'./image/1.jpg')
dst = cv2.Laplacian(img,-1,cv2.CV_64F,ksize=3)
laplacian = cv2.convertScaleAbs(dst)
#image_add=cv2.add(laplacian,img)
image_add0=laplacian+img
image_add=cv2.addWeighted(laplacian,0.5,img,0.5,0)
cv2.imshow('img',img)
cv2.imshow('img_laplacian',laplacian)
cv2.imshow('img_add',image_add)
cv2.imshow('img_add0',image_add0)
cv2.waitKey(0)


