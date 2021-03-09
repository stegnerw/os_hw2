#include <unistd.h>
#include <semaphore.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <cstdlib>
#include <chrono>

// Constants
const int SMOKE_TIME = 500; // ms
const int ROLL_TIME = 250; // ms

// Globals
auto start_time = std::chrono::system_clock::now();
// Amount of supplies on the table
int tobacco = 0;
int paper = 0;
int matches = 0;

// Mutex
sem_t agent_sem; // Signal the agent upon smoking completion
sem_t paper_smoker_sem; // Signal the paper smoker to smoke
sem_t tobacco_smoker_sem; // Signal the tobacco smoker to smoke
sem_t matches_smoker_sem; // Signal the matches smoker to smoke
std::mutex table_mutex; // Protect the table
std::mutex print_mutex; // Prevent cout messages from overlapping

// Forward declarations
void tobacco_smoker();
void paper_smoker();
void matches_smoker();
void agent();
void log(std::stringstream&);

int main() {
	std::stringstream ss;
	// seeding the random number generator
	time_t seed = time(0);
	srand(seed);
	ss << "Begin cigarette smokers program.";
	log(ss);
	
	// Spawn new threads
	std::thread tobacco_thread(tobacco_smoker);
	std::thread paper_thread(paper_smoker);
	std::thread matches_thread(matches_smoker);
	std::thread agent_thread(agent);

	// Threads synchronized
	tobacco_thread.join();
	paper_thread.join();
	matches_thread.join();
	agent_thread.join();

	// Exit
	return 0;
}

// consumer functions
void tobacco_smoker() {
	std::stringstream ss;
	while (true) {

		// Wait for agent to place paper and matches
		sem_wait(&tobacco_smoker_sem);

		// Grab supplies from table
		table_mutex.lock();
		if ((paper > 0) && (matches > 0)) {
			paper--;
			matches--;
		} else { // This should never happen
			ss << "DEADLOCK - Wrong materials are present";
			log(ss);
		}
		table_mutex.unlock();
		ss << "Tobacco smoker rolls his cigarette...";
		log(ss);
		std::this_thread::sleep_for (std::chrono::milliseconds(ROLL_TIME));

		// locks signal to the agent
		ss << "Tobacco smoker is smoking...";
		log(ss);
		std::this_thread::sleep_for (std::chrono::milliseconds(SMOKE_TIME));
		ss << "Tobacco smoker is done.";
		log(ss);

		// Signal the agent we are done
		sem_post(&agent_sem);
	}
}

void paper_smoker() {
	std::stringstream ss;
	while (true) {

		// Wait for agent to place tobacco and matches
		sem_wait(&paper_smoker_sem);

		// Grab supplies from table
		table_mutex.lock();
		if ((tobacco > 0) && (matches > 0)) {
			tobacco--;
			matches--;
		} else { // This should never happen
			ss << "DEADLOCK - Wrong materials are present";
			log(ss);
		}
		table_mutex.unlock();
		ss << "Paper smoker rolls his cigarette...";
		log(ss);
		std::this_thread::sleep_for (std::chrono::milliseconds(ROLL_TIME));


		// locks signal to the agent
		ss << "Paper smoker is smoking...";
		log(ss);
		std::this_thread::sleep_for (std::chrono::milliseconds(SMOKE_TIME));
		ss << "Paper smoker is done.";
		log(ss);

		// Signal the agent we are done
		sem_post(&agent_sem);
	}
}

void matches_smoker() {
	std::stringstream ss;
	while (true) {

		// Wait for agent to place tobacco and paper
		sem_wait(&matches_smoker_sem);

		// Grab supplies from table
		table_mutex.lock();
		if ((tobacco > 0) && (paper > 0)) {
			tobacco--;
			paper--;
		} else { // This should never happen
			ss << "DEADLOCK - Wrong materials are present";
			log(ss);
		}
		table_mutex.unlock();
		ss << "Matches smoker rolls his cigarette...";
		log(ss);
		std::this_thread::sleep_for (std::chrono::milliseconds(ROLL_TIME));

		// locks signal to the agent
		ss << "Matches smoker is smoking...";
		log(ss);
		std::this_thread::sleep_for (std::chrono::milliseconds(SMOKE_TIME));
		ss << "Matches smoker is done.";
		log(ss);

		// Signal the agent we are done
		sem_post(&agent_sem);
	}
}

// producer functions
void agent(){
	std::stringstream ss;
	while (true){

		// Produce random material
		int produced_material = rand() % 3;
		switch (produced_material){
			case 0: { // tobacco and paper are generated
				tobacco++;
				paper++;
				ss << "The agent places tobacco and paper on the table";
				log(ss);
				// Signal the appropriate smoker
				sem_post(&matches_smoker_sem);
				break;
			}
			case 1: { // matches and paper are generated
				matches++;
				paper++;
				ss << "The agent places matches and paper on the table";
				log(ss);
				sem_post(&tobacco_smoker_sem);
				break;
			}
			case 2: { // tobacco and matches are generated
				tobacco++;
				matches++;
				ss << "The agent places tobacco and matches on the table";
				log(ss);
				sem_post(&paper_smoker_sem);
				break;
			}
		}

		// Wait for smokers to finish giving themselves cancer
		sem_wait(&agent_sem);

		// Make sure the table is empty
		table_mutex.lock();
		if ((tobacco == 0) && (matches == 0) && (paper == 0)) {
			ss << "Table is empty.";
			log(ss);
		} else { // This should never happen
			ss << "DEADLOCK - Table is not empty";
			log(ss);
			std::terminate();
		}
		table_mutex.unlock();
	}
}

void log(std::stringstream& ss) {
	print_mutex.lock();
	auto curr_time = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = curr_time - start_time;
	std::cout << elapsed_time.count() << "s:\t" << ss.str() << std::endl;
	ss.str(std::string());
	print_mutex.unlock();
}

