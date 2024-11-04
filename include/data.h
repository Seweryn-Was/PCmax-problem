#include <iostream>
#include <fstream>
#include <random>
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem> 

class Data{
public:
    int n; //number of processes/tasks
    int m; // number of procesors
    std::vector<int> processes; //processes time 

    Data(std::string fileName){
        this->readFromFile(fileName);
    }
    
    Data(){}; 

    ~Data(){}

    int readFromFile(std::string fileName){
        std::ifstream inputFile(fileName);
        if (!inputFile) {
            std::cerr << "Error: could not open the file!" << std::endl;
            return 1;
        }

        inputFile >> this->m; //liczba procesoró 

        inputFile >>  this->n; //liczba procesów zadañ

        processes.resize(n);

        for (int i = 0; i < n; ++i) {
            inputFile >> this->processes[i];
        }

 
        inputFile.close();
        return 0; 
    }

    int generate(std::string filepath, int min_n = 5, int max_n = 100, int min_m = 3, int max_m = 20, int min_t = 1, int max_t = 30) {

        /*std::filesystem::path path(filepath);
        std::filesystem::create_directories(path.parent_path());*/

        std::ofstream outFile(filepath);

        if (!outFile) {
            std::cerr << "Error opening file for writing." << std::endl;
            return 1;
        }

        // Initialize random number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        // Range for `n` and `m`
        std::uniform_int_distribution<> dist_ng(min_n, max_n); // liczba zadañ
        std::uniform_int_distribution<> dist_mg(min_m, max_m);  // liczba procesorów

        int ng = dist_ng(gen);      // Generate n
        int mg = dist_mg(gen);      // Generate m

        // Write n and m to file
        outFile << mg << "\n" << ng << "\n";

        // Range for the m random numbers
        std::uniform_int_distribution<> dist_value(min_t, max_t); // Adjust range for values as needed

        // Generate and save m random numbers
        for (int i = 0; i < ng; ++i) {
            int randomValue = dist_value(gen);
            outFile << randomValue << " ";
        }

        outFile << std::endl;
        outFile.close();

        std::cout << "Data saved to " << filepath << std::endl;

        return 0; 
    }

    
    void displayData(){
        std::cout<<"number of processes(n): "<<this->n<<"\n"; 
        std::cout<<"Processes time: \n";
        for(int i = 0; i<n; i++){
            std::cout<<this->processes[i]<<", ";
        }
        std::cout<<"\n"; 
        std::cout<<"Number of processors(m): "<<this->m<<"\n";
    }
};