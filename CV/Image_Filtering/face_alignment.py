import numpy as np
from skimage import transform
import cv2
import dlib
import math


image = cv2.imread(r'./image/10005.jpeg')
cv2.imshow("visualize",image)
cv2.waitKey(0)
#image1=cv2.resize(image,(540,249))
#cv2.line(image1, (75,125), (125,125), (255,255,0), 1)
#cv2.line(image1, (100,160), (100,160), (255,255,0), 5)
#cv2.line(image1, (80,180), (125,180), (255,255,0), 1)
#cv2.imshow("resize",image1)
def get_landmarks(image):

    predictor_model = 'shape_predictor_68_face_landmarks.dat'
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor(predictor_model)
    img_gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
    rects = detector(img_gray, 0)

    for i in range(len(rects)):
        landmarks = np.matrix([[p.x, p.y] for p in predictor(image, rects[i]).parts()])
    #for i,p in enumerate(predictor(image, rects[i]).parts()):
        #cv2.circle(image, (int(p.x),int(p.y)), 2, (0, 0, 255), 1)
        #cv2.putText(image, str(i),(int(p.x),int(p.y)),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 1, cv2.LINE_AA)

    return landmarks

landmarks=get_landmarks(image)

eye_center_left_x=(landmarks[36, 0] + landmarks[39, 0])/2
eye_center_right_x=(landmarks[42, 0] + landmarks[47, 0])/2
eye_center_left_y=(landmarks[36, 1] + landmarks[39, 1])/2
eye_center_right_y=(landmarks[42, 1] + landmarks[47, 1])/2

X=[
    [eye_center_left_x,eye_center_left_y],
    [eye_center_right_x,eye_center_right_y],
    [landmarks[30,0],landmarks[30,1]],
    [landmarks[48,0],landmarks[48,1]],
    [landmarks[54,0],landmarks[54,1]]
]


Y = [
    [75,125],
    [125,125],
    [100,160],
    [80,180],
    [125,180]
]

def matrix_affine(X,Y):
    X, Y = np.array(X), np.array(Y)
    M0 = transform.SimilarityTransform()
    M0.estimate(X, Y)
    matrix = M0.params[0:2, :]
    return matrix

matrix=matrix_affine(X, Y)

result = cv2.warpAffine(image, matrix, (200, 250),borderValue=0.0)
cv2.imshow("after_warp_affine",result)
cv2.waitKey(0)
cv2.line(result, (75,125), (125,125), (0,255,255), 1)
cv2.line(result, (100,160), (100,160), (0,255,255), 4)
cv2.line(result, (80,180), (125,180), (0,255,255), 1)
cv2.imshow("result",result)
cv2.waitKey(0)