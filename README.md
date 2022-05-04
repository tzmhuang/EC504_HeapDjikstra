# EC504 Project

We implement three alternative priority heap algorithms namely Fibonacci heaps, rank-pairing heaps and violation heap, and tested them on Dijkstra shortest path algorithm.

# Run

To run the algorithm, first compile the program by 

```
cd ./src
make all
```

then run the shortest path program by

```
./shortest input_graph.txt
```

Replace `input_graph.txt` with the path to an input graph. The input graph should be in edge list format with the total number of nodes and total number of edges in the first line.

We provided three sample input graphs under the `input` folder. For example, to run on `Graph1.txt`, run

```
./shortest ../input/Graph1.txt
```

The program should create `../input/Graph1.txt.out` that shows the execution time and the results of the execution. We provided sample outputs for the three example input graphs. The sample outputs can be found under `output`.


# References
[1] Amr Elmasry. The violation heap: A relaxed fibonacci-like heap. In Proceedings of the 16th Annual International Conference on Computing and Combinatorics, COCOON’10, page 479–488, Berlin,
Heidelberg, 2010. Springer-Verlag.

[2] Michael L. Fredman and Robert Endre Tarjan. Fibonacci heaps and their uses in improved network optimization algorithms. J. ACM, 34(3):596–615, jul 1987.

[3] Bernhard Haeupler, Siddhartha Sen, and Robert E. Tarjan. Rank-pairing heaps. In Amos Fiat and Peter Sanders, editors, Algorithms - ESA 2009, pages 659–670, Berlin, Heidelberg, 2009.Springer Berlin Heidelberg. 3

