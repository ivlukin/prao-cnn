# coding=utf-8
import click
import dash
import dash_core_components as dcc
import dash_html_components as html
import datetime
import glob
import logging
import os
import plotly.graph_objs as go
import sys
from threading import Lock

from file_reader import load

logger = logging.getLogger()
handler = logging.StreamHandler(sys.stdout)
formatter = logging.Formatter(
    '%(asctime)s %(levelname)-8s %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.setLevel(logging.DEBUG)

# График аргумента комплексного значения часто называют в данном случае “фазовым спектром”, а график модуля —
# “амплитудным спектром”.
graphs = ['amplitude', 'phase', 'log10']

runtime = {}
runtime_lock = Lock()

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

rays_gradient_north = {'0': 42.13, '1': 41.72, '2': 41.31, '3': 40.89, '4': 40.47, '5': 40.06,
                       '6': 39.64, '7': 39.23, '8': 38.79, '9': 38.38, '10': 37.95, '11': 37.54,
                       '12': 37.11, '13': 37.69, '14': 36.26, '15': 35.85, '16': 35.40, '17': 34.97,
                       '18': 34.54, '19': 34.12, '20': 33.69, '21': 33.25, '22': 32.82, '23': 32.38,
                       '24': 31.94, '25': 31.5, '26': 31.06, '27': 30.61, '28': 30.17, '29': 29.73,
                       '30': 29.29, '31': 28.84, '32': 28.37, '33': 27.92, '34': 27.47, '35': 27.01,
                       '36': 26.56, '37': 26.1, '38': 25.64, '39': 25.18, '40': 24.7, '41': 24.23,
                       '42': 23.76, '43': 23.29, '44': 22.81, '45': 22.34, '46': 21.86, '47': 21.38}

def make_centered_p(text):
    return html.P(text, style={'text-align': 'center', 'font-style': 'bold'})


@click.command('run')
@click.argument('dir_path', type=str)
def run(dir_path):
    runtime['dir_path'] = os.path.expanduser(dir_path)

    if not os.path.exists(runtime['dir_path']):
        logger.error('Directory %s does not exist, exiting', runtime['dir_path'])
        return

    logger.debug(glob.glob(os.path.expanduser(runtime['dir_path'] + os.sep + "*.fou")))
    runtime['files'] = list(
        map(os.path.basename, glob.glob(os.path.expanduser(runtime['dir_path'] + os.sep + "*.fou"))))
    runtime['files'] += list(map(os.path.basename, glob.glob(os.path.expanduser(runtime['dir_path'] + os.sep + "*.praw"))))
    #runtime['files'].sort(key=lambda fname: int(fname[:fname.index(".")]))
    logger.debug("need to sort:")
    logger.debug(runtime['files'])
    if len(runtime['files']) == 0:
        logger.error('List of files is empty, exiting')
        return

    runtime['file'] = None
    with runtime_lock:
        update_file(0)

    app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

    app.layout = html.Div([
        html.H1('PRAO Data Viewer'),
        html.I('That’s one small step for a man, but one giant leap for mankind.'),
        html.Br(),
        html.Br(),

        dcc.Dropdown(
            options=[{'label': runtime['files'][i], 'value': str(i)} for i in range(len(runtime['files']))],
            value=str(runtime['file']),
            id='file-changer'
        ),
        html.Br(),
        html.H4(id='ray-header'),
        html.H4(id='time-header'),

        html.Br(),
        html.H2('signals'),
        dcc.Graph(id='fourier-scatter')

    ], style={'margin-left': '50px', 'margin-right': '50px'})

    @app.callback(
        dash.dependencies.Output('fourier-scatter', 'figure'),
        [dash.dependencies.Input('file-changer', 'value')]
    )
    def update_specter_plot(selected_file):
        logger.debug('enter update specter plot(%s)', selected_file)
        if int(selected_file) != runtime['file']:
            with runtime_lock:
                update_file(selected_file)
        df = runtime['df']
        traces = []
        traces.append(go.Scatter(
            x=df['channel'][1:],
            y=df['modulus'][1:],
            x0=0,
            y0=0,
            mode='lines+markers',
            marker={
                'size': 3
            },
            name='amplitude'
        ))
        logger.debug('traces' + str(traces))
        return {
            'data': traces,
            'layout': go.Layout(
                xaxis={'range': [0, 1500], 'autorange': True},
                yaxis={'range': [0, 2000], 'autorange': True},
                margin={'l': 40, 'b': 40, 't': 10, 'r': 10},
                legend={'x': 1, 'y': 1},
                hovermode='closest'
            )
        }

    @app.callback(
        dash.dependencies.Output('ray-header', 'children'),
        [dash.dependencies.Input('file-changer', 'value')]
    )
    def update_ray_header(selected_file):
        if int(selected_file) != runtime['file']:
            with runtime_lock:
                update_file(selected_file)
        header = runtime['header']
        return 'ray: ' + str(rays_gradient_north[str(int(header['ray_number']) - 1)])

    @app.callback(
        dash.dependencies.Output('time-header', 'children'),
        [dash.dependencies.Input('file-changer', 'value')]
    )
    def update_time_header(selected_file):
        if int(selected_file) != runtime['file']:
            with runtime_lock:
                update_file(selected_file)
        header = runtime['header']
        elapsed_time = int(float(header['msk_begin']))
        return 'msk_begin: ' + datetime.datetime.fromtimestamp(int(elapsed_time)).strftime('%Y-%m-%d %H:%M:%S')

    app.run_server(debug=False)


def update_file(selected_file):
    logging.debug('selected_file = %s, runtime_file = %s', selected_file, runtime['file'])
    if int(selected_file) == runtime['file']:
        return

    runtime['file'] = int(selected_file)
    logger.debug('Loading %s',
                 os.path.expanduser(os.path.join(runtime['dir_path'], runtime['files'][runtime['file']])))
    logger.debug('loading file...')
    df, header = load(os.path.expanduser(os.path.join(runtime['dir_path'], runtime['files'][runtime['file']])))
    runtime['df'] = df
    runtime['header'] = header
    logger.debug('done...')


if __name__ == '__main__':
    run()

rays_gradient_north = {'0': 42.13, '1': 41.72, '2': 41.31, '3': 40.89, '4': 40.47, '5': 40.06,
                       '6': 39.64, '7': 39.23, '8': 38.79, '9': 38.38, '10': 37.95, '11': 37.54,
                       '12': 37.11, '13': 37.69, '14': 36.26, '15': 35.85, '16': 35.40, '17': 34.97,
                       '18': 34.54, '19': 34.12, '20': 33.69, '21': 33.25, '22': 32.82, '23': 32.38,
                       '24': 31.94, '25': 31.5, '26': 31.06, '27': 30.61, '28': 30.17, '29': 29.73,
                       '30': 29.29, '31': 28.84, '32': 28.37, '33': 27.92, '34': 27.47, '35': 27.01,
                       '36': 26.56, '37': 26.1, '38': 25.64, '39': 25.18, '40': 24.7, '41': 24.23,
                       '42': 23.76, '43': 23.29, '44': 22.81, '45': 22.34, '46': 21.86, '47': 21.38}
