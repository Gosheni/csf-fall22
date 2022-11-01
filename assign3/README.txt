
Darren Shih <darshih888@gmail.com>
10:19 PM (8 minutes ago)
to me

Jacob Choi - echoi51
Darren Shih - dshih5

Milestone 1
Jacob, Darren - We met and worked on constructing the base for Assignment 3 cache project.
We implemented our cpp code as far as to handling arguments and running without any issues.

Milestone 2
Jacob, Darren - We met and worked on constructing our cache simulator using different cpp
and hpp files. After construction, Jacob and Darren focused on reducing the size and efficiency
of our code.

Milestone 3
Jacob, Darren - We met and worked on cleaning up the code and working on fifo functionalities
and performing experiments to find the best cache.

Experiments for Best cache:

# Cache type:

Cache size is equal to the multiplication of number of sets by number of blocks by number of bytes per block
To keep tests consistent, we decided to run the following commands to keep cache size consistent at 131072
bytes.

## The following tests keep number of bytes per block consistent:
`./csim 8192 1 16 write-allocate write-through lru < swim.trace` (direct mapped)
    Load hit rate: 219625/220668=0.995273
    Store hit rate: 72012/82525=0.872608
    Total cycles: 13094525
`./csim 2048 4 16 write-allocate write-through lru < swim.trace` (4-way set-associative)
    Load hit rate: 219730/220668=0.995749
    Store hit rate: 72027/82525=0.872790
    Total cycles: 13046630
`./csim 1024 8 16 write-allocate write-through lru < swim.trace` (8-way set-associative)
    Load hit rate: 219733/220668=0.995762
    Store hit rate: 72027/82525=0.872790
    Total cycles: 13045433
`./csim 1 8192 16 write-allocate write-through lru < swim.trace` (fully associative)
    Load hit rate: 219742/220668=0.995804
    Store hit rate: 72028/82525=0.872802
    Total cycles: 13041442
As seen from the results, the load and store hit rates as well as the total cycles do not differ significantly
from test to test.

## The following tests keep number of blocks consistent:
`./csim 4096 4 8 write-allocate write-through lru < swim.trace`
    Load hit rate: 219347/220668=0.994014
    Store hit rate: 64022/82525=0.775789
    Total cycles: 12436647
`./csim 1024 4 32 write-allocate write-through lru < swim.trace`
    Load hit rate: 220054/220668=0.997218
    Store hit rate: 76717/82525=0.929621
    Total cycles: 13610154
`./csim 256 4 128 write-allocate write-through lru < swim.trace`
    Load hit rate: 220374/220668=0.998668
    Store hit rate: 80967/82525=0.981120
    Total cycles: 14399274
`./csim 64 4 512 write-allocate write-through lru < swim.trace`
    Load hit rate: 220504/220668=0.999257
    Store hit rate: 82104/82525=0.994899
    Total cycles: 15961004
`./csim 32 4 1024 write-allocate write-through lru < swim.trace`
    Load hit rate: 220537/220668=0.999406
    Store hit rate: 82297/82525=0.997237
    Total cycles: 17663437
As the number of bytes per block increase, the load hit rate experiences a minor increase and the store hit
rate experiences a significant increase, but flats off at around 128 bytes per block. The number of total
cycles increases significantly the greater the bytes per block, which means the performance decreases.

## The following tests keep number of sets consistent:
`./csim 256 1 512 write-allocate write-through lru < swim.trace`
    Load hit rate: 220155/220668=0.997675
    Store hit rate: 81830/82525=0.991578
    Total cycles: 23935055
`./csim 256 2 256 write-allocate write-through lru < swim.trace`
    Load hit rate: 220444/220668=0.998985
    Store hit rate: 81718/82525=0.990221
    Total cycles: 15071344
`./csim 256 4 128 write-allocate write-through lru < swim.trace`
    Load hit rate: 220374/220668=0.998668
    Store hit rate: 80967/82525=0.981120
    Total cycles: 14399274
`./csim 256 8 64 write-allocate write-through lru < swim.trace`
    Load hit rate: 220257/220668=0.998137
    Store hit rate: 79460/82525=0.962860
    Total cycles: 14034357
`./csim 256 16 32 write-allocate write-through lru < swim.trace`
    Load hit rate: 220069/220668=0.997286
    Store hit rate: 76718/82525=0.929633
    Total cycles: 13597369
As the number of blocks increase, the load hit rate experiences a minor decrease and the store hit rate
experiences increasing greater decrease (especially from 8 to 6 blocks). The total cycles also experiences
a major decrease then flatlines (but still decreasing) around 2 blocks, which means an increase in the
performance of the program.

In conclusion, we decided that 256 as the number of sets, 4 as the number of blocks, and 128 as the number of
bytes per block as the best choice with a high load and store hit rate while having a acceptable performance.


# Writing parameters:

The following tests were conducted for the three possible combinations of write-allocate and write-through.
`./csim 256 4 128 write-allocate write-through lru < swim.trace`
    Load hit rate: 220374/220668=0.998668
    Store hit rate: 80967/82525=0.981121
    Total cycles: 14399274
`./csim 256 4 128 write-allocate write-back lru < swim.trace`
    Load hit rate: 220374/220668=0.998668
    Store hit rate: 80967/82525=0.981121
    Total cycles: 8726941
`./csim 256 4 128 no-write-allocate write-through lru < swim.trace`
    Load hit rate: 219718/220668=0.995695
    Store hit rate: 70275/82525=0.851560
    Total cycles: 11512218
As seen from the results, write-allocate and write-back is the best combination with the least total cycles
(best performance) and the highest load and store hit rate.


# FIFO vs LRU:

The following tests were conducted to show the difference in fifo vs lru.
`./csim 256 4 128 write-allocate write-back lru < swim.trace`
    Load hit rate: 220374/220668=0.998668
    Store hit rate: 80967/82525=0.981121
    Total cycles: 8726941
`./csim 256 4 128 write-allocate write-back fifo < swim.trace`
    Load hit rate: 220345/220668=0.998536
    Store hit rate: 80960/82525=0.981036
    Total cycles: 8842105
From the following results, lru is better with slight less total cycles (better performance) and slightly
higher store and hit rates.


# Conclusion:
In conclusion, we decided that 256 as the number of sets, 4 as the number of blocks, and 128 as the number of
bytes per block, write-through, write-back, and lru as the best optimal choices with a high load rate of
0.998668 and high store hit rate of 0.981121 while having a performance of number of total cycles as 8726941.