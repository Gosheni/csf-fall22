Jacob Choi - echoi51
Darren Shih - dshih5

Assignment 4
Jacob, Darren - We met and worked on constructing the base for Assignment 4 project.
We implemented our mergesort algorithm and integrated parallelism with child processes.
Once the base was implemented together, Jacob focused on fixing any bugs or memory 
leaks that occurred and Darren focused on finishing the experiment report.

REPORT

# Experiments and analysis

1. The amount of time that the parsort program took to sort the test data for each threshold value

time ./parsort /tmp/$(whoami)/test_16M.in 2097152 
real    0m0.420s
user    0m0.405s
sys     0m0.013s

time ./parsort /tmp/$(whoami)/test_16M.in 1048576
real    0m0.264s
user    0m0.372s
sys     0m0.053s

time ./parsort /tmp/$(whoami)/test_16M.in 524288
real    0m0.177s
user    0m0.455s
sys     0m0.037s

time ./parsort /tmp/$(whoami)/test_16M.in 262144
real    0m0.160s
user    0m0.489s
sys     0m0.074s

time ./parsort /tmp/$(whoami)/test_16M.in 131072
real    0m0.143s
user    0m0.495s
sys     0m0.090s

time ./parsort /tmp/$(whoami)/test_16M.in 65536
real    0m0.144s
user    0m0.505s
sys     0m0.103s

time ./parsort /tmp/$(whoami)/test_16M.in 32768
real    0m0.158s
user    0m0.513s
sys     0m0.146s

time ./parsort /tmp/$(whoami)/test_16M.in 16384
real    0m0.163s
user    0m0.537s
sys     0m0.162s

As seen in the data, the trend is that the time taken to sort goes down when threshold value goes down
up until threshold value of 131072. This is because when calling mergesort with lower threshold values,
the program recursively divides the computation, more specifically the sorting and merging of smaller 
chunks, into more forks that are executed in parallel, increasing its efficiency. 

However, it can be seen that around threshold value 65536 and onward, the program's execution time 
increases, likely due to the fact that it has hit the limit of the number of CPU cores available that 
it can use for parallel execution efficiently.