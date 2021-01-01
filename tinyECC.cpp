// **********************************************************************************
// Author: Shubham Annigeri	github.com/ShubhamAnnigeri
// co-Author: Sunit Raut   	github.com/SunitRaut
// **********************************************************************************
// License
// **********************************************************************************
// This program is free software; you can redistribute it 
// and/or modify it under the terms of the GNU General    
// Public License as published by the Free Software       
// Foundation; either version 3 of the License, or        
// (at your option) any later version.                    
//                                                        
// This program is distributed in the hope that it will   
// be useful, but WITHOUT ANY WARRANTY; without even the  
// implied warranty of MERCHANTABILITY or FITNESS FOR A   
// PARTICULAR PURPOSE. See the GNU General Public        
// License for more details.                              
//                                                        
// Licence can be viewed at                               
// http://www.gnu.org/licenses/gpl-3.0.txt
//
// Please maintain this license information along with authorship
// and copyright notices in any redistribution of this code
// **********************************************************************************


#include <Arduino.h>
#include <tinyECC.h>



//Mapping Table stored in PROGRAM MEMORY
const int alpha[MAPPING_TABLE_SIZE] PROGMEM = MAPPING_TABLE_TYPE;


String tinyECC::plaintext;
String tinyECC::ciphertext;

int tinyECC::a;
int tinyECC::b;
int tinyECC::p;

int tinyECC::PubKey[2];
int tinyECC::Pbase[2];
int tinyECC::PrivKey;
int tinyECC::TempArr[2];
int tinyECC::PbaseSer[2];
int tinyECC::PubSer[2];



tinyECC::tinyECC()//Constructor
{
	
	a = 1;
	b = 23;
	p = 991;
	PubKey[0]=775;PubKey[1]=553;
	Pbase[0]=1;Pbase[1]=5;
	PrivKey = 12;
	TempArr[0]=0;TempArr[1]=0;
	PubSer[0]=775;PubSer[1]=553;
	PbaseSer[0]=1;PbaseSer[1]=5;
 
}

void tinyECC::encrypt()
{
	ciphertext = "";			//clear ciphertext
	m=0;					//initialize message hash
	for(int i=0;i<(plaintext.length());i++)
	{  
        	
		uint8_t x=plaintext[i];
		if(MAPPING_TABLE_SIZE==MAPPING_TABLE_10_SIZE)
			x =plaintext[i]-'0';
        	m=(m+x)%50;
        	//printf("\nFirst One = %d\n",x);
        	//Serial.println("Character is "+String(x));
        	int P[2];
        	P[0]=pgm_read_word_near(alpha + 2*x);
        	P[1]=pgm_read_word_near(alpha + 2*x+1);

        	//Serial.println("The point it is mapped to is : ("+String(P[0])+","+String(P[1])+")");

        	encode(P,PubKey,Pbase);
 	
		ciphertext+=String(E[0])+","+String(E[1])+","+String(E[2])+","+String(E[3])+","; 
		//Serial.println(ciphertext.length()); 		
	}
	m = m%50;

}//end of encrypt()


void tinyECC::genSig()
{
	//PrivKey,m
	
    	int k,x,e;
    	int s=0;
    	long int buf=0;
    	int n=997;
    	E[0]=0;
    	E[1]=0;
    	while(s==0 || x==0)
    	{
    		k=(rand()%(p-2))+1;

    		//printf("The base point is (%d,%d)\n",PbaseSer[0],PbaseSer[1]);
		TempArr[0]=Pbase[0];
   		TempArr[1]=Pbase[1];
    		sclr_mult(k,TempArr);
    		//sclr_mult(k,PbaseSer);
    		x=E[0]%n;//Same as r
    		
    		//printf("r=%d\n",x);
    		//printf("Private key is %d\n",PrivKey);
    		//printf("The base point is (%d,%d)\n",Pbase[0],Pbase[1]);
 
    		e=m;//Using message itself as hash
    		e=e%n;
    		//Serial.println("e="+String(e)+"\n");
    		long int a,b;
    		a=(inverse1(k));
    		b=(e+(PrivKey*x));
    		//Serial.println(a);
    		//Serial.println(b);
    		//Serial.println(a*b);
    		buf=(a*b)%n;
    		s=buf;
    		//Serial.println("s="+String(s)+"\n");
    	}
    	Sig[0]=x;
    	Sig[1]=s;
    	//Serial.println("The Signature is ("+String(E[0])+","+String(E[1])+")\n");

}



