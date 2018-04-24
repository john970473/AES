//compile environment : dev C++
#include <iostream>
#include <stdint.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
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
	printf(" %02x",input);
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
	uint8_t b[16] = {a[0],a[5],a[10],a[15],a[4],a[9],a[14],a[3],a[8],a[13],a[2],a[7],a[12],a[1],a[6],a[11]};	 
	
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

void KeySchedule (uint8_t* a, uint8_t* b , uint8_t rcon){
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
		b[i] = c[i];
	
}

void AES_Encrypt(uint8_t* plaintext, uint8_t* ciphertext, uint8_t** key){
	
	uint8_t Rcon[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};
    cout << endl << "--------Encryption--------" << endl;
    
    //add roundkey s0
    cout << "s0:";
    for (int i=0 ; i<16 ; i++){
    	plaintext[i] = GF256_add(plaintext[i],key[0][i]);
    	dec_to_hex(plaintext[i]);
	}
    cout << endl; 
    
    
    //s1~s9
    for (int s=1 ; s <=9 ; s++){
    	cout << "s" << s <<":";
	    for (int i=0 ; i<16 ; i++){
	    	plaintext[i] = SubBytes(plaintext[i]);
		}
	    ShiftRows(plaintext); 
		MixColumns(plaintext);
		KeySchedule(key[s-1],key[s],Rcon[s-1]);
		
		for (int i=0 ; i<16 ; i++){
	    	plaintext[i] = GF256_add(plaintext[i],key[s][i]);
	    	dec_to_hex(plaintext[i]);
		}
	    cout << endl; 
	}
	
	//ciphertext
	cout << "Ciphertext:";
	for (int i=0 ; i<16 ; i++){
	    	plaintext[i] = SubBytes(plaintext[i]);
		}
	ShiftRows(plaintext); 
	//MixColumns(plaintext);
	KeySchedule(key[9],key[10],Rcon[9]);
		
	for (int i=0 ; i<16 ; i++){
	    plaintext[i] = GF256_add(plaintext[i],key[10][i]);
	}

	for (int i=0 ; i<16 ; i++){
	    ciphertext[i] = plaintext[i];
	    dec_to_hex(ciphertext[i]);
	}
	cout << endl;
    
}

void InvShiftRows(uint8_t* a){
	uint8_t b[16] = {a[0],a[13],a[10],a[7],a[4],a[1],a[14],a[11],a[8],a[5],a[2],a[15],a[12],a[9],a[6],a[3]};	 
	
	for (int i = 0; i< 16 ; i++)
		a[i] = b[i];
}

uint8_t InvSubBytes(uint8_t a){

	int b[8];
	b[0] = bin_add(0x52&a) ^0;
	b[1] = bin_add(0x29&a) ^0;
	b[2] = bin_add(0x94&a) ^0;
	b[3] = bin_add(0x4a&a) ^0; 
	b[4] = bin_add(0x25&a) ^0;
	b[5] = bin_add(0x92&a) ^1;
	b[6] = bin_add(0x49&a) ^0;
	b[7] = bin_add(0xa4&a) ^1;

	return GF256_inv(b[0]*128+b[1]*64+b[2]*32+b[3]*16+b[4]*8+b[5]*4+b[6]*2+b[7]*1);
}

void InvMixColumns(uint8_t* a){
	uint8_t b[16] = {0x0e, 0x0b, 0x0d, 0x09, 0x09, 0x0e, 0x0b, 0x0d, 0x0d, 0x09, 0x0e, 0x0b, 0x0b, 0x0d, 0x09, 0x0e}; 
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

void AES_Decrypt(uint8_t* plaintext , uint8_t* ciphertext , uint8_t** key){

	uint8_t Rcon[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};
	

    cout << endl << "--------Decryption--------" << endl;
    
    //add roundkey s'0
    cout << "s'0:";
    for (int i=0 ; i<16 ; i++){
    	ciphertext[i] = GF256_add(ciphertext[i],key[10][i]);
    	dec_to_hex(ciphertext[i]);
	}
    cout << endl; 
    
    
    //s'1~s'9
    for (int s=1 ; s<10 ; s++){
    	cout << "s'" << s << ":";
	    InvShiftRows(ciphertext); 
	    	
		for (int i=0 ; i<16 ; i++){
		    ciphertext[i] = InvSubBytes(ciphertext[i]);
		}
	
		for (int i=0 ; i<16 ; i++){
	    	ciphertext[i] = GF256_add(ciphertext[i],key[10-s][i]);
		}
		
		InvMixColumns(ciphertext);
		
		for (int i=0 ; i<16 ; i++){
	    	dec_to_hex(ciphertext[i]);
		}
		cout << endl;
	
	}
	
	cout << "Plaintext:";
	InvShiftRows(ciphertext); 
	    	
	for (int i=0 ; i<16 ; i++){
	    ciphertext[i] = InvSubBytes(ciphertext[i]);
	}
	
	for (int i=0 ; i<16 ; i++){
    	ciphertext[i] = GF256_add(ciphertext[i],key[0][i]);
	}
		
	for (int i=0 ; i<16 ; i++){
    	dec_to_hex(ciphertext[i]);
	}
	cout << endl;
	
    
}

int main(){
	
	uint8_t* plaintext = new uint8_t[16]();
	uint8_t* ciphertext = new uint8_t[16]();
	uint8_t** key;
	key = new uint8_t*[11];
	for (int i =0 ; i<11 ; i++)
		key[i] = new uint8_t[16]();
	
	cout << "<AES Encryption>" << endl;
	cout << "Plaintext: ";
	input_func(plaintext);
	cout << "Key:	   ";
    input_func(key[0]);

	AES_Encrypt(plaintext, ciphertext, key);
	AES_Decrypt(plaintext, ciphertext, key);

    return 0;
}
