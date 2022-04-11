#include<bits/stdc++.h>
#include <chrono>
using namespace std;

int N;
string X ;
string Y ;
vector<vector<int>> c; // 动态规划表
set<string> lcs;
vector<double> time_list(5);

int lcs_length(int m, int n){
    // 表的大小为(m+1)*(n+1)
    c = vector<vector<int> >(m+1,vector<int>(n+1));

    for(int i=0; i<m+1; ++i){
        for(int j=0; j<n+1; ++j){
            // 第一行和第一列置0
            if (i == 0 || j == 0)
                c[i][j] = 0;

            else if(X[i-1] == Y[j-1])
                c[i][j] = c[i-1][j-1] + 1;

            else
                c[i][j] = max(c[i-1][j], c[i][j-1]);
        }
    }

    return c[m][n];
}


void lcs_print(int i, int j, string lcs_str){
    while (i>0 && j>0){
        if (X[i-1] == Y[j-1]){
            lcs_str.push_back(X[i-1]);
            --i;
            --j;
        }

        else{
            if (c[i-1][j] > c[i][j-1])
                --i;
            else if (c[i-1][j] < c[i][j-1])
                --j;
            else{
                lcs_print(i-1, j, lcs_str);
                lcs_print(i, j-1, lcs_str);
                return;
            }
        }
    }
    reverse(lcs_str.begin(),lcs_str.end());
    lcs.insert(lcs_str);
}


int main(){
    string from = "C:\\Users\\ASUS\\Desktop\\AL\\ex2\\input\\1_2_input.txt";
    string time = "C:\\Users\\ASUS\\Desktop\\AL\\ex2\\output\\time.txt";
    ifstream file_in;
    file_in.open(from);
    ofstream time_out;
    time_out.open(time);
    for(int i = 0; i < 5; i++){

        file_in>>N;
        file_in>>X>>Y;
        int m = X.length();
        int n = Y.length();
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        int length = lcs_length(m, n);
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();

        //cout << "The length of LCS is " << length << endl;
        string str;
        lcs_print(m, n, str);
        ofstream file_out;
        file_out.open("C:\\Users\\ASUS\\Desktop\\AL\\ex2\\output\\result_" + to_string(N) + ".txt");
        file_out << "The number of LCSs is " << lcs.size() << endl;
        set<string>::iterator it = lcs.begin();
        for( ; it!=lcs.end(); it++)
            file_out << *it << endl;
        lcs.clear();

        chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        time_list[i] = time_span.count();
    }
    for(int i = 0; i < 5; i++)
        time_out << time_list[i] << "s" << endl;
    return 0;
}

