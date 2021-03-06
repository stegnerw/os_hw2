#include <thread>
#include <pthread.h>
#include <iostream>
#include <ctime>
#include <mutex>

// global variables
//#define NUM_SMOKERS 3
std::mutex paper;
std::mutex tobacco;
std::mutex matches;
std::mutex done_smoking;

// function declerations
void tobacco_smoker(int &);
void paper_smoker(int &);
void matches_smoker(int &);
void agent(int &);


void tobacco_smoker(int &produced_material){
	if (produced_material == 2){
		done_smoking.lock();
		paper.lock();
		matches.lock();
		std::cout << "Smoker 1 combines the material with their tobacco" << std::endl;
		std::cout << "Smoker 1 rolls a cigarette and smokes it" << std::endl;
		paper.unlock();
		matches.unlock();
		done_smoking.unlock();
	}
}

void paper_smoker(int &produced_material){
	if (produced_material == 3){
		done_smoking.lock();
		tobacco.lock();
		matches.lock();
		std::cout << "Smoker 2 combines the material with their paper" << std::endl;
		std::cout << "Smoker 2 rolls a cigarette and smokes it" << std::endl;
		tobacco.unlock();
		matches.unlock();
		done_smoking.unlock();
	}
}

void matches_smoker(int &produced_material){
	if (produced_material == 1){
		done_smoking.lock();
		paper.lock();
		tobacco.lock();
		std::cout << "Smoker 3 combines the material with their matches" << std::endl;
		std::cout << "Smoker 3 rolls a cigarette and smokes it" << std::endl;
		paper.unlock();
		tobacco.unlock();
		done_smoking.unlock();
	}
}

void agent(int &produced_material){
	std::cout << "The agent randomly places an ingredient on the table" << std::endl;
	done_smoking.lock();
	produced_material = rand()%3 + 1;
	switch (produced_material){
		case 1 :
			std::cout << "The agent places tobacco and paper on the table" << std::endl;
			done_smoking.unlock();
			std::cout << "Smoker 3 is finished with the cigarette" << std::endl;
			break;
		case 2 :
			std::cout << "The agent places matches and paper on the table" << std::endl;
			done_smoking.unlock();
			std::cout << "Smoker 1 is finished with the cigarette" << std::endl;
			break;
		case 3 :
			std::cout << "The agent places tobacco and matches on the table" << std::endl;
			done_smoking.unlock();
			std::cout << "Smoker 2 is finished with the cigarette" << std::endl;
			break;
	}
}


int main() {
	time_t seed = time(0); // define & initialize seed variable
    srand(seed);
	int produced_material;
	std::cout << "Begin cigarette smokers program." << std::endl << std::endl;
	
	std::thread tobacco_thread (tobacco_smoker, &produced_material);
	std::thread paper_thread (paper_smoker, &produced_material);
	std::thread matches_thread (matches_smoker, &produced_material);
	std::thread agent_thread (agent, &produced_material);
	std::cout << "Three smoker threads and the agent thread are initialized" << std::endl;

	tobacco_thread.join();
	paper_thread.join();
	matches_thread.join();
	agent_thread.join();

	return 0;
}