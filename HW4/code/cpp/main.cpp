#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

const int N = 64;
const int n = 8192;
const int d = 12;
const double eta = 0.01;

vector<double> E_in;
vector<double> E_out;

class data_vector {
public:
  double x[d + 1];
  double y;
  void add(data_vector other);
  void sub(data_vector other);
};

class weight_vector {
public:
  double x[d + 1];
  void update(data_vector x);
  weight_vector() {
    memset(x, 0, sizeof(x));
  }
};

double inner_product(data_vector x, weight_vector w) {
  double tmp = 0;
  for (int i = 0; i <= d; i++) {
    tmp += x.x[i] * w.x[i];
  }
  return tmp;
}

void weight_vector::update(data_vector x) {
  double _err = inner_product(x, *this) - x.y;
  for (int i = 0; i <= d; i++) {
    this->x[i] -= eta * _err * x.x[i];
  }
}

data_vector dataset[n];

void parse(){
  ifstream data_in;
  data_in.open("./data.txt");
  for(int i = 0; i < n; i++){
    string tmp;
    getline(data_in, tmp);
    vector<int> spaces;
    vector<int> cols;
    for(int j = 0; j < tmp.size(); j++){
      if(tmp[j] == ' ') spaces.push_back(j);
      if(tmp[j] == ':') cols.push_back(j);
    }
    dataset[i].y = stod(tmp.substr(0, spaces[0]));
    dataset[i].x[0] = 1;
    for(int j = 0; j < 11; j++){
      dataset[i].x[j + 1] = stod(tmp.substr(cols[j] + 1, spaces[j + 1]));
    }
    dataset[i].x[12] = stod(tmp.substr(cols[11] + 1, tmp.size()));
  }
  data_in.close();
}

void sgd(int seed, int t) {
  weight_vector w;
  srand(seed);
  for(int i = 0; i < 10000; i++) {
    int rand_n = rand() % n;
    w.update(dataset[rand_n]);
  }
  if(t % 200 != 0) return;

}

int main() {
  // parse();
  mt19937 generator(0);

  return 0;
}
