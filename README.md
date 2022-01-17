# Woodoku Algorithm
Woodoku is a mobile game that is kinda like Tetris. You can move blocks into specific places and clear out rows, columns, and blocks of 3x3. It's very addictive and I was playing that game for a long time. So, I decided to make an algorithm that solves Woodoku game itself. **It will NOT be perfect, however I will try.** 
- [IOS Appstore](https://apps.apple.com/us/app/woodoku/id1496354836) 
- [Android Google Play](https://play.google.com/store/apps/details?id=com.tripledot.woodoku&hl=ko&gl=US) 


## Versions
Currently there are two versions of this product.
- Python : Using Numpy for ease of use. **Slow but code is easy to understand**.
- C++ (using C functions but using class from C++) : Using old style bit operations for optimization and performance. **Fast but code is difficult to understand without knowledge of bit operations**

With the same brute forcing technique, **C++ version runs in 0.171 sec at the same time Python version runs over 10.4 sec**. C++ is recommended for fast results and optimization. 


## Does it work?
### Its works Okay.
My personal Woodoku Record is around 2100 points. However, even my Python version brute force Woodoku algorithm can beat up to **6285 points**. C++ version is not perfect yet, however it can score up to **4062 points**. Both measured at 2022-01-17 and has enough room for improvements.

### Python Version Proof
![YeahItWorks1](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/YeahItWorks1.png)

### C++ Version Proof
![YeahItWorks1](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/YeahItWorks2.png)

## Implemented Algorithms 
Currently implemented algorithms are like following:
- **Brute Force**: Generate every single possible cases and look for the best case scenario.

## Types of Woodoku Blocks
### There are total 44 of Wodoku Blocks avaiable.
![slide1](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/Slide1.PNG)
![slide2](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/Slide2.PNG)
![slide3](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/Slide3.PNG)
![slide4](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/Slide4.PNG)
![slide5](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/Slide5.PNG)
![slide6](https://github.com/gooday2die/WoodokuAlgorithm/raw/main/picture/Slide6.PNG)

