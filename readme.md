# maat 0.1 - 

Query autocomplete webservice in C++ 

## Usage

./buid.sh - will generate code, compile and build the program

./run.sh locations.in - build + run with all arguments passed

Use the conversion script csv_to_input.rb to process the input given by sql dump

./server.sh locations.in - will start a local server for the demo app


## name origin

in egyptian mythology Ma'at was the goddess of truth, justice and harmony. She was associated with the balance of things on earth.


## algoriths used:

- trie
- [IncNGTrie](http://www.vldb.org/pvldb/vol6/p373-xiao.pdf)
- [edit distance](http://en.wikipedia.org/wiki/Levenshtein_distance)
- heap


## Solution overview

The proposed solution uses a error threshold MAX_VARIATION wich changes the behavior and performance of the solution

### MAX_VARIATION = 0

Query response time ~ 0.0001 for any search

Memory used ~45 MB RAM for 100.000 locations

Does not handle errors

### MAX_VARIATION = 1

Query response time ~ 0.0001 for searches any search with length greater or equal to 3
We can preprocess all the 2 letter searches in order to achieve the best performace

Memory used ~315 MB RAM for 100.000 locations

Handles 1 typo or 1 missing character 

### MAX_VARIATION = 2

Query response time ~ 0.0001 for searches any search with length greater or equal to 3
We can preprocess all the 2 letter searches in order to achieve the best performace

Memory used ~2 GB RAM for 100.000 locations

"nw y" -> "new york city" 

### MAX_VARIATION >= 3

The memory just explodes from here on 
3 -> ~10 GB RAM

A lot of random data appears for small queries as any 3 letter query is kind of the same for the algorithm


### Conclusion

The 2 variation solution seems to behave the best 

## Problems to be solved

[ ] prioritize locations and set some rules (Max 2 states, Max X Y, At least K ...)
[ ] the location data provided is corupted - (id = 191454, name = "lazy y u")









