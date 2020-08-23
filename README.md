# ARTful Skyline Computation for In-Memory Database Systems
The purpose of this work is to adapt and parallelize skyline computation for main memory database systems, as well as to adapt the ART tree for categorical skyline computation. 

Skyline algorithms produce their results much faster when integrated into a database system, instead of running on top of it. Therefore, this work adjusts some of the most common skyline algorithms, so that they can be used as a building block in main memory databases. In addition to that, modern parallelization approaches are introduced and applied to the algorithms. The conducted performance tests show that the suggested parallelization approaches have been successful in reducing the computation time of the algorithms when running in parallelized environments.

Moreover, the ART tree is used to develop a novel skyline algorithm called SARTS. The algorithm is able to progressively output skyline tuples in online environments, while being fast and keeping its memory usage to a minimum. As shown in the evaluation, it can keep up with its predecessor ST-S in terms of computation time, while using up to 20 times less memory at runtime.

The project was advised by **Maximilian E. Schüle** and supervised by **Prof. Dr. Alfons Kemper** at the Chair for Database Systems of the Technical University of Munich (TUM). The paper was originally introduced at the ADBIS 2020 conference. 

## The Skyline Operator
The skyline of a set of tuples consists of those tuples which are not dominated by any other tuple in the set. 
A tuple dominates another tuple if is better in at least one dimension and not worse in all the other dimensions. 
For a more in-depth explanation of the skyline operator and its application domains, refer to: 
S. Borzsony, D. Kossmann, and K. Stocker, *The Skyline operator*, in Proceedings 17th International Conference on Data Engineering, April 2001, pp. 421–430.

## Project Code
The original project code can be found: 
* for skyline algorithms operating on continuous tuple domains on [GitLab](https://gitlab.db.in.tum.de/alex_kulikov/skyline-computation).
* for skyline algorithms operating on categorical tuple domains on [GitLab](https://gitlab.db.in.tum.de/alex_kulikov/skyline-categorical).


For any inquiries contact me at averkulikov@gmail.com. 


