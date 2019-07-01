FLAGS := -std=c++17 -Wall -Wpedantic -Wextra -O3

all: Sort

Sort: testRunner.cpp evolvingList.cpp initialConfiguration.o 
	g++ $(FLAGS) testRunner.cpp evolvingList.cpp initialConfiguration.o -o Sort

initialConfiguration.o: initialConfiguration.cpp
	g++ $(FLAGS) -c initialConfiguration.cpp

PrintConfig: initialConfiguration.cpp printConfigurations.cpp
	g++ $(FLAGS) initialConfiguration.cpp printConfigurations.cpp -o PrintConfigs

clean:
	rm -f Sort initialConfiguration.o PrintConfigs
