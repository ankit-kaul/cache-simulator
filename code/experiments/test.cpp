#include <cstdio>
#include <math.h>
#include <stdio.h>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <iostream>  
#include <bitset>
#include <tuple>
#include <algorithm>
#include <inttypes.h>

using namespace std;

int main() {
    unsigned int c = 0;
    unsigned int b = 0;
    unsigned int s = 0;
	int i, j, k;
    unsigned int tot_stor   = 0;
	
	for(i=0; i<=24; i++) {
		for(j=0; j<=24; j++)
			for(k=0; k<=24; k++){
				c=(pow(2,i));
				b=(pow(2,j));
				s=(pow(2,k));
				if(b <= c && s <= (c/b)){
					tot_stor = (c/b)*(((66-(log2f(b))-(log2f(c/(b*s))))/8) + (b));
					if(tot_stor <= 4096){
						printf("\nc: %d, b: %d, s: %d",c, b, s);
					}
				}
			}
	}


    return 0;
}