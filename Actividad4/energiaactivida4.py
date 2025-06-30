import matplotlib.pyplot as plt

def leer_datos(nombre_archivo):
    Ep, Ec, Et = [], [], []
    with open(nombre_archivo, 'r') as archivo:
        for linea in archivo:
            linea = linea.strip()
            if ',' in linea:
                partes = linea.split(',')
            else:
                partes = linea.split()
            if len(partes) == 3:
                pot, kin, tot = map(float, partes)
                Ep.append(pot)
                Ec.append(kin)
                Et.append(tot)
    return Ep, Ec, Et

def graficar_energias(Ep, Ec, Et):
    plt.figure(figsize=(10, 6))
    plt.plot(Ep, label="Energía Potencial", color='blue')
    plt.plot(Ec, label="Energía Cinética", color='green')
    plt.plot(Et, label="Energía Total", color='red')
    plt.xlabel("Tiempo (fotogramas)")
    plt.ylabel("Energía")
    plt.title("Energías vs Tiempo")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    ruta_salida = 'C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (potencial de Lennard-Jones)/Actividad4/Ploteos/Energías.png'
    plt.savefig(ruta_salida)
    plt.show()

# Cambia este nombre si tu archivo tiene otro nombre
archivo = 'C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (potencial de Lennard-Jones)/Actividad4/ENERGIA.txt'
archivo2 = 'C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (potencial de Lennard-Jones)/Actividad4/ENERGIA.txt'
Ep, Ec, Et = leer_datos(archivo)
graficar_energias(Ep, Ec, Et)



