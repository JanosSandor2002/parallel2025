# Név: Sándor János, Neptunkód: U03GDK
## 1. Prime Number Generator (Prímszámok generálása)

### Feladat specifikációja
A feladat célja, hogy generáljunk egy listát az összes prímszámról 1 és N között. A program OpenCL segítségével párhuzamosítja a számításokat, és a hatékony terheléselosztásért az OpenCL-eszköz (GPU/CPU) work‐groupjai és work‐itemjei felelnek. Minden sorozatban a teljes tartományt egyetlen kernel dolgozza fel, a work‐itemek dinamikusan osztják el egymás között a számpróbálás feladatát.

### Alkalmazott technológia: OpenCL
- **Platform és eszköz inicializálás**  
  - `clGetPlatformIDs()`, `clGetDeviceIDs()`  
- **Kontextus és parancssor**  
  - `clCreateContext()`, `clCreateCommandQueue()`  
- **Memória-bufferek**  
  - `clCreateBuffer()` a kimeneti prímszám-lista és a bemeneti N változó számára  
- **Kernel**  
  - Egyetlen OpenCL C kernel, amely minden work‐itemnél egy sávban dolgozik:  
    ```c
    __kernel void prime_kernel(const unsigned int N, __global int *out, __global int *count) {
        size_t gid = get_global_id(0);
        if (gid >= N-1) return;
        unsigned int n = gid + 2;
        bool is_prime = true;
        for (unsigned int d = 2; d * d <= n; ++d) {
            if (n % d == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            int idx = atomic_inc(count);
            out[idx] = n;
        }
    }
    ```
  - **Work‐group és work‐item beállítás**:  
    `global_size = round_up(N-1, local_size);`
- **Párhuzamos munka**  
  - A host kód beállítja a `N`, az output tömb és a `count` buffer tartalmát, majd elindítja a kernelt `clEnqueueNDRangeKernel()`-kel.

### Teljesítménymérés és vizsgálatok
1. **Szálak/compute units (work‐group) hatása**  
   - Variáld a `local_size` értékét (pl. 32, 64, 128, 256).  
   - Mérd a teljes kernel-futtatási időt (host→device→kernel→device→host).  
   - **Grafikon:** local_size vs. futásidő.

2. **Problémaméret hatása**  
   - Vizsgáld különböző N értékekre: N = 10^4, 10^5, 10^6, 10^7.  
   - **Grafikon:** N vs. futásidő.

3. **Hatékonyság- és költségmérés**  
   - **Work**: Összes elvégzett osztópróba száma = (N-1) × átlagos d-próbák.  
   - **Span**: A kernel leglassabb sávjának futási ideje.  
   - **Efficiency**: Szekvenciális idő / (paralel idő × (global_size/local_size)).  
   - **Grafikon:** local_size vs. efficiency.

### Kimenet
- A `out` buffer tartalmazza a 2 és N közötti prímszámokat, a `count` mutatja a darabszámot.  
- Minden mérési eredményt CSV-be exportálunk:  


## 2. Monte Carlo Simulation (Monte Carlo szimuláció)
### Feladat specifikációja:
A feladat célja a Pi értékének meghatározása a Monte Carlo módszer alkalmazásával. A módszer során véletlenszerű pontokat generálunk az [0,1]×[0,1] négyzeten belül, majd meghatározzuk, hogy hány pont esik az egységsugarú körbe. A körbe eső pontok arányából közelítjük a Pi értékét.

A program párhuzamosan működik, és minden szál a teljes próbálkozásszám egy részhalmazát dolgozza fel, így biztosítva az egyenletes terheléselosztást.

### Alkalmazott technológia:
Pthreads:

A szálkezelést a POSIX Threads (Pthreads) segítségével valósítjuk meg.

Az összes próbálkozás számát (pl. 1.000.000) osztjuk el a rendelkezésre álló szálak között.

Az egyenletes munkamegosztás érdekében minden szál azonos számú pontot generál.

