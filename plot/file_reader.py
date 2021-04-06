import array
import logging
import os
import pandas

logger = logging.getLogger()


def load(filename):
    # get header length
    with open(filename, "rb") as f:
        data = f.read(1024)
        numpar = int(data.split(b'\n')[0].split(b'\t')[1])
        logger.debug('numpar = %d', numpar)

        header_length = sum([len(data.split(b'\n')[i]) + 1 for i in range(numpar)])
        logger.debug('header_length = %d', header_length)

    signals = array.array('f')

    # get buffer of signals size
    buffer_size = os.path.getsize(filename) - header_length

    logger.debug('File size w/o header is {} B'.format(buffer_size))

    # read header and signals
    with open(filename, 'rb') as fp:
        header_raw = fp.read(header_length)
        _header = dict(map(lambda x: (x.split(b'\t')[0], x.split(b'\t')[1]), filter(len, header_raw.split(b'\n'))))
        header = {x.decode("utf-8"): _header[x].decode("utf-8").split('\r')[0] for x in _header}
        logging.debug(header)
        fp.seek(header_length)
        signals.fromfile(fp, buffer_size // 4)

    logger.debug(signals)
    # create dataframe for plot
    dataaslists = [[i, signals[i]] for i in range(0, len(signals))]
    df = pandas.DataFrame(dataaslists, columns=['channel', 'modulus'])

    return df, header
