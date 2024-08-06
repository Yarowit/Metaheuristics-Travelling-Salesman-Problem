#include "graph.hpp"
#include <algorithm>

std::vector<int> Graph::MST::DFSCycle(int startingPoint){
    std::vector<int> cycle;

    std::stack<int> stack;
    std::set<int> visited;

    stack.push(startingPoint);
    visited.insert(startingPoint);

    while(stack.size() > 0){
        int v = stack.top();

        cycle.push_back(v);
        
        visited.insert(v);

        stack.pop();

        for(int child : Edges[v]){
            if(visited.count(child) == 0){
                stack.push(child);
            }
        }
    }
    
    return cycle;
}


std::vector<int> Graph::LocalSearch(std::vector<int> cycle){
    int originalCost = Distance[cycle[0]][cycle[N - 1]];

    for(int i=0; i<N - 1;i++)
        originalCost += Distance[cycle[i]][cycle[i+1]];

        
    originalCost += 1;

    int iterations = 0;
    int bestSwapGain = -1;
    int bestI = 0;
    int bestJ = 0;

    while(bestSwapGain<0){
        // reverse
        std::reverse(cycle.begin()+bestI,cycle.begin()+bestJ+1);

        originalCost += bestSwapGain;


        bestSwapGain = 0;
        iterations++;
        bestI = 0;
        bestJ = 0;

        int i,j;
        int swapGain=0;
        
        i=0; for(j=i+1; j<N-1; j++){
            swapGain = - Distance[cycle[i+N-1]][cycle[i]] - 
                        Distance[cycle[j]][cycle[j+1]] +
                        Distance[cycle[i+N-1]][cycle[j]] +
                        Distance[cycle[i]][cycle[j+1]]; 
            if(swapGain < bestSwapGain){
                bestSwapGain = swapGain;
                bestI = i;
                bestJ = j;
            }
        }
        j=N-1; for(i=1; i<N-1; i++){
            swapGain = - Distance[cycle[i-1]][cycle[i]] - 
                        Distance[cycle[j]][cycle[0]] +
                        Distance[cycle[i-1]][cycle[j]] +
                        Distance[cycle[i]][cycle[0]]; 

            if(swapGain < bestSwapGain){
                bestSwapGain = swapGain;
                bestI = i;
                bestJ = j;
            }
        }
        for(i=1; i<N-2; i++){
            for(j=i+1; j<N-1; j++){
                swapGain = - Distance[cycle[i-1]][cycle[i]] - 
                            Distance[cycle[j]][cycle[j+1]] +
                            Distance[cycle[i-1]][cycle[j]] +
                            Distance[cycle[i]][cycle[j+1]]; 
                if(swapGain < bestSwapGain){
                    bestSwapGain = swapGain;
                    bestI = i;
                    bestJ = j;
                }
            }

        }

    }

    return {originalCost, iterations};
}



std::vector<int> Graph::LocalSearchBestOfN(std::vector<int> cycle){
    int originalCost = Distance[cycle[0]][cycle[N - 1]];

    for(int i=0; i<N - 1;i++)
        originalCost += Distance[cycle[i]][cycle[i+1]];

        
    originalCost += 1;

    int iterations = 0;
    int bestSwapGain = -1;
    int bestI = 0;
    int bestJ = 0;

    while(bestSwapGain<0){
        // reverse
        std::reverse(cycle.begin()+bestI,cycle.begin()+bestJ+1);

        originalCost += bestSwapGain;


        bestSwapGain = 0;
        iterations++;
        bestI = 0;
        bestJ = 0;

        // int i,j;
        // int swapGain=0;
        
        for(int k=0; k<N;k++){
            int i = rng()%(N-1);
            int j = i + rng()%(N-1-i) + 1;
            // std::cout<<"-> "<<i<<" "<<j<<std::endl;
            int pi = (N + i - 1) % N;
            int nj = (N + j + 1) % N;
            int swapGain = - Distance[cycle[pi]][cycle[i]] - 
                            Distance[cycle[j]][cycle[nj]] +
                            Distance[cycle[pi]][cycle[j]] +
                            Distance[cycle[i]][cycle[nj]]; 
            if(swapGain < bestSwapGain){
                bestSwapGain = swapGain;
                bestI = i;
                bestJ = j;
            }

        }

    }

    return {originalCost, iterations};
}