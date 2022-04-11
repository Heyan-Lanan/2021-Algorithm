#include<bits/stdc++.h>
#include<math.h>
#include <chrono>
using namespace std;
ifstream file_in;
ofstream time_out, file_out;
vector<vector<double>> time_list(5, vector<double>(5));
vector<int> D = {2, 2, 10, 10};
vector<int> Q = {13, 1009, 13, 1009};
 
int RABIN_KRAP_MATCHER(string T, string P, int d, int q, int &false_count, vector<int> &find){

    find.clear();
    false_count = 0;
    int n = T.length(), m = P.length();
	int p = 0, t = 0;
    int h = 1;
    for(auto i=0;i<m-1;i++){
        h = h * d % q;       
    }
	    
	for(auto i=0;i<m;i++){
		p = (d * p + P[i]) % q;		
		t = (d * t + T[i]) % q;
	}
	for(auto s = 0; s <= n - m; s++){
		if(p == t){
			if(T.substr(s, m) == P)
				find.push_back(s);              
            else
                false_count++;
		}
		t = (d * (t - T[s] * h % q) + T[s + m]) % q;
		if(t<0)
		    t += q;
	}	
} 
 
int main(){

    string from = "C:/Users/ASUS/Desktop/connecting/vscode/AL4/ex2/input/4_2_input.txt";
    string dest = "C:/Users/ASUS/Desktop/connecting/vscode/AL4/ex2/output/result.txt";
    string time = "C:/Users/ASUS/Desktop/connecting/vscode/AL4/ex2/output/time.txt";

    file_in.open(from);
    file_out.open(dest);
    time_out.open(time);

    for(auto i = 0; i < 5; i++){
        string T, P;
        vector<int> find;
        int count, false_count;
        file_in >> P >> T;

        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        RABIN_KRAP_MATCHER(T, P, 2, 13, false_count, find);
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now(); 
        chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        time_list[i][0] = time_span.count();

        file_out << find.size() << endl << false_count << " ";

        t1 = chrono::steady_clock::now();
        RABIN_KRAP_MATCHER(T, P, 2, 1009, false_count, find);
        t2 = chrono::steady_clock::now(); 
        time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        time_list[i][1] = time_span.count();

        file_out << false_count << " ";

        t1 = chrono::steady_clock::now();
        RABIN_KRAP_MATCHER(T, P, 10, 13, false_count, find);
        t2 = chrono::steady_clock::now(); 
        time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);        
        time_list[i][2] = time_span.count();

        file_out << false_count << " ";

        t1 = chrono::steady_clock::now();
        RABIN_KRAP_MATCHER(T, P, 10, 1009, false_count, find);
        t2 = chrono::steady_clock::now(); 
        time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);        
        time_list[i][3] = time_span.count();

        file_out << false_count << endl;
        for(auto j:find)
            file_out << j + 1 << " ";
        file_out << endl << endl;
        /*
        if(i==0)
            for(auto j = 78; j < 78+8; j++)
                cout << T[j] << " ";
        */
    }
    for(auto i = 0; i < 4; i++){
        time_out << "(" << D[i] << "," << Q[i] << ")" << endl;
        for(auto j = 0; j < 5; j++)
            time_out << time_list[j][i] << "s" << " ";
        time_out << endl;
    }
	
	return 0;
}