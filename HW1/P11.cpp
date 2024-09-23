#include <cstdlib>
#include <cmath>
#include <ctime>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

const int N = 200;
const int n = 47205;
int mnm = 1000;
int update_times[5000] = {0};

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

  int update(data_vector example){
    double inner_product = 0;
    for(pair<int, double> p : example.x){
      if(w[p.first] == 0) continue;
      inner_product += w[p.first] * p.second;
    }
    // cout << "inner product: " << inner_product << '\n';

    if(sign(inner_product) == example.y) return 0;
    double length = 0;
    for(pair<int, double> p : example.x) length += p.second * p.second;
    int count = 0;
    if(example.y){
      while(sign(inner_product) != example.y){
        count++;
        inner_product += length;
      }
    }
    else{
      while(sign(inner_product) != example.y){
        count++;
        inner_product -= length;
      }
    }

    if(example.y){
      for(pair<int, double> p : example.x){
        w[p.first] += count * p.second;
      }
    }
    else{
      for(pair<int, double> p : example.x){
        w[p.first] -= count * p.second;
      }
    }

    return count;
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

  srand(seed);

  while(count < 1000){
    double random = double(rand()) / double(RAND_MAX);
    int index = 199 * random + 1;
    int update_times = weight.update(example_set[index]);

    if(update_times == 0){
      count++;
    }
    else{
      count = 0;
      update_count += update_times;
    }
  }

  if(update_count >= 5000){
    cout << "uh-oh out of bounds";
    return weight;
  }
  update_times[update_count]++;
  cout << update_count << '\n';
  mnm = min(update_count, mnm);
  return weight;
}


int main(){
  init_example_set();
  for(int i = 1; i <= 1000; i++){
    cout << i << ' ';
    train(i);
  }
  cout << mnm;
  return 0;
}

