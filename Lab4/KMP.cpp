#include<bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;
ifstream file_in;
ofstream time_out, file_out;

void getNext(const string &substr, vector<int> &next){

    next.clear();
    next.resize(substr.size());
    int j = -1;
    next[0] = -1;
    for(int i = 1; i < substr.size(); ++i){
        while(j > -1 && substr[j + 1] != substr[i])
            j = next[j];
        if(substr[j + 1] == substr[i])
            ++j;
        next[i] = j;
    }
}

void kmp(const string &str, const string &substr, vector<int> &next){

    vector<int> find;
    int cnt = 0;
    getNext(substr, next);
    int j = -1;
    for(int i = 0; i < str.size(); ++i){

        while(j > -1 && substr[j + 1] != str[i])
            j = next[j];
        if(substr[j + 1] == str[i])
            ++j;
        if(j == substr.size() - 1){
            find.push_back(i-substr.size()+1);
            ++cnt;
            j = next[j];
        }
    }
    file_out << cnt << endl;
    for(auto i:next)
          file_out << i + 1 << " ";
    file_out << endl;     
    for(auto i:find)
          file_out << i + 1 << " ";
    file_out << endl << endl;   
}

int main(){

    string from = "C:/Users/ASUS/Desktop/connecting/vscode/AL4/ex1/input/4_1_input.txt";
    string dest = "C:/Users/ASUS/Desktop/connecting/vscode/AL4/ex1/output/result.txt";
    string time = "C:/Users/ASUS/Desktop/connecting/vscode/AL4/ex1/output/time.txt";

    file_in.open(from);
    file_out.open(dest);
    time_out.open(time);

    string str, substr; 
    vector<int> next;
    for(auto i = 0; i < 5; i++){
        file_in >> substr >> str;
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        kmp(str, substr, next);
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        time_out << (chrono::duration_cast<chrono::duration<double>>(t2 - t1)).count() << "s" << endl;
        /*
        if(i==0)
            for(auto j = 188; j < 188+8; j++)
                cout << str[j] << " ";
        */
    }
       
    return 0;
}