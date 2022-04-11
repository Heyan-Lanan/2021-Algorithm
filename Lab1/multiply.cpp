#include <iostream>
#include<bits/stdc++.h>
#include <algorithm>
#include <chrono>

using namespace std;

ofstream file_out, time_out;
const int maxn = 100 + 50;
double time_list[5];
void print(int s[][150], int i, int j){

    if(i == j)
        file_out << "A" << i;
    else{
        file_out << "(";
        print(s, i, s[i][j]);
        print(s, s[i][j] + 1, j);
        file_out << ")";
    }
}

int main(){
    string from = "C:\\Users\\ASUS\\Desktop\\AL\\ex1\\input\\1_1_input.txt";
    string dest = "C:\\Users\\ASUS\\Desktop\\AL\\ex1\\output\\result.txt";
    string time = "C:\\Users\\ASUS\\Desktop\\AL\\ex1\\output\\time.txt";
    ifstream file_in;
    file_in.open(from);

    file_out.open(dest);
    time_out.open(time);
    for(int i = 0; i < 5; i++){


        long long nums[maxn];
        long long dp[maxn][maxn];
        int s[maxn][maxn];
        int N;
        file_in >> N;
        for (int i = 0; i <= N; i++)
            file_in >> nums[i];

        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        for(int i = 1; i<= N; i++)
            dp[i][i] = 0;
        for (int len = 2; len <= N; len++) {
            for (int i = 1; i <= N - len + 1; i++) {
                int j = i + len - 1;
                dp[i][j] = 9223372036854775807;
                for (int k = i; k < j; k++) {
                    long long tmp = dp[i][k] + dp[k + 1][j] + nums[i-1] * nums[k] * nums[j];
                    if(tmp < dp[i][j]){
                        dp[i][j] = tmp;
                        s[i][j] = k;
                    }
                }
            }
        }
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        file_out << dp[1][N] << endl;
        print(s, 1, N);
        file_out << endl;
        chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        time_list[i] = time_span.count();
        if(i == 0){
            for(int j = 1; j<= N; j++){
                for(int l = j; l <= N; l++)
                    cout << "m[" << j << "][" << l << "]=" << dp[j][l] << " ";
                cout << endl;
            }
            for(int j = 1; j<= N-1; j++){
                for(int l = j+1; l <= N; l++)
                    cout << "s[" << j << "][" << l << "]=" << s[j][l] << " ";
                cout << endl;
            }

        }
    }
    for(int i = 0; i < 5; i++){
        time_out << time_list[i] << "s" << endl;
    }
    return 0;
}