#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <PCMAX/timer.h>
#include "PCMAX/Population.h"
//#include <PCMAX/solution.h>

// m50n1000.txt 
//  average = 9762,10

void log(std::string message) {
    std::cout << message << "\n"; 
}


int main(int argc, char* argv[]) {

    std::string problemName = "m50.txt"; 

    std::string problemInstanceFile = "./res/" + problemName; 
    std::string saveFile = "C:\\Dev\\College\\PCmax-problem\\ans\\" + problemName; 
       
    // Problem Instance Data  
    int n;                          // liczba proces�w
    int cpuNum;                     // liczba procesor�w
    std::vector<int> processes;     // czasy proces�w


    std::ifstream inputFile(problemInstanceFile);
    if (!inputFile) {
        std::cerr << "Error: could not open the file!" << std::endl;
        return 1;
    }

    inputFile >> cpuNum;
    inputFile >> n; 

    processes.resize(n);

    for (int i = 0; i < n; ++i) {
        inputFile >> processes[i];
    }

    inputFile.close();

    pcmax::Timer timer; 
    timer.start();
    Population popul = Population(1000, 100, cpuNum, processes, saveFile);
    timer.stop(); 

    std::cout << "Time: " << timer.elapsedMilliseconds() << "[ms]\n"; 

    std::cin.get(); 
    return 0;
}
