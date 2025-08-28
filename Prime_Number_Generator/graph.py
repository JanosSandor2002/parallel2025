import pandas as pd
import matplotlib.pyplot as plt

# CSV fájl betöltése
data = pd.read_csv('prime_results.csv')

# 1. Problémaméret növelésével hogyan változik a futásidő adott számú számítási egység mellett
# Szűrés: adott számú számítási egység (LocalSize), és a futásidőt (KernelTime) ábrázoljuk
local_size = 32  # Példa: 32-es számítási egység
subset_size_time = data[data['LocalSize'] == local_size]

plt.figure(figsize=(10, 6))
plt.plot(subset_size_time['N'], subset_size_time['KernelTime'], marker='o', color='b')
plt.title(f"Problémaméret vs. Futásidő ({local_size} számítási egységgel)")
plt.xlabel('Problémaméret (N)')
plt.ylabel('Kernel futásidő (ms)')
plt.grid(True)

# PNG fájl mentése
plt.savefig(f"problem_size_vs_time_local_size_{local_size}.png")
plt.show()

# 2. Adott méretű probléma esetén hogyan változik a futásidő a számítási egységek számának növelésével
# Szűrés: adott probléma méret (N), és a számítási egységek (LocalSize) hatása
problem_size = 100000  # Példa: 100000-es probléma méret
subset_threads_time = data[data['N'] == problem_size]

plt.figure(figsize=(10, 6))
plt.plot(subset_threads_time['LocalSize'], subset_threads_time['KernelTime'], marker='o', color='r')
plt.title(f"Számítási egységek száma vs. Futásidő (Problémaméret: {problem_size})")
plt.xlabel('Számítási egységek száma (LocalSize)')
plt.ylabel('Kernel futásidő (ms)')
plt.grid(True)

# PNG fájl mentése
plt.savefig(f"threads_vs_time_problem_size_{problem_size}.png")
plt.show()