void tinyECC::decrypt()
{	
  	String t;
  	int k=0; int x=0;
	m = 0;
	plaintext = ""; 			//clear plaintext

  	for(int i=0;i<ciphertext.length();i++)
  	{
    		if (ciphertext[i]==',')
    		{
      			t=ciphertext.substring(k,i);
      			//Serial.println(t);
      			k=i+1;
      			//encmsg[x/4][x%4]=t.toInt();
			P[x%4]=t.toInt();
			if((x%4)==3)
			{
				decode();
				
				for(int j=0;j<MAPPING_TABLE_SIZE/2;j++)//Loop for searching character the point is mapped to
        			{
                        		int temp0=pgm_read_word_near(alpha + 2*j);
            				int temp1=pgm_read_word_near(alpha + 2*j+1);
            				//Serial.println(temp0);
            				//Serial.println(temp1);
            
            				if(temp0==E[0] && temp1==E[1])
            				{
                				//decrypted[i]=j;
						m = (m + j)%50;
						if(MAPPING_TABLE_SIZE==MAPPING_TABLE_10_SIZE)
							plaintext+=char(j+'0');
						else
							plaintext+=char(j);
						//Serial.println(char(j));
						//Serial.println(plaintext.length());
            				}
        			}
			}
      			x++;
			//Serial.println(String(x));
    		}
	//Serial.println(String(i));
  	}
  	m = m%50;
  	//Sig[0]=encmsg[(x/4)][0];       //last two integers are signature
  	//Sig[1]=encmsg[(x/4)][1];
  	
  	//Serial.println("Signature=("+String(Sig[0])+","+String(Sig[1])+")");
  	//{Serial.print(F("Signature=("));Serial.print(String(Sig[0]));Serial.print(F(","));Serial.print(String(Sig[1]));Serial.print(F(") \n"));}

//	dec_pkt_size=(x-2)/4;


}//end of decrypt()



bool tinyECC::verifySig()
{	
	//PubKey, m, P[2]
   	int n=997;
	P[0]=Sig[0];
	P[1]=Sig[1];
    	//Serial.println("P=("+String(P[0])+","+String(P[1])+")\n");
    	//printf("\n\nIn Verify Function\n\n");
    	//printf("r=%d\n",P[0]);
    	//printf("s=%d\n",P[1]);
   	if (P[1]>=p || P[0]>=p)
        	return 0;
    	else
    	{
         	long int e,w;
         	long int u1,u2;
         	long int buf=0;
         	//m=m%10;
         	//e=(sqrt((alpha[m][0]*alpha[m][0])+(alpha[m][1]*alpha[m][1])));//Using modulus as a Hash function
         	e=m;
         	e=e%n;
         	//Serial.println("\n\nVerification\n\n");
         	//Serial.println(e);

         	//Serial.println("Inverse="+String(inverse1(P[1])));
         	w=inverse1(P[1])%n;
         	//Serial.println(w);

         	buf=(e*w)%n;
         	u1=buf;
         	//Serial.println(u1);
         	buf=(P[0]*w)%n;
         	u2=buf;
         	//Serial.println(u2);
        
        	int P1[2],P2[2],P3[2];
        	//printf("The base point is (%d,%d)\n",PbaseSer[0],PbaseSer[1]);
		TempArr[0]=PbaseSer[0];TempArr[1]=PbaseSer[1];
        	sclr_mult(u1,TempArr);
        	P1[0]=E[0];
        	P1[1]=E[1];
        	//printf("The base point is (%d,%d)\n",Pbase[0],Pbase[1]);
        	//printf("u1*Pbase = (%d,%d)\n",P1[0],P1[1]);
    
        	TempArr[0]=PubKey[0];TempArr[1]=PubKey[1];
        	//printf("The Public Key is (%d,%d)\n",PubKey[0],PubKey[1]);
        	sclr_mult(u2,TempArr);
        	P2[0]=E[0];
        	P2[1]=E[1];
        	//printf("The Public Key is (%d,%d)\n",PubKey[0],PubKey[1]);
        	//printf("u2*Public key = (%d,%d)\n",P2[0],P2[1]);
    
    		//PbaseSer[0]=1;//Deciding the base point here
    		//PbaseSer[1]=5;
    		//printf("The base point is (%d,%d)\n",Pbase[0],Pbase[1]);
    		//PubSer[0]=775;
    		//PubSer[1]=553;
    		//PbaseSer[0]=1;//Deciding the base point here
    		//PbaseSer[1]=5;
        
        	add(P1,P2);
        	P3[0]=E[0];
        	P3[1]=E[1];
        	//printf("Point Generated by Addition = (%d,%d)\n",P3[0],P3[1]);

    		//PbaseSer[0]=1;//Deciding the base point here
    		//PbaseSer[1]=5;
    		//printf("The base point is (%d,%d)\n",Pbase[0],Pbase[1]);
    		//PubSer[0]=775;
    		//PubSer[1]=553;
    		//PbaseSer[0]=1;//Deciding the base point here
    		//PbaseSer[1]=5;
    		//Serial.println(P3[0]);
        	if(isPAI(P3))
            		return 0;
        	if(P3[0]==P[0])
            		return 1;
        	else
            		//printf("\nGenerated Signature is %d. Required Value is %d\n",P3[0],P[0]);
            	return 0;
    }
}






