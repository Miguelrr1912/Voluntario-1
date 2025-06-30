import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# ======================================
# DATOS DE ENTRADA
# ======================================

# Sustituye estos valores por los tuyos:
# 4 pares: T, P, error_T, error_P
T = np.array([0.918918,2.077451, 4.513760, 7.950148])        # Temperatura
P = np.array([0.051709, 0.123603, 0.283571,0.516982 ])        # Presión
err_T = np.array([0.000825, 0.000939, 0.001600, 0.002110])    # Error en T
err_P = np.array([0.006995, 0.013448,0.025037, 0.038302])    # Error en P

# ======================================
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import pearsonr  # <-- import para Pearson

# ======================================
# MODELO LINEAL
# ======================================

def linear(x, m, b):
    return m * x + b

# ======================================
# AJUSTE LINEAL PONDERADO
# ======================================

# El argumento sigma usa err_P como pesos
popt, pcov = curve_fit(linear, T, P, sigma=err_P, absolute_sigma=True)

# Resultados del ajuste
m, b = popt
dm, db = np.sqrt(np.diag(pcov))

print("=====================================")
print("RESULTADOS DEL AJUSTE LINEAL")
print(f"P = ({m:.3f} ± {dm:.3f}) * T + ({b:.3f} ± {db:.3f})")

# ======================================
# COEFICIENTE DE CORRELACIÓN DE PEARSON
# ======================================

r, p_value = pearsonr(T, P)
print(f"Coeficiente de correlación de Pearson: R = {r:.4f}")
print(f"Valor-p asociado: p = {p_value:.4e}")
print("=====================================")

# ======================================
# GRAFICAR RESULTADOS
# ======================================

plt.errorbar(T, P, yerr=err_P, xerr=err_T, fmt='o', capsize=5, label='Datos con errores')
x_fit = np.linspace(min(T)-0.5, max(T)+0.5, 100)
plt.plot(x_fit, linear(x_fit, m, b), 'r-', label='Ajuste lineal')

# Mostrar R en la gráfica
plt.text(min(T), max(P), f"R = {r:.3f}", fontsize=10, verticalalignment='top')

plt.xlabel('Temperatura (T)')
plt.ylabel('Presión (P)')
plt.title('Ajuste lineal P vs T')
plt.legend()
plt.grid(True)
plt.tight_layout()

# Guardar imagen si quieres
plt.savefig('C:/Users/User/Documents/Fisica_compu/Compu/Voluntario 1 (potencial de Lennard-Jones)/Actividad1/Ploteos/ajuste_lineal_PT.png', dpi=300)
plt.show()

print("Gráfico completado.")

