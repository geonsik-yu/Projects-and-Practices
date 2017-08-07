### OpenCL Study Materials
This repository includes introductory study materials of OpenCL and multicore programming, which are covered (or reviewed) in the offline group study sessions in Anam-dong, Seoul. Code examples we produced/practieced are mostly based on textbooks, lecture materials and academic papers mentioned below. Some of the materials mentioned are what we discussed in our sessions but are not implemented. If anybody has spot any problems in our pratice codes or has ideas of how to further improve the GPU manipulation we would love to be noticed about it. Thank you.

### References
[1] Munshi, Aaftab, et al. *OpenCL programming guide*. Pearson Education, 2011.<br/> 
[2] B. Gaster, "Bgaster/opencl-book-samples". *Github repository*. N.p., 2011. Web. 30 June 2017.<br/>
[3] Bernd Burgstaller. "Programming Accelerator with OpenCL." *Multicore Programming*. Yonsei University. Engineering Bld. 2016. Lecture.<br/>
[4] W. Hwu Wen-Mei. "Basic Matrix-Matrix Multiplication." *Heterogeneous Parallel Programming*, Coursera. 2016. Lecture.<br/>
[5] Anton, Howard, and Robert C. Busby. Contemporary linear algebra. Wiley, 2003.<br/>
[6] Cheney, E., and David Kincaid. Numerical mathematics and computing. Nelson Education, 2012.<br/>
[7] Stevens, Cody. GPU-optimized graph theory analysis of allosteric protein signaling networks. Wake Forest University, 2015. <br/>
[8] Redmon, Joseph, and Ali Farhadi. "YOLO9000: better, faster, stronger." arXiv preprint arXiv:1612.08242 (2016).


### List of Code Examples
| Index | Descriptions                                        | Session Date | Remarks  |
| ----- | --------------------------------------------------- | -----------: | -------- |
| ex0   | OpenCL platform and device info.                    | 2017/06/30   |          |
| ex1   | Hello World in OpenCL (Element-wise Square).        | 2017/06/30   |          |
| ex2.1 | Vector addition.                                    | 2017/06/30   |          |
| ex2.2 | Vector addition.                                    | 2017/06/30   |          |
| ex3   | Branch divergence (Not working properly. Why??)     | 2017/08/05   | ???      |
| ex4.0 | matrix multiplication (Sequential)                  | 2017/07/14   |          |
| ex4.1 | OpenCL matrix multiplication: Naive                 | 2017/07/07   |          |
| ex4.2 | OpenCL matrix multiplication: Private memory (Row)  | 2017/07/07   |          |
| ex4.3 | OpenCL matrix multiplication: Row padding           | 2017/07/07   |          |
| ex4.4 | OpenCL matrix multiplication: Local memory (Column) | 2017/07/14   |          |
| ex4.5 | OpenCL matrix multiplication: Precision             | 2017/07/14   |          |
| ex4.6 | OpenCL matrix multiplication: Fewer Synchs          | 2017/08/05   |          |
| ex4.7 | OpenCL matrix multiplication: Tiling                | 2017/--/--   |          |
| ex5.1 | OpenCL matrix transepose: Naive                     | 2017/08/05   |          |
| ex5.2 | OpenCL matrix transepose: Memory coalescing         | 2017/08/05   |          |
| ex6.1 | (Ax = b) Sequential Jacobi Iteration                | 2017/08/05   |          |
| ex6.2 | (Ax = b) GPGPU      Jacobi Iteration                | 2017/08/05   |          |
| ex6.3 | (Ax = b) Sequential Gauss-Seidel                    | 2017/08/--   |          |
| ex6.4 | (Ax = b) GPGPU      Gauss-Seidel                    | 2017/08/--   |          |
| ex6.5 | (Ax = b) Sequential Successive Over-relaxation      | 2017/08/--   |          |
| ex6.6 | (Ax = b) GPGPU      Successive Over-relaxation      | 2017/08/--   |          |
| ex6.7 | (Ax = b) Sequential Power Iteration                 | 2017/09/--   |          |
| ex6.8 | (Ax = b) GPGPU      Power Iteration                 | 2017/09/--   |          |
| ex7   | OpenCV introduction                                 | 2017/08/05   |          |
| ex8   | OpenCL Image filtering: Row operation               | 2017/08/05   |          |
| ex8   | OpenCL Image filtering                              | 2017/08/05   |          |

| ex9.1 | OpenCL Parallel Sum: Naive reduction                | 2017/08/05   |          |
| ex9.2 | OpenCL Parallel Sum: Control divergence             | 2017/08/05   |          |
| ex9.3 | OpenCL Parallel Prefix Sum: Naive                   | 2017/08/05   |          |
| ex9.4 | OpenCL Parallel Prefix Sum: Work-inefficient        | 2017/08/05   |          |
| ex9.5 | OpenCL Parallel Prefix Sum: Work-efficient          | 2017/08/05   |          |
| ex10.1| Sequential Floyd-Warshall algorithm                 | 2017/08/--   |          |
| ex10.2| OpenCL Floyd-Warshall algorithm                     | 2017/08/--   |          |


### Offline Session Contributors
Geonsik Yu and Noori Kim.
