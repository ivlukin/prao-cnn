from keras.models import Sequential
from keras.layers import Dense, Conv1D, MaxPooling1D, Flatten


def load_file(file_path):
    with open(file_path, "r") as infile:
        line = infile.read().split("#")

    content = []
    for l in line:
        content.append([float(n) for n in l.split(";")])

    return content


x_pulsars_validate = load_file("/home/sorrow/Рабочий стол/total_validate.txt")
x_dump_validate = load_file("/home/sorrow/Рабочий стол/dump_validate.txt")
x_pulsars = load_file("/home/sorrow/learndata/total.txt")
x_dump = load_file("/home/sorrow/dump.txt")

X, Y = [], []
for pulsar_signals in x_pulsars:
    X.append(pulsar_signals)
    Y.append(0)
for dump_signals in x_dump:
    X.append(dump_signals)
    Y.append(1)

model = Sequential()
model.add(Conv1D(kernel_size=9, padding=3, strides=2, filters=32, activation='relu'))
model.add(Conv1D(kernel_size=7, padding=3, strides=2, filters=64, activation='relu'))
model.add(MaxPooling1D())
model.add(Conv1D(kernel_size=5, padding=2, strides=2, filters=64, activation='relu'))
model.add(Conv1D(kernel_size=5, padding=2, strides=2, filters=128, activation='relu'))
model.add(MaxPooling1D())
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dense(2))

model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
model.fit(X, Y, epochs=150, batch_size=64)
