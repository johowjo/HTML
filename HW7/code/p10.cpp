#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// int train_data[2000][500];
int train_data[500][2000];
int test_data[500][600];
int label[2000];
int test_label[600];
vector<int> theta[500];
double u[2000];

int dim[500];
int s[500];
int t[500];
double a[500];

void read_data() {
  fstream train_in("./train_data.txt");
  fstream test_in("./test_data.txt");

  string tmp;

  int count = 0;
  while(getline(train_in, tmp)) {
    vector<int> spac;
    vector<int> cols;

    for(int i = 0; i < tmp.size(); i++) {
      if(tmp[i] == ' ') spac.push_back(i);
      if(tmp[i] == ':') cols.push_back(i);
    }

    if(tmp[0] == '-') {
      label[count] = -1;
    } else {
      label[count] = 1;
    }

    for(int i = 0; i < 499; i++) {
      train_data[i][count] = stoi(tmp.substr(cols[i] + 1, spac[i + 1]));
    }
    train_data[499][count] = stoi(tmp.substr(cols[499] + 1, tmp.size()));
    

    count++;
  }

  count = 0;
  while(getline(test_in, tmp)) {
    vector<int> spac;
    vector<int> cols;

    for(int i = 0; i < tmp.size(); i++) {
      if(tmp[i] == ' ') spac.push_back(i);
      if(tmp[i] == ':') cols.push_back(i);
    }

    if(tmp[0] == '-') {
      test_label[count] = -1;
    } else {
      test_label[count] = 1;
    }

    for(int i = 0; i < 499; i++) {
      test_data[i][count] = stoi(tmp.substr(cols[i] + 1, spac[i + 1]));
    }
    test_data[499][count] = stoi(tmp.substr(cols[499] + 1, tmp.size()));
    

    count++;
  }

  train_in.close();
  test_in.close();
}

void init_params() {
  for(int i = 0; i < 2000; i++) u[i] = (double)1/2000;

  for(int i = 0; i < 500; i++) {
    theta[i].push_back(0);
    for(int j = 1; j < 2000; j++) {
      if (train_data[i][j - 1] != train_data[i][j]) {
        theta[i].push_back(j);
      }
    }
  }
}

