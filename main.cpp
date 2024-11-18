#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <PCMAX/timer.h>
//#include <PCMAX/solution.h>

void log(std::string message) {
    std::cout << message << "\n"; 
}

struct Process {
    int id; 
    int time; 
};

struct Cpu {
    int id;
    int end_time = 0;
};


class Individual {
public:
    Individual() = default; 

    static std::pair<Individual, Individual> crossover(Individual a, Individual b) {
        size_t mid = a.genSequence.size()/ 2;

        // Swap the second halves of the vectors
        for (size_t i = mid; i < a.genSequence.size(); ++i) {
            std::swap(a.genSequence[i], b.genSequence[i]);
        }
        a.score(); 
        b.score(); 

        return std::make_pair(a, b); 
    }

    void score() {
        std::unordered_map<int, int> cpus;

        int fitness = 0; 
        for (int i = genSequence.size() - 1; i >= 0; --i) {
            cpus[genSequence[i]] += fitnessCryteria[i]; 
        }

        for (auto c : cpus) {
            if (c.second > fitness) {
                fitness = c.second; 
            }
        }

        fitness_score = fitness;
    }

    int fitness() const {
        return fitness_score; 
    }

    void mutate(float muttationRate) {

    }

    void print() {
        std::cout << "Cmax:" << fitness_score<<"\t";
        for (int g : genSequence) {
            std::cout << g << ", ";
        }
        std::cout << "\n";
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
    int fitness_score = 0;
    
};

std::vector<int> Individual::fitnessCryteria;

class Population {
public:
    Population(int populationSize,int generationsNum,  int cpuNum, const std::vector<int>& processes_times)
        :individuals( populationSize%2 == 0 ? populationSize : populationSize + 1), cpuNum(cpuNum), generationsNum(generationsNum){

        processes.resize(processes_times.size());

        for (int i = 0; i < processes_times.size(); ++i) {
            processes[i].id = i; 
            processes[i].time = processes_times[i];
        }

        Individual::setCryteria(processes_times);

        std::random_device rd;
        generator.seed(rd());

        for (int i = 0; i < populationSize; ++i) {
            individuals[i] = createIndividualPermutation();
            //individuals[i].print(); 
        } 
        startGeneticAlgorithm();

        std::sort(individuals.begin(), individuals.end(), [](const Individual& a, const Individual& b) {
            return a.fitness() < b.fitness();
        });
        std::cout << "BEST Cmax:" << individuals[0].fitness() << "\n"; 
    }

    std::vector<Individual> getIndividuals() {
        return individuals; 
    }

private:
    std::vector<Individual> individuals;
    std::vector<Process> processes;
    int cpuNum;
    int generationsNum;

    std::mt19937 generator;

    Individual createIndividualPermutation() {
        std::shuffle(processes.begin(), processes.end(), generator);
        std::vector<int> gen(processes.size());

        std::vector<Cpu> cpus;
        for (int i = 0; i < cpuNum; ++i) {
            cpus.push_back({ i });
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

    void startGeneticAlgorithm() {
        for (int i = 0; i < generationsNum; ++i) {
            selection(); 
            crossover(); 
            mutation(); 
        }
    }

    void selection() {
        //add to selected Individual for crossover / mutation 
        std::sort(individuals.begin(), individuals.end(), [](const Individual& a, const Individual& b) {
            return a.fitness() < b.fitness();
        });

        individuals.erase(individuals.begin() + (int)(individuals.size()/2), individuals.end());
    }

    void crossover() {
        std::shuffle(individuals.begin(), individuals.end(), generator);

        int crossover_num = individuals.size() % 2 == 0 ? individuals.size() : individuals.size()-1;
        for (int i = 0; i < crossover_num; i += 2) {
            std::pair<Individual, Individual> childs = Individual::crossover(individuals[i], individuals[i + 1]);
            individuals.push_back(childs.first); 
            individuals.push_back(childs.second); 
        }
    }

    void mutation() {
        //to do
    }




};


int main() {
    //system("cls");
    //pcmax::PCmax solution; 

    //solution.getProblemInstancesFromDirectory("./res"); 
    //solution.run(); // run problem solution for provided problem insatnces 

    int cpuNum = 3; 

    std::vector<int> processes; 
    //processes.push_back(3); 
    //processes.push_back(1);
    //processes.push_back(5);
    //processes.push_back(4);
    //processes.push_back(6);
    //processes.push_back(1);
    //processes.push_back(2);
    //processes.push_back(1);

    std::ifstream inputFile("./res/m50n1000.txt");
    if (!inputFile) {
        std::cerr << "Error: could not open the file!" << std::endl;
        return 1;
    }

    inputFile >> cpuNum; //liczba procesoró 

    int n; 
    inputFile >> n; //liczba procesów zadañ

    processes.resize(n);

    for (int i = 0; i < n; ++i) {
        inputFile >> processes[i];
    }


    inputFile.close();

    pcmax::Timer timer; 
    timer.start();
    Population popul = Population(1000, 20, cpuNum, processes);
    timer.stop(); 

    std::cout << "Time: " << timer.elapsedMilliseconds() << "[ms]\n"; 

    std::cin.get(); 
    return 0;
}
