#pragma once
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <unordered_map>
#include <queue>

class Individual {
public:
    Individual() = default;

    static std::pair<Individual, Individual> crossover(Individual a, Individual b, std::mt19937& generator) {
        size_t size = a.genSequence.size(); // Number of processes
        Individual offspring1 = a;          // Start with a copy of the parents
        Individual offspring2 = b;

        // Dynamic state of CPU times for both offspring
        std::vector<int> cpuTimes1(a.cpuNum, 0);
        std::vector<int> cpuTimes2(b.cpuNum, 0);

        // Initialize CPU times for offspring from parents
        for (size_t i = 0; i < size; ++i) {
            cpuTimes1[offspring1.genSequence[i]] += Individual::fitnessCryteria[i];
            cpuTimes2[offspring2.genSequence[i]] += Individual::fitnessCryteria[i];
        }

        // Priority queues to maintain CPU load for efficient rebalancing
        std::priority_queue<std::pair<int, int>> cpuLoad1, cpuLoad2;
        for (int cpu = 0; cpu < a.cpuNum; ++cpu) {
            cpuLoad1.emplace(cpuTimes1[cpu], cpu);
            cpuLoad2.emplace(cpuTimes2[cpu], cpu);
        }

        for (size_t i = 0; i < size; ++i) {
            int process = i;
            int taskTime = Individual::fitnessCryteria[process];

            int currentCpu1 = offspring1.genSequence[process];
            int currentCpu2 = offspring2.genSequence[process];

            int bestCpu1 = currentCpu1, bestCpu2 = currentCpu2;
            int bestCmax1 = *std::max_element(cpuTimes1.begin(), cpuTimes1.end());
            int bestCmax2 = *std::max_element(cpuTimes2.begin(), cpuTimes2.end());

            for (int cpu = 0; cpu < a.cpuNum; ++cpu) {
                cpuTimes1[currentCpu1] -= taskTime;
                cpuTimes1[cpu] += taskTime;

                int newCmax1 = *std::max_element(cpuTimes1.begin(), cpuTimes1.end());
                if (newCmax1 < bestCmax1) {
                    bestCmax1 = newCmax1;
                    bestCpu1 = cpu;
                }

                cpuTimes1[currentCpu1] += taskTime;
                cpuTimes1[cpu] -= taskTime;

                cpuTimes2[currentCpu2] -= taskTime;
                cpuTimes2[cpu] += taskTime;

                int newCmax2 = *std::max_element(cpuTimes2.begin(), cpuTimes2.end());
                if (newCmax2 < bestCmax2) {
                    bestCmax2 = newCmax2;
                    bestCpu2 = cpu;
                }

                cpuTimes2[currentCpu2] += taskTime;
                cpuTimes2[cpu] -= taskTime;
            }

            offspring1.genSequence[process] = bestCpu1;
            cpuTimes1[currentCpu1] -= taskTime;
            cpuTimes1[bestCpu1] += taskTime;

            offspring2.genSequence[process] = bestCpu2;
            cpuTimes2[currentCpu2] -= taskTime;
            cpuTimes2[bestCpu2] += taskTime;
        }

        offspring1.score();
        offspring2.score();

        return { offspring1, offspring2 };
    }

    std::unordered_map<int, int> getCpuTimes() {
        return cpuTimes;
    }

    int calculateContribution(int process, int cpu1, int cpu2) {
        int oldcpu1 = cpuTimes[cpu1], oldcpu2 = cpuTimes[cpu2];

        cpuTimes[cpu1] -= Individual::fitnessCryteria[process]; 
        cpuTimes[cpu2] += Individual::fitnessCryteria[process];

        int newCmax = 0;
        for (const auto& entry : cpuTimes) {
            newCmax = std::max(newCmax, entry.second);
        }

        cpuTimes[cpu1] = oldcpu1; 
        cpuTimes[cpu2] = oldcpu2; 

        return newCmax; 

    }

    void score() {

        cpuTimes = {};

        int fitness = 0;
        for (int i = genSequence.size() - 1; i >= 0; --i) {
            cpuTimes[genSequence[i]] += fitnessCryteria[i];
        }
        int n = 0;
        for (auto c : cpuTimes) {
            if (c.second > fitness) {
                fitness = c.second;
            }
        }

        fitness_score = fitness;
    }

