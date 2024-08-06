#include "graph.hpp"

int main(int argc, char** argv){
    ListGraph graph(argv[1]);
    ListGraph* mst = (ListGraph*) graph.MinimalSpanningTree();
    mst->printEdgesToFile("tree",argv[1]);
    ListGraph DFS = mst->DFSTraversal();
    DFS.printEdgesToFile("path",argv[1]);
    
    MatrixGraph randomPaths(graph);
    
    std::cout<<"10: "<<randomPaths.shuffle(10)<<std::endl;
    std::cout<<"50: "<<randomPaths.shuffle(50)<<std::endl;
    std::cout<<"1000: "<<randomPaths.shuffle(1000)<<std::endl;
}

//fanf