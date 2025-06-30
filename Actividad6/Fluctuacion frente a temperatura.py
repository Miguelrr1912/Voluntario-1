import numpy as np
import matplotlib.pyplot as plt

# Parámetros
dt = 0.002  # ajusta si tu paso de tiempo es diferente

# Carga de datos
flucs = np.loadtxt('C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (Potencial de Lennard-Jones)/Actividad6/FluctuaciÃ³n.txt')

# Crear eje de tiempos reales
tiempos = np.arange(len(flucs)) * dt

# Crear la figura
plt.figure(figsize=(10, 6))
plt.plot(tiempos, flucs, linestyle='-', linewidth=0.8, color='blue')

# Etiquetas
plt.xlabel('Tiempo (t)')
plt.ylabel(r'$\langle (r(t) - r(0))^2 \rangle$')
plt.title('Fluctuaciones con respecto al tiempo')
plt.grid(True)

# Guardar y mostrar
plt.tight_layout()
plt.savefig('C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (Potencial de Lennard-Jones)/Actividad6/Ploteos/fluctuaciones_tiempo.png')
plt.show()

