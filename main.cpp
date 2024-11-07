#include <iostream>
#include <PCMAX/solution.h>

int main() {
    system("cls");
    pcmax::PCmax solution; 

    solution.getProblemInstancesFromDirectory("./res"); 
    solution.run(); // run problem solution for provided problem insatnces 

    std::cin.get(); 
    return 0;
}