void tinyECC::encode(int m[2],int Pb[2],int Pbase[2])
{
	do{
    		E[0]=0;
    		E[1]=0;
    		E[2]=0;
    		E[3]=0;
    		//int Pm[2]={0,0};
    		int out[4]={0,0,0,0};

	    	//Pm[0]=m[0];
    		//Pm[1]=m[1];
    		//Serial.println("Message=("+String(m[0])+","+String(m[1])+")\n");
    		int temp1[2];
    		int k=(rand()%50);
		TempArr[1]=PubKey[1];
    		TempArr[0]=PubKey[0];
    		sclr_mult(k,TempArr);
    		//sclr_mult(k,Pb);
    		temp1[0]=E[0];
    		temp1[1]=E[1];

    		add(m,temp1);
    		out[2]=E[0];
    		out[3]=E[1];
   		TempArr[0]=Pbase[0];
   		TempArr[1]=Pbase[1];
    		sclr_mult(k,TempArr);
   	 	//sclr_mult(k,Pbase);
    		out[0]=E[0];
    		out[1]=E[1];    
		E[2]=out[2];
		E[3]=out[3];
    		//printf("\n\n %d,%d",out[2],out[3]);
	}while((E[0]==0)||(E[1]==0)||(E[2]==0)||(E[3]==0));
    	return;
}




void tinyECC::sclr_mult(int k,int P[2])		//using LSB first algorithm
{
	E[0]=0;
    	E[1]=1;
    	int i;
    	int Q[2]={0,0};
    	Q[0]=0;
    	Q[1]=0;
   
    	for(i=31;i>=0;i--)
    	{
        	if((k>>i)&1)
            	break;
 	}
    	//Serial.println("MSB lies on position "+String(i));
    	//printf("\n%d\n",i);
    	for(int j=0;j<=i;j++)
    	{
        	//Serial.println("Q=("+String(Q[0])+","+String(Q[1])+")");
        	//Serial.println("P=("+String(P[0])+","+String(P[1])+")");
        	if((k>>j)&1)
        	{
            		add(Q,P);
            		Q[0]=E[0];
            		Q[1]=E[1];
            		//Serial.println("P+Q=");
            		//Serial.println("Q=("+String(Q[0])+","+String(Q[1])+")");
        	}
        	//Serial.println("P=2*P");
        	add(P,P);
        	P[0]=E[0];
        	P[1]=E[1];
        	//Serial.println("P=("+String(P[0])+","+String(P[1])+")");
       		// Serial.println("End of loop\n\n");
    	}
    	//Serial.println(String(Q[0])+","+String(Q[1]));
    	E[0]=Q[0];
    	E[1]=Q[1];
    	return;
}

