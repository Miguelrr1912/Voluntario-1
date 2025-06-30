import numpy as np
import matplotlib.pyplot as plt

# Carga de datos (una temperatura y una fluctuación por línea)
temps = np.loadtxt('C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (Potencial de Lennard-Jones)/Actividad7/Temperatura.txt')
flucs = np.loadtxt('C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (Potencial de Lennard-Jones)/Actividad7/FluctuaciÃ³n.txt')

# Dibujo rápido: Temperatura vs Fluctuación
plt.plot(temps, flucs, 'o-', markersize=3, linewidth=0.5)
plt.xlabel('Temperatura')
plt.ylabel(r'Fluctuación $(r_{1}(t)-r_{2}(t))^2$')
plt.title('Fluctuación vs Temperatura')
plt.grid(True)

# Guardar y mostrar
plt.tight_layout()
plt.savefig('C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (potencial de Lennard-Jones)/Actividad7/Ploteos/fluct_vs_temp.png', dpi=300)
plt.show()

