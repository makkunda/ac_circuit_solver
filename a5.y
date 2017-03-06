%{
	#include <stdio.h>
	#include <stdlib.h>
    #include <math.h>
	#include <string.h>
	#include "a5.h"
	#define write_file stdout
	int SynErr;
	int passivecount;
	int activecount ;
	int netcount; 
	passive pass[10000];
	int netposition[10000];
        active  act[10000];
    int coun_net[10000];
    char* netp[10000];
	int exzero;
%}

%union {
	char* stri;
}

%type<stri> resistor inductor capacitor voltage current ;
%token<stri> RNAME LNAME CNAME IND CAP RES NET NUMBER VOLT CURRENT NAME
%token<stri> SINE FREQ DELAY
%token CP
%token EOL
%%
start : 
		|resistor start
		|inductor start
		|capacitor start
		|voltage start
		|current start; 
resistor: NAME NET NET RES {create_passive($1,$2,$3,$4,0);} 
	 | NAME NET NET RES EOL {create_passive($1,$2,$3,$4,0);} ;
inductor: NAME NET NET IND {create_passive($1,$2,$3,$4,1);} 
	|NAME NET NET IND EOL {create_passive($1,$2,$3,$4,1);} ;

capacitor: NAME NET NET CAP {create_passive($1,$2,$3,$4,2);}
	| NAME NET NET CAP EOL {create_passive($1,$2,$3,$4,2);} ;

voltage: VOLT NET NET SINE NUMBER NUMBER FREQ DELAY NUMBER CP {create_active($1,$2,$3,$5,$6,$7,$8,$9,0);} 
	|VOLT NET NET SINE NUMBER NUMBER FREQ DELAY NUMBER CP EOL {create_active($1,$2,$3,$5,$6,$7,$8,$9,0);}
	|VOLT NET NET SINE NUMBER NUMBER FREQ DELAY CP {create_active($1,$2,$3,$5,$6,$7,$8,"0",0);fprintf(write_file1,"damping constant missing \n");}
	|VOLT NET NET SINE NUMBER NUMBER FREQ DELAY CP EOL {create_active($1,$2,$3,$5,$6,$7,$8,"0",0);fprintf(write_file1,"damping constant missing \n");} ;

current:CURRENT NET NET SINE NUMBER NUMBER FREQ DELAY NUMBER CP {create_active($1,$2,$3,$5,$6,$7,$8,$9,1);} 
	|CURRENT NET NET SINE NUMBER NUMBER FREQ DELAY NUMBER CP EOL {create_active($1,$2,$3,$5,$6,$7,$8,$9,1);}
	|CURRENT NET NET SINE NUMBER NUMBER FREQ DELAY CP {create_active($1,$2,$3,$5,$6,$7,$8,"0",1);fprintf(write_file1,"damping constant missing \n");} 
	|CURRENT NET NET SINE NUMBER NUMBER FREQ DELAY CP EOL {create_active($1,$2,$3,$5,$6,$7,$8,"0",1);fprintf(write_file1,"damping constant missing \n");} ;


%%

