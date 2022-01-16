# Woodoku Algorithm in C++
### Solving Woodoku with C++ 
By previous Python version, I realized that using Numpy for Woodoku is painfully slow. Thus, I did some bitwise operations and other tricks to make everything faster with C++. Plus with some help from gcc optimization O3.
## Implemented Algorithms till now
 ### BruteForceStupid
 Literally brute forces every single possible cases that can be made by positioning 3 pieces (9^6 cases). Python `Woodoku_Algorithm.py` does the same thing, however in painfully slow time. Running the same process with the same block shapes and exact same conditions with the same possible cases, **C++ version runs in 0.171 sec at the same time Python version runs over 10.4 sec**. 
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
My personal Woodoku Record is around 2100 points. However, even my Python version brute force Woodoku algorithm can beat up to **6285 points**. C++ version is not perfect yet, however it can score up to **4062 points**. Both measured at 2022-01-17 and has enough room for improvements.

### Python Version Proof
![YeahItWorks1](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/YeahItWorks1.png)

### C++ Version Proof
![YeahItWorks1](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/YeahItWorks2.png)

