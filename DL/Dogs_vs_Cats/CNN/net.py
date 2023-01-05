from dir import train_dir 
from dir import validation_dir
from keras import layers
from keras import models
from tensorflow.keras import optimizers
from keras.preprocessing.image import ImageDataGenerator
from keras.preprocessing import image
import matplotlib.pyplot as plt

model=models.Sequential()

# block 1
model.add(layers.Conv2D(32,(3,3),activation="relu",input_shape=(150,150,3)))
model.add(layers.MaxPool2D((2,2)))

# block 2
model.add(layers.Conv2D(64,(3,3),activation="relu"))
model.add(layers.MaxPool2D((2,2)))

# block 3
model.add(layers.Conv2D(128,(3,3),activation="relu"))
model.add(layers.MaxPool2D((2,2)))

# block 4
model.add(layers.Conv2D(128,(3,3),activation="relu"))
model.add(layers.MaxPool2D((2,2)))

# output
model.add(layers.Flatten())  #展开为向量
#--------------------------------------------------------------
# 添加dropout层
model.add(layers.Dropout(0.5))
#--------------------------------------------------------------
model.add(layers.Dense(512,activation="relu"))
model.add(layers.Dense(1,activation="sigmoid"))     

model.summary()  # 打印网络结构，可以看到网络结构表



# loss的选择从上图可以看出二分类问题选择binary_crossentropy
model.compile(loss='binary_crossentropy',
              optimizer=optimizers.RMSprop(lr=1e-4),
              metrics=['accuracy'])

def generator():
    i=0
    while True:
        i+=1
        yield i

for item in generator():
    print(item)
    if item>4:
        break



train_datagen=ImageDataGenerator(rescale=1./255,
                                 rotation_range=40,        #随机旋转的角度范围(0~180°)     range代表范围
                                width_shift_range=0.2,    # width_shift/height_shift:在图像水平或垂直方向上平移的范围(按总宽总高比例)
                                height_shift_range=0.2,   
                                shear_range=0.2,          # 随机错切变换的角度
                                zoom_range=0.2,           # 图像随机缩放的范围
                                horizontal_flip=True,     # 随机将一半图像水平翻转
                                )  # 将所有图像乘以1/255缩放，归一化

validation_datagen=ImageDataGenerator(rescale=1./255)

# 参数分别指目标目录、调整为多少尺寸、batch_size、标签类型(因为使用了binary_crossentropy损失，所以需要用二进制标签)
train_generator=train_datagen.flow_from_directory(train_dir,
                                                 target_size=(150,150),
                                                  batch_size=20,
                                                  class_mode="binary"
                                                 )
validation_generator=validation_datagen.flow_from_directory(validation_dir,
                                                 target_size=(150,150), 
                                                  batch_size=20,
                                                  class_mode="binary"
                                                 )

# 查看train_generator产生的数据和标签的性质
for data_batch,labels_batch in train_generator:
    print("data batch shape:",data_batch.shape)
    print("lables batch shape:",labels_batch.shape)
    break

history=model.fit_generator(train_generator,
                           steps_per_epoch=100,  
                           epochs=100,
                           validation_data=validation_generator,
                           validation_steps=50)


model.save("cats_and_dogs_small_V3.h5")  # 保存到脚本文件所在目录

import matplotlib.pyplot as plt
acc=history.history['accuracy']
print("训练最佳精度：",max(acc))
val_acc=history.history['val_accuracy']
print("验证最佳精度：",max(val_acc))

loss=history.history['loss']
val_loss=history.history['val_loss']

epochs=range(1,len(acc)+1)

plt.plot(epochs,acc,'bo',label='Training acc')
plt.plot(epochs,val_acc,'b',label='Validation acc')
plt.title('Training and validation accuracy')
plt.legend()  #创建图例

plt.figure()

plt.plot(epochs,loss,'bo',label='Training loss')
plt.plot(epochs,val_loss,'b',label='Validation loss')
plt.title('Training and validation loss')
plt.legend()  #创建图例

plt.show()
