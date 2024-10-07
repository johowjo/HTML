#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

const int N = 200;
const int n = 47205;
const int T = 74;
int mnm = 1000;
ofstream data_out;

bool sign(double a){
  if(a <= 0) return false;
  return true;
}

class data_vector{
public:
  bool y;
  vector<pair<int, double> > x;

  void init(string data_string){
    y = true;
    if(data_string[1] != ' '){
      y = false;
    }

    vector<int> indexes;
    for(int i = 0; i < data_string.length(); i++){
      if(data_string[i] == ' ') indexes.push_back(i);
    }
    indexes.push_back(data_string.length());
    x.push_back((make_pair(0, 1)));

    for(int i = 0; i < indexes.size() - 1; i++){
      string data_substr = data_string.substr(indexes[i], indexes[i + 1]);
      int pos;
      for(int j = 0; j < data_substr.length(); j++){
        if(data_substr[j] == ':'){
          pos = j;
          break;
        }
      }

      int index = stoi(data_substr.substr(0, pos));
      double value = stod(data_substr.substr(pos + 1, data_substr.length()));
      x.push_back(make_pair(index, value));
    }
  }

  data_vector(){}
};

class weight_vector{
public:
  vector<double> w;

  weight_vector(){
    w.assign(n + 1, 0);
  }

  bool update(data_vector example){
    double inner_product = 0;
    for(pair<int, double> p : example.x){
      if(w[p.first] == 0) continue;
      inner_product += w[p.first] * p.second;
    }
    // cout << "inner product: " << inner_product << '\n';

    if(sign(inner_product) == example.y) return false;

    if(example.y){
      for(pair<int, double> p : example.x){
        w[p.first] += p.second;
      }
    }
    else{
      for(pair<int, double> p : example.x){
        w[p.first] -= p.second;
      }
    }

    return true;
  }
};

data_vector example_set[N + 1];

void init_example_set(){
  ifstream data_in;
  data_in.open("./data.binary");
  for(int i = 1; i <= N; i++){
    string tmp;
    getline(data_in, tmp);
    // cout << tmp[1];
    example_set[i].init(tmp);
  }
  data_in.close();
}

weight_vector train(int seed){
  weight_vector weight;
  int count = 0;
  int update_count = 0;
  data_out << 0 << ' ';

  srand(seed);

  while(count < 1000){
    double random = double(rand()) / double(RAND_MAX);
    int index = 199 * random + 1;
    bool if_update = weight.update(example_set[index]);

    if(!if_update){
      count++;
      continue;
    }
    else{
      count = 0;
      update_count++;
    }

    if(update_count > T) continue;
    double length = 0;
    for(double d : weight.w) length += d * d;
    length = sqrt(length);
    data_out << length << ' ';
    if(update_count == T) data_out << '\n';
  }

  cout << update_count << '\n';
  mnm = min(update_count, mnm);
  return weight;
}


int main(){
  init_example_set();
  data_out.open("./P11_data.txt");
  for(int i = 1; i <= 1000; i++){
    cout << i << ' ';
    train(i);
  }
  data_out.close();
  cout << mnm;
  return 0;
}

