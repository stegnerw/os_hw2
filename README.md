# Homework 2 - Operating Systems
Homework 2 for Operating Systems class.
In this assignment, we have to implement 2 algorithms:
- [The Sleeping Barber Problem](https://en.wikipedia.org/wiki/Sleeping_barber_problem)
- [The Cigarette Smokers Problem](https://en.wikipedia.org/wiki/Cigarette_smokers_problem)

We implemented the homework files in c++. Both problems utilize semaphores, mutexes and threads. 

## Sleeping Barber

## Cigarette Smoker
4 threads are used (3 smokers and 1 agent). The threads lock shared resources like the ingredients (paper, matches, tobacco) and the table. Each smoker thread signals to the agent once they have finished smoking their cigarette, so that the agent can place two new random ingredients. 

Although the table mutex isn't needed for the problem, we included one in case the problem scope ever changes (EX/ the table needs to be locked for multiple agents). 

## Usage
You can run both files by using our make file, which attaches the necessary args to the command line to run the pthreads. 

## Contributions
Wayne worked on the Sleeping Barber problem, created the make file, and initialized our shared repository. Bayley worked on the Cigarette Smoker Problem.
Both students went through code review of each others work, and discussed implementation with each other throughout the process.

## Contact info
- Wayne Stegner - stegnewe@mail.uc.edu
- Bayley King - king2b3@mail.uc.edu
