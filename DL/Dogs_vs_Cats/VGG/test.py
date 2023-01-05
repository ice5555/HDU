import tensorflow as tf
import numpy as np
from keras.models import load_model
import cv2
from dir import train_dir 
from dir import test_dir 
from dir import train_generator
from dir import test_generator
def show(image):
    cv2.namedWindow('test', 0)
    cv2.imshow('test', image)
    # 0任意键终止窗口
    cv2.waitKey(0)
    cv2.destroyAllWindows()
covn_base = tf.keras.applications.VGG16(weights='imagenet', include_top=False, input_shape=(200, 200, 3))
cat_dog_model = load_model('./cat_dog_model.h5')
image = cv2.imread('cat1.jpeg')
resize_image = cv2.resize(image, (200, 200), interpolation=cv2.INTER_AREA)
input_data = np.expand_dims(resize_image, axis=0)
result = int(cat_dog_model.predict(covn_base.predict(input_data))[0][0])
if result == 1:
    print("狗")
if result == 0:
    print("猫")
show(resize_image)
