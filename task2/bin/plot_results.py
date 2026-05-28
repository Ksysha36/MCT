import csv
import matplotlib.pyplot as plt
import numpy as np

# Чтение CSV
h = []
errC = []
errL2 = []
assem = []
solve = []

with open('results.csv', 'r') as f:
    reader = csv.DictReader(f)
    for row in reader:
        h.append(float(row['h']))
        errC.append(float(row['C_norm']))
        errL2.append(float(row['L2_norm']))
        assem.append(float(row['Assembly_time']))
        solve.append(float(row['Solve_time']))

h = np.array(h)
errC = np.array(errC)
errL2 = np.array(errL2)
assem = np.array(assem)
solve = np.array(solve)

# График сходимости
plt.figure(figsize=(8,6))
plt.loglog(h, errC, 'o-', label='C-норма')
plt.loglog(h, errL2, 's-', label='L2-норма')
plt.loglog(h, h**2, 'k--', label='$h^2$')
plt.xlabel('Шаг сетки h')
plt.ylabel('Ошибка')
plt.legend()
plt.grid(True)
plt.savefig('convergence_2d.png')
plt.close()   # вместо plt.show()

# График времени
plt.figure(figsize=(8,6))
N = 1.0 / h
plt.loglog(N, assem, 'o-', label='Сборка матрицы')
plt.loglog(N, solve, 's-', label='Решение системы')
plt.xlabel('Число узлов N')
plt.ylabel('Время (с)')
plt.legend()
plt.grid(True)
plt.savefig('timing_2d.png')
plt.close()   # вместо plt.show()

print("Графики сохранены: convergence_2d.png, timing_2d.png")