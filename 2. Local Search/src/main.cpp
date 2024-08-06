#include "graph.hpp"

void experiment1(Graph* graph){
    graph->GenerateMST();
    const int m = sqrt(graph->N);

    int minCycle = graph->mst.cost * 2;
    ulong averageCycleSum = 0;
    ulong averageIteraionsSum = 0;

    for(int i = 0; i < m; i++){
        std::vector<int> cycle = graph->mst.DFSCycle(graph->rng()%graph->N);
        // std::cout<<"cd"<<std::endl;
        std::vector<int> result = graph->LocalSearchOptimized(cycle);
        // std::cout<<"Ld"<<std::endl;
        
        averageCycleSum += result[0];
        averageIteraionsSum += result[1];

        if(result[0] < minCycle)
            minCycle = result[0];
    }

    std::cout<<"Najmniejszy cykl: "<<minCycle<<std::endl;
    std::cout<<"Średni cykl: "     <<averageCycleSum / m<<std::endl;
    std::cout<<"Średnie iteracje: "<<averageIteraionsSum / m<<std::endl;
}

int main(int argc, char** argv){
    Graph graph(argv[1],argv[2]);
    experiment1(&graph);
    // graph.GenerateMST();
    // std::vector<int> cycle = graph.mst.DFSCycle(0);
    // std::cout<<graph.LocalSearch(cycle)<<std::endl;
    // std::cout<<"-> "<<graph.mst.DFSTraversal(0).cost<<std::endl;

}