main(int argc, char* argv[])
{

freopen(argv[1],"r",stdin);
freopen(argv[2],"a",stdout);
    write_file1=fopen("error.txt","w");
    //char zero[] = "0";
   // strcpy(netp[0],"0");
   netp[0]=(char*)calloc(1,10*sizeof(char));
   strcpy(netp[0],"ground");
  // printf("%s\n",netp[0]);
   netp[900]=(char*)calloc(1,10*sizeof(char));
  // strcpy(netp[900],"ground");
   //printf("%s\n",netp[0]);
    passivecount = 0;
    activecount = 0;
    netcount = 1;
    exzero = 0;
    int i,j;
   // printf("hello\n");
    netposition[0] = 0;coun_net[0]=0;
    strcpy(netp[0],"ground");
    //printf("%s\n",netp[0]);
               yyparse();
               long long int sizey=50*(long long int)(passivecount+activecount)+100;
               long long int sizex=10+128*(long long int)(netcount+1);

     fprintf(stdout, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?> \n <svg  width=\"%lld\" height=\"%lld\"     xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\"      version=\"1.1\" id=\"svg3654\">\n",sizex,sizey);

    // printf("%d,%d,%d\n",netcount,passivecount,activecount );
    // printf("hello\n");
    // printf("%d",SynErr);

    fflush(stdout);
    if(exzero)
           draw_ground(10,10,50*(passivecount+activecount),"");
       else
               fprintf(write_file1,"Ground Absent\n");

//printf("hello\n");
fflush(stdout);
    for(i=1;i<netcount;i++){
       //char iname[10];
      // sprintf(iname,"Net%d",netposition[i]-1);
       //    char xx[] = "00";
       //    strcpy(xx,netp[i]);
            //fprintf(write_file1,"%s\n",netp[i]);
           draw_net(10+128*i,10,50*(passivecount+activecount),netp[i]);
    }
    for(i=0;i<passivecount;i++)
     {
       int start = search(pass[i].n1,netcount);
       int end = search(pass[i].n2,netcount);
       if(start>end){int temp=start;start=end;end=temp;}
       if(start!=end)
       {draw_passive(10+start*128,50*i,10+end*128,50*i,pass[i].total,pass[i].type);coun_net[start]++;coun_net[end]++;}
        else
        {
            fprintf(write_file1,"Component %s shorted, hence ignored\n",pass[i].total);
        }
     }
    for(i=0;i<activecount;i++)
     {
       int start = search(act[i].n1,netcount);
       int end = search(act[i].n2,netcount);
              if(start>end){int temp=start;start=end;end=temp;}
    //               FILE *pp=fopen("dub.txt","w");
    // fprintf(pp, "%s11\n",act[i].total);
    if(start!=end)
       {draw_active(10+start*128,50*(i+passivecount),10+end*128,50*(i+passivecount),act[i].total,act[i].type);coun_net[start]++;coun_net[end]++;}
    else
        {
            fprintf(write_file1,"Component %s shorted, hence ignored\n",pass[i].total);
        }

    //fprintf(pp, "%s22\n",act[i].total);
     }

     for(int i=0;i<netcount;i++)
         if(coun_net[i]<2)
         {
             fprintf(write_file1,"Open Circuit at %s\n",netp[i]);
             break;
         }


    fprintf(write_file, "</svg>");
    fflush(stdout);
    double pi = 4*atan(1);
    complex* inf = retzero();
    inf->r = pow(10,10);
    complex* negone = construct(-1,0);
    complex* one = construct(1,0);
    complex** final;
    complex** temp;
    final=(complex **)malloc(sizeof(complex*)*netcount);
    temp=(complex **)malloc(sizeof(complex*)*netcount);
    for(i=0;i<netcount;i++)
    {
        final[i] = retzero();
    }
    for(i=0;i<activecount;i++)
    {
        double w = 2*pi*(act[i].freq);
        matrix* A;
        A = construct_matrix(netcount,netcount);
        for(j=0;j<passivecount;j++)
        {
            int n1 = pass[j].net1;
            int n2 = pass[j].net2;
            complex* vall = retzero();
            if(pass[i].type==0)
            {
                vall->r = -1/(pass[i].val);
            }
            else if(pass[i].type==1)
            {
                vall->c = 1/(w*pass[i].val);
            }
            else{
                vall->c = -1*w*pass[i].val;
            }
            A->start[n1][n2] = add(vall,A->start[n1][n2]);
            A->start[n1][n1] = add(A->start[n1][n1],mult(negone,vall));
            A->start[n2][n2] = add(A->start[n2][n2],mult(negone,vall));
        }
        for(j=0;j<activecount;j++)
        {
            if(j!=i)
            {
                if(act[i].type==0)
                {
                    int n1 = act[i].net1;
                    int n2 = act[i].net2;
                    A->start[n1][n2] = add(mult(negone,inf),A->start[n1][n2]);
                    A->start[n1][n1] = add(A->start[n1][n1],inf);
                    A->start[n2][n2] = add(A->start[n2][n2],inf);
                }
            }
        }
        matrix* Ainv = inverse(A);
        complex** B;
        B=(complex **)malloc(sizeof(complex *)*netcount);
        for(j=0;j<netcount;j++)
        { B[j] = retzero();temp[j] = retzero();}
        // fill B properly not sure how
        int k;
        // temp = Ainv*B
        for(j=0;j<netcount;j++)
        {
            for(k=0;k<netcount;k++)
            {
                temp[j] = add(temp[j],mult(Ainv->start[j][k],B[k]));
            }
            final[j] = add(final[j],temp[j]);
        }
    }
    FILE* res;
    res = fopen("results.txt","r");
    fprintf(res,"VOLTAGES\n");
    for(i=0;i<passivecount;i++)
    {
        complex* valll = sub(final[pass[i].net1],final[pass[i].net2]);
        fprintf(res,"%s %lf %lf \n",pass[i].name,ab(valll),angle(valll));
    }
    for(i=0;i<activecount;i++)
    {
        complex* valll = sub(final[act[i].net1],final[act[i].net2]);
        fprintf(res,"%s %lf %lf \n",act[i].name,ab(valll),angle(valll));
    }
    
    fprintf(res,"\nCURRENTS\n");
}

yyerror(char *s)
{
	//printf("reachyyerr\n");
	//yyparse();
	//fprintf(write_file1,"error reached at %s \n",s);
	SynErr = 1;
	throw_err1();
}
