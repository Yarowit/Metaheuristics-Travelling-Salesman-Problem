#include "graph.hpp"


std::vector<int> Graph::TabuSearch(std::vector<int> cycle, int tabuListSize){
    int currentCost = Distance[cycle[0]][cycle[N - 1]];

    for(int i=0; i<N - 1;i++)
        currentCost += Distance[cycle[i]][cycle[i+1]];

        
    currentCost += 1;

    int iterations = 0;
    // int bestI = 0;
    // int bestJ = 0;

    std::deque<std::vector<int>> tabuList;
    for(int i=0;i<tabuListSize;i++)
        tabuList.push_back(cycle);
    
    int noBetterCounter = 0;
    int bestCost = currentCost+1;
    
    

    while(noBetterCounter < 2*tabuListSize){

        iterations++;
        

        std::deque<int> bestI, bestJ;
        std::deque<int> difference;

        for(int i=0;i<tabuListSize+1;i++){
            bestI.push_back(0);
            bestJ.push_back(0);
            difference.push_back(INT_MAX);
        }

        int i,j;
        i=0; for(j=i+1; j<N-1; j++){
            int swapGain = - Distance[cycle[i+N-1]][cycle[i]] - 
                        Distance[cycle[j]][cycle[j+1]] +
                        Distance[cycle[i+N-1]][cycle[j]] +
                        Distance[cycle[i]][cycle[j+1]]; 

            int k=tabuListSize;
            if(difference[k]>swapGain){
                while(k-1>=0 && difference[k-1]>swapGain)
                    k--;
                    
                bestI.pop_back();
                bestJ.pop_back();
                difference.pop_back();
                bestI.emplace(bestI.begin()+k,i);
                bestJ.emplace(bestJ.begin()+k,j);
                difference.emplace(difference.begin()+k,swapGain);
                
            }
        }
        j=N-1; for(i=1; i<N-1; i++){
            int swapGain = - Distance[cycle[i-1]][cycle[i]] - 
                        Distance[cycle[j]][cycle[0]] +
                        Distance[cycle[i-1]][cycle[j]] +
                        Distance[cycle[i]][cycle[0]]; 

            int k=tabuListSize;
            if(difference[k]>swapGain){
                while(k-1>=0 && difference[k-1]>swapGain)
                    k--;
                    
                bestI.pop_back();
                bestJ.pop_back();
                difference.pop_back();
                bestI.emplace(bestI.begin()+k,i);
                bestJ.emplace(bestJ.begin()+k,j);
                difference.emplace(difference.begin()+k,swapGain);
                
            }
        }
        for(i=1; i<N-2; i++){
            for(j=i+1; j<N-1; j++){
                int swapGain = - Distance[cycle[i-1]][cycle[i]] - 
                            Distance[cycle[j]][cycle[j+1]] +
                            Distance[cycle[i-1]][cycle[j]] +
                            Distance[cycle[i]][cycle[j+1]]; 

                int k=tabuListSize;
                if(difference[k]>swapGain){
                    while(k-1>=0 && difference[k-1]>swapGain)
                        k--;
                        
                    bestI.pop_back();
                    bestJ.pop_back();
                    difference.pop_back();
                    bestI.emplace(bestI.begin()+k,i);
                    bestJ.emplace(bestJ.begin()+k,j);
                    difference.emplace(difference.begin()+k,swapGain);
                    
                }
            }

        }

        while(true){
            std::vector<int> c = cycle;
            std::reverse(c.begin()+bestI.front(),c.begin()+bestJ.front()+1);
            bool isInList = false;
            for(int j=0; j<tabuList.size();j++){
                if(tabuList[j]==c){
                    isInList = true;
                    break;
                }
            }
            if(isInList){
                bestI.pop_front();
                bestJ.pop_front();
                difference.pop_front();
            }else{
                cycle = c;
                tabuList.pop_front();
                tabuList.push_back(c);
                currentCost += difference.front();

                break;
            }
        }
        if(currentCost < bestCost){
            bestCost = currentCost;
            noBetterCounter = 0;
        }else{
            noBetterCounter++;
        }
        
    }

    return {bestCost, iterations};
}


std::vector<int> Graph::ModifiedTabuSearch(std::vector<int> cycle, int tabuListSize){
    int currentCost = Distance[cycle[0]][cycle[N - 1]];

    for(int i=0; i<N - 1;i++)
        currentCost += Distance[cycle[i]][cycle[i+1]];

        
    currentCost += 1;

    int bestCost = currentCost;

    int iterations = 0;
    int bestSwapGain = -1;
    int bestI = 0;
    int bestJ = 0;

    std::vector<std::vector<int>> tabuList;

    while(tabuList.size()<tabuListSize){
        // reverse
        std::reverse(cycle.begin()+bestI,cycle.begin()+bestJ+1);

        currentCost += bestSwapGain;
        if(currentCost<bestCost) bestCost = currentCost;
        // std::cout<<"cc  : "<<currentCost<<std::endl;

        bestSwapGain = INT_MAX;
        iterations++;
        bestI = 0;
        bestJ = 0;

        int i,j;
        int swapGain=0;

        int swps = 0;
        
        i=0; for(j=i+1; j<N-1; j++){
            swapGain = - Distance[cycle[i+N-1]][cycle[i]] - 
                        Distance[cycle[j]][cycle[j+1]] +
                        Distance[cycle[i+N-1]][cycle[j]] +
                        Distance[cycle[i]][cycle[j+1]]; 
            if(swapGain < bestSwapGain){
                std::reverse(cycle.begin()+i,cycle.begin()+j+1);
                if(std::find(tabuList.begin(),tabuList.end(),cycle)==tabuList.end()){
                    bestSwapGain = swapGain;
                    bestI = i;
                    bestJ = j;
                    swps++;
                }
                std::reverse(cycle.begin()+i,cycle.begin()+j+1);
            }
        }
        j=N-1; for(i=1; i<N-1; i++){
            swapGain = - Distance[cycle[i-1]][cycle[i]] - 
                        Distance[cycle[j]][cycle[0]] +
                        Distance[cycle[i-1]][cycle[j]] +
                        Distance[cycle[i]][cycle[0]]; 

            if(swapGain < bestSwapGain){
                std::reverse(cycle.begin()+i,cycle.begin()+j+1);
                if(std::find(tabuList.begin(),tabuList.end(),cycle)==tabuList.end()){
                    bestSwapGain = swapGain;
                    bestI = i;
                    bestJ = j;
                    swps++;
                }
                std::reverse(cycle.begin()+i,cycle.begin()+j+1);
            }
        }
        for(i=1; i<N-2; i++){
            for(j=i+1; j<N-1; j++){
                swapGain = - Distance[cycle[i-1]][cycle[i]] - 
                            Distance[cycle[j]][cycle[j+1]] +
                            Distance[cycle[i-1]][cycle[j]] +
                            Distance[cycle[i]][cycle[j+1]]; 
                if(swapGain < bestSwapGain){
                    std::reverse(cycle.begin()+i,cycle.begin()+j+1);
                    if(std::find(tabuList.begin(),tabuList.end(),cycle)==tabuList.end()){
                        bestSwapGain = swapGain;
                        bestI = i;
                        bestJ = j;
                    swps++;
                    }
                    std::reverse(cycle.begin()+i,cycle.begin()+j+1);
                }
            }

        }
        tabuList.push_back(cycle);

    }
    return {bestCost, iterations};
}