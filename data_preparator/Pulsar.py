from datetime import datetime

import sys


class Pulsar(object):
    """
    name - имя
    raj - звездное время
    decj - склонение в градусах
    period - период
    dm - ???
    count - число ярких событий в год
    msk_begin - дата и время начала работы преобразования Фурье
    """
    name = ""
    raj = ""
    degree = 0.0
    period = 0.0
    dm = 0.0
    count = 0
    msk_begin = datetime.now()
    range = ""

    rays_gradient_north = {'0': 42.13, '1': 41.72, '2': 41.31, '3': 40.89, '4': 40.47, '5': 40.06,
                           '6': 39.64, '7': 39.23, '8': 38.79, '9': 38.38, '10': 37.95, '11': 37.54,
                           '12': 37.11, '13': 37.69, '14': 36.26, '15': 35.85, '16': 35.40, '17': 34.97,
                           '18': 34.54, '19': 34.12, '20': 33.69, '21': 33.25, '22': 32.82, '23': 32.38,
                           '24': 31.94, '25': 31.5, '26': 31.06, '27': 30.61, '28': 30.17, '29': 29.73,
                           '30': 29.29, '31': 28.84, '32': 28.37, '33': 27.92, '34': 27.47, '35': 27.01,
                           '36': 26.56, '37': 26.1, '38': 25.64, '39': 25.18, '40': 24.7, '41': 24.23,
                           '42': 23.76, '43': 23.29, '44': 22.81, '45': 22.34, '46': 21.86, '47': 21.38}

    rays_gradient_south = {'0': 20.8, '1': 20.4, '2': 19.9, '3': 19.4, '4': 18.9, '5': 18.4,
                           '6': 17.9, '7': 17.4, '8': 16.9, '9': 16.4, '10': 15.8, '11': 15.3,
                           '12': 14.8, '13': 14.3, '14': 13.7, '15': 13.2, '16': 12.6, '17': 12.1,
                           '18': 11.5, '19': 11.0, '20': 10.4, '21': 9.8, '22': 9.3, '23': 8.7,
                           '24': 8.1, '25': 7.5, '26': 6.9, '27': 6.3, '28': 5.7, '29': 5.0,
                           '30': 4.4, '31': 3.8, '32': 3.1, '33': 2.5, '34': 1.8, '35': 1.1,
                           '36': 0.4, '37': -0.3, '38': -1.0, '39': -1.7, '40': -2.5, '41': -3.2,
                           '42': -4.0, '43': -4.8, '44': -5.6, '45': -6.5, '46': -7.3, '47': -8.2}

    def __init__(self, line):
        parts = line.split()
        self.name = parts[0]
        self.raj = parts[1]
        self.degree = self.get_decimal_degree(parts[2])
        self.period = float(parts[3])
        self.dm = float(parts[4])
        self.count = int(parts[6][1:])
        self.msk_begin = datetime.strptime(parts[7], '%Y-%m-%dT%H:%M:%S')

    def get_decimal_degree(self, decj):
        decj = decj[1:]
        parts = decj.split(':')
        hour = int(parts[0])
        minute = int(parts[1])
        seconds = 0.0
        if len(parts) > 2:
            seconds = float(parts[2])
        as_decimal = (minute * 60 + seconds) / 3600
        return hour + as_decimal

    def __str__(self):
        return "Name: {name}, raj: {raj}, degree: {degree}, period: {period}, dm: {dm}, count: {count}".format(
            name=self.name, raj=self.raj, degree=self.degree, period=self.period, dm=self.dm, count=self.count)

    def ray(self):
        smallest = sys.float_info.max
        ray = -1
        for ray_number, degree in self.rays_gradient_north.items():
            modul = abs(self.degree - degree)
            if modul < smallest:
                smallest = abs(self.degree - degree)
                ray = int(ray_number) + 1
        if ray > -1 and smallest < 1:
            self.range = "N1"
            return ray

        smallest = sys.float_info.max
        for ray_number, degree in self.rays_gradient_south.items():
            modul = abs(self.degree - degree)
            if modul < smallest:
                smallest = abs(self.degree - degree)
                ray = int(ray_number) + 1

        if ray > -1 and smallest < 1:
            self.range = "N2"
            return ray
        return None
