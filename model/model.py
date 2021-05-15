import math
from keras.models import Sequential
from keras.layers import Dense, Conv1D, MaxPooling1D, Flatten, Activation, GlobalMaxPooling1D
import numpy as np


def load_file(file_path):
    with open(file_path, "r") as infile:
        line = infile.read().split("#")

    content = []
    for l in line:
        kek = []
        for n in l.split(";"):
            isignal = float(n)
            if math.isnan(isignal) or math.isinf(isignal):
                kek.append(0)
            else:
                kek.append(isignal)
        content.append(kek)

    return content


print("reading...")
x_pulsars_validate = load_file("/home/sorrow/Рабочий стол/total_validate.txt")
x_dump_validate = load_file("/home/sorrow/Рабочий стол/dump_validate.txt")
x_pulsars = load_file("/home/sorrow/learndata/total.txt")
x_dump = load_file("/home/sorrow/dumpdata/total.txt")

data_length = min(len(x_pulsars), len(x_dump))
x_pulsars = x_pulsars[:data_length]
x_dump = x_dump[:data_length]

X, Y = [], []
X_validate, Y_validate = [], []
for pulsar_signals in x_pulsars:
    to_append = pulsar_signals[1:]
    X.append(np.array(to_append).reshape((len(to_append), 1)))
    Y.append(np.array([0]))

for dump_signals in x_dump:
    to_append = dump_signals[1:]
    X.append(np.array(to_append).reshape((len(to_append), 1)))
    Y.append(np.array([1]))

for validate_pulsars_signals in x_pulsars_validate:
    to_append = validate_pulsars_signals[1:]
    X_validate.append(np.array(to_append).reshape((len(to_append), 1)))
    Y_validate.append(np.array([0]))

for validate_dump_signals in x_dump_validate:
    to_append = validate_dump_signals[1:]
    X_validate.append(np.array(to_append).reshape((len(to_append), 1)))
    Y_validate.append(np.array([1]))

X = np.array(X)
Y = np.array(Y)
X_validate = np.array(X_validate)
Y_validate = np.array(Y_validate)

model = Sequential()
model.add(
    Conv1D(kernel_size=9, padding='same', strides=2, filters=32, activation='relu'))
model.add(Conv1D(kernel_size=7, padding='same', strides=2, filters=64, activation='relu'))
model.add(MaxPooling1D())
model.add(Conv1D(kernel_size=5, padding='same', strides=2, filters=64, activation='relu'))
model.add(Conv1D(kernel_size=5, padding='same', strides=2, filters=128, activation='relu'))
model.add(GlobalMaxPooling1D())
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dense(1, activation='sigmoid'))

print("compiling...")
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
print("fitting...")

model.fit(X, Y, epochs=100, batch_size=64, verbose=2)

_, accuracy = model.evaluate(X_validate, Y_validate)
print('Accuracy: %.2f' % (accuracy * 100))
model.save("prao_model")
