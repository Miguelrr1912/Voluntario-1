import numpy as np
import matplotlib.pyplot as plt

# Carga de datos (una fluctuación por línea)
flucs = np.loadtxt('C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (Potencial de Lennard-Jones)/Actividad7/FluctuaciÃ³n.txt')

# Crear la figura
plt.figure()
plt.plot(flucs, marker='o', linestyle='-', linewidth=0.1, color='blue', markersize=3)

# Etiquetas de ejes y título
plt.xlabel('Índice de muestra')
plt.ylabel('Fluctuación $(r_{1}(t)-r_{2}(t))^2$')
plt.title('Evolución de fluctuaciones')

# Opcional: grilla para facilitar la lectura
plt.grid(True)

# Ajuste de márgenes y guardado
plt.tight_layout()
plt.savefig('fluctuaciones.png')

# Mostrar en pantalla
plt.show()
