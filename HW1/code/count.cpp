#pragma GCC Optimize("Ofast")
#include <bits/stdc++.h>
#define IOIOOIOIO ios::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define int long long
#define pii pair<int, int>
#define mp make_pair
#define ff first
#define ss second
#define pb push_back
#define yy "yEs\n"
#define nn "nO\n"
using namespace std;


void solve(){
  ifstream data_in;
  data_in.open("./P12_data.txt");
  // 101, 101
  int case1;
  // 101, 102
  int case2;
  // 102, 101
  int case3;
  // 102, 102
  int case4;
  for(int i = 1; i <= 100; i++){
    string tmp;
    getline(data_in, tmp);
    int ind = 0;
    while(tmp[ind] != ' ') ind++;
    int m1 = stoi(tmp.substr(0, ind));
    int m2 = stoi(tmp.substr(ind + 1, tmp.size()));
    if(m1 == 101 && m2 == 101) case1++;
    else if(m1 == 101 && m2 == 102) case2++;
    else if(m1 == 102 && m2 == 101) case3++;
    else if(m1 == 102 && m2 == 102) case4++;
  }
  cout << "101, 101: " << case1 << '\n';
  cout << "101, 102: " << case2 << '\n';
  cout << "102, 101: " << case3 << '\n';
  cout << "102, 102: " << case4 << '\n';
  data_in.close();
}

signed main(){
  IOIOOIOIO; 
  solve();
  return 0;
}

