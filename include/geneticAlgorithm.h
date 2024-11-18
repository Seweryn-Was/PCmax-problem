#include <vector>
#include <algorithm>

struct Chromosome{
	int index; 
	int value; 
};

class GeneticAlgorithm {
public:
	GeneticAlgorithm(int populationSize, int bestIndividualsNum, int randomIndividualsNum, int generationsNum) :
		populationSize(populationSize), bestIndividualsNum(bestIndividualsNum), randomIndividualsNum(randomIndividualsNum), generationsNum(generationsNum) {};
	
	void solve(int p, std::vector<int>) {
	}

protected:
	int populationSize;
	int bestIndividualsNum;
	int randomIndividualsNum;
	int generationsNum;

	
};

//class Genome {
//public:
//	void getFitness() {
//
//	}
//	void mutate() {
//
//	}
//	static void crossover(Genome* a, Genome* b) {
//
//	}
//private:
//	int fitness;
//	std::vector<int> data;
//
//};



class Individual {
public: 
	static void crossover(Individual* a, Individual* b) {

	}
	
	int fitness() {
		
	}

	void mutate(float muttationRate) {

	}
private:
	std::vector<int> genSequence;
	int fitness_score; 
	
};