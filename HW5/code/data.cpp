#include <fstream>
using namespace std;

int main() {
  fstream data_in;
  fstream data_out;
  data_in.open("mnist.scale.t");
  data_out.open("test_data.txt");
  string tmp;
  while(getline(data_in, tmp)){
    // if(tmp[0] == '2') {
    //   tmp[0] = '1';
    //   data_out << '-' << tmp << '\n';
    // }
    // else if(tmp[0] == '6') {
    //   tmp[0] = '1';
    //   data_out << tmp << '\n';
    // }
    if(tmp[0] == '2' || tmp[0] == '6') data_out << tmp << '\n';
  }
  data_out.close();
  data_in.close();
}
