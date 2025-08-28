import pandas as pd
import matplotlib.pyplot as plt

# 1) Beolvassuk a kimeneti fájlt
df = pd.read_csv("fibonacci_mapping_results.csv", skip_blank_lines=True)

# 2) Tisztítjuk és típusokra konvertáljuk
for col in ['Size','Threads','SequentialTime','ParallelTime','Speedup','Efficiency']:
    df[col] = pd.to_numeric(df[col], errors='coerce')

# 3) Két DataFrame: naive és cache
df_naive = df[df['Variant']=='naive']
df_cache = df[df['Variant']=='cache']

# --- Grafikon: Tömbméret vs Futásidő (fix Threads=4) ---
plt.figure(figsize=(8,5))
for dfv, label in [(df_naive,'Naive'), (df_cache,'Cache')]:
    sub = dfv[dfv['Threads']==4]
    plt.plot(sub['Size'], sub['ParallelTime'], 'o-', label=label)
plt.xlabel("Tömbméret (N)")
plt.ylabel("Párhuzamos futásidő (s)")
plt.title("Tömbméret vs Futásidő (4 szál)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("size_vs_time.png")
plt.show()

# --- Új grafikon: Tömbméret vs Speedup (fix Threads=4) ---
plt.figure(figsize=(8,5))
for dfv, label in [(df_naive,'Naive'), (df_cache,'Cache')]:
    sub = dfv[dfv['Threads']==4]
    plt.plot(sub['Size'], sub['Speedup'], 'o-', label=label)
plt.xlabel("Tömbméret (N)")
plt.ylabel("Speedup")
plt.title("Tömbméret vs Speedup (4 szál)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("size_vs_speedup.png")
plt.show()
