# Hledání nejkratších cest v grafu (NCG)

## Popis algoritmů

### Floyd-Warshall

Floyd–Warshallův algoritmus je algoritmus dynamického programování pro nalezení nejkratších cest mezi všemi páry vrcholů v grafu (v tomto případě orientovaném). 
Pracuje s maticí vzdáleností, kde neexistující hrana je reprezentována jako $+\infty$ a na diagonále jsou nuly. 
Algoritmus postupně „povoluje“ meziuzly $k = 0..n−1$ a pro každou trojici vrcholů $(i, j, k)$ aktualizuje vzdálenost podle pravidla: $D[i][j] = min(D[i][j], D[i][k] + D[k][j])$. 
Po dokončení obsahuje matice $D$ nejkratší vzdálenosti mezi všemi páry vrcholů. 
Časová složitost je $O(n^3)$, paměťová náročnost $O(n^2)$.
Předpokladem pro správnou funkci algoritmu, je absence záporných hran v grafu.

### Dijkstra

Dijkstrův algoritmus hledá nejkratší cesty z jednoho zdroje do všech ostatních vrcholů v grafu s nenegativními váhami.
Používá greedy přístup s prioritní frontou.
Inicializuje pole $dist[]$, kde $dist[source]=0$ a ostatní = $\infty$.
Opakovaně vybírá nezpracovaný vrchol $u$ s nejmenší $dist[u]$, označí ho za zpracovaný a pro každého souseda $v$ provede relaxaci:
pokud $dist[v] > dist[u] + w(u,v)$, nastaví $dist[v] = dist[u] + w(u,v)$ (a vloží/aktualizuje $v$ v haldě).
Po vyprázdnění haldy obsahuje $dist[]$ nejkratší vzdálenosti od zdroje.
Časová složitost je $O((V + E) log V)$ s binární haldou a seznamem sousedů (nebo $O(V^2)$ při matici sousednosti).
Pro porovnání s Floyd-Warshallovým algoritmem, je třeba volat Dijkstrův algortimus z každého vrcholu.

## Optimalizace algortimu

### Floyd-Warshall

Pro Floyd-Warshallův algoritmus jsem využil blokovou optimalizaci [1].
Jak tato optimalizace funguje, je detailněji popsáno v odkazovné práci. 
Jedná se o rozdělení práce do tří částí, kdy každá z nich může být vykonávána efektivně a paralelně. 
Optimalizace zároveň zahrnuje aplikaci loop tilinigu. 
Vektorizace proběhla pro nejvnitřnější cykly.

### Dijkstra

Implementoval jsem dvě varianty Dijkstrova algoritmu.
První je pro husté grafy, prochází se vždy všechny vrcholy.
Druhá je pro řídké grafy, a pracuje se seznamem sousedů. 
Obě variatny používají jako implmentaci haldy std::priorty_queue, nad std::deque. 

Smotná paralelizace je poměrně jednoduchá.
Jedná se pouze o paralelní spuštění algoritmu z jednotlivých vrcholů.

Díky datovým závislostem, a nutnosti použití haldy, není optimalizace triviální.
Pro obě verze jsem aplikoval loop unrolling vnitřního cyklu, aby mohla proběhnout vektorizace.
Vnitřní cyklus totiž obsahuje podmínku, díky které by byl nevektorizovatelný. 

### Porovnání výkonnosti všech verzí

Měřeno na serveru s AMD EPYC 7543P, 32 jader, 64 vláken.

Časy jsou v milisekundách.

| Počet vrcholů | Šance na hranu | Floyd-Warshall | Dijkstra | Floyd-Warshall OMP | Dijkstra OMP | Dijkstra OMP seznam |
|---------------|----------------|----------------|----------|--------------------|--------------|---------------------|
| 1000          | 1              | 157            | 3070     | 285                | 106          | 92                  |
| 2000          | 1              | 1474           | 21252    | 927                | 770          | 663                 |
| 3000          | 1              | 7442           | -        | 1731               | 2481         | 2165                |
| 4000          | 1              | 19657          | -        | 3445               | 5838         | 5121                |
| 5000          | 1              | -              | -        | 5427               | 11080        | 9634                |
| 5000          | 0.1            | -              | -        | -                  | 11919        | 4090                |
| 5000          | 0.01           | -              | -        | -                  | 10790        | 592                 |
| 5000          | 0.001          | -              | -        | -                  | 10244        | 182                 |

## Závěr

Pro větší počet jader (64), došlo ke zrychlení obou algoritmů. 
Konkrétně zrychlení Floyd-Warshallova algortimu, je patrné až pro větší instance, a není moc velké. 
Je to dáno hlavně tím, že algoritmus není triviálně paralelizovatelný.
Oproti tomu, zrychlení OMP varianty Dijsktrova algoritmu, je téměř přímo úměrné počtu vláken.
Rozdíl mezi dvěma variantami, se projevil v rychlosti běhu, při nižší šanci na hranu.
Zajímavé je, že varianta se seznamem sousedů, je rychlejší i pro velkou šanci na hranu.
Bude to dáno tím, že se v mojí implementaci přidávají vrcholy do haldy opakovaně.

### Zdroje

- [1] Lund, B. & Smith, J. (2010). A Multi-Stage CUDA Kernel for Floyd–Warshall. CoRR, abs/1001.4108. http://arxiv.org/abs/1001.4108 (arXiv:1001.4108)
