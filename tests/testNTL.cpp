#include <NTL/ZZ.h>

int main(){
	NTL::ZZ p;
	NTL::conv(p, "115792089210356248762697446949407573530086143415290314195533631308867097853951");
	NTL::ZZ a = p;
	NTL::ZZ x;
	a--;

	for(int i = 0; i < 100000; i++){
		if(NTL::Jacobi(a, p) == 1){
			NTL::SqrRootMod(x, a, p);
		}
		--a;
	}

	return 0;
}