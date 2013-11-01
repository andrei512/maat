# maat 0.1 - 

Query autocomplete webservice in C++ 

## Requirements

- Mac OS X or Linux - I have no intention to add support for [TempleOS](http://www.templeos.org/) or Windows 
- ruby 1.9.3 or above


## Usage

./buid.sh - will generate code, compile and build the program

./run.sh locations.in - build + run with all arguments passed

Use the conversion script csv_to_input.rb to process the input given by sql dump

In order to run the webservice localy

```shell
bundle install # this will install the necesary ruby gems
./server.sh locations.in # will start a local server for the demo app
```

To test the app use Safari or Firefox to open [app/index.html](app/index.html)

Chrome has some issues with AJAX from local files 


## Name origin

in egyptian mythology Ma'at was the goddess of truth, justice and harmony. She was associated with the balance of things on earth.


## Algorithms used

- trie
- [IncNGTrie](http://www.vldb.org/pvldb/vol6/p373-xiao.pdf)
- [edit distance](http://en.wikipedia.org/wiki/Levenshtein_distance)
- heap
- C++ STL [vector](http://www.cplusplus.com/reference/vector/vector/)


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

for 3 the it eats up ~10 GB RAM

A lot of random data appears for small queries as any 3 letter query is kind of the same for the algorithm

### Conclusion

The 2 variation solution seems to behave the best 

## Problems to be solved

- Prioritize locations and set some rules (Max 2 states, Max X Y, At least K ...)
- The location data provided is corupted - (id = 191454, name = "lazy y u")









