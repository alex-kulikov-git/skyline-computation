# ARTful Skyline Computation for In-Memory Database Systems
This work reviews and improves several state-of-the-art algorithms for Skyline computation. 
Further, it proposes a novel, memory-efficient Skyline algorithm called S-ART-S, which bases on its predecessor ST-S. 

The project was advised by Maximilian E. Schüle and supervised by Prof. Dr. Alfons Kemper at the Chair for Database Systems of the Technical University of Munich (TUM). The paper was originally introduced at the ADBIS 2020 conference. 

The skyline of a set of tuples consists of those tuples which are not dominated by any other tuple in the set. 
A tuple dominates another tuple if is better in at least one dimension and not worse in all the other dimensions. 
For a more in-depth explanation of the skyline operator and its application domains, refer to: 

The most common applications of the skyline operation include online approximation algorithms and tasks that make use of pareto optimization. 

Example: 
Imagine an online search engine for holiday booking. 
There is one user that looks to book a holiday trip to Stockholm. 
For this purpose the user wants to get a range of suitable hotels suggested, from which he/she can make their final booking choice.
The criteria the user prioritizes include: room costs, proximity to city center, as well as credit cards being accepted for payment. 
As soon as the user has entered these criteria, the search engine delivers a list of hotels suggested for further review. 
Each of the hotels from the list fulfills the following properties: 
- There exists no other hotel which would be cheaper with other criteria kept equal
- There exists no other hotel which would be closer to city center with other criteria kept equal
- There exists no other hotel which would accept credit cards while the current one doesn't and other criteria kept equal. 
The delivered set of results poses the skyline of all possible hotels in the database of the search engine. 
It enables a fast and convenient way for the user to make his/her final choice based on smartly preselected suggestions. 
