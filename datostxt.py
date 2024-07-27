import serial
import pandas as pd
from datetime import datetime

# Abrir la comunicación serial con el puerto USB
port = serial.Serial('/dev/tty.usbserial-0001', 9600)

try:
    # Crear un DataFrame vacío
    df = pd.DataFrame(columns=['Fecha', 'Datos'])
    i = 0  # Índice para añadir filas al DataFrame

    while True:  # Bucle infinito para leer datos continuamente
        # Leer datos del puerto serial
        data = port.readline().decode('utf-8').rstrip('\r\n')

        # Si hay datos, procesarlos y guardar el mensaje en el archivo TXT
        if data:
            # Obtener la fecha actual formateada
            fecha_actual = datetime.now().strftime('%Y-%m-%d')

            # Crear el mensaje deseado
            mensaje = f'Comando = "La temperatura es"; Respuesta = "{data} grados centigrados el {fecha_actual} ". '

            # Añadir el mensaje como una nueva fila en el DataFrame
            df.loc[i] = [fecha_actual, mensaje]
            i += 1

            # Guardar el mensaje en el archivo TXT
            with open('datoscasa2.txt', 'r', encoding='utf-8') as f:
                lineas = f.readlines()
                lineas = lineas[:-1]
                lineas.append(mensaje + '\n')
                lineas.append("'''")
            with open('datoscasa2.txt', 'w', encoding='utf-8') as f:
                f.write(''.join(lineas))

except KeyboardInterrupt:
    # Cerrar el puerto cuando se detiene el programa con Ctrl+C
    print("Programa interrumpido por el usuario.")
    port.close()
    
except serial.SerialException:
    # Manejo del error cuando no hay dispositivo conectado al puerto USB
    print("No hay ningún dispositivo conectado al puerto USB.")
    
except Exception as e:
    # Manejo de cualquier otro tipo de error
    print(f"Se ha producido un error: {e}")
    port.close()
