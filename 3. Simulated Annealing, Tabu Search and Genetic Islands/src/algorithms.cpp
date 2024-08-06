#include "graph.hpp"


bool DisjoinedSet :: insert(int a, int b){
    int setA = -1;
    int setB = -1;
    
    for(int i=0; i < size(); i++){
        if(at(i).count(a) == 1){
            setA = i;
            if(setB != -1) break;
        }
        if(at(i).count(b) == 1){
            setB = i;
            if(setA != -1) break;
        }
    }
    
    if(setA > -1  && setB > -1){
        if(setA == setB)
            return false;
            
        at(setA).merge(at(setB));
        erase(std::find(begin(), end(), at(setB)));
    }else if(setA > -1){
        at(setA).insert(b);
    }else if(setB > -1){
        at(setB).insert(a);
    }else{
        std::set<int> newSet = std::set<int>();
        newSet.insert(a);
        newSet.insert(b);
        push_back(newSet);
    }

    return true;
}

void Graph::GenerateMST(){
    mst = MST(this);
    // generuje zbiór elementów {koszt, a, b} (a,b - punkty) do posortowania
    Vec2d CostEdge;
    CostEdge.reserve(N*(N-1)/2);

    for(int a = 0; a < N; a++){
        for(int b = a + 1; b < N; b++){
            CostEdge.push_back(
                std::vector{
                    metric(Pos[a], Pos[b]),
                    a, 
                    b
                }
            );
        }
    }
    
    
    DisjoinedSet dSet;
    mst.Edges = Vec2d(N);
    int inserted = 0;

    std::sort(CostEdge.begin(), CostEdge.end(), [](auto &left, auto &right){return left[0] > right[0];});
    
    while (inserted < N - 1){
        std::vector<int> edge = CostEdge.back();
        CostEdge.pop_back();
        int a = edge[1];
        int b = edge[2];
        
        if(dSet.insert(a,b)){
            mst.Edges[a].push_back(b);
            mst.Edges[b].push_back(a);
            inserted++;
            mst.cost += metric(Pos[a], Pos[b]);
        }
        
    }
    std::cout<<"Długość drzewa: "<<mst.cost<<std::endl;
}

Graph::MST Graph::MST::DFSTraversal(int startingPoint){
    MST Traversal(graph);
    // Vec2d Edges = Vec2d(N);

    std::stack<int> stack;
    std::set<int> visited;

    stack.push(startingPoint);
    visited.insert(startingPoint);

    // int cost = 0;
    int oldv = startingPoint;
    // int i=0;
    while(stack.size() > 0){
        int v = stack.top();
        
        Traversal.Edges[oldv].push_back(v);
        Traversal.Edges[v].push_back(oldv);
        // Traversal.Edges[i++] = (std::vector{oldv,v});
        Traversal.cost += graph->Distance[oldv][v];

        visited.insert(v);
        stack.pop();

        for(int child : Edges[v]){
            if(visited.count(child) == 0){
                stack.push(child);
            }
        }
        oldv = v;
    }
    
    Traversal.Edges[startingPoint].erase(Traversal.Edges[startingPoint].begin());
    Traversal.Edges[startingPoint].push_back(oldv);
    Traversal.Edges[oldv].push_back(startingPoint);
    // Traversal.Edges[0][0] = oldv;
    Traversal.cost += graph->Distance[oldv][startingPoint];

    // std::cout<<"Długość ścieżki: "<<sum<<std::endl;
    return Traversal;
}

int Graph::shuffle(int minRange){
    std::vector<int> order(N), bestOrder(N);
    int i = 0;
    std::generate(order.begin(), order.end(), [&i](){return i++;});
    int avsum = 0;
    
    for(int i = 0; i< 1000/minRange;i++){
        int minval = INT_MAX;
        for(int j = 0; j< minRange;j++){
            std::shuffle(order.begin(), order.end(), rng);
            int sum = 0;
            for(int k = 1; k < order.size();k++)
                sum += Distance[order[k-1]][order[k]];
                
            sum += Distance[order[order.size()-1]][order[0]];
            if(sum < minval){
                minval = sum;
                bestOrder = order;
            }
        }
        avsum += minval;
    }
    
    if(minRange == 1000){
        std::ofstream res;
        res.open("random/"+filename);
        
        for(int i=1; i<bestOrder.size();i++)
            res<<bestOrder[i-1]<<" "<<bestOrder[i]<<std::endl;

        res<<bestOrder[bestOrder.size()-1]<<" "<<bestOrder[0]<<std::endl;
        res.close();
    }

    return avsum /(1000/minRange);
    /*
    */
}