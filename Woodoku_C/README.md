# Woodoku Algorithm in C++
### Solving Woodoku with C++ 
By previous Python version, I realized that using Numpy for Woodoku is painfully slow. Thus, I did some bitwise operations and other tricks to make everything faster with C++. Plus with some help from gcc optimization O3.
## Implemented Algorithms till now
### The basic Idea
All algorithms generate all possible cases that can be made by positioning 3 pieces. 9^6 cases are generated and by those generated cases, we figure out which combination of positioning returns the best results. 
### BruteForceStupid
Finds out which positioning of 3 pieces result the highest score. If there are multiple ones with the same result, the method picks out the last one. This is implemented as `BruteForceStupid` class.
### EmptySpace
Finds out which positioning of 3 pieces result the most empty spaces. If there are multiple ones with the same result, the method picks out the last one. Implemented as `SurvivalMethod` class.
### HeuristicsMethod
Finds out which positioning of 3 pieces result the most heuristics score. If there are multiple ones with the same result, the method picks out the last one. Implemented as `heuristicsMethod` class. In this class, it calculates heuristics score in the following equation below. (Till now, 2022-01-17)

    HeuristcsScore = Score + (81 - Empty Space Groups) / 10 + (81 - Filled Groups) / 10
It calculates empty space groups by using DFS and figure out how many empty space groups are there in the field by this combination. I am trying to make heuristics formula that minimizes the number of same heuristics score. If that is minimized, the function will have less chance of picking one randomly without any reason.

### HeuristicsThreadMethod
The older `HeuristicsMethod` had several flaws.
- Order of placing shapes were fixed to 0, 1, 2 order.
- Did NOT clear field while blocks were being places.
- If it could not put shapes in 0, 1, 2 order, it gave up.

The new `HeuristicsThreadMethod` was made to fix both problems. This is designed to find out every single possible cases that occur not just by putting shapes in 0, 1, 2. This tries every order possible. That is 3! cases. However, since this method has to calculate 6 more times than original `HeuristicsMethod`. Thus Threading was introduced. Every possible cases which is
- 0, 1, 2 -> Thread 1
- 0, 2, 1 -> Thread 2
- 1, 0, 2 -> Thread 3
- 1, 2, 0 -> Thread 4
- 2, 1, 0 -> Thread 5
- 2, 0, 1 -> Thread 6
Are each put on different thread to save computing time. However, due to threading overhead, it takes a bit more time compared to the original `HeuristicsMethod`. But can survive a slight more turns that it originally would have.


### Comparison
![comp_2022_01_19.png](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/comp_2022_01_19.png)

## So.. how is this everything really implemented?
### Short Answer : Bitwise Operations.
As I have mentioned up above earlier, using Python and Numpy is painfully slow even with i9 CPU. The Woodoku Field itself, and all shapes are represented in **bit** level. Not even **byte** level. I know its 21st century and there are no reason for this much optimization, but I just wanted to make things faster and faster.

### WOODOKU Map
A Woodoku map (field) is represented as a Class. In that class there is a `private` array for representing the field. Woodoku map is 9 * 9 size. Thus this means that we need 81 bits to represent the whole  Woodoku map. That being said, I am going to use 3 `uint32_t`. Using three 32 bit integer will be enough to represent the whole map itself. Each variables will be representing 3 rows. (3 * 9 = 27 < 32).

- **Indexing**: Since we have extra 5 bits left, we are going to start representing the rows starting from 26th bit. `SMALLTYPE Field::getPixelValue(SMALLTYPE x, SMALLTYPE y)` in `Field.cpp` has rough ideas about how I am indexing those bits.
- **Scoring**: We have to calculate the scores in order for DFS or Brute force to work. We could have used `for` loops or `if` statements for taking care of scoring. However, as you probably know, using `if` and `for` loops are pricey in terms of computer resources. Also, we are going to use scoring function in every single cases. So in order for us to make function run faster, we have to cut time in scoring process. Thus I have chosen **Bitwise Operations** for this solution. It is well known fact that **Bitwise Operations** are 'cheaper' in terms of computer resources and processors are extremely good at it. I tried my best to use as little `if` statements as possible. The basic ideas are mentioned in `SMALLTYPE Field::calculateScore()`.   

### Shapes
A Woodoku shape is represented as a Class. In that class, there is a `private` `uint32_t` variable for representing the shapes into bits. As you probably have guessed now, all Woodoku shapes can be represented in 5*5 matrix. That being said, 5*5 = 25 < 32. So we are able to represent a single Woodoku shape into a `uint32_t` variable. 

- **Indexing**: Similar to Woodokup Maps. check `bool Shape::getPixel(SMALLTYPE x, SMALLTYPE y)` for more information.
- **Checking Blocks with Field**: Since whether if you are using brute force or using a good algorithms, we are going to be checking if a block can be fit into the field. However, till now, the checking process is NOT done by Bitwise Operations. **TODO : Making every checking process done by Bitwise Operations**

## So... Is this really fast and does it work?
### Its fast and works Okay.
My personal Woodoku Record is around 2100 points. However, even my Python version brute force Woodoku algorithm can beat up to **6285 points**. C++ version is not perfect yet, however it can score up to **8209 points**. Both measured at 2022-01-19 and has enough room for improvements.

### Python Version Proof
![YeahItWorks1](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/YeahItWorks1.png)

### C++ Version Proof
![YeahItWorks1](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/YeahItWorks2.png)


## Your algorithm is dumb. I want mine in it
Yeah I admit the fact that I am using 6 nested for loops for these algorithms. Also I am a noob for computer science and still in learning process. If you happen to have better idea than mine, you can try making your own algorithm. 
1. Make a child algorithm of class `Algorithm`. 
2. Override member function `findBestFuture` with using your algorithm. 
3. Return `bfResult` variable as return value for the algorithm.
4. from `main.cpp` generate your algorithm object and throw it as argument for `runAlgorithm` 

# Till now I found out that...
- This is too much luck based game. Even if with a well designed algorithm, 3 turns of bad pieces in a row can ruin the field. Even Tetris has a rule of specific shapes and tiles coming in a order, however this does NOT have any patterns.
