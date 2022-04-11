#include<bits/stdc++.h>
using namespace std;
#define maxn 100
vector<int> parent(maxn);
vector<int> rank_(maxn);
int N[5] = {10, 15, 20, 25, 30};
int NUM[5] = {10, 15, 20, 25, 30};

int findset(int x){    //路径压缩启发策略
    if(parent[x]==x)
        return x;
    else
        return parent[x]=findset(parent[x]);
}

void link(int x,int y){  //按秩合并启发策略
    if(rank_[x]>rank_[y]){
        parent[y]=x;
    }
    else{
        parent[x]=y;
        if(rank_[y]==rank_[x])
            rank_[y]=rank_[y]+1;
    }
}

void makeset(int x){
    parent[x]=x;
    rank_[x]=0;
}

void union_(int x, int y, int i){
    int xx = findset(x);
    int yy = findset(y);
    if(xx != yy){
        link(xx, yy);
        NUM[i]--;
    }
}

int main(){

    //vector<chrono::steady_clock::time_point> timelist(10);
    string from = "C:/Users/ASUS/Desktop/AL/AL2/ex2/input/2_2_input.txt";
    string dest = "C:/Users/ASUS/Desktop/AL/AL2/ex2/output/result.txt";
    string time = "C:/Users/ASUS/Desktop/AL/AL2/ex2/output/time.txt";
    ifstream file_in;
    ofstream file_out, time_out;
    file_in.open(from);
    file_out.open(dest);
    time_out.open(time);
    for(auto i = 0; i < 5; i++){
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        for(auto j = 0; j < N[i]; j++)
            makeset(j);
        for(auto j = 0; j < N[i]; j++)
            for(auto k = 0; k < N[i]; k++){
                int tmp = 0;
                file_in >> tmp;
                if(tmp)
                    union_(j, k, i);
            }
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        time_out << (chrono::duration_cast<chrono::duration<double>>(t2 - t1)).count() << 's' << endl;
    }
    for(auto i = 0; i < 5; i++)
        file_out << "n = " << N[i] << " " << NUM[i] << endl;
    return  0;
}