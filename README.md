# PHILOSOPHERS

My 8th project at 42 Network's Hive Helsinki 🐝

Simulating the Dining Philosophers problem with multi-threading and mutexes!

> [!TIP]
> If you're at a 42 school and doing this project: It's genuinely so much better to ask fellow students instead of reading online solutions ✨

---

## Description

This project's about simulating the [Dining Philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem), by creating a thread for each philosopher and synchronizing their forks as mutexes.

According to the assignment:
- A philosopher should:
	- Always either think, eat, or sleep
	- Start sleeping after eating
	- Start thinking after sleeping
- Philosophers should not communicate
- The simulation ends when a philosopher dies of starvation

The program requires the following parameters:
- `number_of_philosophers`: Amount of philosophers (and an equal amount of forks)
- `time_to_die`: Milliseconds (from start of simulation or start of latest meal) until starvation
- `time to eat`: Milliseconds it takes to eat
- `time to sleep`: Milliseconds it takes to sleep
- `number_of_times_each_philosopher_must_eat` (Optional): The simulation ends once every philosopher eats this much  

<sub>(The program is not meant to handle more than 200 philosophers, or less than 60ms `time_to_*` values)</sub>

Noteworthy cases:
- A single philosopher should always end up dying, since it can't get a second forks
- An even number of philosophers need 2*`time_to_eat` (plus a proportionate few millisecs of buffer time) to survive indefinitely - the even and odd philosophers alternate in eating cycles
- An odd number of philosophers need 3*`time_to_eat` (plus the buffer time) to survive indefinitely - the last philosopher needs its own time to eat, as the philosopher before it (last - 1) and after it (1st philo) already occupy two eating cycles

---

## Usage

> [!NOTE]
> Code was written and tested for Linux (using Hive's Ubuntu iMacs)

- Run `make` (or even `make color`!)
- Run the program as `./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep]`, optionally also adding a `[number_of_times_each_philosopher_must_eat]`

> [!IMPORTANT]
> The program relies on having system resources available in a timely manner. Having other programs open (or valgrind) can delay this program's threads and cause untimely deaths

---

## Credits

- [Eve Keinan](https://github.com/EvAvKein)
