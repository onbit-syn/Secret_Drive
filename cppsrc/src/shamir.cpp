#include <iostream>
#include <vector>
#include <random>
#include "GF256.h"
#include "shamir.h"

using namespace std;
using namespace GF256;
using namespace shamir;

typedef vector<vector<point>> shares;

/*Creating of shares shamir (n,k) scheme*/

shares* shamir::createShares(string secret,int n,int k) {
  random_device device;
  default_random_engine generator(device());
  uniform_int_distribution<int> distribution(0,255);
  shares* allShares = new shares(n);
  for(char data:secret) {
  byte coeff[k];
  coeff[0] = (int)data;
    for(int i=1;i<k;i++) {
        coeff[i] = distribution(generator);
    }
    point temp;
    byte x,y;
    for(int i=0;i<n;i++) {
        x = (int)(i + 1);
        y = 0;
        for(int i=0;i<k;i++) {
            y = y + (coeff[i] * power(x,i));
        }
        temp.x = x;
        temp.y = y;
        (*allShares)[i].push_back(temp);
      }
    }
    return allShares;
  }

string shamir::getSecret(shares* Kshares,int k) {
    if((*Kshares).size() < (unsigned int)k) {
      cout << "Atleast " << k << " shares are required " << endl;
      return "";
    }
    string secret = "";
    int secretSize = (*Kshares)[0].size();
    vector<point> curr_shares(k);
    for(int di=0;di<secretSize;di++) {
        for(int i=0;i<k;i++) {
            curr_shares[i] = (*Kshares)[i][di];
        }
        secret += interpolate(curr_shares);
    }
    return secret;
}


void shamir::init() {
  gen_multipletable();
}
