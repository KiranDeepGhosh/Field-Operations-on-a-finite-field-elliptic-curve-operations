#include<stdio.h>
#include<stdlib.h>

// Function to convert a hexadecimal string
unsigned char hextoint(char C) 
{
    if (C >= '0' && C <= '9') 
    {
        return C - '0';
    } else if (C >= 'a' && C <= 'f') 
    {
        return C - 'a' + 10;
    } else if (C >= 'A' && C <= 'F') 
    {
        return C - 'A' + 10;
    }
perror("Error: Invalid hexadecimal character\n");
exit(EXIT_FAILURE);

} 

//parsing hex to 2^8 base array
void parse (char* A, unsigned char B[32] )
 {
 for (int i=0;i<32;i++)
    {
 	B[i]=hextoint(A[2*i])*16+hextoint(A[2*i+1]);  	
    }
 
 
 } 
 //parsing hex to 2^8 base array for mu
void parse_mu (char* A, unsigned char B[34] )
 {
 for (int i=0;i<34;i++)
    {
 	B[i]=hextoint(A[2*i])*16+hextoint(A[2*i+1]);  	
    }
 
 
 } 

//convert from 2^8 to 2^29
void base_change (unsigned char B[32],long long unsigned int C[10])
 {
 C[9]= B[31] | (B[30]<< 8 ) | (B[29]<< 16) |(B[28] & 0x1f) <<24 ;
 C[8]=(B[28]>>5 ) | (B[27] << 3) | (B[26]<< 11) | (B[25] <<19) | (B[24] & 0x03) << 27;
 C[7]=(B[24]>>2 ) | (B[23] << 6) | (B[22]<< 14) | (B[21] & 0x7f )<< 22;
 C[6]=(B[21]>>7) | (B[20]<< 1) | (B[19]<< 9) | (B[18]<<17) | (B[17] & 0x0f) << 25;
 C[5]=(B[17]>>4) | (B[16]<< 4) | (B[15]<<12) | (B[14]<<20) | (B[13] & 0x01) << 28;
 C[4]=(B[13]>>1) | (B[12]<<7 ) | (B[11]<<15) | (B[10] & 0x3f)<< 23;
 C[3]=(B[10]>>6) | (B[9]<<2 ) | (B[8]<<10) | (B[7]<<18) | (B[6] & 0x07)<<26;
 C[2]=(B[6]>>3) | (B[5]<<5 ) | (B[4]<<13) | (B[3]<< 21);
 C[1]= B[2] | (B[1]<< 8) | (B[0]<< 16);
 C[0]=0; 
 }
  
//convert from 2^8 to 2^29 for mu
void base_change_mu ( unsigned char B[34],long long unsigned int C[10])
 {
 C[9]= B[33] | (B[32]<< 8 ) | (B[31]<< 16) |(B[30] & 0x1f) <<24 ;
 C[8]=(B[30]>>5 ) | (B[29] << 3) | (B[28]<< 11) | (B[27] <<19) | (B[26] & 0x03) << 27;
 C[7]=(B[26]>>2 ) | (B[25] << 6) | (B[24]<< 14) | (B[23] & 0x7f )<< 22;
 C[6]=(B[23]>>7) | (B[22]<< 1) | (B[21]<< 9) | (B[20]<<17) | (B[19] & 0x0f) << 25;
 C[5]=(B[19]>>4) | (B[18]<< 4) | (B[17]<<12) | (B[16]<<20) | (B[15] & 0x01) << 28;
 C[4]=(B[15]>>1) | (B[14]<<7 ) | (B[13]<<15) | (B[12] & 0x3f)<< 23;
 C[3]=(B[12]>>6) | (B[11]<<2 ) | (B[10]<<10) | (B[9]<<18) | (B[8] & 0x07)<<26;
 C[2]=(B[8]>>3) | (B[7]<<5 ) | (B[6]<<13) | (B[5]<< 21);
 C[1]= B[4] | (B[3]<< 8) | (B[2]<< 16) | (B[1] & 0x1f) <<24 ;
 C[0]=(B[1]>>5 ) | (B[0] << 3) ; 
 }
  
//field multiplication function
void field_mult(long long unsigned int A[10], long long unsigned int B[10],long long unsigned int C[19])
 {
 for (int i=0;i<19;i++)
	{
	C[i]=0;
	}
 for (int i=0;i< 10;i++)
	{
	for (int j=0;j<10;j++)
		{
		C[i+j]=C[i+j]+A[i]*B[j];	
		}	
	}
	long long unsigned int X=0;
	for(int i=18;i>=0;i--)
		{
		C[i]=C[i]+X;
		X=C[i] >> 29;
		C[i]=C[i] & 0x1fffffff;
		}	
 }

