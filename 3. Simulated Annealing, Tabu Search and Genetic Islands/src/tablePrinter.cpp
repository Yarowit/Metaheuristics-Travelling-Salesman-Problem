#include <iostream>
#include <vector>
using namespace std;
void print(vector<string> columnNames, vector<string> rowNames, vector<vector<int>> values){
    int r,k;
    cout<<"\\hline\n";
    for(k=0;k<columnNames.size()-1;k++)
        cout<<columnNames[k]<<" & ";
    cout<<columnNames[columnNames.size()-1]<<"\\\\"<<std::endl;
    
    for(r=0;r<rowNames.size();r++){
        cout<<"\\hline \n"<<rowNames[r];
        for(k=0;k<columnNames.size()-1;k++){
            cout<<" & "<<values[r][k]; 
        }
        cout<<"\\\\"<<std::endl;
    }
    cout<<"\\hline\n";
}