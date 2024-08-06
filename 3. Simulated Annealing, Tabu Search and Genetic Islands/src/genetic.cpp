#include "graph.hpp"
#include <set>

using Tup = std::tuple<std::vector<std::vector<int>>,std::vector<int>>;

std::tuple<std::vector<int>,int> Graph::ModifiedTabuSearchForGenetics(std::vector<int> cycle, int tabuListSize){
    int currentCost = Distance[cycle[0]][cycle[N - 1]];

    for(int i=0; i<N - 1;i++)
        currentCost += Distance[cycle[i]][cycle[i+1]];

        
    currentCost += 1;

    int bestCost = currentCost;
    std::vector<int> bestCycle;

    int iterations = 0;
    int bestSwapGain = -1;
    int bestI = 0;
    int bestJ = 0;

    std::vector<std::vector<int>> tabuList;

    while(tabuList.size()<tabuListSize){
        // reverse
        std::reverse(cycle.begin()+bestI,cycle.begin()+bestJ+1);

        currentCost += bestSwapGain;
        if(currentCost<bestCost){ 
            bestCost = currentCost;
            bestCycle = cycle;
        }
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
        // std::cout<<"swps: "<<swps<<" BSG: "<<bestSwapGain<<" TLS: "<<tabuList.size()<<std::endl;
        tabuList.push_back(cycle);

    }
    // std::cout<<"ret"<<std::endl;
    return {tabuList[tabuList.size()-1],bestCost};
    return {bestCycle,bestCost};
}


std::tuple<std::vector<std::vector<int>>,std::vector<int>> generateStartingPopulation(int populationSize, Graph* graph){
    // w pełni losowe
    std::vector<int> cycle(graph->N);
    for(int j=0;j<graph->N;j++){
        cycle[j] = j;
    }
    std::vector<std::vector<int>> cycles; 
    std::vector<int> costs; 
    for(int i=0;i<populationSize;i++){
        std::shuffle(cycle.begin(), cycle.end(), graph->rng); 
        std::vector<int> newCycle; 
        int cost;
        std::tie(newCycle,cost) = graph->ModifiedTabuSearchForGenetics(cycle,10);
        // std::vector<int> newCycle(graph->N);
        // int cost = graph->Distance[cycle[0]][cycle[graph->N - 1]];
        // for(int j=0;j<graph->N-1;j++){
        //     newCycle[j] = cycle[j];
        //     cost += graph->Distance[cycle[j]][cycle[j+1]];
        // }
        // newCycle[graph->N-1] = cycle[graph->N-1];
        cycles.push_back(newCycle);
        costs.push_back(cost);
    }

    return {cycles,costs};
}

inline void deb(int mes){
    // std::cout<<" -- "<<mes<<std::endl;
}
inline void deb(std::string mes){
    // std::cout<<" -- "<<mes<<std::endl;
}

