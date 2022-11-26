import cv2
import numpy as np
import random
import skimage
import math

def Gaussian_pyramid(image,level):#高斯金字塔
    temp = image.copy()
    pyramid_images = []
    for i in range(level):

        dst = cv2.pyrDown(temp)
        pyramid_images.append(dst)

        cv2.imshow("pyramid_down_"+str(i+1), dst)
        cv2.waitKey(0)
        temp = dst.copy()

    return pyramid_images[level-1]


def gaussian_noise(image, mean=0.1, sigma=0.001):    # 添加高斯噪声

    image = np.array(image / 255, dtype=float)
    noise = np.random.normal(mean, sigma ** 0.5, image.shape)
    out = image + noise
    if out.min() < 0:
        low_clip = -1.
    else:
        low_clip = 0.
    out = np.clip(out, low_clip, 1.0)
    out = np.uint8(out * 255)
    return out

def sp_noise(image, prob):    # 添加椒盐噪声

    output = np.zeros(image.shape, np.uint8)
    thres = 1 - prob
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            rdn = random.random()
            if rdn < prob:
                output[i][j] = 0
            elif rdn > thres:
                output[i][j] = 255
            else:
                output[i][j] = image[i][j]
    return output

def rmse(image0, image):
    row,col=image0.shape[:2]
    mse=(np.square(np.subtract(image0,image))).mean()
    rmse=math.sqrt(mse)
    print(rmse)
    return rmse


image = cv2.imread(r'image/2.jpeg')


img_sp = sp_noise(image, 0.05)#椒盐
img_gaussian = gaussian_noise(image, 0.1, 0.03)
rmse(image, image)
cv2.imshow('image', image)
Gaussian_pyramid(image,5)
cv2.waitKey(0)

cv2.imshow('img_sp', img_sp)
cv2.imshow('img_gaussian', img_gaussian)
cv2.waitKey(0)
rmse(image,img_sp)
rmse(image,img_gaussian)


img_gaussian_gaussianblur=cv2.GaussianBlur(img_gaussian, (5, 5), 1)#高斯平滑
img_gaussian_medianblur = cv2.medianBlur(img_gaussian,5)#中值滤波
rmse(image,img_gaussian_gaussianblur)
rmse(image,img_gaussian_medianblur)



img_sp_gaussianblur = cv2.GaussianBlur(img_sp,(5, 5), 1)
img_sp_medianblur=cv2.medianBlur(img_sp,5)
rmse(image,img_sp_gaussianblur)
rmse(image,img_sp_medianblur)

cv2.imshow('img_gaussian', img_gaussian)
cv2.imshow('img_gaussian_gaussianblur', img_gaussian_gaussianblur)
cv2.imshow('img_gaussian_medianblur', img_gaussian_medianblur)
cv2.waitKey(0)

cv2.imshow('img_sp', img_sp)
cv2.imshow('img_sp_gaussianblur', img_sp_gaussianblur)
cv2.imshow('img_sp_medianblur', img_sp_medianblur)



cv2.waitKey(0)

