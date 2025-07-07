import numpy as np
import matplotlib.pyplot as plt

# Lee los datos 
fluctuacion = np.loadtxt("C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (Potencial de Lennard-Jones)/Actividad7/FluctuaciÃ³n.txt")
temperatura = np.loadtxt("C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (Potencial de Lennard-Jones)/Actividad7/Temperatura.txt")

# === Graficar ===
plt.figure(figsize=(8,6))
plt.plot(temperatura, fluctuacion, 'o-', color='blue', label='Separación cuadrática media')
plt.xlabel('Temperatura promedio')
plt.ylabel('Separación cuadrática media')
plt.title('Separación cuadrática media vs Temperatura')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (Potencial de Lennard-Jones)/Actividad7/Separacion_cuadratica_media_vs_T.png", dpi=300)
plt.show()