int Graph::Genetic(int islands,int populationPerIsland, int generationsPerEpoch, int Epochs){
    // tutaj siedzi wywoływanie generateStartingPoints
    islands = islands/2 *2;
    std::vector<std::tuple<std::vector<std::vector<int>>,std::vector<int>>> Islands;

    for(int i=0; i<islands;i++){
        std::vector<std::vector<int>> population;
        std::vector<int> costs;
        Islands.push_back(generateStartingPopulation(populationPerIsland*5, this));
    }
    deb(1);

    auto IslandProcess = [](std::tuple<std::vector<std::vector<int>>,std::vector<int>>* population, Graph* graph, int generationsPerEpoch, int populationPerIsland){
        std::vector<std::vector<int>> cycles;
        std::vector<int> costs;
        std::tie(cycles,costs) = *population;
        deb(30);
        (*population) = graph->Island( cycles, costs,  generationsPerEpoch,populationPerIsland);
        deb(40);
    };

    int globalMin = INT_MAX;
    // const int cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(islands);
    for(int e = 0; e < Epochs; e++){
        deb(2);
        std::cout<<"Started: "<<e<<std::endl;
        for(int i=0;i<islands;i++){
            // IslandProcess(&(Islands[i]), this,generationsPerEpoch,populationPerIsland);
            threads[i] = std::thread(IslandProcess, &(Islands[i]), this,generationsPerEpoch,populationPerIsland);
        }
        for(int i=0;i<islands;i++){
            threads[i].join();
        }
        std::cout<<"Joined: "<<e<<std::endl;
        deb(3);
        std::shuffle(Islands.begin(), Islands.end(), rng); 
        for(int p=0;p<islands;p += 2){
            std::vector<std::vector<int>> island1,island2;
            std::vector<int> costs1,costs2;
            std::tie(island1,costs1) = Islands[p];
            std::tie(island2,costs2) = Islands[p+1];

            // indeksy które zamienię - pierwsza połowa tablicy
            std::vector<int> order(island1.size());
            for(int i=0;i<island1.size();i++)
                order[i] = i;
            // std::shuffle(order.begin(), order.end(), rng);
            // TODO tutaj jest shuffle migracji
            //swapy
            for(int i=0;i<island1.size()/50;i++){
                int swapIndex = order[i];
                std::vector<int> tempPop = island1[swapIndex];
                int tempCost = costs1[swapIndex];
                island1[swapIndex] = island2[swapIndex];
                costs1[swapIndex] = costs2[swapIndex];
                island2[swapIndex] = tempPop;
                costs2[swapIndex] = tempCost;
            }
            Islands[p] = {island1,costs1};
            Islands[p+1] = {island2,costs2};
            
            int min = *(std::min_element(costs1.begin(),costs1.end()));
            if(min<globalMin) globalMin = min;
            min = *(std::min_element(costs2.begin(),costs2.end()));
            if(min<globalMin) globalMin = min;
        }
    //     for(int p=0;p<islands;p++){
    //     std::vector<std::vector<int>> island;
    //     std::vector<int> costs;
    //     std::tie(island,costs) = Islands[p];
    //     int min = *(std::min_element(costs.begin(),costs.end()));
    //     if(min<globalMin) globalMin = min;
    // }
    }
    
    // znajdywanie najlepszego elementu
    
    
    return globalMin;
}

std::tuple<std::vector<int>,std::vector<int>,int,int> OX(std::vector<int> parenta,std::vector<int> parenta2, std::vector<int> parentb,int index,int N,Graph* graph){
    std::set<int> taken;
    std::vector<int> child1(N),child2(N);
    for(int i=index;i!=(index+N/2)%N; i=(i+1)%N){
        child1[i] = parentb[i];
        taken.insert(parentb[i]);
    }
    int j = 0;
    for(int i=(index+N/2)%N;i!=index; i=(i+1)%N){
        if(taken.count(parenta[j])) j++;
        child1[i] = parenta[j++];
    }

    int cost1 = graph->Distance[child1[0]][child1[graph->N - 1]];
    for(int j=0;j<graph->N-1;j++){
        cost1 += graph->Distance[child1[j]][child1[j+1]];
    }
    taken.clear();

    for(int i=index;i!=(index+N/2)%N; i=(i+1)%N){
        child2[i] = parenta[i];
        taken.insert(parenta[i]);
    }
    j = 0;
    for(int i=(index+N/2)%N;i!=index; i=(i+1)%N){
        if(taken.count(parentb[j])) j++;
        child2[i] = parentb[j++];
    }
    int cost2 = graph->Distance[child2[0]][child2[graph->N - 1]];
    for(int j=0;j<graph->N-1;j++){
        cost2 += graph->Distance[child2[j]][child2[j+1]];
    }

    return {child1,child2,cost1,cost2};
}

