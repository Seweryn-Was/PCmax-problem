#pragma once
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <unordered_map>
#include <numeric>
#include <queue>

#include "Individual.h"

struct Process {
    int id;
    int time;
};

struct Cpu {
    int id;
    int end_time = 0;
};

class Population {
public:
    Population(int populationSize, int generationsNum, int cpuNum, const std::vector<int>& processes_times, const std::string& fileToSaveBestIndividual)
        :individuals(populationSize), cpuNum(cpuNum), generationsNum(generationsNum) {

        createProcessesVector(processes_times); 

        Individual::setCryteria(processes_times);
        Individual::cpuNum = cpuNum;

        std::random_device rd;
        generator = std::mt19937(rd());

        for (int i = 0; i < populationSize; ++i) {
            individuals[i] = createIndividualPermutation();
        }
        bestEver = individuals[0]; 
        printBestInPopulation(0); 


        std::cout << "<==Starting Algorithm==>\n"; 
        startGeneticAlgorithm();

        bestEver.print(); 

        bestEver.saveToFileIfGreater(fileToSaveBestIndividual);
    }

    std::vector<Individual> getIndividuals() {
        return individuals;
    }

private:
    std::vector<Individual> individuals;
    std::vector<Process> processes;
    int cpuNum;
    int generationsNum;
    Individual bestEver;

    std::mt19937 generator;

    void startGeneticAlgorithm() {
        for (int i = 0; i < generationsNum; ++i) {
            std::cout << "GENERATION: " << i << "\n";
            selection(SelectionType::Elitism);
            crossover();
            mutation(0.001);
            printBestInPopulation(i);
        }

        std::sort(individuals.begin(), individuals.end(), [](const Individual& a, const Individual& b) {
            return a.fitness() < b.fitness();
            });

        if (bestEver.fitness() > individuals[0].fitness()) {
            bestEver = individuals[0];
        }

    }

    void createProcessesVector(const std::vector<int>& processes_times) {
        processes.resize(processes_times.size());

        for (int i = 0; i < processes_times.size(); ++i) {
            processes[i].id = i;
            processes[i].time = processes_times[i];
        }
    }

    enum SelectionType
    {
        Roulette, 
        Tournament, 
        Rank, 
        Elitism

    };

    Individual createIndividualPermutation() {
        std::shuffle(processes.begin(), processes.end(), generator);
        std::vector<int> gen(processes.size());

        std::vector<Cpu> cpus;
        for (int i = 0; i < cpuNum; ++i) {
            cpus.push_back({ i, 0 });
        }

        for (Process p : processes) {
            Cpu c = cpus.back();
            cpus.pop_back();

            c.end_time += p.time;
            gen[p.id] = c.id;

            int i = cpus.size() - 1;
            for (; i >= 0 && cpus[i].end_time < c.end_time; --i) {}
            cpus.insert(cpus.begin() + (i + 1), c);
        }

        Individual temp(gen, cpus[0].end_time);
        return temp;
    }



    void selection(SelectionType type = SelectionType::Roulette) {
        switch (type){
        case Population::Roulette:
            rouletteSelection();
            break;
        case Population::Tournament:
            tournamentSelection();
            break;
        case Population::Rank:
            rankSelection();
            break;
        case Population::Elitism:
            elitismSelection(); 
            break;
        }
    }

    void crossover() {
        //std::shuffle(individuals.begin(), individuals.end(), generator);

        int crossover_num = individuals.size() % 2 == 0 ? individuals.size() : individuals.size() - 1;
        for (int i = 0; i < crossover_num; i += 2) {
            std::pair<Individual, Individual> childs = Individual::crossover(individuals[i], individuals[i + 1], generator);
            
            individuals.push_back(childs.first);
            individuals.push_back(childs.second);
        }
    }

    void mutation(float mutationRate = 0.0001) {
        for (int i = 0; i < individuals.size(); ++i) {
            individuals[i].mutate(mutationRate, generator);
        }
    }

    void printBestInPopulation(int genNum) {
        int cmax = individuals[0].fitness();
        //std::cout << "Print Population Cmax: \t";
        //std::cout << individuals[0].fitness()<<"[x] > ";
        int sum = 0; 
        
        for (int i = 1; i < individuals.size(); i++) {
            //std::cout << individuals[i].fitness();
            sum += individuals[i].fitness(); 
            if (cmax > individuals[i].fitness()) {
                cmax = individuals[i].fitness();
                //std::cout << "[x]";
            }
            //std::cout << " > ";
        }


        std::cout<< "\tgeneration size: " << individuals.size() << "\n\tavreage cmax:" << sum / individuals.size() << "\n\tbest cmax in generation:" << cmax<< "\n\tbest ever cmax:"<<bestEver.fitness() << "\n\n";
    }

    void rouletteSelection() {

    }

    void tournamentSelection(int tournamentSize = 3) {

    }

    void rankSelection(){
       
    }

    void elitismSelection() {
        int eliteCount = individuals.size() / 2;

        std::sort(individuals.begin(), individuals.end(), [](const Individual& a, const Individual& b) {
            return a.fitness() < b.fitness();
        });

        if (bestEver.fitness() > individuals[0].fitness()) {
            bestEver = individuals[0]; 
        }
        individuals.erase(individuals.begin() + eliteCount, individuals.end());
    }
};