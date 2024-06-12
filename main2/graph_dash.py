# Importando as bibliotecas necessárias
import dash
from dash.dependencies import Input, Output
import dash_core_components as dcc
import dash_html_components as html
import plotly.graph_objs as go
import serial
import pandas as pd

serial_port = serial.Serial('COM10', 9600)  # Substitua 'COMx' pela porta serial correta


# Inicializando o objeto Dash
app = dash.Dash(__name__)

# Conectando-se à porta serial

# Layout do aplicativo
app.layout = html.Div(children=[
    html.H1(children='Aplicação Dash com Valores da Serial'),

    dcc.Graph(
        id='live-graph',
    ),
    
    dcc.Interval(
        id='interval-component',
        interval=1000,  # Atualização a cada 1 segundo
        n_intervals=0
    )
])

# Callback para atualizar o gráfico
@app.callback(Output('live-graph', 'figure'),
              [Input('interval-component', 'n_intervals')])
def update_graph(n):
    # Lendo dados da serial
    serial_data = serial_port.readline().decode().strip()
    doc = json.loads(serial_data)

    #x_value, y_value = map(float, serial_data.split(','))
    x_value = doc['temp']
    y_value = doc['condu']

    # Atualizando o gráfico
    trace = go.Scatter(x=[x_value], y=[y_value], mode='lines+markers')
    layout = go.Layout(title='Gráfico em Tempo Real', xaxis=dict(title='X'), yaxis=dict(title='Y'))
    return {'data': [trace], 'layout': layout}

# Executando o servidor
if __name__ == '__main__':
    app.run_server(debug=True)
