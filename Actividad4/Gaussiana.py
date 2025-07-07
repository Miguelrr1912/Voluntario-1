import numpy as np
import matplotlib.pyplot as plt

# Función para leer velocidades y temperatura
def leer_datos(nombre_archivo):
    with open(nombre_archivo, 'r') as f:
        lineas = [float(line.strip()) for line in f if line.strip() != '']
        T = lineas[-1]             # Última línea es la temperatura
        velocidades = np.array(lineas[:-1])  # Todas las anteriores son velocidades
    return velocidades, T

# Distribución tipo Maxwell (1D, masa y k_B = 1)
def distribucion_gaussiana(v, T):
    return (1 / T) * v * np.exp(-v**2 / (2 * T))

# Programa principal
def main():
    archivo = 'C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (potencial de Lennard-Jones)/Actividad4/Temperatura.txt'  # Cambia esto por el nombre de tu archivo
    velocidades, T = leer_datos(archivo)

    # Histograma de velocidades (normalizado)
    conteo, bins, _ = plt.hist(velocidades, bins=50, density=True, alpha=0.6, label='Histograma de velocidades')

    # Crear curva teórica
    v_vals = np.linspace(0, max(velocidades) * 1.1, 500)
    f_vals = distribucion_gaussiana(v_vals, T)

    # Superponer distribución teórica
    plt.plot(v_vals, f_vals, color='red', label=f'Distribución teórica (T = {T:.2f})')

    plt.xlabel('Velocidad')
    plt.ylabel('Densidad de probabilidad')
    plt.title('Velocidades vs distribución teórica')
    plt.legend()
    plt.grid(True)
    ruta_salida = 'C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (potencial de Lennard-Jones)/Actividad4/Ploteos/histograma_velocidades.png'
    plt.savefig(ruta_salida)
    plt.show()



# Ejecutar
if __name__ == "__main__":
    main()
