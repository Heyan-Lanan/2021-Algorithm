#include<bits/stdc++.h>
#include<vector>
using namespace std;
vector<int> dimension = {27, 27, 81, 81, 243, 243, 729, 729};
vector<string> suffix = {"11", "12", "21", "22", "31", "32", "41", "42"};

void Bellman_Ford(vector<vector<int>> w, vector<int> &d, vector<int> &pi, vector<pair<int, int>> E){
    //cout << w.size() << endl << E.size()<< endl;
    for(auto i = 0; i < w.size() ; i++)
        for(auto edge: E){
            int u = edge.first, v = edge.second;
            if(d[v] > d[u] + w[u][v] && d[u] != 20000000){
                d[v] = d[u] + w[u][v];
                pi[v] = u;
            }
        }         
}

int main(){

    string from = "C:/Users/ASUS/Desktop/connecting/vscode/test/ex1/input/input";
    string dest = "C:/Users/ASUS/Desktop/connecting/vscode/test/ex1/output/result";
    string time = "C:/Users/ASUS/Desktop/connecting/vscode/test/ex1/output/time.txt";
    ofstream time_out;
    time_out.open(time);
    for(auto i = 0; i < 8; i++){

        ifstream file_in;
        ofstream file_out;
        vector<vector<int>> w(dimension[i], vector<int>(dimension[i], 30000000));
        vector<int> d(dimension[i], 20000000);
        vector<int> pi(dimension[i], -1);
        vector<pair<int, int>> E;
        d[0] = 0;
        file_in.open(from + suffix[i] + ".txt");
        char tmp_c;
        int tmp_i;
        for(auto j = 0; j < dimension[i]; j++)
            for(auto k = 0; k < dimension[i]; k++){
                file_in >> tmp_i;
                file_in >> tmp_c;
                if(tmp_i){
                    w[j][k] = tmp_i;
                    E.push_back(pair<int, int>(j, k));
                }
                    
            }


        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();       
        Bellman_Ford(w, d, pi, E);
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();

        file_out.open(dest + suffix[i] + ".txt");
/*
        for(auto edge: E){
            int u = edge.first, v = edge.second;
            file_out << u << " " << v << endl;
        }   
*/
        for(auto j = 0; j < dimension[i]; j++){
            if(pi[j] != -1){
                file_out << "0," << j << "," << d[j] << ";";
                int tmp = j;
                stack<int> path;
                while(tmp != 0)
                    path.push(tmp = pi[tmp]);
                while(!path.empty()){
                    file_out << path.top() << ",";
                    path.pop();
                }        
                file_out << j << endl;
            }            
        }
        time_out << (chrono::duration_cast<chrono::duration<double>>(t2 - t1)).count() << "s" << endl;               
    }
    return 0;
}