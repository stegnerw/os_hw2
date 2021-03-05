#include<thread>
#include<pthread.h>
#include<iostream>
#include<ctime>

#define NUM_SMOKERS 3

void tobacco_smoker(){
	std::cout << "Smoker 1 combines the material with their tobacco" << std::endl;
	std::cout << "Smoker 1 rolls a cigarette and smokes it" << std::endl;

}

void paper_smoker(){
	std::cout << "Smoker 2 combines the material with their paper" << std::endl;
	std::cout << "Smoker 2 rolls a cigarette and smokes it" << std::endl;

}

void matches_smoker(){
	std::cout << "Smoker 3 combines the material with their matches" << std::endl;
	std::cout << "Smoker 3 rolls a cigarette and smokes it" << std::endl;

}

void agent(){
	std::cout << "The agent randomly places an ingredient on the table" << std::endl;
}


int main() {
	time_t seed = time(0); // define & initialize seed variable
    srand(seed);
	std::cout << "Begin cigarette smokers program." << std::endl << std::endl;
	
	std::thread tobacco_thread (tobacco_smoker);
	std::thread paper_thread (paper_smoker);
	std::thread matches_thread (matches_smoker);
	std::thread agent_thread (agent);
	std::cout << "Three smoker threads and the agent thread are initialized" << std::endl;

	int count = 0;
	int rand_num;
	while (count < 10) {
		
		agent_thread.join();
		rand_num = rand()%3 + 1;;
		switch (rand_num){
			case 1 :
				// Tobacco
				// Paper
				std::cout << "The agent places tobacco and paper on the table" << std::endl;
				matches_thread.join();
				std::cout << "Smoker 3 is finished with the cigarette" << std::endl;
				break;
			case 2 :
				// Paper
				// Matches
				std::cout << "The agent places matches and paper on the table" << std::endl;
				tobacco_thread.join();
				std::cout << "Smoker 1 is finished with the cigarette" << std::endl;
				break;
			case 3 :
				// Matches
				// Tobacco
				std::cout << "The agent places tobacco and matches on the table" << std::endl;
				paper_thread.join();
				std::cout << "Smoker 2 is finished with the cigarette" << std::endl;
				break;
		}
		count++;
	}

	return 0;
}