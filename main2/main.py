import serial
import json

# Configurar porta serial
ser = serial.Serial('COM10', 9600)  # Substitua 'COM1' pela porta serial que você está usando

while True:
    # Ler linha da porta serial
    data = ser.readline().decode().strip()
    
    try:
        # Tentar decodificar JSON
        doc = json.loads(data)
        print("JSON recebido:", doc)
        print("JSON recebido TEMPERATURA:", doc['temp'])
        print("JSON recebido CONDUTIVIMETRO:", doc['condu'])
        
        # Agora você pode acessar os dados do JSON normalmente
        # Exemplo: print(json_data['chave'])
        
    except json.JSONDecodeError:
        print("Erro ao decodificar JSON:", data)

# Fechar porta serial no final
ser.close()
