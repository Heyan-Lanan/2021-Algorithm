#include<bits/stdc++.h>
#include<vector>
using namespace std;
vector<int> dimension = {27, 27, 81, 81, 243, 243, 729, 729};
vector<string> suffix = {"11", "12", "21", "22", "31", "32", "41", "42"};

void Bellman_Ford(vector<vector<int>> w, vector<int> &d, vector<pair<int, int>> E){
    for(auto i = 0; i < w.size() ; i++)
        for(auto edge: E){
            int u = edge.first, v = edge.second;
            if(d[v] > d[u] + w[u][v]){
                d[v] = d[u] + w[u][v];
            }
        }
}

void Dijkstra(vector<vector<int>> w, vector<pair<int, int>> &dj, vector<int> &result){

    while(!dj.empty()){
        auto min = dj.front();
        int min_at = 0;
        for(auto i = 0; i < dj.size(); i++){
            if(dj[i].second < min.second){
                min = dj[i];
                min_at = i;
            }
        }
            
        result[min.first] = min.second;
        //cout << min.first << ":" << result[min.first] << " ";
        dj.erase(dj.begin() + min_at);
        for(auto i = 0; i < dj.size(); i++)
            if(dj[i].second > min.second + w[min.first][dj[i].first])
                dj[i].second = min.second + w[min.first][dj[i].first];
    }
    //cout << endl;
}

int main(){

    string from = "C:/Users/ASUS/Desktop/connecting/vscode/test/ex2/input/input";
    string dest = "C:/Users/ASUS/Desktop/connecting/vscode/test/ex2/output/result";
    string time = "C:/Users/ASUS/Desktop/connecting/vscode/test/ex2/output/time.txt";
    ofstream time_out;
    time_out.open(time);

    for(auto i = 0; i < 8; i++){
        ifstream file_in;
        ofstream file_out;
        vector<vector<int>> w(dimension[i]+1, vector<int>(dimension[i]+1, 20000000));
        vector<pair<int, int>> E;
        vector<int> d(dimension[i] + 1, 0);
        vector<vector<int>> duv(dimension[i], vector<int>(dimension[i]));
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

        for(auto k = 0; k < dimension[i]; k++){
            w[dimension[i]][k] = 0;
            E.push_back(pair<int, int>(dimension[i], k));
        }

        Bellman_Ford(w, d, E);
        vector<int> h(d);
        // for(auto j:h)
        //    cout<<j<< " ";
        for(auto edge: E){
            int u = edge.first, v = edge.second;
            w[u][v] = w[u][v] + h[u] - h[v];

        }

        file_out.open(dest + suffix[i] + ".txt");
        for(auto j = 0; j < dimension[i]; j++){

            vector<pair<int, int>> dj(dimension[i]);
            for(auto k = 0; k < dimension[i]; k++)
                dj[k] = pair<int, int>(k, 20000000);
            dj[j] = pair<int, int>(j, 0);
            vector<int> result(dimension[i], 20000000);
            Dijkstra(w, dj, result);
            for(auto k = 0; k < dimension[i]; k++){
                if(result[k]!=20000000)
                    file_out << result[k] + h[k] - h[j] << " ";
                else
                    file_out << "X" << " ";
            }
            file_out << endl;

        }
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        time_out << (chrono::duration_cast<chrono::duration<double>>(t2 - t1)).count() << "s" << endl;
    }
    return 0;
}