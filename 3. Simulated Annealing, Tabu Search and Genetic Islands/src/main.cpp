#include "graph.hpp"
#include <thread>

void experiment2(Graph* graph){

    const int m = graph->N > 1000 ? 100 : graph->N;

    int minCycle = INT_MAX;
    ulong averageCycleSum = 0;
    ulong averageIteraionsSum = 0;
    
    

    const int cpus = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(cpus);
    // std::vector<std::vector<int>> cycles(cpus);
    int results[cpus][3];
    int chunk = m / cpus;

    auto process = [](std::vector<int> cycle, Graph* graph, int iterations, int* avSum, int* itSum, int* minCycle){
        int averageCycleSum = 0;
        int averageIteraionsSum = 0;
        int minCycleVal = INT_MAX;
        for(int i = 0; i < iterations; i++){
            std::shuffle(cycle.begin(), cycle.end(), graph->rng); //losowy cykl
            std::vector<int> result = graph->LocalSearch(cycle);
            averageCycleSum += result[0];
            averageIteraionsSum += result[1];

            if(result[0] < minCycleVal)
                minCycleVal = result[0];
        }

        *avSum = averageCycleSum;
        *itSum = averageIteraionsSum;
        *minCycle = minCycleVal;
    };

    std::vector<int> cycle(graph->N);
    for(int j=0;j<graph->N;j++){
        cycle[j] = j;
    }
    for(int i=0;i<cpus;i++){
        threads[i] = std::thread(process, cycle, graph,chunk,&results[i][0],&results[i][1],&results[i][2]);
    }
    for(int i=0;i<cpus;i++){
        // std::thread thr(proc, i<<10);
        threads[i].join();
        averageCycleSum += results[i][0];
        averageIteraionsSum += results[i][1];
        if(minCycle > results[i][2])
            minCycle = results[i][2];
    }


    std::cout<<"Najmniejszy cykl: "<<minCycle<<std::endl;
    std::cout<<"Średni cykl:      "<<averageCycleSum / (chunk*cpus)<<std::endl;
    std::cout<<"Średnie iteracje: "<<averageIteraionsSum / (chunk*cpus)<<std::endl;
}

void annealing(Graph* graph){
    const int m = 3000;

    const int cpus = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(cpus);
    
    int results[cpus][3];
    int chunk = m / cpus;

    auto process = [](std::vector<int> cycle, Graph* graph, int iterations, int* avSum, int* itSum, int* minCycle, float startingTemp, float tempMultiplier, int repeatsPerEpoch){
        int averageCycleSum = 0;
        int averageIteraionsSum = 0;
        int minCycleVal = INT_MAX;
        for(int i = 0; i < iterations; i++){
            std::shuffle(cycle.begin(), cycle.end(), graph->rng); //losowy cykl
            std::vector<int> result = graph->SimulatedAnnealing(cycle,startingTemp,tempMultiplier,repeatsPerEpoch);
            averageCycleSum += result[0];
            averageIteraionsSum += result[1];

            if(result[0] < minCycleVal)
                minCycleVal = result[0];
        }

        *avSum = averageCycleSum;
        *itSum = averageIteraionsSum;
        *minCycle = minCycleVal;
    };

    std::vector<int> cycle(graph->N);
    for(int j=0;j<graph->N;j++){
        cycle[j] = j;
    }
    
    for(int t=1; t < 30; t++){
        float T = -300.0 -t*5.0;
        for(int m=0; m < 1; m++){
            float M = 0.87;
            for(int e=1; e < 2; e++){
                int E = graph->N;

                int minCycle = INT_MAX;
                ulong averageCycleSum = 0;
                ulong averageIteraionsSum = 0;

                for(int i=0;i<cpus;i++){
                    threads[i] = std::thread(process, cycle, graph,chunk,&results[i][0],&results[i][1],&results[i][2],T,M,E);
                }
                for(int i=0;i<cpus;i++){
                    // std::thread thr(proc, i<<10);
                    threads[i].join();
                    averageCycleSum += results[i][0];
                    averageIteraionsSum += results[i][1];
                    if(minCycle > results[i][2])
                        minCycle = results[i][2];
                }
                std::cout<<std::endl<<T<<" "<<M<<" "<<E<<std::endl;
                std::cout<<"Najmniejszy cykl: "<<minCycle<<std::endl;
                std::cout<<"Średni cykl:      "<<averageCycleSum / (chunk*cpus)<<std::endl;
                std::cout<<"Średnie iteracje: "<<averageIteraionsSum / (chunk*cpus)<<std::endl;
                for(int i=0;i<minCycle;i+=20)
                    std::cout<<"|";
                
            }
        }
    }
}