std::tuple<std::vector<int>,std::vector<int>,int,int> PMX(std::vector<int> parenta,std::vector<int> parenta2, std::vector<int> parentb,int index,int N,Graph* graph){
    for(int i=index;i!=(index+N/2)%N; i=(i+1)%N){
        int val = parentb[i];
        *(std::find(parenta.begin(),parenta.end(),val)) = parenta[i];
        parenta[i] = parentb[i];
    }
    int cost1 = graph->Distance[parenta[0]][parenta[graph->N - 1]];
    for(int j=0;j<graph->N-1;j++){
        cost1 += graph->Distance[parenta[j]][parenta[j+1]];
    }


    for(int i=index;i!=(index+N/2)%N; i=(i+1)%N){
        int val = parenta2[i];
        *(std::find(parentb.begin(),parentb.end(),val)) = parentb[i];
        parentb[i] = parenta2[i];
    }
    int cost2 = graph->Distance[parentb[0]][parentb[graph->N - 1]];
    for(int j=0;j<graph->N-1;j++){
        cost2 += graph->Distance[parentb[j]][parentb[j+1]];
    }

    return {parenta,parentb,cost1,cost2};
}


void printSum(std::vector<int> v){
    int min = INT_MAX;
    int max = INT_MIN;
    int sum = 0;
    for(auto el : v){
        if(el < min) min = el;
        if(el > max) max = el;
        sum += el;
    }
    // std::cout<<"S: "<<sum/v.size()<<" min: "<<min<<" max: "<<max<<std::endl;
}

std::vector<int> GenParents(std::vector<int> weights, std::mt19937 rng,int samples){
    int rnd_max = weights.size();
    int ntake = samples;

    /* initialize random sampler */
    unsigned int seed = 12345;
    // std::mt19937 rng(seed);

    /* determine smallest power of two that is larger than N */
    int tree_levels = ceil(log2((double) rnd_max));

    /* initialize vector with place-holders for perfectly-balanced tree */
    std::vector<double> tree_weights(1<<(tree_levels + 1));

    /* compute sums for the tree leaves at each node */
    int offset = (1<<tree_levels) - 1;
    for (int ix = 0; ix < rnd_max; ix++) {
        tree_weights[ix + offset] = weights[ix];
    }
    for (int ix = (1<<(tree_levels+1)) - 1; ix > 0; ix--) {
        tree_weights[(ix - 1) / 2] += tree_weights[ix];
    }

    /* sample according to uniform distribution */
    double rnd_subrange, w_left;
    double curr_subrange;
    int curr_ix;
    std::vector<int> sampled(ntake);
    for (int el = 0; el < ntake; el++) {

        /* go down the tree by drawing a random number and
           checking if it falls in the left or right sub-ranges */
        curr_ix = 0;
        curr_subrange = tree_weights[0];
        for (int lev = 0; lev < tree_levels; lev++) {
            rnd_subrange = std::uniform_real_distribution<double>(0, curr_subrange)(rng);
            w_left = tree_weights[2 * curr_ix + 1];
            curr_ix = 2 * curr_ix + 1 + (rnd_subrange >= w_left);
            curr_subrange = tree_weights[curr_ix];
        }

        /* finally, add element from this iteration */
        sampled[el] = curr_ix - offset;

        /* now remove the weight of the chosen element */
        tree_weights[curr_ix] = 0;
        for (int lev = 0; lev < tree_levels; lev++) {
            curr_ix = (curr_ix - 1) / 2;
            tree_weights[curr_ix] =   tree_weights[2 * curr_ix + 1]
                                    + tree_weights[2 * curr_ix + 2];
        }
    }
    return sampled;
}

