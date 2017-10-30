sort: InitialConfig.o 
	g++ -std=c++11 -Wall -O3 testRunner.cpp evolvingList.cpp initialConfiguration.o -o Sort

InitialConfig.o:
	g++ -std=c++11 -Wall -O3 -c initialConfiguration.cpp

printConfig:
	g++ -std=c++11 -Wall -O3 initialConfiguration.cpp printConfigurations.cpp -o PrintConfigs