void testTabuParameters(Graph* graph){
    const int m = graph->N > 1000 ? 100 : graph->N;
    const int cpus = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(cpus);
    
    std::cout<<"N: "<<graph->N<<" cpus: "<<cpus<<std::endl;

    int results[cpus][3];
    int chunk = m / cpus;

    auto process = [](std::vector<int> cycle, Graph* graph, int iterations, int tabuListSize, int* avSum, int* itSum, int* minCycle){
        int averageCycleSum = 0;
        int averageIteraionsSum = 0;
        int minCycleVal = INT_MAX;
        for(int i = 0; i < iterations; i++){
            std::shuffle(cycle.begin(), cycle.end(), graph->rng); //losowy cykl
            std::vector<int> result = graph->TabuSearch(cycle,tabuListSize);
            averageCycleSum += result[0];
            averageIteraionsSum += result[1];

            if(result[0] < minCycleVal)
                minCycleVal = result[0];
        }

        *avSum = averageCycleSum;
        *itSum = averageIteraionsSum;
        *minCycle = minCycleVal;
    };

    std::vector<int> cycle(graph->N);
    for(int j=0;j<graph->N;j++){
        cycle[j] = j;
    }

    std::vector<int> tls = {3,7,20,40};

    for(int z=0;z<tls.size();z++){
        std::cout<<"N / tls: "<<graph->N<<" / "<<tls[z]<<" reps: "<<chunk*cpus<<std::endl;
        
        int minCycle = INT_MAX;
        ulong averageCycleSum = 0;
        ulong averageIteraionsSum = 0;

        for(int i=0;i<cpus;i++){
            threads[i] = std::thread(process, cycle, graph,chunk,tls[z],&results[i][0],&results[i][1],&results[i][2]);
        }
        for(int i=0;i<cpus;i++){
            threads[i].join();
            averageCycleSum += results[i][0];
            averageIteraionsSum += results[i][1];
            if(minCycle > results[i][2])
                minCycle = results[i][2];
        }
        std::cout<<"Najmniejszy cykl: "<<minCycle<<std::endl;
        std::cout<<"Średni cykl:      "<<averageCycleSum / (chunk*cpus)<<std::endl;
        std::cout<<"Średnie iteracje: "<<averageIteraionsSum / (chunk*cpus)<<std::endl;
    }
}

void testAnnealingParameters(Graph* graph){
    const int m = 100;

    const int cpus = std::thread::hardware_concurrency();
    
    std::vector<std::thread> threads(cpus);
    
    int results[cpus][3];
    int chunk = m / cpus;

    auto process = [](std::vector<int> cycle, Graph* graph, int iterations, int* avSum, int* itSum, int* minCycle, float startingTemp, float tempMultiplier, int repeatsPerEpoch){
        int averageCycleSum = 0;
        int averageIteraionsSum = 0;
        int minCycleVal = INT_MAX;
        for(int i = 0; i < iterations; i++){
            std::shuffle(cycle.begin(), cycle.end(), graph->rng); //losowy cykl
            std::vector<int> result = graph->SimulatedAnnealing(cycle,startingTemp,tempMultiplier,repeatsPerEpoch);
            averageCycleSum += result[0];
            averageIteraionsSum += result[1];

            if(result[0] < minCycleVal)
                minCycleVal = result[0];
        }

        *avSum = averageCycleSum;
        *itSum = averageIteraionsSum;
        *minCycle = minCycleVal;
    };

    std::vector<int> cycle(graph->N);
    for(int j=0;j<graph->N;j++){
        cycle[j] = j;
    }
    std::vector<int> res = {0,0,0,0,0,0,0,0};
    std::vector<int> stat = {0,0,0,0,0,0,0,0};
    for(int reps=0;reps<100;reps++){
        std::vector<float> temps = {0.1,0.4,0.8,1,1.3,2,2.6,3};
    for(int t =1;t<9 ;t++){
        // 2.2 jesr lepsze od 1.9 i mniejszych
        
        float T = t*0.01*graph->N;
        for(int mk=0; mk < 1; mk++){
            float M = 0.85;           
            for(int e=1; e < 2; e++){
                int E = 100;                    

                int minCycle = INT_MAX;
                ulong averageCycleSum = 0;
                ulong averageIteraionsSum = 0;
                for(int i=0;i<cpus;i++){
                    threads[i] = std::thread(process, cycle, graph,chunk,&results[i][0],&results[i][1],&results[i][2],T,M,E);
                }
                for(int i=0;i<cpus;i++){
                    // std::thread thr(proc, i<<10);
                    threads[i].join();
                    averageCycleSum += results[i][0];
                    averageIteraionsSum += results[i][1];
                    if(minCycle > results[i][2])
                        minCycle = results[i][2];
                }
                
                res[t-1] = minCycle;
                
            }
        }
    }
        int mkin = INT_MAX;
        int mm=0;
        for(int ps = 0;ps<8;ps++)
            if(res[ps]<mkin){
                mkin= res[ps];
                mm=ps;
            }
        stat[mm]++;
    }
    for(int i=0;i<8;i++){
        std::cout<<"\n";
        for(int k=0;k<stat[i];k++)
            std::cout<<"|";
    }

            
        
        
    


    
}

