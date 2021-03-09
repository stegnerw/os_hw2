# Homework 2 - Operating Systems
Homework 2 for Operating Systems class.
In this assignment, we have to implement 2 algorithms:
- [The Sleeping Barber Problem](https://en.wikipedia.org/wiki/Sleeping_barber_problem)
- [The Cigarette Smokers Problem](https://en.wikipedia.org/wiki/Cigarette_smokers_problem)

We implemented the homework files in c++. Both problems utilize semaphores, mutexes and threads.

## Sleeping Barber
The barber runs on a dedicated thread, and then customers are dispatched randomly between 250 and 500 ms apart.
We dispatch 20 customers, but technically it can run indefinitely.
There are 2 semaphores: one to signal the barber to wake up (this also keeps track of the number of waiting customers for the barber), and one to signal to the customers that the barber is available.
There is a mutex used to ensure mutual exclusion when accessing the number of available waiting chairs, as well as for tracking the total number of customers.
The total number of customers is tracked so the barber knows when to leave.

In this problem and the next problem, there is an additional mutex called `print_mutex`.
This mutex is used to ensure that messages are printed to the terminal without other threads interrupting them, as some issues with `std::cout` messages being broken up were occurring.

## Cigarette Smoker
4 threads are used (3 smokers and 1 agent). The threads lock shared resources like the ingredients (paper, matches, tobacco) and the table.
The agent places resources and signals the corresponding smoker using a semaphore.
Each smoker thread signals to the agent once they have finished smoking their cigarette using a semaphore, so that the agent can place two new random ingredients.

Although the table mutex isn't needed for the problem, we included one in case the problem scope ever changes (EX/ the table needs to be locked for multiple agents).

## Usage
You can run both files by using our `makefile`, which attaches the necessary args to the command line to utilize the `<semaphore.h>` header.
Just run `make` from the base directory.
If you cannot run the makefile, you can just use the `g++` arguments found in the `makefile`

## Contributions
Wayne worked on the Sleeping Barber problem, created the make file, and initialized our shared repository.
Bayley worked on the Cigarette Smoker Problem.
Both students went through code review of each others work, and discussed implementation with each other throughout the process.

## Contact info
- Wayne Stegner - stegnewe@mail.uc.edu
- Bayley King - king2b3@mail.uc.edu
