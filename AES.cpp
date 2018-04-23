//compile environment : dev C++
#include <iostream>
#include <stdint.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
using namespace std;
#define MAX 300


void input_func(uint8_t* input){
uint8_t temp[49];
int ch,i,c;
int temp1[49];
for(c=0;c<48;c++)
temp[c]=getchar(); //collect input(hex)

for(c=0;c<48;c++){ //transform to decimal
if(temp[c] >= '0' && temp[c] <= '9') temp1[c]= temp[c]-'0';
if(temp[c] >= 'a' && temp[c] <= 'f') temp1[c]= 10+temp[c]-'a';
if(temp[c] >= 'A' && temp[c] <= 'F') temp1[c]= 10+temp[c]-'A';
}

for(i=0;i<16;i++)
input[i]=((16*temp1[3*i])+temp1[3*i+1]); //transform to binary
}

void dec_to_hex(uint8_t input){
	cout << " ";
	int one = input/16;
	if (one == 10)
		cout << 'a' ;
	else if (one == 11)
		cout << 'b' ;
	else if (one == 12)
		cout << 'c' ;
	else if (one == 13)
		cout << 'd' ;
	else if (one == 14)
		cout << 'e' ;
	else if (one == 15)
		cout << 'f' ;
	else 
		cout << one ;
		
	int two = input%16;
	if (two == 10)
		cout << 'a' ;
	else if (two == 11)
		cout << 'b' ;
	else if (two == 12)
		cout << 'c' ;
	else if (two == 13)
		cout << 'd' ;
	else if (two == 14)
		cout << 'e' ;
	else if (two == 15)
		cout << 'f' ;
	else 
		cout << two ;
	
}

uint8_t GF256_add(uint8_t a, uint8_t b){
	return a^b;
}
uint8_t GF256_mult_x(uint8_t a, uint8_t mx){

} 
uint8_t GF256_mult(uint8_t a, uint8_t b){
	uint8_t p=0;
    uint8_t carry;
    int i;
    for(i=0;i<8;i++)
    {
        if(b & 1)
            p ^=a;
        carry = a & 0x80;
        a = a<<1;
        if(carry)
            a^=0x1b;
        b = b>>1;
    }
    return p;
} 

uint8_t GF256_inv(uint8_t a){
	uint8_t j, b = a;
    for (j = 14; --j;)              /* for j from 13 downto 1 */
        b = GF256_mult(b, j&1 ? b : a);   /* alternatively square and multiply */
    return b;
} 

int bin_add(uint8_t a){
	int n = int(a);
	int count = 0;
	while (n >= 2){
		if(n%2!=0)
			count++;
		n /= 2;
	}
	count += n ;
//	cout<<count;
	if(count %2 == 1)
		return 1;
	else
		return 0;
}

uint8_t SubBytes(uint8_t a){
	uint8_t k = GF256_inv(a);
	int b[8];
	b[0] = bin_add(0xf8&k) ^ 0;
	b[1] = bin_add(0x7c&k) ^ 1;
	b[2] = bin_add(0x3e&k) ^ 1;
	b[3] = bin_add(0x1f&k) ^ 0;
	b[4] = bin_add(0x8f&k) ^ 0;
	b[5] = bin_add(0xc7&k) ^ 0;
	b[6] = bin_add(0xe3&k) ^ 1;
	b[7] = bin_add(0xf1&k) ^ 1;	

	return (b[0]*128+b[1]*64+b[2]*32+b[3]*16+b[4]*8+b[5]*4+b[6]*2+b[7]*1);
}

void ShiftRows(uint8_t* a){
	uint8_t* b = new uint8_t[16]();
	b[0]=a[0];
	b[1]=a[5];
	b[2]=a[10];
	b[3]=a[15];
	b[4]=a[4];
	b[5]=a[9];
	b[6]=a[14];
	b[7]=a[3];
	b[8]=a[8];
	b[9]=a[13];
	b[10]=a[2];
	b[11]=a[7];
	b[12]=a[12];
	b[13]=a[1];
	b[14]=a[6];
	b[15]=a[11];	 
	
	for (int i = 0; i< 16 ; i++)
		a[i] = b[i];
}

