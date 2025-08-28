import pandas as pd
import matplotlib.pyplot as plt

# Eredmények beolvasása
df = pd.read_csv("montecarlo_results_with_size.csv")

# 1. Futásidő a problémaméret függvényében
plt.figure(figsize=(8,5))
plt.plot(df["ProblemSize"], df["SequentialTime"], label="Szekvenciális", marker='o')
plt.plot(df["ProblemSize"], df["ParallelTime"], label="Párhuzamos", marker='o')
plt.xlabel("Problémaméret (pontok száma)")
plt.ylabel("Futásidő (s)")
plt.title("Futásidő a problémaméret függvényében")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("problem_size_vs_time.png")
plt.show()

# 2. Speedup a problémaméret függvényében
plt.figure(figsize=(8,5))
plt.plot(df["ProblemSize"], df["Speedup"], label="Speedup", marker='o', color="green")
plt.xlabel("Problémaméret (pontok száma)")
plt.ylabel("Speedup")
plt.title("Speedup a problémaméret függvényében")
plt.grid(True)
plt.tight_layout()
plt.savefig("problem_size_vs_speedup.png")
plt.show()

# 3. Efficiency a problémaméret függvényében
plt.figure(figsize=(8,5))
plt.plot(df["ProblemSize"], df["Efficiency"], label="Efficiency", marker='o', color="red")
plt.xlabel("Problémaméret (pontok száma)")
plt.ylabel("Hatékonyság")
plt.title("Hatékonyság a problémaméret függvényében")
plt.grid(True)
plt.tight_layout()
plt.savefig("problem_size_vs_efficiency.png")
plt.show()
