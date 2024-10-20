#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

class Data{
public:
    int n; //number of processes/tasks
    int m; // number of procesors
    std::vector<int> processes; //processes time 

    Data(std::string fileName){
        this->readFromFile(fileName);
    }
    ~Data(){}

    int readFromFile(std::string fileName){
        std::ifstream inputFile(fileName);
        if (!inputFile) {
            std::cerr << "Error: could not open the file!" << std::endl;
            return 1;
        }

        inputFile >>  this->n;

        processes.resize(n);

        for (int i = 0; i < n; ++i) {
            inputFile >> this->processes[i];
        }

        inputFile >>  this->m;
        inputFile.close();
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