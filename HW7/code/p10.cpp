#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#define pii pair<int, int>
using namespace std;

// int train_data[2000][500];
// int train_data[500][2000];
// int train_data_or[500][2000];
pii train_data[500][2000];
int test_data[500][600];
int label[2000];
int test_label[600];
vector<int> theta[500];
double u[2000];

int dim[500];
int s[500];
double t[500];
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
      // train_data_or[i][count] = stoi(tmp.substr(cols[i] + 1, spac[i + 1]));
      train_data[i][count] = make_pair(stoi(tmp.substr(cols[i] + 1, spac[i + 1])), label[count]);
      // train_data[i].push_back(make_pair(label[count], stoi(tmp.substr(cols[i] + 1, spac[i + 1]))));
    }
    // train_data_or[499][count] = stoi(tmp.substr(cols[499] + 1, tmp.size()));
    train_data[499][count] = make_pair(stoi(tmp.substr(cols[499] + 1, tmp.size())), label[count]);
    // train_data[499].push_back(make_pair(label[count], stoi(tmp.substr(cols[499] + 1, tmp.size()))));
    

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

  for(int i = 0; i < 500; i++) sort(train_data[i], train_data[i] + 2000);

  for(int i = 0; i < 500; i++) {
    theta[i].push_back(0);
    for(int j = 1; j < 2000; j++) {
      if(train_data[i][j] < train_data[i][j - 1]) {
        cout << "error";
      }
      if (train_data[i][j - 1].first != train_data[i][j].first) {
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
    for(int j = 0; j < 2000; j++) {
      if(train_data[i][j].second == -1) {
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

    for(int j = 1; j < theta[i].size(); j++) {
      double pos_dif = 0;
      double neg_dif = 0;
      for(int k = theta[i][j - 1]; k < theta[i][j]; k++) {
        if(train_data[i][k].second == 1) {
          pos_dif += u[k] / sum;
        } else {
          neg_dif += u[k] / sum;
        }
      }

      pos_err = pos_err + pos_dif - neg_dif;
      neg_err = neg_err - pos_dif + neg_dif;

      if(pos_err < 0 || pos_err > 1) {
        cout << "error at:\n";
        cout << "iteration " << n << '\n';
        cout << "dimension: " << i << '\n';
        cout << pos_dif << ' ' << neg_dif << '\n';
        // cout << pos_err << ' ' << neg_err;
        return;
      }

      if(neg_err < opt_err) {
        opt_dim = i;
        opt_err = neg_err;
        opt_t = (double(train_data[i][theta[i][j - 1]].first) + double(train_data[i][theta[i][j]].first)) / 2;
        opt_s = -1;
      }
      if(pos_err < opt_err) {
        opt_dim = i;
        opt_err = pos_err;
        opt_t = (double(train_data[i][theta[i][j - 1]].first) + double(train_data[i][theta[i][j]].first)) / 2;
        opt_s = 1;
      }
      // if(opt_t == 476) {
      //   cout << train_data[i][theta[i][j - 1]].first << ' ' << train_data[i][theta[i][j]].first;
      //   return;
      // }
    }
  }

  double e_u = opt_err;
  // cout << e_u << '\n';
  double dia = sqrt(1 / e_u - 1);
  cout << "sum of u " << sum << '\n';
  cout << "optimal error " << opt_err << '\n';
  cout << "optimal theta " << opt_t << '\n';
  cout << "optimal s " << opt_s << '\n';
  cout << "optimal dimension " << opt_dim << '\n';
  cout << "diamond " << dia << '\n';
  // cout << dia << '\n';
  double e = 0;
  double e_ = 0;
  for(int i = 0; i < 2000; i++) {
    if(train_data[opt_dim][i].first < opt_t && opt_s == train_data[opt_dim][i].second) {
      e_ = e_ + u[i] / sum;
      u[i] = u[i] * dia;
      e = e + 1;
      // cout << '+' << '\n';
    } else if(train_data[opt_dim][i].first > opt_t && opt_s != train_data[opt_dim][i].second) {
      e_ = e_ + u[i] / sum;
      u[i] = u[i] * dia;
      // cout << '+' << '\n';
      e = e + 1;
    } else {
      u[i] = u[i] / dia;
      cout << i << ' ';
      // cout << '-' << '\n';
    }
  }

  // cout << e_ << ' ' << opt_err << '\n';


  // double sum_after = 0;
  // for(int i = 0; i < 2000; i++) sum_after += u[i];
  // cout << "U ratio " << sum_after / sum << '\n';

  dim[n] = opt_dim;
  // cout << opt_dim << '\n';
  t[n] = opt_t;
  s[n] = opt_s;
  a[n] = log(dia);

  p10 << e / 2000 << '\n';
  p10_u << e_u << '\n';
}

void run_test(int n, fstream &ein, fstream &eout, fstream &p10, fstream &p10_u) {
  find_best_hyp(n, p10, p10_u);
  double e_out = 0;
  for(int i = 0; i < 600; i++) {
    double score = 0;
    for(int j = 0; j <= n; j++) {
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
    for(int j = 0; j <= n; j++) {
      if(s[j] == 1 && train_data_or[dim[j]][i] > t[j]) {
        score += a[j];
      } else if (s[j] == -1 && train_data_or[dim[j]][i] < t[j]) {
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

  // cout << e_in / 2000 << '\n';
  // cout << e_out / 600 << '\n';
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


  // run_test(0, ein, eout, p10, p10_u);
  // double sum = 0;
  // for(int i = 0; i < 2000; i++) sum += u[i];
  // for(int i = 90; i < 91; i++) {
  //   double pos_err = 0;
  //   double neg_err = 0;
  //   for(int j = 0; j < 2000; j++) {
  //     if(train_data[i][j].second == -1) {
  //       pos_err += u[j] / sum;
  //     } else {
  //       neg_err += u[j] / sum;
  //     }
  //   }
  //
  //   for(int j = 1; j < theta[i].size(); j++) {
  //     double pos_dif = 0;
  //     double neg_dif = 0;
  //     for(int k = theta[i][j - 1]; k < theta[i][j]; k++) {
  //       if(train_data[i][k].second == 1) {
  //         pos_dif += u[k] / sum;
  //         // cout << pos_dif << '\n';
  //       } else {
  //         neg_dif += u[k] / sum;
  //       }
  //     }
  //
  //     // cout << pos_dif << '\n';
  //       
  //     cout << pos_err << ' ' << neg_err << '\n';
  //     cout << pos_dif << ' ' << neg_dif << '\n';
  //
  //     pos_err = pos_err + pos_dif - neg_dif;
  //     neg_err = neg_err - pos_dif + neg_dif;
  //     if(pos_err < 0 || pos_err > 1) {
  //       cout << "error at:\n";
  //       cout << "dimension: " << i << '\n';
  //       cout << pos_dif << ' ' << neg_dif << '\n';
  //       cout << pos_err << ' ' << neg_err;
  //     }
  //
  //   }
  // }


  // run_test(0, ein, eout, p10, p10_u);
  for(int i = 0; i < 1; i++) {
    run_test(i, ein, eout, p10, p10_u);
    cout << i << '\n';
  }
  // for(int i : theta[90]) {
  //   cout << train_data[90][i].first << ' ';
  // }
  // for(int i = 0; i < theta[90].size(); i++) cout << theta[90][i] << ' ';


  return 0;
}
