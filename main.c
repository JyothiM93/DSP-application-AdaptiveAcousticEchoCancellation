#include <processor_include.h>
#include <sysreg.h>
#include <signal.h>
#include <string.h>
#include <filters.h>
#include <stdio.h>
#include <stdlib.h>

#include "inputsig.h"
#include "framework.h"
#define N 128
#define L 64
static int enable,ledcounter;


int n,i,s,k;
static float a=0.42,c=0.01;

float w[L]={0},xin[L]={0},x[N],Z[L],e,y[L],mu,U,wtemp[L],powerD[N],powerE[N],ERLE[L],A[N-L],B[N-L],C[L],QW;
int j,i,n,m;

 static float pm echo[N];

void process(int sig )

{int n;
	sample_t *audioin  = dsp_get_audio();
    sample_t *audioout = dsp_get_audio();
     for(n=0; n<N; ++n) {
        x[n] = audioin[n].left;
       // x[n]=audioin[n].right;
         }
if(enable) 
{
	
   for(n=0;n<N;n++)
	{
	  for(i=0;i<=n;i++)
	  {
	  	if(n-i>=0)
	     {
	       echo[n]+=h[i]*x[n-i];
	      }
	      else
	      {
	      	
	     
	      }
	 }
	 
	 
	}
	
/*	 for (i=0;i<64;i++)
	      {
	      state[i]=0;
	      }*/
/*	for(i=0; i<128; i++)
	{
	//	echo[i]=fir(x[i],h,state,64);
		printf(" %G\n",echo[i]);
	}*/
	
		/*printf("\n state[L]=%f",state[L]);*/
	  for(m=0;m<N;m++)
	  {
	  	
	       for(i=0;i<L;i++)      
	        { 
	          Z[i]=xin[i];
	        }

	        for(j=0;j<L;j++)
	           {
	           	if(j==0)
	           	{
		           xin[0]=x[m];
		             
		            }
		            else
		            {
		            	xin[j]=Z[j-1];
		             }
	           }
		  
	      for(i=0;i<L;i++)
	      {     
	       	y[m]+=w[i]*x[i];    
	      }    
	      e=echo[m]-y[m];
	      
	    for(n=0;n<L;n++)
	    {
		QW+=xin[n]*xin[n];
	    }
	    U=c+QW;
	    mu= a*(1/(float)U);
	    
	    for(n=0;n<L;n++)
         	{
		      wtemp[n]=w[n]+(2*mu*e*xin[n]);
		      w[n]=wtemp[n];
		       }
	    

	
	powerD[m]=echo[m]*echo[m];
	powerE[m]=e*e;
	
		
    
    
	           
  }
	     
	
for(i=0;i<N-L;i++)
{
	for(m=0;m<L;m++)
	{
		A[i]+=powerD[i+m];
		B[i]+=powerE[i+m];

		
	}
	A[i]=A[i]/(float)L;
	B[i]=B[i]/(float)L;
	
	
	
}
for(i=0;i<L;i++)
	{
		C[i]=A[i]/((float)B[i]);
		ERLE[i]=10*log10f(C[i]);
		
     	//printf("%G\n",ERLE[i]);

		
	}
       for(n=0; n<N; n++)
           {
              audioout[n].left = y[n];
              audioout[n].right = y[n];
              
            }		

		
	

}
else
{
	for(n=0; n<N; n++)
    	 {
           audioout[n].left = x[n];
           audioout[n].right = x[n];
    }
}
}
	
	 
	  



static void keyboard(int sig)
{
    unsigned int keys = dsp_get_keys();
    
    if(keys & 1) {
        enable = 1;
    } else if(keys & 2) {
        enable = 0;
    }
    else if(keys & 4) {
        enable = 1;
    }
    else if(keys & 8) {
        enable = 0;
    }
}

static void timer(int sig)
{  
    ledcounter = (ledcounter+1) & 0x3F;
    dsp_set_leds(ledcounter);
}

void main()
{   

	    memset(Z, 0, sizeof(Z));
    
    // Setup the DSP framework
    dsp_init();
 
    // Register interrupt handlers:
    // SIG_SP1: the audio callback
    // SIG_USR0: the keyboard callback
    // SIG_TMZ: the timer callback
    interrupt(SIG_SP1, process);
    interrupt(SIG_USR0, keyboard);
    interrupt(SIG_TMZ, timer);
    timer_set(9830400, 9830400);
    timer_on();
    
    // Enable the DSP framework.
    dsp_start();
    
    // Everything is handled by the interrupt handlers, so just put an empty
    // idle-loop here. If not, the program falls back to an equivalent idle-loop
    // in the run-time library when main() returns.
    for(;;) {
        idle();
    }
}