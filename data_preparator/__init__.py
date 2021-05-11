# глобальные входные данные
# дата начала наблюдений
# длина наблюдений
# выходная папка
import datetime

from Pulsar import Pulsar
import json
from pathlib import Path
from shutil import copyfile

content = []
with open("pulsars.txt") as f:
    content = f.readlines()


def prepare_config(pulsar):
    config_data = {}
    duration = 204.8
    start_date_shifted = pulsar.msk_begin - datetime.timedelta(seconds=duration / 2)
    config_data['startDate'] = start_date_shifted.strftime('%Y-%m-%d %H:%M:%S')
    config_data['endDate'] = start_date_shifted.strftime('%Y-%m-%d %H:%M:%S')
    config_data['observationLength'] = 90
    config_data['range'] = pulsar.range
    config_data['outputPath'] = '/home/sorrow/prao-output-main/{name}/'.format(name=pulsar.name)
    config_data['mode'] = 'pnt'
    config_data["fileListPath"] = "/home/sorrow/CLionProjects/prao-cnn/01-15-list-files.txt"
    config_data["calibrationListPath"] = "/home/sorrow/CLionProjects/prao-cnn/01-15-calb.txt"
    config_data["durationStarSeconds"] = duration
    config_data["summationEnabled"] = False
    config_data["writeRawData"] = False
    return config_data


def extract_specters(dir_path, ray):
    Path(dir_path + "actual").mkdir(parents=True, exist_ok=True)
    pathlist = Path(dir_path).glob('**/{ray}.fou'.format(ray=ray))
    for path in pathlist:
        dest = dir_path + "actual/" + path.parts[-2] + "_" + path.parts[-1]
        copyfile(str(path), dest)



for line in content:
    if "NAME" in line:
        continue
    pulsar = Pulsar(line)
    if pulsar.ray() is not None:
        config = prepare_config(pulsar)
        config_path = 'configs/{name}_config.json'.format(name=pulsar.name)
        with open(config_path, 'w') as outfile:
            json.dump(config, outfile)
        #TODO exec prao-cnn
        extract_specters(config['outputPath'], pulsar.ray())


