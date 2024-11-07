#include <iostream>
#include <fstream>
#include <random>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <filesystem> 
#include <memory>

#include<PCMAX/timer.h>

namespace pcmax {

    class ProblemInstance {
    public:
        int n; //number of processes/tasks
        int m; // number of procesors
        std::string filepath;
        std::vector<int> processes; //processes time 

        ProblemInstance(std::string fileName) {
            this->readFromFile(fileName);
        }

        ProblemInstance() {}

        ~ProblemInstance() {}

        int readFromFile(std::filesystem::path file_path) {
            filepath = file_path.generic_string();
            std::ifstream inputFile(file_path);
            if (!inputFile) {
                std::cerr << "Error: could not open the file!" << std::endl;
                return 1;
            }

            inputFile >> this->m; //liczba procesoró 

            inputFile >> this->n; //liczba procesów zadañ

            processes.resize(n);

            for (int i = 0; i < n; ++i) {
                inputFile >> this->processes[i];
            }


            inputFile.close();
            return 0;
        }

        int generate(std::string filepath, int min_n = 5, int max_n = 100, int min_m = 3, int max_m = 20, int min_t = 1, int max_t = 30) {

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
                outFile << randomValue << "\n";
            }

            outFile << std::endl;
            outFile.close();

            std::cout << "Data saved to " << filepath << std::endl;

            return 0;
        }


        void displayData() {
            std::cout << "plik: " << this->filepath << "\n";
            std::cout << "number of processes(n): " << this->n << "\n";
            std::cout << "Processes time: \n";
            for (int i = 0; i < n; i++) {
                std::cout << this->processes[i] << ", ";
            }
            std::cout << "\n";
            std::cout << "Number of processors(m): " << this->m << "\n";
        }
    };

    //Solution class
    class PCmax {
    public: 

        void run() {
            Timer timer; 

            for (auto data : problems) {
                std::cout << "<==========" <<data->filepath<<"==========>\n\t";
                int res; 

                timer.start();
                res = procesory_2(data->m, data->processes);
                timer.stop();

                timer.displayTime(); 
                std::cout << "\tCmax : " << res << "\n\t";
                std::cout << "\n";
                
            }
        }

        void addProblemInstance(ProblemInstance instance) {
            problems.push_back(std::make_shared<ProblemInstance>(instance));
        }

        void clearProblemInstances() {
            problems.clear(); 
        }

        void getProblemInstancesFromDirectory(const std::string& directoryPath) {
            try {
                for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
                    if (entry.is_regular_file()) {
                        ProblemInstance instance;
                        instance.readFromFile(entry.path());
                        addProblemInstance(instance);
                    }
                }
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Error accessing directory: " << e.what() << "\n";
            }
        }

    private: 
        std::vector<std::shared_ptr<ProblemInstance>> problems;

        int procesory(int liczba, std::vector<int>& procesy) {
            std::unordered_map<int, int> endTime;
            int i = (int)procesy.size(), x = 0, max = INT_MIN;
            endTime[0] = liczba;
            while (i) {
                if (endTime.find(x) != endTime.end()) {
                    while (endTime[x] && i != 0) {
                        ++endTime[x + procesy[procesy.size() - i]];
                        if ((x + procesy[procesy.size() - i]) > max) {
                            max = x + procesy[procesy.size() - i];
                        }
                        --i;
                        --endTime[x];
                    }
                }
                ++x;
            }
            return max;
        }

        int procesory_2(int liczba, std::vector<int>& procesy) {
            std::map<int, int> endTime;
            int i = (int)procesy.size(), x = 0, max = INT_MIN;
            endTime[0] = liczba;
            while (x < i) {
                int pom = endTime.begin()->first;
                for (int y = endTime.begin()->second; y > 0 && x < i; --y) {
                    ++endTime[pom + procesy[x]];
                    if ((pom + procesy[x]) > max)
                        max = pom + procesy[x];
                    ++x;
                }
                endTime.erase(endTime.begin());
            }
            return max;
        }
    };

}
