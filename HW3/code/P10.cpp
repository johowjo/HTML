#include "eigen-3.4.0/Eigen/Dense"
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>

#define pb push_back

using namespace std;

const int K = 8192;
const int d = 12;
const int N = 32;

vector<double> data_vec[K];
int y[K];

void parse(string s, int n){
  int ind = 0;
  while(s[ind] != ' ') ind++;
  y[n] = stod(s.substr(0, ind));
  vector<int> col;
  vector<int> spa;
  for(int i = ind + 1; i < s.size(); i++){
    if(s[i] == ' ') spa.pb(i);
    if(s[i] == ':') col.pb(i);
  }
  if(col.size() != 12){
    cout << "parse error!";
    return;
  }
  for(int i = 0; i < 11; i++){
    data_vec[n].pb(stod(s.substr(col[i] + 1, spa[i])));
  }
  data_vec[n].pb(stod(s.substr(col[11] + 1, s.size())));
}

void init_data() {
  ifstream data_in;
  data_in.open("./data.txt");
  for(int i = 0; i < K; i++){
    string tmp;
    getline(data_in, tmp);
    data_vec[i].pb(double(1));
    parse(tmp, i);
  }
  data_in.close();
}

void run(ofstream& data_out, int seed){
  // pick N random samples
  vector<int> sam;
  srand(seed);
  while(true){
    if(sam.size() == N) break;
    int ind = int((double(rand()) / double(RAND_MAX)) * K);
    if(find(sam.begin(), sam.end(), ind) != sam.end()) continue;
    sam.pb(ind);
  }
  // create X matrix and y vector
  Eigen::Matrix<double, N, d + 1> mat;
  Eigen::Matrix<double, N, 1> y_vec;
  for(int i = 0; i < sam.size(); i++){
    y_vec(i, 0) = y[sam[i]];
    for(int j = 0; j <= d; j++){
      mat(i, j) = data_vec[sam[i]][j];
    }
  }
  // get weight vector
  auto pinv = mat.completeOrthogonalDecomposition().pseudoInverse();
  auto w = pinv * y_vec;
  // calcualte E
  double in = 0;
  // int in_count = 0;
  double out = 0;
  // int out_count = 0;
  for(int i = 0; i < K; i++){
    Eigen::Matrix<double, 1, d + 1> x_vec;
    for(int j = 0; j <= d; j++){
      x_vec(0, j) = data_vec[i][j];
    }
    double result = (w * x_vec)(0, 0);
    double err = (result - y[i]) * (result - y[i]);
    if(find(sam.begin(), sam.end(), i) != sam.end()){
      out += err;
      // out_count++;
    }
    else{
      in += err;
      // in_count++;
    }
  }
  double E_in = in / (K - N);
  double E_out = out / N;
  data_out << E_in << ' ' << E_out << '\n';
  cout << E_in << ' ' << E_out << '\n';
  // cout << in_count << ' ' << out_count;
}


// void test_eigen(){
//   Eigen::MatrixXd A(2, 2);
//   A(0, 0) = 1;
//   A(0, 1) = 1;
//   A(1, 0) = 2;
//   A(1, 1) = 4;
//   Eigen::MatrixXd pinv = A.completeOrthogonalDecomposition().pseudoInverse();
//   cout << pinv;
// }

int main(){
  init_data();
  ofstream data_out;
  data_out.open("./tmp.txt");
  run(data_out, 0);
  data_out.close();

  return 0;
}