    int fitness() const {
        return fitness_score;
    }

    void mutate(float mutationRate, std::mt19937& generator) {
        std::uniform_real_distribution<> dist(0.0, 1.0);
        std::uniform_int_distribution<> geneDist(0, genSequence.size() - 1);
        //std::cout << "\nINDIVIDUAL:\n";

        for (size_t i = 0; i < genSequence.size(); ++i) {
            float t = dist(generator); 
            if ( t < mutationRate) {
                size_t swapIndex = geneDist(generator); 
                std::swap(genSequence[i], genSequence[swapIndex]);
                //std::cout <<t<< " MUTATION";
            }
        }
        score(); 
    }

    void print() {
        std::cout << "Cmax:" << fitness_score << "\n";
        std::unordered_map<int, int> cpus;

        int fitness = 0;
        for (int i = genSequence.size() - 1; i >= 0; --i) {
            cpus[genSequence[i]] += Individual::fitnessCryteria[i];
        }
        int n = 0;
        for (auto c : cpus) {
            std::cout << "cpu: " << n << " " << c.second << "\n";
            if (c.second > fitness) {
                fitness = c.second;
            }
            n++; 
        }

        fitness_score = fitness;
        std::cout << "Cmax:" << fitness_score << "\n";
    }



    void saveToFileIfGreater(const std::string& filename) {

        std::ifstream infile(filename);
        if (infile.good()) {
            std::ifstream infile(filename);

            if (!infile) {
                std::cerr << "Error: Could not open the file!" << std::endl;
                return;
            }

            int firstNumber;
            bool isFileEmpty = infile.peek() == std::ifstream::traits_type::eof();

            if (!isFileEmpty) {
                infile >> firstNumber;
            }

            infile.close();

            if (isFileEmpty || firstNumber > fitness_score) {
                saveData(filename);
            }
            else {
                std::cout << "Cmax is greater than the value. File remains unchanged." << std::endl;
            }
        }
        else {
            saveData(filename); 
        }
        

    }

    void saveData(std::string filename) {
        std::ofstream outfile(filename);

        if (!outfile) {
            std::cerr << "Error: Could not open the file for writing!" << std::endl;
            return;
        }

        outfile << fitness_score <<"\n";

        for (int i = 0; i < cpuNum; i++) {
            for (int j = 0; j < genSequence.size(); j++) {
                if (genSequence[j] == i) {
                    outfile << fitnessCryteria[j] << " ";
                }
            }
            outfile << "\n";
        }


        outfile.close();
        std::cout << "File has been overwritten with new data." << std::endl;
    }
    

    friend class Population;
private:
    Individual(std::vector<int> genSequence, int fitness_score)
        : genSequence(genSequence), fitness_score(fitness_score) {}

    static void setCryteria(std::vector<int> cryteria) {
        fitnessCryteria = cryteria;
    }

    std::vector<int> genSequence;
    static std::vector<int> fitnessCryteria;
    std::unordered_map<int, int> cpuTimes;
    static int cpuNum;
    int fitness_score = 0;

};

std::vector<int> Individual::fitnessCryteria;
int Individual::cpuNum;



//std::uniform_int_distribution<> dist(1, a.genSequence.size() - 1);
//size_t crossoverPoint = dist(generator);

//for (size_t i = crossoverPoint; i < a.genSequence.size(); ++i) {
//    std::swap(a.genSequence[i], b.genSequence[i]);
//}
//a.score();
//b.score();
/*size_t size = a.genSequence.size();
Individual offspring1 = a;
Individual offspring2 = b;

for (size_t i = 0; i < size; ++i) {
    int process = i;

    int cpu1 = a.genSequence[process];
    int cpu2 = b.genSequence[process];

    int contribution1 = a.calculateContribution(process, cpu1, cpu2);
    int contribution2 = b.calculateContribution(process, cpu2, cpu1);

    if (contribution1 > contribution2) {
        offspring1.genSequence[process] = cpu1;
        offspring2.genSequence[process] = cpu2;
    }
    else {
        offspring1.genSequence[process] = cpu2;
        offspring2.genSequence[process] = cpu1;
    }
}

offspring1.score();
offspring2.score();

return { offspring1, offspring2 };*/