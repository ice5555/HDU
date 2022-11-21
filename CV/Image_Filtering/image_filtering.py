import cv2
import random
import numpy as np
import math


image = cv2.imread(r"./image/1.jpg")
cv2.imshow("visualize", image)
cv2.waitKey(0)
#print(image)


def Gaussian_pyramid(image,level):#高斯金字塔
    temp = image.copy()
    pyramid_images = []
    for i in range(level):

        dst = cv2.pyrDown(temp)
        pyramid_images.append(dst)

        cv2.imshow("pyramid_down_"+str(i), dst)
        cv2.waitKey(0)
        temp = dst.copy()

    return pyramid_images[level-1]
Gaussian_pyramid(image,5)

def gaussian_noise(image, mean=0.1, sigma=0.1):#添加高斯噪声

    image = np.array(image / 255, dtype=np.float32)  
    #print(image)
    noise = np.random.normal(mean, sigma, image.shape).astype(dtype=np.float32)
    gaussian_image = image + noise  
    gaussian_image = np.clip(gaussian_image, 0, 1)
    gaussian_image = np.uint8(gaussian_image * 255)
    cv2.imshow("gaussian_noise:"+str(mean)+" "+str(sigma),gaussian_image)
    cv2.waitKey(0)
    return gaussian_image

def salt_pepper_noise(image, prob=0.01):#添加椒盐噪声

    
    image = np.array(image / 255, dtype=np.float32)
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            if random.random() < prob:
                image[i][j] = 0 if random.random() < 0.5 else 255
            else:
                image[i][j] = image[i][j]
    cv2.imshow("salt_pepper_noise:"+str(prob),image)
    cv2.waitKey(0)
    return image

def rmse(image0, image):
    row,col=image0.shape[:2]
    mse=(np.square(np.subtract(image0,image))/(row*col)).mean()
    rmse=math.sqrt(mse)
    return rmse


gaussian_image=image
gaussian_image=gaussian_noise(gaussian_image,mean=0.1, sigma=0.1)#高斯噪声

print("rmse of gaussian_noise:"+str(rmse(image,gaussian_image)))

salt_pepper_image=image
salt_pepper_image=salt_pepper_noise(salt_pepper_image,prob=0.01)#椒盐噪声
print("rmse of salt_pepper_noise:"+str(rmse(image,salt_pepper_image)))

#-----------------------------------------------------------------

Gaussianblur_image = cv2.GaussianBlur(gaussian_image, (9, 9), 1)#高斯平滑
cv2.imshow("Gaussianblur_gaussian_image",Gaussianblur_image)
cv2.waitKey(0)
print("rmse of Gaussianblur_gaussian_image:"+str(rmse(image,Gaussianblur_image)))

Medianblur_image=cv2.medianBlur(gaussian_image,3)#中值滤波
cv2.imshow("Medianblur_gaussian_image",Medianblur_image)
cv2.waitKey(0)
print("rmse of Medianblur_gaussian_image:"+str(rmse(image,Medianblur_image)))


#-------------------------------------------------------

Gaussianblur_image = cv2.GaussianBlur(salt_pepper_image, (9, 9), 1)#高斯平滑
cv2.imshow("Gaussianblur_salt_pepper_image",Gaussianblur_image)
cv2.waitKey(0)
print("rmse of GaussianBlur:"+str(rmse(image,Gaussianblur_image)))

Medianblur_image=cv2.medianBlur(salt_pepper_image,3)#中值滤波
cv2.imshow("Medianblur_salt_pepper_image",Medianblur_image)
cv2.waitKey(0)
print("rmse of Medianblur_salt_pepper_image:"+str(rmse(image,Medianblur_image)))















cv2.destroyAllWindows()