//subtraction function 
void subs (long long unsigned int A[10], long long unsigned int B[10],long long unsigned int C[10])
 {
	unsigned int carry = 1;
	for (int i=9; i>=0; i--) 
		{
		C[i] = A[i] + (B[i]^0x1fffffff) + carry;
		carry = C[i] >> 29;
		C[i] = C[i] & 0x1fffffff;
		}
 }
//compare function  
int compare (long long unsigned int A[10], long long unsigned int B[10])
{
int flag=1;
for (int i=0; i<=9; i++) 
	{
	 if(A[i]<B[i])
		{
		flag=0;
		break;
		}
	
	}
return flag;

} 

//Barret Reduction for the output of the multiplication
void barrett (long long unsigned int Z[19], long long unsigned int p[10],long long unsigned int mu[10],long long unsigned int result[10] )
 {
 long long unsigned int A[19] = {0};
 field_mult(Z+1, mu, A);
 long long unsigned int Q[10] = {0};
 for (int i=1;i<10;i++)
 	Q[i]=A[i-1];
 long long unsigned int B[19]={0};
 field_mult(Q, p, B);
 subs(Z+9, B+9, result);
 long long unsigned int temp[10];
	
    while(compare(result,p)==1)
    	{
    	subs(result, p, temp);
    	for (int i=0;i<10;i++)
 	{
 	result[i]=temp[i];}
    	}

 }
void exponent_l2r (long long unsigned int G[10],long long unsigned int n[10],long long unsigned int p[10],long long unsigned int mu[10], long long unsigned int H[10])
 {
 long long unsigned int temp[19], temp1[19];
    for(int i=0;i<9;i++)
	H[i]=0; 
 H[9]=1;
    for(int i=0;i<10;i++)
    {
      for(int j=28;j>=0;j--)
      {
       field_mult(H,H,temp);
       barrett(temp, p, mu, H);
 	  if((n[i]>>j)&1 == 1)
 	  	{field_mult(H,G, temp1);
 	  	barrett(temp1,p, mu, H);
 	  	}
 	 }	
     }

 }
 void exponent_r2l (long long unsigned int G[10],long long unsigned int n[10],long long unsigned int p[10],long long unsigned int mu[10], long long unsigned int H[10])
 {
 long long unsigned int temp[19], temp1[19];
    for(int i=0;i<9;i++)
	H[i]=0; 
 H[9]=1;
    for(int i=9;i>=0;i--)
    {
      for(int j=0;j<29;j++)
      {
      if((n[i]>>j)&1 == 1)
 	  	{field_mult(H,G, temp1);
 	  	barrett(temp1,p, mu, H);
 	  	}
       field_mult(G,G,temp);
       barrett(temp, p, mu, G);
 	 }	
     }

 }

void main ()
 {
 char P []="e92e40ad6f281c8a082afdc49e1372659455bec8ceea043a614c835b7fe9eff5";
 char MU[]="046435b5a40bbb8b91a5ac84a4a1180915a5eeac095be5dc75ddafa730293ae00018";
 unsigned char B1[32],B2[32],p1[32],mu1[34];
 unsigned char A[10];
 long long unsigned int p[10], mu[10];
 long long unsigned int result[10];
 long long unsigned int result1[10];
 parse (P, p1);
 base_change (p1,p);
 parse_mu (MU, mu1);
 base_change_mu (mu1,mu);
 long long unsigned int C1[10],C2[10], C[19], X[10];
 char S1[]= "00000000000000000000000000000000000000000000000254ab485555111100";
 char S2[]= "4a3cfcc426f60b120461ba149c4c9541117311291218b187aa18e7610b5d9cb8";
 parse (S1, B1);
 base_change (B1,C1);
 parse (S2, B2);
 base_change (B2,C2);
 //field_mult(C1,C2,C);
 
// barrett(C,p,mu,result);
 //exponent_l2r(C1,C2, p, mu, result);
 //exponent_r2l(C1,C2, p, mu, result1);

 for(int i=0;i<10;i++)
 	{
 	printf("%llx", C1[i]);
 	printf("\n");
	}
	printf("hello");
	
for(int i=0;i<10;i++)
 	{
 	printf("%llx", C2[i]);
 	printf("\n");
	}
 }
