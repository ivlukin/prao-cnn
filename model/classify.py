
import os
from pathlib import Path

import numpy as np
from array import array
from shutil import copyfile

from tensorflow import keras

INPUT_FOLDER = '/home/sorrow/latest_data/'
OUTPUT_FOLDER = os.path.join(INPUT_FOLDER, "classified")
Path(OUTPUT_FOLDER).mkdir(parents=True, exist_ok=True)


def read_fou_file(file_path):
    # get header length
    with open(file_path, "rb") as f:
        data = f.read(1024)
        numpar = int(data.split(b'\n')[0].split(b'\t')[1])
        header_length = sum([len(data.split(b'\n')[i]) + 1 for i in range(numpar)])

    signals = array('f')
    buffer_size = os.path.getsize(file_path) - header_length

    with open(file_path, 'rb') as fp:
        fp.seek(header_length)
        signals.fromfile(fp, buffer_size // 4)

    return signals


prao_model = keras.models.load_model("prao_model")
count = 0
for path in Path(INPUT_FOLDER).glob("**/*.fou"):
    signals = np.array(read_fou_file(path)[1:])
    signals = signals.reshape((1, signals.shape[0], 1))
    prediction = prao_model.predict(signals)
    count += 1
    if prediction.min() < 0.5:
        print("found!")
        new_name = "{tm}_{ray}".format(tm=path.parts[-2], ray=path.parts[-1])
        new_name = os.path.join(OUTPUT_FOLDER, new_name)
        copyfile(path, new_name)
    if count > 200:
        break
