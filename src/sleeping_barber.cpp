#include <unistd.h>
#include <semaphore.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <cstdlib>
#include <chrono>

// Constants
const int NUM_WAIT_CHAIRS = 3;
const int NUM_CUSTOMERS = 20; // Total number of customer threads to schedule
const int HAIRCUT_TIME = 1000; // ms

// Globals
int open_waiting_chairs = NUM_WAIT_CHAIRS; // All the chairs start open
int num_customers = 0; // Used to let the barber know when to exit the thread
auto start_time = std::chrono::system_clock::now();

// Mutex
sem_t waiting_customers_sem; // Signal to the barber when people are waiting
sem_t barber_sem; // Signal to customers when the barber is available
std::mutex chairs_mutex; // Protect the number of chairs
std::mutex num_customers_mutex; // Protect the total customers counter
std::mutex print_mutex; // Prevent cout messages from overlapping

// Forward declarations
void customer(const int&);
void barber();
void log(std::stringstream&); // Thread safe print function with timestamp

int main() {
	std::stringstream ss;
	ss << "Begin sleeping barber program.";
	log(ss);

	// Seed PRNG
	srand(0);
	
	// Initialize semaphores
	sem_init(&waiting_customers_sem, 0, 0);
	sem_init(&barber_sem, 0, 0);

	// Start barber thread
	std::thread barber_thread(barber);

	// Send customers at random times
	std::thread customer_threads[NUM_CUSTOMERS];
	for (int i = 0; i < NUM_CUSTOMERS; i++) {

		// Wait for 250-500 ms
		int wait_time = (rand() % 250) + 250;
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
		customer_threads[i] = std::thread(customer, (i+1));
	}

	// Wait for all customers to leave
	for (int i = 0; i < NUM_CUSTOMERS; i++) {
		customer_threads[i].join();
	}

	// Wait for barber to leave
	barber_thread.join();
	return 0;
}

void customer(const int& customer_id) {
	std::stringstream ss;
	ss << "Customer " << customer_id << " arriving.";
	log(ss);

	// Lock waiting chairs resources to check for open waiting chairs
	chairs_mutex.lock();
	if (open_waiting_chairs > 0) {

		// Grab a chair
		open_waiting_chairs--;

		// Let the barber know you are here
		sem_post(&waiting_customers_sem);

		// Done with chairs critical section
		chairs_mutex.unlock();

		// Chill until the barber is ready
		sem_wait(&barber_sem);

		// Hair is getting cut
		ss << "Customer " << customer_id << " getting hair cut.";
		log(ss);
	} else {

		// No chairs available so unlock and leave
		chairs_mutex.unlock();
		ss << "Customer " << customer_id << " no waiting chairs.";
		log(ss);

		// Increment number of customers because the barber will not see them
		num_customers_mutex.lock();
		num_customers++;
		num_customers_mutex.unlock();
	}
}

void barber() {
	std::stringstream ss;
	ss << "Barber arriving.";
	log(ss);
	num_customers_mutex.lock();
	while(num_customers < NUM_CUSTOMERS) {
		num_customers_mutex.unlock();

		// Tell customers you are ready
		sem_post(&barber_sem);

		// Sleeping until customers arrive
		sem_wait(&waiting_customers_sem);

		// Take customer from waiting chairs to barber chair
		chairs_mutex.lock();
		open_waiting_chairs++;
		chairs_mutex.unlock();

		// Cutting hair
		ss << "Cutting hair...";
		log(ss);
		std::this_thread::sleep_for(std::chrono::milliseconds(HAIRCUT_TIME));

		// Increment customer count
		num_customers_mutex.lock();
		num_customers++;
	}
	num_customers_mutex.unlock();
	ss << "Barber leaving.";
	log(ss);
}

void log(std::stringstream& ss) {
	print_mutex.lock();
	auto curr_time = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = curr_time - start_time;
	std::cout << elapsed_time.count() << "s:\t" << ss.str() << std::endl;
	ss.str(std::string());
	print_mutex.unlock();
}

