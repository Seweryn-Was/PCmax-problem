#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <PCMAX/timer.h>
#include "PCMAX/Population.h"
#include <thread>
#include <vector>
#include <map>
//#include <PCMAX/solution.h>

// m50n1000.txt 
//  average = 9762,10

int procesory(int liczba, std::vector<int> &procesy) {
    std::map<int,int> endTime;
    int i = procesy.size(), x = 0, max = INT_MIN;
    endTime[0] = liczba;
    while(x < i){
        int pom = endTime.begin()->first;
        for(int y = endTime.begin()->second; y > 0 && x < i; --y) {
            ++endTime[pom + procesy[x]];
            if((pom + procesy[x]) > max)
                max = pom + procesy[x];
            ++x;
        }
        endTime.erase(endTime.begin());
    }
    return max;
}


void log(std::string message) {
    std::cout << message << "\n"; 
}

void tworzenie_populacji(int populationSize, int durationTime, int cpuNum, const std::vector<int>& processes, const std::string& saveFile, Individual& bestIndividual) {
    Population popul = Population(populationSize, durationTime, cpuNum, processes, saveFile, bestIndividual);
}


int main(int argc, char* argv[]) {
    unsigned int total_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::string problemName = "m50n1000.txt";

    std::string problemInstanceFile = "./res/" + problemName; 
    std::string saveFile = "C:\\Dev\\" + problemName;
    std::vector <Individual> bestIndividuals;
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

    bestIndividuals.resize(total_threads);

    processes.resize(n);

    for (int i = 0; i < n; ++i) {
        inputFile >> processes[i];
    }

    inputFile.close();

    pcmax::Timer timer; 
    timer.start();
    for (unsigned int i = total_threads; i > 0; --i) {
        threads.push_back(std::thread(tworzenie_populacji, 100, 10, cpuNum, processes, saveFile, std::ref(bestIndividuals[i-1])));
    }
    for (auto& t : threads) {
        t.join();
    }

    Individual bestIndividual;
    bestIndividual.fitness_score = INT_MAX;
    for(auto x: bestIndividuals) {
        if(bestIndividual.fitness_score > x.fitness_score) {
            bestIndividual = x;
        }
    }

    //bestIndividual.print();
    //bestIndividual.saveData(saveFile);
    timer.stop();
    std::cout << problemName << "\n";
    std::cout << "Zachlanny: " << procesory(cpuNum, processes) << "\n";
    std::cout << "Genetyczny: " << bestIndividual.fitness_score << "\n";

    std::cout << "Time: " << timer.elapsedMilliseconds() << "[ms]\n";

    //std::cin.get();
    return 0;
}