void MixColumns(uint8_t* a){
	uint8_t b[16] = {0x02, 0x03, 0x01, 0x01, 0x01, 0x02, 0x03, 0x01, 0x01, 0x01, 0x02, 0x03, 0x03, 0x01, 0x01, 0x02}; 
	uint8_t* c = new uint8_t[16]();
	uint8_t k = 0 ;
	int n = 0;
	for (int i=0,j=0 ; i< 16 ; i++){
		k =  k ^ GF256_mult(b[i],a[j]);
		j++;
		if(j==4){
			j = 0;
			c[n] = k;
			k = 0;
			n++;
			
		}
	}
	
	for (int i=0, j=4 ; i< 16 ; i++){
		k =  k ^ GF256_mult(b[i],a[j]);
		j++;
		if(j==8){
			j = 4;
			c[n] = k;
			k = 0;
			n++;
		}
	}
	for (int i=0,j=8 ; i< 16 ; i++){
		k =  k ^ GF256_mult(b[i],a[j]);
		j++;
		if(j==12){
			j = 8;
			c[n] = k;
			k = 0;
			n++;
		}
	}
	for (int i=0,j=12 ; i< 16 ; i++){
		k =  k ^ GF256_mult(b[i],a[j]);
		j++;
		if(j==16){
			j = 12;
			c[n] = k;
			k = 0;
			n++;
		}
	}
	for (int i = 0; i< 16 ; i++)
		a[i] = c[i];
	
}

void KeySchedule (uint8_t* a, uint8_t rcon){
	uint8_t Rot[4] = {a[13],a[14],a[15],a[12]};
	
	uint8_t* c = new uint8_t[16]();
	for (int i =0 ; i < 4 ; i++)
		Rot[i] = SubBytes(Rot[i]);
		
	c[0] = a[0] ^ Rot[0] ^ rcon;
	for (int i =1 ; i < 4 ; i++)
		c[i] = a[i] ^ Rot[i];
		
	for (int i =0 ; i < 12 ; i++)
		c[i+4] = a[i+4] ^ c[i];
		
	for (int i = 0; i< 16 ; i++)
		a[i] = c[i];
	
}

int main(){
	uint8_t* plaintext = new uint8_t[16]();
	uint8_t* key = new uint8_t[300]();
	uint8_t* shift_row = new uint8_t[16]();
	cout << hex << +(0x36^0x9f^0x01);
	cout << "<AES Encryption>" << endl;
	cout << "Plaintext: ";
	input_func(plaintext);
	cout << "Key:	   ";
    input_func(key);
    cout << endl << "--------Encryption--------" << endl;
    cout << "s0:";
    for (int i=0 ; i<16 ; i++){
    	plaintext[i] = GF256_add(plaintext[i],key[i]);
    	dec_to_hex(plaintext[i]);
	}
    cout << endl; 
    
    for (int i=0 ; i<16 ; i++){
    	plaintext[i] = SubBytes(plaintext[i]);
    	dec_to_hex(plaintext[i]);
	}
    cout << endl; 
    
    ShiftRows(plaintext);
    for (int i=0 ; i<16 ; i++){
    	dec_to_hex(plaintext[i]);
	}
	cout << endl; 
	MixColumns(plaintext);
    for (int i=0 ; i<16 ; i++){
    	dec_to_hex(plaintext[i]);
	}
	cout << endl; 

	KeySchedule(key,0x01);
	for (int i=0 ; i<16 ; i++){
    	dec_to_hex(key[i]);
	}
	cout << endl; 
	
	cout << "s1:";
	for (int i=0 ; i<16 ; i++){
    	plaintext[i] = GF256_add(plaintext[i],key[i]);
    	dec_to_hex(plaintext[i]);
	}
    cout << endl; 

    return 0;
}
