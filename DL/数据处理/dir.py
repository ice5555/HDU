from keras.preprocessing.image import ImageDataGenerator#,array_to_img
#img_to_array,load_img
from keras.preprocessing.image import image_utils
datagen = ImageDataGenerator(
    rotation_range=40,      # 随机旋转角度
    width_shift_range=0.2,  # 随机水平平移
    height_shift_range=0.2, # 随机竖直平移
    rescale=1./255,         # 数值归一化
    shear_range=0.2,        # 随机裁剪
    zoom_range=0.2,         # 随机放大
    horizontal_flip=True,   # 水平翻转
    fill_mode="nearest"     # 填充方式
)

# 载入图片
img = image_utils.load_img("./dog.2.jpg")
# 将图片转化为array数据格式
x = image_utils.img_to_array(img)
# (280, 300, 3) = (H,W,channels)
print(x.shape)
# 给图片增加一个维度 加这个维度主要是因为训练的时候需要一个四维的图片
x = x.reshape((1,)+x.shape)
# (1, 280, 300, 3) = （batch_size,H,W,channels）
print(x.shape)

i = 0
# 生成21张图片
# flow随机生成图片 save_prefix为新生成名字的前缀
for batch in datagen.flow(x,batch_size=1,save_to_dir='temp',save_prefix="cat",save_format="jpeg"):
    # 执行20次
    i += 1
    if i>20:
        break
