#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <bits/stdc++.h>

using Vec2d = std::vector<std::vector<int>>;


class DisjoinedSet : public std::vector<std::set<int>>{
public:
    /// @brief Wrzuca element do zbioru zbiorów
    /// @param a element
    /// @param b element
    /// @return Czy dodano element
    bool insert(int a, int b);
};



class Graph{
public:
    std::string filename;
    /// @brief Liczba wierzchołków
    int N;
    /// @brief Pozycje wierzchołków 
    Vec2d Pos;

    /// @brief Odległości między wierzchołkami
    /// Distance[a][b] = metric(a,b);
    Vec2d Distance;
    
    std::random_device rd; 
    std::default_random_engine rng = std::default_random_engine { rd() };

    
    class MST{
    public:
        Graph* graph;
        /// @brief Edges[i] = lista sąsiadów i
        Vec2d Edges;
        int cost = 0;
        MST(){}
        MST(Graph* parent){
            graph = parent;
            Edges = Vec2d(graph->N);
        }

        void printEdgesToFile(std::string folder, std::string filename){
            std::ofstream res;
            
            res.open(folder + "/" + filename);

            for(int i=0; i<graph->N;i++){
                for(int v : Edges[i])
                    if(i<v)
                        res<<i<<" "<<v<<std::endl;
            }
            
            res.close();
        }
    
        MST DFSTraversal(int startingPoint);

        // generuje sam cykl DFSa
        std::vector<int> DFSCycle(int startingPoint);
        // wywołujemy dla jakiegoś DFSa (cyklu)
    };
    // zwraca dwójkę {wynik, liczba iteracji}
    std::vector<int> LocalSearch(std::vector<int> cycle);
    //Wybiera najlepszego z n sąsiadów
    std::vector<int> LocalSearchBestOfN(std::vector<int> cycle);

    std::vector<int> SimulatedAnnealing(std::vector<int> cycle, float startingTemp, float tempMultiplier, int repeatsPerEpoch);

    std::vector<int> TabuSearch(std::vector<int> cycle, int tabuListSize);
    std::vector<int> ModifiedTabuSearch(std::vector<int> cycle, int tabuListSize);
    
    int Genetic(int islands,int populationPerIsland, int generationsPerEpoch, int Epochs);
    std::tuple<std::vector<std::vector<int>>,std::vector<int>> Island(std::vector<std::vector<int>> cycles,std::vector<int> costs, int generations, int populationPerIsland);
    std::tuple<std::vector<int>,int> ModifiedTabuSearchForGenetics(std::vector<int> cycle, int tabuListSize);

    bool Accept(float T, float difference);

    /// @brief Minimalne drzewo rozpinające
    MST mst;

    int metric(std::vector<int> a, std::vector<int> b){
        double dist = sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) ); 
        return (int)(round(dist));
    }
    
    void GenerateMST();
    
    int shuffle(int minRange);

    Graph(std::string folder,std::string filename){
        this->filename = filename;
        std::ifstream file;
        file.open(folder+"/"+filename);
        std::string line;
        // load data
        while(getline(file, line)){
// std::cout<<"L "<<line<<std::endl;
            std::stringstream linestream(line);
            std::string word;
            getline(linestream, word, ' ');

            // std::cout<<"w "<<word<<std::endl;
            std::string ncs = "NODE_COORD_SECTION";
            // std::cout<<"wncs "<<word.compare(ncs)<<" "<<std::endl;
            if(word==ncs)
            // if(word.compare("NODE_COORD_SECTION")==0)
                break;
const std::string dim =  "DIMENSION";
            if(word== dim){
            // if(word.compare("DIMENSION")==0){
                getline(linestream, word, ' ');
                getline(linestream, word, ' ');
            // std::cout<<"ww "<<word<<std::endl;
                N = std::stoi(word);
                
            // std::cout<<"n "<<N<<std::endl;
            getline(file,line);
            getline(file,line);
            break;
            }
        }

        Pos.reserve(N);

// std::cout<<":)"<<std::endl;
        // load points
        for(int i = 0; i < N; i++){
// std::cout<<":()"<<i<<std::endl;
            getline(file, line);
            std::stringstream linestream(line);
            std::string number;

            getline(linestream, number, ' ');
// std::cout<<":(:"<<number<<std::endl;
            int n = std::stoi(number);

            getline(linestream, number, ' ');
            int x = std::stoi(number);

            getline(linestream, number, ' ');
            int y = std::stoi(number);
            
            Pos.push_back(std::vector<int>{x,y});
        }

// std::cout<<":)"<<std::endl;
        file.close();
// std::cout<<":)"<<std::endl;

        // załaduj Distance
        Distance.reserve(N);

        for(int a = 0; a < N; a++){
            Distance.push_back(std::vector<int>(N)); 

            for(int b = 0; b < N; b++){
                int dist = metric(Pos[a], Pos[b]);
                Distance[a][b] = dist;
            }
        }

        // std::cout<<"N: "<<N<<std::endl;
    }
};

