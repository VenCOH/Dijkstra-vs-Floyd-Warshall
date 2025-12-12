# Hledání nejkratších cest v grafu (NCG)

## Popis algoritmů

### Floyd-Warshall

Floyd–Warshallův algoritmus je algoritmus dynamického programování pro nalezení nejkratších cest mezi všemi páry vrcholů v grafu (orientovaném nebo neorientovaném). 
Pracuje s maticí vzdáleností, kde neexistující hrana je reprezentována jako +∞ a na diagonále jsou nuly. 
Algoritmus postupně „povoluje“ meziuzly k = 0..n−1 a pro každou trojici vrcholů (i, j, k) aktualizuje vzdálenost podle pravidla: D[i][j] = min(D[i][j], D[i][k] + D[k][j]). 
Po dokončení obsahuje matice D nejkratší vzdálenosti mezi všemi páry vrcholů. Časová složitost je O(n^3), paměťová náročnost O(n^2). 
Záporný cyklus lze detekovat tak, že pokud po běhu algoritmu existuje i s D[i][i] < 0, graf obsahuje záporný cyklus dosažitelný z i. 
Rekonstrukci skutečných cest lze provést pomocí pomocné matice předchůdců (next), kterou aktualizujete při každé zlepšující aktualizaci.


### Dijkstra

Dijkstrův algoritmus hledá nejkratší cesty z jednoho zdroje do všech ostatních vrcholů v grafu s nenegativními váhami. 
Používá greedy přístup s prioritní frontou (min-heap). 
Inicializuje pole dist[], kde dist[source]=0 a ostatní = +∞, a pole parent[] pro rekonstruování cest. 
Opakovaně vybírá nezpracovaný vrchol u s nejmenší dist[u], označí ho za zpracovaný a pro každého souseda v provede relaxaci: 
pokud dist[v] > dist[u] + w(u,v), nastaví dist[v] = dist[u] + w(u,v) a aktualizuje parent[v] = u (a vloží/aktualizuje v v haldě). 
Po vyprázdnění haldy obsahuje dist[] nejkratší vzdálenosti od zdroje; cesty se získají sledováním parent[] zpět ke zdroji. 
Časová složitost je O((V + E) log V) s binární haldou a seznamem sousedů (nebo O(V^2) při matici sousednosti). 
Algoritmus nefunguje se zápornými hranami (v takových případech použijte Bellman–Ford).


