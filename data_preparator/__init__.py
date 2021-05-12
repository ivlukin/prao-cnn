# глобальные входные данные
# дата начала наблюдений
# длина наблюдений
# выходная папка
import datetime
import os
import statistics

import array

from Pulsar import Pulsar
import json
from pathlib import Path
from shutil import copyfile
from os import listdir
from os.path import isfile, join

content = []
with open("pulsars.txt") as f:
    content = f.readlines()


def prepare_config(pulsar):
    config_data = {}
    duration = 204.8
    start_date_shifted = pulsar.msk_begin - datetime.timedelta(seconds=duration / 2)
    config_data['startDate'] = start_date_shifted.strftime('%Y-%m-%d %H:%M:%S')
    config_data['endDate'] = start_date_shifted.strftime('%Y-%m-%d %H:%M:%S')
    config_data['observationLength'] = 149
    config_data['range'] = pulsar.range
    config_data['outputPath'] = '/home/sorrow/prao-output-main/{name}/'.format(name=pulsar.name)
    config_data['mode'] = '.pnt'
    config_data["fileListPath"] = "/home/sorrow/CLionProjects/prao-cnn/01-15-list-files.txt"
    config_data["calibrationListPath"] = "/home/sorrow/CLionProjects/prao-cnn/01-15-calb.txt"
    config_data["durationStarSeconds"] = duration
    config_data["summationEnabled"] = False
    config_data["writeRawData"] = False
    config_data["step"] = 60
    config_data["deviceNumber"] = 1
    return config_data


def extract_specters(dir_path, ray):
    Path(dir_path + "actual").mkdir(parents=True, exist_ok=True)
    raylist = [ray]
    if ray + 1 <= 48:
        raylist.append(ray + 1)
    if ray - 1 >= 1:
        raylist.append(ray - 1)
    for _ray in raylist:
        pathlist = Path(dir_path).glob('**/{ray}.fou'.format(ray=_ray))
        for path in pathlist:
            dest = dir_path + "actual/" + path.parts[-2] + "_" + path.parts[-1]
            copyfile(str(path), dest)


def read_fou_file(file_path):
    # get header length
    with open(file_path, "rb") as f:
        data = f.read(1024)
        numpar = int(data.split(b'\n')[0].split(b'\t')[1])
        header_length = sum([len(data.split(b'\n')[i]) + 1 for i in range(numpar)])

    signals = array.array('f')
    buffer_size = os.path.getsize(file_path) - header_length

    with open(file_path, 'rb') as fp:
        fp.seek(header_length)
        signals.fromfile(fp, buffer_size // 4)

    return signals


def has_pulsar(signals):
    isignals = signals[1:]
    count = 0
    index = 0
    for i in range(len(isignals) // 50):
        batch = isignals[index:index + 50]
        if (max(batch) / statistics.median(batch)) > 5:
            count += 1
        index += 50

    return count > 5


def analyze_specters(dir_path):
    files = [f for f in listdir(dir_path) if isfile(join(dir_path, f)) and ".fou" in f]
    for file_path in files:
        amplitudes = read_fou_file(join(dir_path, file_path))
        if not has_pulsar(amplitudes):
            print("there is no pulsar on {file}".format(file=file_path))
            os.remove(file_path)


def copy_all_fou_files(src_dir, dest_dir):
    pathlist = Path(src_dir).glob('**/*.fou')

    for path in pathlist:
        dest = join(dest_dir, path.parts[-1])
        copyfile(str(path), dest)


def merge_all_specters(dir_path):
    result = []
    files = [f for f in listdir(dir_path) if isfile(join(dir_path, f)) and ".fou" in f]
    for file_path in files:
        amplitudes = read_fou_file(join(dir_path, file_path))
        result.append(";".join([str(x) for x in amplitudes]))

    str_result = "#".join(result)
    with open(dir_path + "/total.txt", 'w') as out:
        out.write(str_result)

for line in content:
    if "NAME" in line:
        continue
    pulsar = Pulsar(line)

    if pulsar.ray() is not None and pulsar.range == "N1":
        print("processing", pulsar.name)
        config = prepare_config(pulsar)
        config_path = 'configs/{name}_config.json'.format(name=pulsar.name)
        with open(config_path, 'w') as outfile:
            json.dump(config, outfile)
        command = "/home/sorrow/CLionProjects/prao-cnn/cmake-build-release/prao_cnn -config {config}".format(
            config=config_path)
        print(command)
        os.system(command)
        extract_specters(config['outputPath'], pulsar.ray())
        analyze_specters(join(config['outputPath'], "actual"))
        copy_all_fou_files(join(config['outputPath'], "actual"), "/home/sorrow/learndata")
merge_all_specters("/home/sorrow/learndata")
