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

    int metric(std::vector<int> a, std::vector<int> b){
        double dist = sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) ); 
        return (int)(round(dist));
    }

    Graph(int n){
        N = n;
    }
    
    Vec2d generateAllEdgesWithCost(){
        Vec2d E;
        E.reserve(N*(N-1)/2);

        for(int a = 0; a < N; a++){
            for(int b = a + 1; b < N; b++){
                E.push_back(
                    std::vector{
                        metric(Pos[a], Pos[b]),
                        a, 
                        b
                    }
                );
            }
        }
        
        return E;
    }

    Graph* MinimalSpanningTree();

    Graph(std::string filename){
        this->filename = filename;
        std::ifstream file;
        file.open("data/"+filename);
        std::string line;

        // load data
        while(getline(file, line)){
            std::stringstream linestream(line);
            std::string word;
            getline(linestream, word, ' ');

            if(word == "NODE_COORD_SECTION")
                break;

            if(word == "DIMENSION"){
                getline(linestream, word, ' ');
                getline(linestream, word, ' ');
                N = std::stoi(word);
            }
        }

        Pos.reserve(N);

        // load points
        for(int i = 0; i < N; i++){
            getline(file, line);
            std::stringstream linestream(line);
            std::string number;

            getline(linestream, number, ' ');
            int n = std::stoi(number);

            getline(linestream, number, ' ');
            int x = std::stoi(number);

            getline(linestream, number, ' ');
            int y = std::stoi(number);
            
            Pos.push_back(std::vector{x,y});
        }

        file.close();
    }
};

class ListGraph : public Graph{
public:
    Vec2d Edges;
    
    ListGraph(int n) : Graph(n){
        Edges = Vec2d(N);
    };   

    void printEdgesToFile(std::string folder, std::string filename){
        std::ofstream res;
        

        res.open(folder + "/" + filename);


        for(int i=0; i<N;i++){
            for(int v : Edges[i])
                if(i<v)
                    res<<i<<" "<<v<<std::endl;
        }
        // for(auto edge : Edges)
        //     res<<edge[0]<<" "<<edge[1]<<std::endl;

        res.close();

    }

    ListGraph DFSTraversal();
    ListGraph(std::string filename) : Graph(filename){};
};

class MatrixGraph : public Graph{
public:
    /// @brief Odległości między wierzchołkami
    Vec2d Distance;
    
    std::random_device rd = std::random_device {}; 
    std::default_random_engine rng = std::default_random_engine { rd() };

    MatrixGraph(std::string filename) : Graph(filename){
        Distance.reserve(N);

        for(int a = 0; a < N; a++){
            Distance.push_back(std::vector<int>(N)); 

            for(int b = 0; b < N; b++){
                int dist = metric(Pos[a], Pos[b]);
                Distance[a][b] = dist;
            }
        }
    }

    MatrixGraph(ListGraph graph) : Graph(graph.N){
        this->filename = graph.filename;
        
        Pos = graph.Pos;
        Distance.reserve(N);

        for(int a = 0; a < N; a++){
            Distance.push_back(std::vector<int>(N)); 

            for(int b = 0; b < N; b++){
                int dist = metric(Pos[a], Pos[b]);
                Distance[a][b] = dist;
            }
        }
    }

    int shuffle(int minRange);
};