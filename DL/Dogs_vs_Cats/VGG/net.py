import keras
import tensorflow as tf
from keras import layers
import numpy as np
import os
import shutil
import matplotlib.pyplot as plt
from tensorflow.keras.applications import VGG16

from dir import train_generator

covn_base = VGG16(weights='imagenet', include_top=False, input_shape=(200,200,3))
covn_base.summary()
batch_size = 20
def extract_features(data_generator, sample_count):
    i = 0
    features = np.zeros(shape=(sample_count, 6, 6, 512))
    labels = np.zeros(shape=(sample_count))
    for inputs_batch, labels_batch in data_generator:
        features_batch = covn_base.predict(inputs_batch)
        features[i * batch_size : (i+1)*batch_size] = features_batch
        labels[i*batch_size:(i+1)*batch_size] = labels_batch
        i+=1
        if i * batch_size >= sample_count:
            break
    return features, labels

train_featrues, train_labels = extract_features(train_generator, 2000)
test_featrues, test_labels = extract_features(test_generator, 1000)

model = keras.Sequential()
model.add(layers.GlobalAveragePooling2D(input_shape=(6, 6, 512)))
model.add(layers.Dense(512, activation='relu'))
model.add(layers.Dropout(0.5))
model.add(layers.Dense(1, activation='sigmoid'))
model.compile(optimizer=tf.keras.optimizers.Adam(lr=0.0005/10), loss='binary_crossentropy', metrics=['acc'])
history = model.fit(train_featrues,train_labels, epochs=50, 
batch_size=50, validation_data=(test_featrues, test_labels))
plt.plot(range(50), history.history.get('val_acc'), c='r', label='val_acc')
plt.plot(range(50), history.history.get('acc'), c='b', label='acc')
plt.legend
model.save('cat_dog_model.h5')

