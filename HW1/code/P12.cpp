#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

const int N = 200;
const int n = 47205;
ofstream data_out;
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

    x.push_back(make_pair(0, 1));
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

  int update1(data_vector example){
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

  bool update2(data_vector example){
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

void train(int seed, vector<int> &v1, vector<int> &v2){
  weight_vector w1;
  weight_vector w2;
  int count1 = 0;
  int count2 = 0;
  int update_count1 = 0;
  int update_count2 = 0;

  srand(seed);

  while(count1 < 1000){
    double random = double(rand()) / double(RAND_MAX);
    int index = 199 * random + 1;
    bool if_update = w1.update1(example_set[index]);

    if(!if_update){
      count1++;
    }
    else{
      count1 = 0;
      update_count1++;
    }
  }

  while(count2 < 1000){
    double random = double(rand()) / double(RAND_MAX);
    int index = 199 * random + 1;
    int update_times = w2.update1(example_set[index]);

    if(update_times == 0){
      count2++;
    }
    else{
      count2 = 0;
      update_count2 += update_times;
    }
  }

  v1.push_back(update_count1);
  v2.push_back(update_count2);
}


int main(){
  init_example_set();
  data_out.open("./P12_data.txt");
  for(int i = 0; i < 100; i++){
    vector<int> v1;
    vector<int> v2;
    for(int j = 0; j < 1000; j++){
      train(1000 * i + j, v1, v2);
    }
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    int m1 = (v1[499] + v1[500]) / 2;
    int m2 = (v2[499] + v2[500]) / 2;

    cout << m1 << ' ' << m2 << '\n';
    data_out << m1 << ' ' << m2 << '\n';
  }
  data_out.close();
  return 0;
}

