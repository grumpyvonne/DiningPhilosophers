# DiningPhilosophers
Dining Philosophers problem solution using mutexes.
5 processes are created, one for each philosopher. Processes share 5 variables. Each process can be in either of two states - eating or thinking. In order to start eating process needs to grab two variables. Once finished, process releases variables and starts thinking untill it needs to eat again.
