#include <fstream>
#include <iostream>
#include <cstdlib>
#include <utility>
using namespace std;

const double p = 0.15;
const int N = 12;
ofstream data_out;

void init_data(vector<pair<double, bool> > &data, int seed){
  if(data.size() != N){
    cout << "data set size incorrect!\n";
    return;
  }

  srand(seed);
  for(int i = 0; i < N; i++){
    double x = double(rand()) / double(RAND_MAX) * 2 - 1;
    bool y = x > 0;
    double tmp = double(rand()) / double(RAND_MAX);
    if(tmp < 0.15) y = !y;
    data[i] = make_pair(x, y);
  }
}

struct result{
  pair<bool, double> x;
  double E_in;

  result(pair<bool, double> p, double d){
    x = p;
    E_in = d;
  }
};

result train(int seed){
  vector<pair<double, bool> > data(N);
  init_data(data, seed);
  sort(data.begin(), data.end());

  vector<double> theta;
  theta.push_back(-1);
  for(int i = 0; i < N - 1; i++){
    if(data[i].first == data[i + 1].first) continue;
    theta.push_back((data[i].first + data[i + 1].first) / 2);
  }
  pair<int, double> mnm = make_pair(1, -1);
  int mnm_count = N + 1;
  for(double t : theta){
    bool s = true;
    while(true){
      int count = 0;
      for(auto d : data){
        bool y = ((s ? 1 : -1) * ((d.first - t) > 0 ? 1 : -1)) == 1;
        if(y != d.second) count++;
      }
      if(count < mnm_count){
        mnm = make_pair(s ? 1 : -1, t);
        mnm_count = count;
      }
      else if(count == mnm_count){
        if(double(mnm.first) * mnm.second > (s ? 1 : -1) * t) mnm = make_pair(s ? 1 : -1, t); 
      }
      if(s) s = false;
      else break;
    }
  }

  double E_in = double(mnm_count) / N;
  data_out << E_in << ' ';
  return result(mnm, E_in);
}


int main(){
  data_out.open("./11data.txt");
  vector<double> dif; 
  for(int i = 0; i < 2000; i++){
    auto r = train(i);
    auto g = r.x;
    int s = g.first ? 1 : -1;
    double theta = g.second;
    double v = s * (0.5 - p);
    double u = 0.5 - v;
    double E_out = u + v * abs(theta);
    data_out << E_out << '\n';
    dif.push_back(E_out - r.E_in);
  }
  cout << "median: " << (dif[999] + dif[1000]) / 2;
  

  data_out.close();
  return 0;
}