void experiment(Graph* graph){
    const int m = graph->N > 1000 ? 100 : graph->N;    
    // const int m = 16;    
    const int cpus = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(cpus);
    
    // std::cout<<"N: "<<graph->N<<" cpus: "<<cpus<<std::endl;

    auto processTabuModifed = [](std::vector<int> cycle, Graph* graph, int iterations, int tabuListSize, int* avSum, int* itSum, int* minCycle){
        int averageCycleSum = 0;
        int averageIteraionsSum = 0;
        int minCycleVal = INT_MAX;
        for(int i = 0; i < iterations; i++){
            std::shuffle(cycle.begin(), cycle.end(), graph->rng); //losowy cykl
            std::vector<int> result = graph->ModifiedTabuSearch(cycle,tabuListSize);
            averageCycleSum += result[0];
            averageIteraionsSum += result[1];

            if(result[0] < minCycleVal)
                minCycleVal = result[0];
        }

        *avSum = averageCycleSum;
        *itSum = averageIteraionsSum;
        *minCycle = minCycleVal;
    };

    auto processAnnealing = [](std::vector<int> cycle, Graph* graph, int iterations, int* avSum, int* itSum, int* minCycle, float startingTemp, float tempMultiplier, int repeatsPerEpoch){
        int averageCycleSum = 0;
        int averageIteraionsSum = 0;
        int minCycleVal = INT_MAX;
        for(int i = 0; i < iterations; i++){
            std::shuffle(cycle.begin(), cycle.end(), graph->rng); //losowy cykl
            std::vector<int> result = graph->SimulatedAnnealing(cycle,startingTemp,tempMultiplier,repeatsPerEpoch);
            averageCycleSum += result[0];
            averageIteraionsSum += result[1];

            if(result[0] < minCycleVal)
                minCycleVal = result[0];
        }

        *avSum = averageCycleSum;
        *itSum = averageIteraionsSum;
        *minCycle = minCycleVal;
    };

    std::vector<int> cycle(graph->N);
    for(int j=0;j<graph->N;j++){
        cycle[j] = j;
    }

    
    if(true){
        int minCycle = INT_MAX;
        ulong averageCycleSum = 0;
        ulong averageIteraionsSum = 0;
        int results[cpus][3];
        int chunk = m / cpus;

        const int tabuListSize = graph->N*4/3 * 3/2;

        // for(int i=0;i<1;i++){
        for(int i=0;i<cpus;i++){
            threads[i] = std::thread(processTabuModifed, cycle, graph,chunk,tabuListSize,&results[i][0],&results[i][1],&results[i][2]);
        }
        // for(int i=0;i<1;i++){
        for(int i=0;i<cpus;i++){
            threads[i].join();
            averageCycleSum += results[i][0];
            averageIteraionsSum += results[i][1];
            if(minCycle > results[i][2])
                minCycle = results[i][2];
        }
        std::cout<<graph->N<<" & Tabu & "<<minCycle<<" & "<<averageCycleSum / (chunk*cpus)<<" & "<<averageIteraionsSum / (chunk*cpus)<<" \\\\"<<std::endl;
    }
    if(true){
        int minCycle = INT_MAX;
        ulong averageCycleSum = 0;
        ulong averageIteraionsSum = 0;
        int results[cpus][3];
        int chunk = m / cpus;

        // const float startingTemp = 0.03*graph->N;
        const float startingTemp = 0.001*graph->N;
        // const float startingTemp = graph->N;
        const int repeatsPerEpoch = 1000;
        // const float multiplier = 0.88;
        const float multiplier = 0.85;

        for(int i=0;i<cpus;i++){
            threads[i] = std::thread(processAnnealing, cycle, graph,chunk,&results[i][0],&results[i][1],&results[i][2],startingTemp,multiplier,repeatsPerEpoch);
        }
        for(int i=0;i<cpus;i++){
            threads[i].join();
            averageCycleSum += results[i][0];
            averageIteraionsSum += results[i][1];
            if(minCycle > results[i][2])
                minCycle = results[i][2];
        }
        std::cout<<graph->N<<" & Wyż. & "<<minCycle<<" & "<<averageCycleSum / (chunk*cpus)<<" & "<<averageIteraionsSum / (chunk*cpus)<<" \\\\"<<std::endl;
    }

}

void genetic(Graph* graph){
    int islands = std::thread::hardware_concurrency();
    // int islands = 2;
    std::cout<<"Cores: "<<islands<<std::endl;
    int populationPerIsland =  20;
    // int populationPerIsland =  (int(std::floor(sqrt(graph->N)))/2) *2;
    // int populationPerIsland =  (int(std::floor((graph->N)))/2) *2;
    int generationsPerEpoch = 10;
    int Epochs = 10;
    int result = graph->Genetic(islands,populationPerIsland, generationsPerEpoch, Epochs);
    std::cout<<"Najmniejszy wynik: "<<result<<std::endl;
}

int calcCost(std::vector<int> parenta, Graph* graph){
    int cost1 = graph->Distance[parenta[0]][parenta[graph->N - 1]];
    for(int j=0;j<graph->N-1;j++){
        cost1 += graph->Distance[parenta[j]][parenta[j+1]];
    }
    return cost1;
}


int main(int argc, char** argv){
    Graph graph(argv[1],argv[2]);
    
    genetic(&graph);
}