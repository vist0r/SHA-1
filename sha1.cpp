#include <bits/stdc++.h>
typedef unsigned  uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char uint8_t;

using namespace std;
const uint32_t LEN_512 = 1 << 9;
const uint32_t LEN_448 = 448;
const uint8_t GRUOP1 = 1 << 7;
const uint64_t CONST_CHAR[4] = {
	0x5A827999,0x6ED9EBA1,0x8F1BBCDC,0xCA62C1D6
};

uint32_t ft(uint32_t &B,uint32_t &C,uint32_t &D,int t){
	if (t >= 0 && t <= 19){
		return (B & C) | ((~B) & D);
	}
	if (t >= 20 && t <= 39){
		return B ^ C ^ D;
	}
	if (t >=40 && t <= 59){
		return (B & C) | (B & D) | (C & D);
	}
	if (t >= 60 && t <= 79){
		return B ^ C ^ D; 
	}
}

void print(vector<uint64_t> &v){
	for (int i = 0; i < (uint64_t)v.size(); i++){
		cout << v[i] << endl;
	}
	cout << endl;
}
void push_group(vector<uint64_t> &v,uint64_t &index,uint64_t num){
	if (index == 7){
		v.push_back(num);
	}else {
		uint64_t now = (uint64_t)v.size() - 1;
		v[now] = (v[now] << 8) + num;
	}
	--index;
	index = (index + 8) % 8;
}
void div_group(vector<uint64_t> &v, uint64_t dig,uint64_t &pow,uint64_t &group){
	v.push_back((uint64_t)(dig >> 32));
	v.push_back((uint64_t)(dig & 0xffffffff));
	group += (pow == 7); 
	pow = (pow + 1) % 8;
}
uint32_t shift_L(uint32_t &x,uint32_t k){
	return (uint32_t)((x << k) | (x >> (32 - k)));
}
string SHA_1(const char msg[]){
	uint32_t CACHE[5] = {
		0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,0xC3D2E1F0 
	};
	uint64_t _l_m = strlen(msg);
	uint64_t _l = _l_m << 3;
	uint32_t  _r = LEN_448 - (_l %  LEN_512);
	assert(_r % 8 == 0);
	uint64_t _L = (_l_m >> 9) + 1;
	vector<uint64_t> digest_array;
	uint64_t pow = 7;
	for (int i = 0; i < _l_m; i++){
		push_group(digest_array,pow,(uint64_t)msg[i]);
	}
	push_group(digest_array,pow,GRUOP1);
	for (int i = 0; i < (_r >> 3) - 1; i++){
		push_group(digest_array,pow,0);
	}
	uint64_t tmp = _l;
	for (int i = 7; i >= 0; i--){
		push_group(digest_array,pow,(tmp >> (i << 3)));
		tmp = tmp & ((1LL << (i << 3)) - 1);
	}
	vector<uint64_t> Y[_L];
	pow = 0;
	uint64_t group = 0;
	for (int i = 0; i < (uint64_t)digest_array.size(); i++)
	{
		div_group(Y[group],digest_array[i],pow,group);
	}
	vector<uint32_t> W[_L];
	uint32_t TEMP[5];
	for (int i = 0; i < 5; i++){
		TEMP[i] = CACHE[i];
	}
	for (int i = 0; i < _L; i++){
		 for (int k = 0; k < 16; k++){
		 	 W[i].push_back((uint32_t)Y[i][k]);
		 }
		 //cout << endl;
		 for (int k = 16; k < 80; k++){
		 	W[i].push_back(W[i][k - 3] ^ W[i][k - 8] ^ W[i][k - 14] ^ W[i][k - 16]);
		 	W[i][k] = shift_L(W[i][k],1);
		 }
		 for (int k = 0;  k < 80; k++){
		 	uint32_t T = shift_L(CACHE[0],5) + ft(CACHE[1],CACHE[2],CACHE[3],k) + CACHE[4] + W[i][k] + CONST_CHAR[k / 20];
		 	CACHE[4] = CACHE[3];
		 	CACHE[3] = CACHE[2];
		 	CACHE[2] = shift_L(CACHE[1],30);
		 	CACHE[1] = CACHE[0];
		 	CACHE[0] = T;
		 }
		 for (int k = 0; k < 5; k++){
		 	TEMP[k] += CACHE[k];
		 }
	}
	stringstream ans;
	for (int k = 0; k < 5; k++){
		 ans << hex << TEMP[k];
	}
	string z;
	z.clear();
	string res = ans.str();
	for (int i = (int)res.size(); i < 40; i++)
		z.push_back('0');
	return z + res;
	//print(digest_array);
}
int main(){
	cout << SHA_1("dfasdfajkgljdfgljaskjdglajslfgjlkajfdlaljsdflasdf") << endl;
	//cout << CHAR_END << endl;
}