Ha egy szál hamarabb végez, lehetőséget biztosítunk dinamikus munkamegosztásra (pl. munkakosár vagy „work stealing”), hogy más szálak munkáját segítse.

### Teljesítménymérés és vizsgálatok:
- 1. Szálak számának hatása a futásidőre:

Vizsgálat különböző szálkészletekkel (pl. 1, 2, 4, 8, 16 szál).

Mérés: futásidő vs. szálak száma.

Grafikon készül a szálak számának hatásáról.

- 2. Problémaméret hatása:

A szimuláció során generált pontok számát növeljük lépcsőzetesen (pl. 500000, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000, 4500000, 5000000).

Mérés: futásidő vs. generált pontok száma.

Ennek célja a skálázhatóság és teljesítmény alakulásának megfigyelése a problémaméret növekedésével.

- 3. Hatékonysági mutatók:

Speedup: A szekvenciális és párhuzamos futásidő hányadosa.

Hatékonyság: A speedup osztva a szálak számával.

Munkaelosztás mérése: Vizsgáljuk, mennyire egyenletesen sikerült szétosztani a pontgenerálási munkát a szálak között.

### Kimenet:
A program a végén kiírja a becsült Pi értéket és a teljes futásidőt.

Méri az egyes szálak teljesítményét is, és opcionálisan logolhatja a szálankénti eredményeket.

A futásidő-adatokból grafikonokat generálunk:

Szálak száma vs. futásidő

Generált pontok száma vs. futásidő

Hatékonyság vs. szálak száma
## 3. Fibonacci Leképezés Tömbre (Fibonacci Mapping on Array Elements)
Feladat specifikációja:
A feladat célja egy tömb (pl. data) feltöltése véletlenszerű pozitív egész számokkal egy adott intervallumból (pl. 1–1000 vagy 1–10000). Ezután minden data[i] érték alapján meghatározzuk a data[i]-edik Fibonacci-számot, és az eredményt eltároljuk egy másik tömbben (result[i]).

Például, ha data[3] = 5, akkor result[3] = F(5) = 5.

Mivel a Fibonacci-számok számítása során korábban kiszámított értékek újra felhasználhatók, így az optimalizálás és a hatékony szálkezelés kulcsfontosságú.

### Alkalmazott technológia:
OpenMP:

A párhuzamos feldolgozást OpenMP segítségével valósítjuk meg.

Minden szál a data tömb egy részhalmazán dolgozik.

Lehetőség van:

egyszerű szálfüggetlen számításra (rekurzió vagy iteráció minden elemhez),

közös Fibonacci-cache használatára (pl. memoizációval, közösen használt tömbben).

Szinkronizációs kihívások: ha többen próbálják ugyanazt a Fibonacci értéket egyszerre kiszámolni, megfelelő zárolás vagy atomic műveletek lehetnek szükségesek.

### Teljesítménymérés és vizsgálatok:
- 1. Szálak számának hatása a futásidőre:

Mérés különböző OpenMP num_threads beállításokkal (pl. 1, 2, 4, 8, 16 szál).

Grafikon: szálak száma vs. futásidő

- 2. Problémaméret vizsgálata:

Különböző tömbméretekkel (data mérete): pl. 700000, 750000, 800000, 850000, 900000, 950000, 1000000 elem.

Grafikon: tömbméret vs. futásidő

- 3. Optimalizációs lehetőségek és hatékonysági mutatók:

Memoizáció/caching használatának hatása.

Speedup és hatékonyság mérése.

Vizsgálat: érdemes-e újraszámolni minden Fibonacci-számot külön, vagy inkább megosztani a korábban kiszámolt értékeket.

### Kimenet:
A result tömb tartalmazza a data tömb minden elemének megfelelő Fibonacci-számot.

Mérési eredmények alapján grafikonokat generálunk:

Szálak száma vs. futásidő

Tömbméret vs. futásidő

Hatékonyság vs. szálak száma