import cv2
import dlib
import matplotlib.pyplot as plt
import numpy as np
import math
from matplotlib.pyplot import imshow 
from PIL import Image,ImageDraw



image = cv2.imread(r'./image/10001.jpg')

def get_landmarks(image):

    predictor_model = 'shape_predictor_68_face_landmarks.dat'
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor(predictor_model)
    img_gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
    rects = detector(img_gray, 0)

    for i in range(len(rects)):
        landmarks = np.matrix([[p.x, p.y] for p in predictor(image, rects[i]).parts()])
    for i,p in enumerate(predictor(image, rects[i]).parts()):
        cv2.circle(image, (int(p.x),int(p.y)), 2, (0, 0, 255), 1)
        #cv2.putText(image, str(i),(int(p.x),int(p.y)),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 1, cv2.LINE_AA)

    return landmarks

landmarks = get_landmarks(image)
cv2.imshow("visiualize", image)
cv2.waitKey(0)



def single_face_alignment(image, landmarks):
    eye_center = ((landmarks[36, 0] + landmarks[45, 0]) * 1. / 2,   (landmarks[36, 1] + landmarks[45, 1]) * 1. / 2)
    dx = (landmarks[45, 0] - landmarks[36, 0]) 
    dy = (landmarks[45, 1] - landmarks[36, 1])

    angle = math.atan2(dy, dx) * 180. / math.pi  

    RotateMatrix = cv2.getRotationMatrix2D(eye_center, angle, scale=1)  
    align_face = cv2.warpAffine(image, RotateMatrix, (image.shape[0], image.shape[1]))  
    return align_face

image=single_face_alignment(image, landmarks)
cv2.imshow("face_alignment", image)
cv2.waitKey(0)


def crop_face(image,landmarks): 
    eye_center=(landmarks[45, 1] + landmarks[36, 1])/2
    lip_center=(landmarks[54, 1] + landmarks[48, 1])/2
    mid_part = lip_center - eye_center
    top = eye_center - mid_part / (55/200)*(125/200)
    bottom = lip_center+ mid_part/(55/200)*(20/200)
    h=bottom-top

    eye_center_left=(landmarks[36, 0] + landmarks[39, 0])/2
    eye_center_right=(landmarks[42, 0] + landmarks[47, 0])/2
    
    mid_part = eye_center_right -  eye_center_left
    left=eye_center_left-mid_part/(50/250)*(75/250)
    right=eye_center_right+mid_part/(50/250)*(125/250)
    w=right-left

    pil_img = Image.fromarray(image)
    left, top, right, bottom = [int(i) for i in [left, top, right, bottom]]
    cropped_img = pil_img.crop((left, top, right, bottom))
    cropped_img = np.array(cropped_img)
    return cropped_img

image0=crop_face(image, landmarks)
image0=cv2.resize(image0,(250,200))


cv2.line(image0, (75,125), (125,125), (0,255,0), 1)
cv2.line(image0, (80,180), (125,180), (0,255,0), 1)
cv2.line(image0, (100,160), (100,160), (0,255,0), 5)
cv2.imshow("result_of_250*200", image0)
cv2.waitKey(0)


def crop_face_another(image,landmarks): 
    eye_center=(landmarks[45, 1] + landmarks[36, 1])/2
    lip_center=(landmarks[54, 1] + landmarks[48, 1])/2
    mid_part = lip_center - eye_center
    top = eye_center - mid_part / (55/250)*(125/250)
    bottom = lip_center+ mid_part/(55/250)*(70/250)
    h=bottom-top

    eye_center_left=(landmarks[36, 0] + landmarks[39, 0])/2
    eye_center_right=(landmarks[42, 0] + landmarks[47, 0])/2
    
    mid_part = eye_center_right -  eye_center_left
    left=eye_center_left-mid_part/(50/200)*(75/200)
    right=eye_center_right+mid_part/(50/200)*(75/200)
    w=right-left

    pil_img = Image.fromarray(image)
    left, top, right, bottom = [int(i) for i in [left, top, right, bottom]]
    cropped_img = pil_img.crop((left, top, right, bottom))
    cropped_img = np.array(cropped_img)
    return cropped_img

image1=crop_face_another(image, landmarks)
image1=cv2.resize(image1,(200,250))

cv2.line(image1, (75,125), (125,125), (0,255,0), 1)
cv2.line(image1, (100,160), (100,160), (0,255,0), 5)
cv2.line(image1, (80,180), (125,180), (0,255,0), 1)
cv2.imshow("result_of_200*250", image1)
cv2.waitKey(0)

cv2.destroyAllWindows()




















