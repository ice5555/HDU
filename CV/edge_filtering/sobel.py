# 索贝尔算子
import cv2
import numpy as np 
import scipy
import math
from matplotlib.pyplot import imshow
import matplotlib.pyplot as plt 

import seaborn as sns


img = cv2.imread(r'./image/1.jpg')
cv2.imshow('img',img)
img = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)  
dx = cv2.Sobel(img, cv2.CV_64F, dx=1, dy=0, ksize=3)
sobelx=cv2.convertScaleAbs(dx)
dy = cv2.Sobel(img, cv2.CV_64F, dx=0, dy=1, ksize=3)
sobely=cv2.convertScaleAbs(dy)
result=cv2.addWeighted(sobelx,0.5,sobely,0.5,0)

cv2.imshow('img_grey', img)
#cv2.imshow('imgx', sobelx)
#cv2.imshow('imgy', sobely)
cv2.imshow('img_sobel', result)
cv2.waitKey(0)

sns.set()
plt.subplot(2,2,1)
sns.heatmap(sobelx,cmap='GnBu',xticklabels=False, yticklabels=False)
plt.title('sobelx') 
plt.subplot(2,2,2)
sns.heatmap(sobely,cmap='GnBu',xticklabels=False, yticklabels=False)
plt.title('sobely') 
plt.subplot(2,2,3)
sns.heatmap(result,cmap='GnBu',xticklabels=False, yticklabels=False)
plt.title('sobel') 
plt.subplot(2,2,4)
sns.heatmap(np.arctan2(dy,dx),cmap='GnBu',xticklabels=False, yticklabels=False)
plt.title('arctan') 
plt.show()
