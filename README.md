# study-cpp-projects
### c_labs
Some C laboratory works, completed during first 1 year of university: 

*  queue.c - implementation of queue data structure  
*  rec_file_searcher.c - tries to recursively search file containing single word "Minotaur" in folder "labyrinth". Initial file must be named "file.txt" and placed somewhere in the "labyrinth" folder. Initial file can linked to other files:  
`@include file2.txt`  
`@include file3.txt`  
And linked files can link to another files, and etc. Finally, some file must contain "Minotaur". As result, program prints paths of files, which led to Minotaur, to file "result.txt". Cycled file links (file2.txt -> file3.txt -> file2.txt) are forbidden.  
*  valid_simple_html.c - using implemented stack check if simple (like `<tag>test</tag>` == not nested) xml (tag names doesn't matter, checks only braces, except for non-closing tags) is valid. Support non-closing `<br>` and `<hr>` tags (following is valid: `<closing-tag><br>test</closing-tag>`)  
*  doubly_linked_list - **course work**: C implementation of doubly-linked list, with bubble sort function

### algo
Some algorithms, implemented during course "Construction and analysis of algorithms" using C++
Includes:  

*  Ford-Fulkerson implementation (max_flow.cpp)  
*  A* implementation (astar.cpp)  
*  Knuth–Morris–Pratt implementation (kmp.cpp)  

### prefix_code_course_work
**Course work**: C++ implementation of Huffman coding and Shannon-Fano coding algorithms. Implemented build of a coding tree, encoding/decoding of a text file using computer codes, along with simple GUI using Qt.

### qt_bmp_simple_painter
**Course work**: C++ Qt implementation of simple bmp painter. Supports: drawing lines, inverting a part of drawing area, cropping a part of drawing area, changing drawing color, control over line thickness. Image format: bmp24.

### qt_bfs_visualization
C++ Qt visualization of BFS graph search alhorithm, supports reading graph from file, examples provided in /doc

### qt_dijkstra_visualization
C++ Qt visualization of Dijkstra graph  alhorithm, supports reading graph from file, examples provided in /doc

### qt_bin_tree
C++ simple implementation of binary tree with Qt GUI

### qt_rec_check_expr
C++ Qt implementation of recursive check for provided expression spec:
```
expr  ::= token | (expr sign expr)
token ::= a..z
sign  ::= - | + | *
```

### qt_nqueen_visualization
C++ Qt visualizaion of solving Nqueen problem using backtracking. Supports two modes: 

* manual placing first queen, then solving given case  
* solving all cases, prints them to file "solutions.txt"  
