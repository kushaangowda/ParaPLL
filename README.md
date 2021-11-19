# ParaPLL

Our implementation of this [paper](https://dl.acm.org/doi/10.1145/3225058.3225061) as a part of CS - 309 course's final project

## Problem Statement
Parallelize shortest-path distance query on large-scale weighted graphs

## Clone this repository
```bash
git clone https://github.com/kushaangowda/ParaPLL.git
```

## Testing
```bash
g++ ParaPLL.cpp -fopenmp             # compiling ParaPLL
./a.out                              # execution
```

## Custom Dataset
The dataset should be a .txt file, of this format  
Line 1: 2 space separated integers consisting of number of vertices (<strong>nv</strong>) and number of edges (<strong>ne</strong>)  
Line 2 onwards: <strong>ne</strong> lines consisting of 3 space separated integers consisting of source vertex (<strong>s</strong>), destination vertex (<strong>d</strong>) and edge weight (<strong>w</strong>)  
For example: [test-20-98.txt](Datasets/test-20-98.txt)  

## Team Members
[Kushaan Gowda](https://github.com/kushaangowda)  
[Kola Sai Ram](https://github.com/sairam135)  
[Mayank Raj](https://github.com/mayank-raj15)  
[Abhinav Reddy]()
