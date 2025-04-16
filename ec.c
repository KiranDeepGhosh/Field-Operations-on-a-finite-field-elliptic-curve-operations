#include<stdio.h>
#include<stdlib.h>

long long unsigned int q[10]={0,0xe92e40,0x15ade503,0x122820ab,0x1ee24f09,0x17265945,0xb7d919d,0x1a810e98,0xa641adb, 0x1fe9eff3};//p-2


// Function to convert a hexadecimal string
unsigned char hextoint (char C) 
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
	if (A[i] > B[i]) break;
    else if(A[i] < B[i]){
    	flag = 0;
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
 
 
 //point addition function in c
typedef struct 
 {
    long long unsigned int x[10];
    long long unsigned int y[10];
 } point;
 
 //addition function 
void addition(long long unsigned int A[10], long long unsigned int B[10],long long unsigned int C[10])
{
 	long long unsigned int carry = 0;
	for (int i=9; i>=0; i--){
		C[i] = A[i] + B[i] + carry;
		carry = C[i] >> 29;
		C[i] = C[i] & 0x1fffffff;
	}
}

//addition function mod p
void modular_addition(long long unsigned int A[10], long long unsigned int B[10],long long unsigned int C[10], long long unsigned int p[10])
{
 	long long unsigned int carry = 0;
	for (int i=9; i>=0; i--){
		C[i] = A[i] + B[i] + carry;
		carry = C[i] >> 29;
		C[i] = C[i] & 0x1fffffff;
	}
	if(compare(C,p))
	{
		subs(C,p,C);
	}
}
//subtraction function mod p
void modular_subs (long long unsigned int A[10], long long unsigned int B[10],long long unsigned int C[10], long long unsigned int p[10])
 {
       if(compare(B,A)){
		addition(A,p,A);}
	unsigned int carry = 1;
	for (int i=9; i>=0; i--) 
		{
		C[i] = A[i] + (B[i]^0x1fffffff) + carry;
		carry = C[i] >> 29;
		C[i] = C[i] & 0x1fffffff;
		}
 }
//addition function of two points on an elliptic curve
void point_addition (point p1, point p2, point *p3, long long unsigned int p[10], long long unsigned int mu[10])
 {
 long long unsigned int temp[10];
 long long unsigned int temp1[10];
 long long unsigned int temp3[10];
 long long unsigned int temp4[19];

 modular_subs (p2.y,p1.y,temp,p);
 modular_subs (p2.x,p1.x,temp1,p);
 exponent_l2r(temp1, q, p, mu, temp3); //q=p-2, new p for ec
 field_mult(temp,temp3,temp4);
 barrett(temp4, p, mu, temp);
 field_mult(temp,temp,temp4);
 barrett(temp4, p, mu, temp1);
 
 modular_subs(temp1,p1.x,temp1,p);	
 modular_subs(temp1,p2.x,p3->x,p);
	
 modular_subs(p1.x,p3->x,temp3,p);
 
 field_mult(temp3,temp,temp4);
 barrett(temp4, p, mu, temp1);
 	
 modular_subs(temp1,p1.y,p3->y,p);
 
 /*for(int i=0;i<10;i++)
 	{
 	printf("%llx", p3->x[i]);
 	printf("\n");
	}
	printf("next\n");
for(int i=0;i<10;i++)
 	{
 	printf("%llx", p3.y[i]);
 	printf("\n");
	}*/
 }
 //addition function of two same points on an elliptic curve
 void point_doubling (point p1,long long unsigned int a[10], point *p3, long long unsigned int p[10], long long unsigned int mu[10])
 {
 long long unsigned int temp[19];
 long long unsigned int temp1[10];
 long long unsigned int temp2[10];
 long long unsigned int temp4[10];
 long long unsigned int temp3[19];
 field_mult(p1.x,p1.x,temp);
 barrett(temp, p, mu, temp1);
 
 modular_addition(temp1,temp1,temp2,p);
 		
 modular_addition(temp2,temp1,temp2,p);

 modular_addition(temp2,a,temp2,p);//temp2=a+3x^2
 	 
 modular_addition(p1.y,p1.y,temp1,p);
 
 exponent_l2r(temp1, q, p, mu, temp4); //q=p-2
 
 field_mult(temp4,temp2,temp);
 barrett(temp, p, mu, temp2);//temp2=a+3x1^2/2y1
 
 field_mult(temp2,temp2,temp);
 barrett(temp, p, mu, temp1);
 
 modular_addition(p1.x,p1.x,temp4,p);
 	
 modular_subs(temp1,temp4, p3->x,p); 	
 	
 modular_subs(p1.x,p3->x,temp1,p);
 	
 
 field_mult(temp2,temp1,temp);
 barrett(temp,p, mu,temp4);
 
 
 modular_subs(temp4,p1.y,p3->y,p);
 
 /*for(int i=0;i<10;i++)
 	{
 	printf("%llx", p3->x[i]);
 	printf("\n");
	}
	printf("next\n");
for(int i=0;i<10;i++)
 	{
 	printf("%llx", p3->y[i]);
 	printf("\n");
	}*/
 }
//multiplication by a scelar(n) function  
// Scalar multiplication function
/*void scalar_mult(point A, long long unsigned int n[10], point *B,
                 long long unsigned int p[10], long long unsigned int mu[10]) 
{
    for (int i = 0; i < 10; i++) {
    B->x[i] = 0;
    B->y[i] = 0;
}


    for (int i = 0; i < 10; i++) 
    {
        for (int j = 28; j >= 0; j--) 
        {
            // Double the current point
            point_doubling(B, a, *B, p, mu);

            // If the current bit of scalar `n` is set, add point `A` to `B`
            if (((n[i] >> j) & 1) == 1) 
            {
                point_addition(A, B, *B, p, mu);
            }
        }
    }
}*/


void main()
{
point q1,q2,q3;
 char P []="e92e40ad6f281c8a082afdc49e1372659455bec8ceea043a614c835b7fe9eff5";
 char MU[]="046435b5a40bbb8b91a5ac84a4a1180915a5eeac095be5dc75ddafa730293ae00018";
 unsigned char B1[32], x1[32],y1[32],P1[32],mu1[34];
 long long unsigned int X1[10], Y1[10];
 long long unsigned int p[10], mu[10];
 long long unsigned int result[10];
 long long unsigned int result1[10];
 long long unsigned int C1[10],C2[10], C[19], X[10];
 parse (P, P1);
 base_change (P1,p);
 parse_mu (MU, mu1);
 base_change_mu (mu1,mu);
char A[]=  "02e963f7a5b6206e63c014e86070d01a2421b417c0b64ba9fd166a6d32975a06";
char B[]=  "7a1b32bd8d31ae39d81dd28e3ce95ae1aec3e451fd7593c04ee29f91f5b85abd";
 parse (A, B1);
 base_change (B1,C1);
 /*parse (B, B2);
 base_change (B2,C2);*/
 
char x_coord[]="8dc9a3f04b03a9ebdbf59cf9a4c36a98637ac1dbd40b32b3cc665c037fa38a40"; //x1
char y_coord[]="9f61abf18ec248a6987c574d1677105261558bacff9e1d133120e21dbb86d34c";//y1
char x_dbl[]="d61f0e20925a3adef43848693dc5e37063d9cdccb4fc654822de368bef31c371";//double x1
char y_dbl[]="4a3cfcc426f60b120461ba149c4c9541117311291218b187aa18e7610b5d9cb8";//double y1
char sc[]="00000000000000000000000000000000000000000000000254ab485555111100";
long long unsigned int n[10]={0,0,0,0,0,0,0,0x95,0x55a42aa,0x15111100};
 parse (x_coord, x1);
 base_change (x1,q1.x);
 parse (y_coord, y1);
 base_change (y1,q1.y);
 
 parse (x_dbl, x1);
 base_change (x1,q2.x);
 parse (y_dbl, y1);
 base_change (y1,q2.y);
 
 //point_doubling(q1,C1,&q3,p,mu);
 point_addition(q1,q2,&q3,p, mu); 
 
 //scelar_mult(q1,n,&q3,p,mu);

for(int i=0;i<10;i++)
 	{
 	printf("%llx", q3.x[i]);
 	printf("\n");
	}
	printf("next\n");
for(int i=0;i<10;i++)
 	{
 	printf("%llx", q3.y[i]);
 	printf("\n");
	}


}
