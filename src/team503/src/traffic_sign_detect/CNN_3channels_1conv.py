import pickle
from sklearn.utils import shuffle
import tensorflow as tf
import cv2

TRAIN_DATA_DIR = "data/raw/training/augmented"
# TRAIN_DATA_DIR = "data/raw/testing"
TEST_DATA_DIR = "data/raw/testing"
CNN_MODEL_DIR = "pickle/3cnn_1conv_10epoch_s28.ckpt"
PICKLE_IMGS_DIR = "pickle/train_imgs.pkl"
PICKLE_LABELS_DIR = "pickle/test_labels.pkl"
NUM_CLASSES = 11
IMG_SIZE = 28

def deepnn(x):
    with tf.name_scope('reshape'):
        x_image = x
        # x_image = tf.placeholder([-1, 28, 28, 3])

    # First convolutional layer - maps one grayscale image to 32 feature maps.
    with tf.name_scope('conv1'):
        W_conv1 = weight_variable([5, 5, 3, 32])
        b_conv1 = bias_variable([32])
        h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)

    # Pooling layer - downsamples by 2X.
    with tf.name_scope('pool1'):
        h_pool1 = max_pool_2x2(h_conv1)

    with tf.name_scope('fc1'):
        W_fc1 = weight_variable(
            [int(IMG_SIZE / 2) * int(IMG_SIZE / 2) * 32, 1024])
        b_fc1 = bias_variable([1024])

        h_pool1_flat = tf.reshape(
            h_pool1, [-1, int(IMG_SIZE / 2) * int(IMG_SIZE / 2) * 32])
        h_fc1 = tf.nn.relu(tf.matmul(h_pool1_flat, W_fc1) + b_fc1)

    # Dropout - controls the complexity of the model, prevents co-adaptation of
    # features.
    with tf.name_scope('dropout'):
        keep_prob = tf.placeholder(tf.float32)
        h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

    # Map the 1024 features to NUM_CLASSES classes, one for each digit
    with tf.name_scope('fc2'):
        W_fc2 = weight_variable([1024, NUM_CLASSES])
        b_fc2 = bias_variable([NUM_CLASSES])

        y_conv = tf.matmul(h_fc1_drop, W_fc2) + b_fc2
    return y_conv, keep_prob


def conv2d(x, W):
    """conv2d returns a 2d convolution layer with full stride."""
    return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')


def max_pool_2x2(x):
    """max_pool_2x2 downsamples a feature map by 2X."""
    return tf.nn.max_pool(x, ksize=[1, 2, 2, 1],
                          strides=[1, 2, 2, 1], padding='SAME')


def weight_variable(shape):
    """weight_variable generates a weight variable of a given shape."""
    initial = tf.truncated_normal(shape, stddev=0.1)
    return tf.Variable(initial)


def bias_variable(shape):
    """bias_variable generates a bias variable of a given shape."""
    initial = tf.constant(0.1, shape=shape)
    return tf.Variable(initial)



