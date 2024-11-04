#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>

#include "timer.h"
#include "data.h"

using namespace std;

int procesory(int liczba, vector<int> &procesy) {
    unordered_map<int,int> endTime;
    int i = procesy.size(), x = 0, max = INT_MIN;
    endTime[0] = liczba;
    while(i){
        if(endTime.find(x) != endTime.end()) {
            while(endTime[x] && i!=0) {
                ++endTime[x + procesy[procesy.size() - i]];
                if((x + procesy[procesy.size() - i]) > max) {
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

int main() {

    Timer timer; 
    system("cls");
    std::string filename = "example_data.txt";
    Data data = Data();
    data.generate(filename);
    data.readFromFile(filename);
    data.displayData(); 
    
    timer.start();
    cout << "Cmax : "<< procesory(data.m, data.processes) << "\n";
    timer.stop(); 

    std::cout<<"Execution time(ms): " << std::setprecision(10) <<timer.elapsedMilliseconds()<<"\n"; 
    std::cin.get(); 
    return 0;
}
