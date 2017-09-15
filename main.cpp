#include <iostream>
#include <array>
#include <cstdlib>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif


float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);



#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }

  return 0;

  //type convert
  int n, functionid, intens; float a, b, sum, xp, temp;
    functionid = atoi(argv[0]);
    a = atof(argv[1]);
    b = atof(argv[2]);
    n = atoi(argv[3]);
    intens = atoi(argv[4]);


  //  Call functions within an array
  switch(functionid) {
      case 1 :
      for  (int i = 0; i< n-1 ; i++){

          xp = a + (i+0.5)*(b-a)/n;
          temp += f1(xp,intens);
          sum = temp*(b-a)/n;
      }
         break;
      case 2 :
      for  (int i = 0; i< n-1 ; i++){

          xp = a + (i+0.5)*(b-a)/n;
          temp += f2(xp,intens);
          sum = temp*(b-a)/n;

      }
         break;
      case 3:
      for  (int i = 0; i< n-1 ; i++){

          xp = a + (i+0.5)*(b-a)/n;
          temp += f3(xp,intens);
          sum = temp*(b-a)/n;

      }
         break;
      case 4 :
      for  (int i = 0; i< n-1 ; i++){

          xp = a + (i+0.5)*(b-a)/n;
          temp += f4(xp,intens);
          sum = temp*(b-a)/n;

      }
         break;
      default :
         cout << "Invalid function" << endl;
   }


    cout << "The integral of function" << functionid << "from"<< a <<"to" << b << "is" << sum << endl;

}
