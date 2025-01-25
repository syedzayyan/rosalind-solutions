# Rosalind Solutions in C

### Disclaimer
I am not responsible for these programs. If stuff happens because I have forgotten to free memory or you get trauma from reading my horrible comments and code I warned you. Sometimes I look at ChatJibbity to solve problems so whatever maybe I am cheating in solving these problems too. Or maybe I am gaming Github Copilot to be trained on crap code.

### Who does this in C
I am solving the problems on [Rosalind](https://rosalind.info/problems/list-view/) in C because I ...... am trying to be a REAL PROGRAMMER! No, I think it's because a lot of the solutions on the web and on the site itself seems to be in Python. I don't want to learn Perl (The old Bioinformaticsy language) and I have my qualms with Rust (you never know I might just start to again). Besides nothing is implemented in C like not even a basic hashmap. I kind of like it. 

### How to compile do anything?
The code is structured in folders with a common.c and common.h file thing at the top. The "common" folder contains some common data structure that I needed to implement to do basically anything in C. Well to be fair I did say I wanted to do these anyway so it's kinda my fault.

The make file takes in a filename, so at the root type in something like `make overlap_graph/og.c` and then things get compiled. You run the program and magic preferably happens.

### Whats in the Common Folder
Since C practically has nothing in it's standard library, I needed to implement all sorts of things. Dictionaries? What's that. Sets? Make your own. As I go along problem solving I put all my data structure things there. Things may have version twos and three and used all over the place. As I said, I am solving things still and stuff is hard!