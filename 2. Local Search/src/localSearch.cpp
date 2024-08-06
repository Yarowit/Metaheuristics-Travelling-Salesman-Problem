#include "graph.hpp"

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
        
    int iterations = 0;
    while(true){
        iterations++;
        int bestI = 0;
        int bestJ = 0;
        int bestSwapGain = 0;

        // i=0 j=N-1
        

        for(int i=0; i<N - 1;i++){
            for(int j=i+1; j < N; j++){
                // zamiana i z j
                int pi = (N + i - 1) % N;
                int ni = (N + i + 1) % N;
                int pj = (N + j - 1) % N;
                int nj = (N + j + 1) % N;
                int swapGain =  Distance[cycle[pi]][cycle[j]] + 
                                Distance[cycle[j]][cycle[ni]] + 
                                Distance[cycle[pj]][cycle[i]] + 
                                Distance[cycle[i]][cycle[nj]] ;
                if(j != ni)
                    swapGain += -Distance[cycle[i]][cycle[ni]] -
                                Distance[cycle[pj]][cycle[j]] ;
                if(j != pi)
                    swapGain += -Distance[cycle[pi]][cycle[i]] -
                                Distance[cycle[j]][cycle[nj]] ;

                if(swapGain < bestSwapGain){
                    bestSwapGain = swapGain;
                    bestI = i;
                    bestJ = j;
                    // std::cout<<i<<":"<<j<<" : "<<bestSwapGain<<std::endl;
                }
            }
        }

        // std::cout<<originalCost<<" : "<<bestSwapGain<<std::endl;

        if(bestSwapGain == 0)
            return {originalCost, iterations};
        // if(bi-- ==0) return 0;
        
        int tmp = cycle[bestI];
        cycle[bestI] = cycle[bestJ];
        cycle[bestJ] = tmp;

        originalCost += bestSwapGain;
    }
}


std::vector<int> Graph::LocalSearchOptimized(std::vector<int> cyclev){
    int cycle[N]; int DistanceA[N][N];
    for(int i=0;i<N;i++)
        cycle[i] = cyclev[i];
        
    for(int i=0;i<N;i++)    
        for(int j=0;j<N;j++)
            DistanceA[i][j] = Distance[i][j];
        

    int originalCost = Distance[cycle[0]][cycle[N - 1]];

    for(int i=0; i<N - 1;i++)
        originalCost += DistanceA[cycle[i]][cycle[i+1]];
        
    originalCost += 1;

    int iterations = 0;
    int bestSwapGain = -1;
    int bestI = 0;
    int bestJ = 0;
    while(bestSwapGain<0){

        int tmp = cycle[bestI];
        cycle[bestI] = cycle[bestJ];
        cycle[bestJ] = tmp;

        originalCost += bestSwapGain;


        bestSwapGain = 0;
        iterations++;
        bestI = 0;
        bestJ = 0;
        // int i,j;
        // // i=0 1<j<N-1
        // i=0; for(int j=2; j<N - 1;j++){
        //     int swapGain =  DistanceA[cycle[N-1]][cycle[j]] + 
        //                     DistanceA[cycle[j]][cycle[i+1]] + 
        //                     DistanceA[cycle[j-1]][cycle[i]] + 
        //                     DistanceA[cycle[i]][cycle[nj]] ;
        // }
        // j=N-1 0<i<N-2

        // j=i+1

        // i=0 j=n-1

        // Wszystko pomiędzy (i>0 i+1<j<N-1)
        

        for(int i=0; i<N - 1;i++){
            for(int j=i+1; j < N; j++){
                // zamiana i z j
                int pi = (N + i - 1) % N;
                int ni = (N + i + 1) % N;
                int pj = (N + j - 1) % N;
                int nj = (N + j + 1) % N;
                int swapGain =  DistanceA[cycle[pi]][cycle[j]] + 
                                DistanceA[cycle[j]][cycle[ni]] + 
                                DistanceA[cycle[pj]][cycle[i]] + 
                                DistanceA[cycle[i]][cycle[nj]] ;
                if(j != ni)
                    swapGain += -DistanceA[cycle[i]][cycle[ni]] -
                                DistanceA[cycle[pj]][cycle[j]] ;
                if(j != pi)
                    swapGain += -DistanceA[cycle[pi]][cycle[i]] -
                                DistanceA[cycle[j]][cycle[nj]] ;

                if(swapGain < bestSwapGain){
                    bestSwapGain = swapGain;
                    bestI = i;
                    bestJ = j;
                    // std::cout<<i<<":"<<j<<" : "<<bestSwapGain<<std::endl;
                }
            }
        }

        // std::cout<<originalCost<<" : "<<bestSwapGain<<std::endl;

        
        
    }
    return {originalCost, iterations};
}