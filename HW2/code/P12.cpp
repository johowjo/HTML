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

double train(int seed){
  vector<pair<double, bool> > data(N);
  init_data(data, seed);
  sort(data.begin(), data.end());

  srand(seed);
  bool s = (double(rand()) / double(RAND_MAX)) > 0.5;
  double t = double(rand()) / double(RAND_MAX) * 2 - 1;
  int count = 0;
  for(auto d : data){
    bool y = ((s ? 1 : -1) * ((d.first - t) > 0 ? 1 : -1)) == 1;
    if(y != d.second) count++;
  }
  double E_in = double(count) / N;
  double v = (s ? 1 : -1) * (0.5 - p);
  double u = 0.5 - v;
  double E_out = u + v * abs(t);
  data_out << E_in << ' ' << E_out << '\n';
  return E_out - E_in;
}


int main(){
  data_out.open("./12data.txt");
  vector<double> dif;
  for(int i = 0; i < 2000; i++){
    dif.push_back(train(i));
  }
  cout << "median: " << (dif[999] + dif[1000]) / 2;

  data_out.close();
  return 0;
}
