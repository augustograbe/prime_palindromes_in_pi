# Prime Palindrome in Pi, using parallel programming

**An algorithm that find the first prime palindrome with N digits in PI, using parallel programming to improve performance**

## Running the project

```
$ git clone https://github.com/augustograbe/prime_palindromes_in_pi
$ cd prime_palindromes_in_pi
$ gcc ppp.c -o ppp -lpthread 
```

**Now, the algorithm can run**

```
$ ./ppp <N_SIZE> FILE_1.txt FILE_2.txt ... FILE_N.txt
```

## Test setup
Intel core i5 8th Gen ( 4 core/ 8 logical processors )
20 GB ram 2400 Mhz
HD 5400rpm 
Windows 10

---

## explain the code?

- **#define BUFFER_SIZE 1000000**
    - You can use the BUFFER_SIZE to set the size of buffer that the code can scan per cicle
- **#define NTHREADS 4**
    - You can use NTHREADS to set the number of threads that the algorithm will use, we recomend that set to numbers of logical process in your cpu
- **function is_prime()**
    - check if the current number is prime, you can edit this part to change for other forms that check if the current number is prime
- **function is_odd_palindrome()**
    - check if the current number is palindrome
- **function search_prime_palindrome()**
    - This is the parallel function, here has the logical that do the program run

---

## Results

| palindrome size | palindrome find | position |
| --- | --- | --- |
| 2 | 11 | 95 |
| 3 | 383 | 26 |
| 5 | 38183 | 489 |
| 7 | 9149419 | 13.902 |
| 9 | 318272813 | 129.080 |
| 11 | 93276367239 | 1.358.671 |
| 13 | 3030944490303 | 21.577.552 |
| 15 | 192821202128291 | 30.060.611 |
| 17 | 51779612621697715 | 1.818.540.034 |
| 19 | 1673530243420353761 | 836.814.880 |
| 21 | 151978145606541879151 | 140.608.833.878 |
| 23 | 36336834991019943863363 | 76.459.065.026 |

## Time senquential VS Time Parallel

| palindrome size | senquential time | time 2 Threads | time 4 Threads | time 8 Threads |
| --- | --- | --- | --- | --- |
| 2 | 0.001 s | 0.002 s | 0.002 s | 0.002 s |
| 3 | 0.001 s | 0.002 s | 0.002 s | 0.002 s |
| 5 | 0.002 s | 0.002 s | 0.002 s | 0.002 s |
| 7 | 0.002 s | 0.003 s | 0.003 s | 0.003 s |
| 9 | 0.004 s | 0.005 s | 0.008 s | 0.008 s |
| 11 | 0.029 s | 0.023 s | 0.016 s | 0.014 s |
| 13 | 0.41   s | 0.23   s | 0.16   s | 0.13   s |
| 15 | 0.57   s | 0.33   s | 0.22   s | 0.17   s |
| 17 | 35.2   s | 24.5   s | 19.5   s | 12.7   s |
| 19 | 16.3   s | 9        s | 7.6     s | 6        s |
| 21 | 1:12   h | 42min | 32 min | 28 min |