std::tuple<std::vector<std::vector<int>>,std::vector<int>> Graph::Island(std::vector<std::vector<int>> cycles, std::vector<int> costs, int generations,int populationPerIsland){
    // Bierze wektor cykli i ich wartości
    // zwraca wektor tylu samu cykli i ich wartości
    int parentsSize =  populationPerIsland;
    int childrenSize = parentsSize*2;
    float mutationProbability = 0.01;
    int tabuListSize = 10;
    // int tabuListSize = N/100;
    // int tabuListSize = parentsSize*10;
    

    
    std::mt19937 gen(rd());
    
    for(int g = 0; g<generations;g++){
        printSum(costs);
        // ustawianie wag
        std::vector<float> weight(costs.size());
        int max = *(std::max_element(costs.begin(),costs.end()));
        // std::cout<<std::endl;
        for(int i=0;i<costs.size();i++){
            // weight[i] = (max+1-costs[i]);
            weight[i] = 1.0/pow(max-costs[i],3);
            // weight[i] = (max+1-costs[i])*(max+1-costs[i])*(max+1-costs[i]);
            // std::cout<<weight[i]<<std::endl;
        }
        // std::cout<<std::endl;
        deb("weights");
        deb("gen"+std::to_string(g));
        // Wybór za pomocą ruletki z wagami

        // std::vector<int> choices = GenParents(weight,gen,parentsSize);
        std::vector<int> choices ;
        for(int i=0;i<parentsSize;i++){
            std::discrete_distribution<> weights(weight.begin(),weight.end());
            int choice = weights(gen);
            // while (std::find(choices.begin(),choices.end(),choice) != choices.end()){
            choice = weights(gen);
            // }
            // weight.erase(weight.begin()+choice);
            weight[choice] = 0;
            choices.push_back(choice);
        }
        
        std::vector<std::vector<int>> parents;
        std::vector<int> parentCosts;
        for(int i=0;i<parentsSize;i++){
            parents.push_back(cycles[choices[i]]);
            parentCosts.push_back(costs[choices[i]]);
        }
        // std::cout<<"Parent: "<<std::endl;
        // printSum(parentCosts); std::cout<<" :Parent"<<std::endl;

        deb("parents chosen");
        // mamy wektor rodziców z ich kosztami
        // krzyżowanie
        std::vector<std::vector<int>> children;
        std::vector<int> childrenCosts;
        for(int i=0;i<childrenSize;i++){
            int a = rng()%parentsSize;
            int b = rng()%parentsSize;
            while(a==b) b = rng()%parentsSize;

            int index = rng()%N;
            
            std::vector<int> child1, child2;
            int cost1,cost2;

            std::tie(child1,child2,cost1,cost2) = PMX(parents[a],parents[a],parents[b],index,N,this);
            children.push_back(child1);
            childrenCosts.push_back(cost1);
            children.push_back(child2);
            childrenCosts.push_back(cost2);
        }   
        
        // std::cout<<"Children: "<<std::endl;
        // printSum(parentCosts); std::cout<<" :Children"<<std::endl;
        // deb("crossed");

        // Mutacje
        // reverse jak w local searchu
        for(int m=0;m<childrenSize*2*mutationProbability;m++){
            int ind = rng()%children.size();
            // std::vector<int> * cycle = &(children[ind]);
            int i = rng()%(N-1);
            int j = i + rng()%(N-1-i) + 1;

            int pi = (N + i - 1) % N;
            int nj = (N + j + 1) % N;
            int difference = - Distance[children[ind][pi]][children[ind][i]] - 
                            Distance[children[ind][j]][children[ind][nj]] +
                            Distance[children[ind][pi]][children[ind][j]] +
                            Distance[children[ind][i]][children[ind][nj]]; 
            childrenCosts[ind] += difference;
            std::reverse(children[ind].begin()+i,children[ind].begin()+j+1);
        }

        deb("mutated");

        // Memetyka
        //Tabu search
        for(int c=0;c<childrenSize;c++){

            std::vector<int> cycle = children[c];
            int cost;
            std::tie(cycle,cost) = ModifiedTabuSearchForGenetics(cycle,tabuListSize);
            children[c] = cycle;
            childrenCosts[c] = cost;
        }
        parents.insert(parents.end(),children.begin(),children.end());
        parentCosts.insert(parentCosts.end(),childrenCosts.begin(),childrenCosts.end());
        cycles = parents;
        costs = parentCosts;
        // cycles = children; 
        // costs = childrenCosts;
    }
    deb("returning");
    // TODO - sortowanie wyniku
    std::vector<std::pair<int,std::vector<int>>> s;
    for(int i=0;i<cycles.size();i++){
        s.push_back(std::make_pair(costs[i],cycles[i]));
    }
    std::sort(s.begin(),s.end());
    for(int i=0;i<cycles.size();i++){
        std::pair p = s[i];
        costs[i] = p.first;
        cycles[i] = p.second;
        // s.push_back(std::make_pair(costs[i],cycles[i]));
    }


    return {cycles,costs};
    // CO CHCE ZWRÓCIĆ
}
