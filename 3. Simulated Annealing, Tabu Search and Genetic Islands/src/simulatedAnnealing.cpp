#include "graph.hpp"
std::uniform_real_distribution<float> distribution(0.0, 1.0);

bool Graph::Accept(float T, float difference){
    // solutiondiff będzie ujemna
    float e = exp(difference/T);
    // std::cout<<difference<<":"<<T<<" -> "<<e<<std::endl;
    float r = distribution(rng);
    if(r < e)
        return true;
    else
        return false;
}

std::vector<int> Graph::SimulatedAnnealing(std::vector<int> cycle, float startingTemp, float tempMultiplier, int repeatsPerEpoch){
    // algoryt mzatrzyma się, gdy przez dwi epoki minimum się nie zmieni
    int originalCost = Distance[cycle[0]][cycle[N - 1]];

    for(int i=0; i<N - 1;i++)
        originalCost += Distance[cycle[i]][cycle[i+1]];

        
    originalCost += 1;

    int epochs = 0;

    float T = startingTemp;

    int minimalSolution = originalCost;
    int currentSolution = minimalSolution;

    int noBetterCounter = 0;

    // while(minCost1 != minCost3){
    while(noBetterCounter < 50){
        epochs++;
        bool foundBetter = false;
        for(int e = 0; e < repeatsPerEpoch; e++){
            int i = rng()%(N-1);
            int j = i + rng()%(N-1-i) + 1;

            int pi = (N + i - 1) % N;
            int nj = (N + j + 1) % N;
            int difference = - Distance[cycle[pi]][cycle[i]] - 
                            Distance[cycle[j]][cycle[nj]] +
                            Distance[cycle[pi]][cycle[j]] +
                            Distance[cycle[i]][cycle[nj]]; 

            if(difference <= 0 || Accept(T,(-1.0)*difference)){
                
                currentSolution += difference;
                if(currentSolution < minimalSolution){
                    minimalSolution = currentSolution;
                    noBetterCounter = 0;
                    foundBetter = true;
                }else{
                    // noBetterCounter++;
                }

                std::reverse(cycle.begin()+i,cycle.begin()+j+1);
            }
        }
        if(!foundBetter) noBetterCounter++;

        T *= tempMultiplier;
    }

    minimalSolution = Distance[cycle[0]][cycle[N - 1]];

    for(int i=0; i<N - 1;i++)
        minimalSolution += Distance[cycle[i]][cycle[i+1]];
    return {minimalSolution, epochs};
}