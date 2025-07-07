import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Leer datos del archivo 
def leer_datos(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        bloques = archivo.read().strip().split('\n\n')
    datos = []
    for bloque in bloques:
        particulas = [list(map(float, linea.strip().split(',')))
                      for linea in bloque.strip().split('\n')]
        datos.append(particulas)
    return np.array(datos)  # shape: (num_frames, N, 2)

# Parámetros
L = 4
dt = 0.002   # paso de tiempo real
archivo = 'C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (Potencial de Lennard-Jones)/Actividad6/SALIDA.txt'
datos = leer_datos(archivo)
num_frames = len(datos)
tiempos = np.arange(num_frames) * dt

# Crear figura y ejes
fig, ax = plt.subplots()
ax.set_xlim(0, L)
ax.set_ylim(0, L)
ax.set_aspect('equal')

# Crear los artistas marcándolos como animated=True
scat  = ax.scatter([], [], s=50, color='blue', animated=True)
title = ax.text(0.5, 1.02, '', transform=ax.transAxes,
                ha='center', animated=True)

# Función de actualización
def actualizar(frame):
    posiciones = datos[frame]
    scat.set_offsets(posiciones)
    title.set_text(f'Tiempo t = {tiempos[frame]:.2f}')
    return scat, title

# Crear animación con blit=True para rendimiento
anim = FuncAnimation(
    fig, actualizar,
    frames=num_frames,
    interval=0.1,  # ms entre frames
    blit=True
)

plt.show()

ruta_salida = 'C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (potencial de Lennard-Jones)/Actividad6/Ploteos/animación.mp4'
anim.save(ruta_salida, fps=300, extra_args=['-vcodec', 'libx264'])
