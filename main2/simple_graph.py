import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import json

ser = serial.Serial('COM12', 9600)  # Substitua 'COM1' pela porta serial que você está usando

# Carregar a imagem
logo = plt.imread('./ifes.png')  # Substitua pelo caminho da sua imagem


fig, axis = plt.subplots(1, 2) 

xs = []
ysTemp = []
ysCondu = []

# Initialize communication with TMP102
#tmp102.init()

# This function is called periodically from FuncAnimation
def animate(i, xs, ys_temp, ys_condu):

    # Read temperature (Celsius) from TMP102
    data = ser.readline().decode().strip()
    doc = json.loads(data)
    temp_c = round(doc['temp'], 2)
    condu = round(doc['condu'], 2)

    # Add x and y to lists
    xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
    ys_temp.append(temp_c)
    ys_condu.append(condu)

    # Limit x and y lists to 20 items
    xs = xs[-10:]
    ys_temp = ys_temp[-10:]
    ys_condu = ys_condu[-10:]

    # Draw x and y lists
    axis[0].clear()
    axis[0].plot(xs, ys_temp)
    axis[0].set_title("Temperatura C")

    axis[1].clear()
    axis[1].plot(xs, ys_condu)
    axis[1].set_title("Condutimetro ppm")


    # Format x-axis labels for both subplots
    for ax in axis:
        for label in ax.get_xticklabels():
            label.set_rotation(45)
            label.set_ha('right')

    # Format plot
    #plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)



# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ysTemp, ysCondu), interval=1000)


# Adicionar a imagem no canto superior esquerdo
newax = fig.add_axes([0.03, 0.03, 0.1, 0.1], anchor='NW', zorder=-1)
newax.imshow(logo)
newax.axis('off')

plt.show()