void tinyECC::add( int P[2],int Q[2])
{
    	//printf("\nA=(%d,%d)\nB=(%d,%d)\n",A[0],A[1],B[0],B[1]);
   	E[0]=0;
    	E[1]=0;
   
    	int C[2]={0,0};
    	C[0]=0;
    	C[1]=0;
    	long int A[2],B[2];
    	long long int x=0,l=0;         //int C[2];
        //int C[2];
    	A[0]=P[0];
    	A[1]=P[1];
    	B[0]=Q[0];
    	B[1]=Q[1];
    	if (isPAI(P) && isPAI(Q))  // PAI + PAI = PAI
    	{
        	E[0]=0;
        	E[1]=0;
        	return;
    	}
    	if (P[0]==0 && P[1]==0)
    	{
        	E[0]=Q[0];
        	E[1]=Q[1];
        	return;
    	}
    	if (Q[0]==0 && Q[1]==0)
    	{
        	E[0]=P[0];
        	E[1]=P[1];
        	return;
    	}
    	//if (A[1]==(p-B[1]))
    	//{
    	//    return C;
    	//}
    	if ((A[0]==B[0]) && (A[1]==B[1]))
    	{
        	if (A[1] % p == 0)              // Vertical tangent
        	{
          		E[0]=0;
          		E[1]=0;
          		return;
       	 	}
        	//Serial.println("A=("+String(A[0])+","+String(A[1])+")");
        	//Serial.println("B=("+String(B[0])+","+String(B[1])+")");
        	x=((3*(A[0]*A[0]))+a)*inverse(2*A[1]);
        	l=((x*x)-(2*A[0]))%p;
        	C[0]=l;
        	l=((x*(A[0]-C[0]))-A[1])%p;
        	C[1]=l;
        	//Serial.println("Point Doubling");
        	//Serial.println("buf=("+String(buf[0])+","+String(buf[1])+")");
        	//C[0]=((A[0]*A[0])%p+(b*inverse(A[0]*A[0]))%p)%p;//For Binary Curves
        	//C[1]=((A[0]*A[0])%p+((A[0]+(A[1]*inverse(A[0]))*C[0]))%p+(C[0])%p)%p;//For Binary Curves
    	}
   	else
    	{
        	if ((B[0] - A[0]) % p == 0)    // Vertical secant
        	{        
          		E[0]=0;
          		E[1]=0;
          		return;
        	}
        	x=(B[1]-A[1])*inverse(B[0]-A[0]);
        	l=((x*x)-(A[0]+B[0]))%p;
        	//printf("\nl=%lld",l);
        	C[0]=l;
        	l=((x*(A[0]-C[0]))-A[1])%p;
        	C[1]=l;
        	//printf("\nThis is being executed  %d\n");
        	//C[0]=((((A[1]+B[1])*inverse(A[0]+B[0]))*((A[1]+B[1])*inverse(A[0]+B[0])))%p + ((A[1]+B[1])*inverse(A[0]+B[0]))%p + A[0]%p + B[0]%p + a%p)%p;//For Binary Curves
        	//C[1]=((((A[1]+B[1])*inverse(A[0]+B[0]))*(A[0]+C[0]))+C[0]+A[1])%p;//For Binary Curves
    	}
    	if (C[0]<0)
        	C[0]=p+C[0];
    	if (C[1]<0)
       		C[1]=p+C[1];
    	E[0]=C[0];
    	E[1]=C[1];
    	return;
}




int tinyECC::isPAI(int *point)
{
    	return ((point[0]==0) && (point[1]==0));
}

int* tinyECC::getPAI(int *point)
{
    	point[0] = 0;
    	point[1] = 0;
    	return point;
}

long int tinyECC::inverse(long int num)
{
    	int i=1;
   	if (num<0)
        num=p+num;
    	for (i=1;i<p;i++)
    	{
        	if(((num*i)%p)==1)
           	break;
    	}
    	//printf("inverse=%d,%d",i,num);
    	return i;
}

int tinyECC::inverse1(int num)
{
    	long int n=997,i,x;
    	x=num;
    	if (num<0)
        	num=n+num;
    	for (i=1;i<n;i++)
    	{
        	if(((x*i)%n)==1)
        	{
            	//Serial.println("inverse="+String(i)+"\nNumber is "+String(num));
            	break;
        	}
    	}
    	return i;
}





void tinyECC::decode()
{
    	E[0]=0;
    	E[1]=0;
    	int pt1[2],pt2[2],buf[2];
    	int x[2]={0,0};
    
    	pt1[0]=P[0];
    	pt1[1]=P[1];
    	pt2[0]=P[2];
    	pt2[1]=P[3];

    	sclr_mult(PrivKey,pt1);
    	buf[0]=E[0];
    	buf[1]=E[1];
    	buf[1]=p-buf[1];

    	add(pt2,buf);
    	x[0]=E[0];
    	x[1]=E[1];
    	//=printf("Received is %d",x);
    	//m=floor((x-1)*inverse(random));
    	//Serial.println("The Decoded points are ("+String(x[0])+","+String(x[1])+")");
    	return;
}