void find_best_hyp(int n, fstream &p10, fstream &p10_u) {
  int opt_dim = -1;
  double opt_err = 1000000;
  double opt_t = -1;
  int opt_s = -1;

  double sum = 0;
  for(int i = 0; i < 2000; i++) sum += u[i];
  // cout << sum << '\n';

  for(int i = 0; i < 500; i++) {
    double pos_err = 0;
    double neg_err = 0;
    double best_t = -1;
    int best_s;
    double best_err;
    for(int j = 0; j < 2000; j++) {
      if(label[j] == -1) {
        pos_err += u[j] / sum;
      } else {
        neg_err += u[j] / sum;
      }
    }

    if(neg_err < opt_err) {
      opt_dim = i;
      opt_err = neg_err;
      opt_t = -1;
      opt_s = -1;
    }
    if(pos_err < opt_err) {
      opt_dim = i;
      opt_err = pos_err;
      opt_t = -1;
      opt_s = 1;
    }

    // if (pos_err < neg_err) {
    //   best_s = 1;
    //   best_err = pos_err;
    // } else {
    //   best_s = -1;
    //   best_err = neg_err;
    // }

    for(int j = 1; j < theta[i].size(); j++) {
      double pos_dif = 0;
      double neg_dif = 0;
      for(int k = theta[i][j - 1]; k < theta[i][j]; k++) {
        if(label[k] == 1) {
          pos_dif += u[k] / sum;
        } else {
          neg_dif += u[k] / sum;
        }
      }

      pos_err = pos_err + pos_dif - neg_dif;
      neg_err = neg_err - pos_dif + neg_dif;

      if(neg_err < opt_err) {
        opt_dim = i;
        opt_err = neg_err;
        opt_t = -1;
        opt_s = -1;
      }
      if(pos_err < opt_err) {
        opt_dim = i;
        opt_err = pos_err;
        opt_t = -1;
        opt_s = 1;
      }

      // cout << pos_err + neg_err << '\n';
      //
      // if(neg_err <= best_err) {
      //   best_t = (double)(train_data[i][j - 1] + train_data[i][j]) / 2;
      //   best_err = neg_err;
      //   best_s = -1;
      // }
      // if(pos_err < best_err) {
      //   best_t = (double)(train_data[i][j - 1] + train_data[i][j]) / 2;
      //   best_err = pos_err;
      //   best_s = 1;
      // } 

      // cout << best_err << '\n';
    }

    // if(opt_err == -1) {
    //   opt_dim = i;
    //   opt_s = best_s;
    //   opt_t = best_t;
    //   opt_err = best_err;
    // } else if(best_err <= opt_err) {
    //   if (best_err < opt_err || best_s * best_t < opt_s * opt_t) {
    //     opt_dim = i;
    //     opt_s = best_s;
    //     opt_t = best_t;
    //     opt_err = best_err;
    //   }
    //   // cout << best_err << '\n';
    // }
    // cout << opt_err << '\n';
  }

  double e = opt_err;
  // cout << e << '\n';
  // double sum = 0;
  // for(int i = 0; i < 2000; i++) sum += u[i];
  double dia = sqrt(1 / e - 1);
  // cout << dia << '\n';
  // vector<int> cor;
  // vector<int> inc;
  double e_normal = 0;
  for(int i = 0; i < 2000; i++) {
    if(train_data[opt_dim][i] < opt_t && opt_s == label[i]) {
      // inc.push_back(i);
      u[i] *= dia;
      e_normal += 1;
    } else if(train_data[opt_dim][i] > opt_t && opt_s != label[i]) {
      // inc.push_back(i);
      u[i] *= dia;
      e_normal += 1;
    } else {
      // cout << "foo";
      // cor.push_back(i);
      u[i] /= dia;
    }
  }

  dim[n] = opt_dim;
  t[n] = opt_t;
  s[n] = opt_s;
  a[n] = log(dia);

  p10 << e_normal / 2000 << '\n';
  p10_u << e << '\n';

  // cout << "optimal dimension: " << opt_dim << '\n';
  // cout << "optimal theta: " << opt_t << '\n';
  // cout << "optimal s: " << opt_s << '\n';
  // cout << "optimal err: " << e << '\n';
}

void run_test(int n, fstream &ein, fstream &eout, fstream &p10, fstream &p10_u) {
  find_best_hyp(n, p10, p10_u);
  double e_out = 0;
  for(int i = 0; i < 600; i++) {
    double score = 0;
    for(int j = 0; j < n; j++) {
      if(s[j] == 1 && test_data[dim[j]][i] > t[j]) {
        score += a[j];
      } else if (s[j] == -1 && test_data[dim[j]][i] < t[j]) {
        score += a[j];
      } else {
        score -= a[j];
      }
    }
    if(score < 0 && test_label[i] == 1) {
      e_out = e_out + 1;
    } else if (score > 0 && test_label[i] == -1) {
      e_out = e_out + 1;
    }
  }

  double e_in = 0;
  for(int i = 0; i < 2000; i++) {
    double score = 0;
    for(int j = 0; j < n; j++) {
      if(s[j] == 1 && train_data[dim[j]][i] > t[j]) {
        score += a[j];
      } else if (s[j] == -1 && train_data[dim[j]][i] < t[j]) {
        score += a[j];
      } else {
        score -= a[j];
      }
    }
    if(score < 0 && label[i] == 1) {
      e_in = e_in + 1;
    } else if (score > 0 && label[i] == -1) {
      e_in = e_in + 1;
    }
  }

  // cout << e_in << '\n';
  ein << e_in / 2000 << '\n';
  eout << e_out / 600 << '\n';
}

int main() {
  read_data();
  init_params();

  fstream ein("./e_in.txt");
  fstream eout("./e_out.txt");
  fstream p10("./p10.txt");
  fstream p10_u("./p10_u.txt");

  for(int i = 0; i < 500; i++) {
    run_test(i, ein, eout, p10, p10_u);
    cout << i << '\n';
    // cout << a[i] << '\n';
    // for(int j = 0; j < 2000; j++) cout << u[j] << ' ';
    // cout << '\n';
  }


  return 0;
}
