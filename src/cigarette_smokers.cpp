#include <thread>
#include <pthread.h>
#include <iostream>
#include <ctime>
#include <mutex>
#include <chrono>

// global variables
//#define NUM_SMOKERS 3
std::mutex paper;
std::mutex tobacco;
std::mutex matches;
std::mutex done_smoking;
int produced_material = 0;

// function declerations
void tobacco_smoker();
void paper_smoker();
void matches_smoker();
void agent();

// consumer functions
void tobacco_smoker(){
	while (true){
		if (produced_material == 2){
			// lock two resources on the table
			paper.lock();
			matches.lock();
			// locks signal to the agent
			done_smoking.lock();
			std::cout << "Smoker 1 combines the material with their tobacco" << std::endl;
			std::cout << "Smoker 1 rolls a cigarette and smokes it" << std::endl;
			// clears the table
			paper.unlock();
			matches.unlock();
			// signals to agent
			done_smoking.unlock();
			std::cout << "Smoker 1 is finished with the cigarette" << std::endl;
			produced_material = 0;
		}
	}
}

void paper_smoker(){
	while (true){
		if (produced_material == 3){
			// locks signal to the agent
			done_smoking.lock();
			// lock two resources on the table
			tobacco.lock();
			matches.lock();
			std::cout << "Smoker 2 combines the material with their paper" << std::endl;
			std::cout << "Smoker 2 rolls a cigarette and smokes it" << std::endl;
			// clears the table
			tobacco.unlock();
			matches.unlock();
			// signals to agent
			done_smoking.unlock();
			std::cout << "Smoker 2 is finished with the cigarette" << std::endl;
			produced_material = 0;
		}
	}
}

void matches_smoker(){
	while (true){
		if (produced_material == 1){
			// locks signal to the agent
			done_smoking.lock();
			// lock two resources on the table
			paper.lock();
			tobacco.lock();
			std::cout << "Smoker 3 combines the material with their matches" << std::endl;
			std::cout << "Smoker 3 rolls a cigarette and smokes it" << std::endl;
			// clears the table
			paper.unlock();
			tobacco.unlock();
			// signals to agent
			done_smoking.unlock();
			std::cout << "Smoker 3 is finished with the cigarette" << std::endl;
			produced_material = 0;
		}
	}
}

// producer functions
void agent(){
	while (true){
		// locks table
		done_smoking.lock();
		// generates a radnom number between 1-3
		produced_material = rand()%3 + 1;
		switch (produced_material){
			case 1 : // tobacco and paper are generated 
				std::cout << "The agent places tobacco and paper on the table" << std::endl;
				done_smoking.unlock();
				break;
			case 2 : // matches and paper are generated 
				std::cout << "The agent places matches and paper on the table" << std::endl;
				done_smoking.unlock();
				break;
			case 3 : // tobacco and matches are generated 
				std::cout << "The agent places tobacco and matches on the table" << std::endl;
				done_smoking.unlock();
				break;
		}
		// sleeps the agent for 1 second
		std::this_thread::sleep_for (std::chrono::seconds(1));
	}
}


int main() {
	// seeding the random number generator
	time_t seed = time(0);
	srand(seed);
	std::cout << "Begin cigarette smokers program." << std::endl << std::endl;
	
	// spawn new threads
	std::thread tobacco_thread (tobacco_smoker);
	std::thread paper_thread (paper_smoker);
	std::thread matches_thread (matches_smoker);
	std::thread agent_thread (agent);
	std::cout << "Three smoker threads and the agent thread are initialized" << std::endl;

	// threads synchronized
	tobacco_thread.join();
	paper_thread.join();
	matches_thread.join();
	agent_thread.join();

	return 0;
}