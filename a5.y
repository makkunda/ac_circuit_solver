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
    int voltcount;
	passive pass[10000];
	int netposition[10000];
        active  act[10000];
    int coun_net[10000];
    char* netp[10000];
	int exzero;
    void printcomplex(complex* xx,char* s)
    {
        fprintf(write_file1,"%s : %lf %lf \n",s,xx->r,xx->c);
    }
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

voltage: VOLT NET NET SINE NUMBER NUMBER FREQ DELAY NUMBER CP {create_active($1,$2,$3,$5,$6,$7,$8,$9,0);voltcount++;}
	|VOLT NET NET SINE NUMBER NUMBER FREQ DELAY NUMBER CP EOL {create_active($1,$2,$3,$5,$6,$7,$8,$9,0);voltcount++;}
	|VOLT NET NET SINE NUMBER NUMBER FREQ DELAY CP {create_active($1,$2,$3,$5,$6,$7,$8,"0",0);fprintf(write_file1,"damping constant missing \n");voltcount++;}
	|VOLT NET NET SINE NUMBER NUMBER FREQ DELAY CP EOL {create_active($1,$2,$3,$5,$6,$7,$8,"0",0);fprintf(write_file1,"damping constant missing \n");voltcount++;} ;

current:CURRENT NET NET SINE NUMBER NUMBER FREQ DELAY NUMBER CP {create_active($1,$2,$3,$5,$6,$7,$8,$9,1);} 
	|CURRENT NET NET SINE NUMBER NUMBER FREQ DELAY NUMBER CP EOL {create_active($1,$2,$3,$5,$6,$7,$8,$9,1);}
	|CURRENT NET NET SINE NUMBER NUMBER FREQ DELAY CP {create_active($1,$2,$3,$5,$6,$7,$8,"0",1);fprintf(write_file1,"damping constant missing \n");} 
	|CURRENT NET NET SINE NUMBER NUMBER FREQ DELAY CP EOL {create_active($1,$2,$3,$5,$6,$7,$8,"0",1);fprintf(write_file1,"damping constant missing \n");} ;


%%

main(int argc, char* argv[])
{
    voltcount = 0;
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
    FILE* res;
    res = fopen("results.txt","w");
    double pi = 4*atan(1);
    complex* inf = retzero();
    inf->r = pow(10,10);
    complex* negone = construct(-1,0);
    complex* one = construct(1,0);
    complex** final;
    complex** temp;
    matrix* A;

    qsort(act,activecount,(sizeof(act[0])),comparator);


    for(i=0;i<activecount;i++)
    {
        temp=(complex **)malloc(sizeof(complex*)*(netcount+voltcount));
        for(j=0;j<(netcount+voltcount);j++)
        {
            temp[j] = retzero();
        }
        double w = 2*pi*(act[i].freq);
        fprintf(res,"FREQUENCY %lfKHz \n",act[i].freq/1000);
        
        
        A = construct_matrix(netcount+voltcount-1,netcount+voltcount-1);
        for(j=0;j<passivecount;j++)
        {
            int n1 = pass[j].net1-1;
            int n2 = pass[j].net2-1;
            complex* vall = retzero();
            if(pass[j].type==0)
            {
                vall->r = -1/(pass[j].val);
            }
            else if(pass[j].type==1)
            {
                vall->c = 1/(w*pass[j].val);
            }
            else{
                vall->c = -1*w*pass[j].val;
            }
            if(n1!=(-1) && n2!=(-1)){
            A->start[n1][n2] = add(vall,A->start[n1][n2]);
            A->start[n1][n1] = add(A->start[n1][n1],mult(negone,vall));
            A->start[n2][n2] = add(A->start[n2][n2],mult(negone,vall));
            A->start[n2][n1] = add(vall,A->start[n2][n1]);
            // fprintf(write_file1,"reach1\n");
            // printcomplex(A->start[n1][n1],"");
            // printcomplex(A->start[n1][n2],"");
            }
            else if(n1!=-1)
            {
                A->start[n1][n1] = add(A->start[n1][n1],mult(negone,vall));
                
                            // fprintf(write_file1,"reach2%d\n",n1);
                            // printcomplex(A->start[n1][n1],"");

            }
            else
            {
                A->start[n2][n2] = add(A->start[n2][n2],mult(negone,vall));
                            // fprintf(write_file1,"reach3%d\n",n2);
                            // printcomplex(A->start[n2][n2],"");

             }
             
        }
        // int ii,jj;
        // for(ii=0;ii<A->row;ii++)
        // {
        //     for(jj=0;jj<A->col;jj++)
        //     {
        //         fprintf(write_file1,"%.14lf,%.14lf ",(A->start[ii][jj])->r,(A->start[ii][jj])->c);
        //     }
        //     fprintf(write_file1,"\n");
        // }

        for(j=0;j<activecount;j++)
        {
            if(act[j].type==0)
            {
                int n1 = act[j].net1-1;
                int n2 = act[j].net2-1;
                int kk = act[j].active_count-1;
                if(n1!=-1){
                    A->start[n1][netcount+kk] = construct(-1,0);
                    A->start[netcount+kk][n1] = construct(-1,0);}
                if(n2!=-1){
                    A->start[n2][netcount+kk] = construct(1,0);
                    A->start[netcount+kk][n2] = construct(1,0);}

            }   
        }
        // int ii,jj;
        // for(ii=0;ii<A->row;ii++)
        // {
        //     for(jj=0;jj<A->col;jj++)
        //     {
        //         fprintf(write_file1,"%.6lf,%.6lf ",A->start[ii][jj]->r,A->start[ii][jj]->c);
        //     }
        //     fprintf(write_file1,"\n");
        // }

        matrix* Ainv = inverse(A);
        // for(ii=0;ii<Ainv->row;ii++)
        // {
        //     for(jj=0;jj<Ainv->col;jj++)
        //     {
        //         fprintf(write_file1,"%.6lf,%.6lf ",Ainv->start[ii][jj]->r,Ainv->start[ii][jj]->c);
        //     }
        //     fprintf(write_file1,"\n");
        // }

        complex** B;
        B=(complex **)malloc(sizeof(complex *)*(netcount+voltcount-1));
        for(j=0;j<(netcount+voltcount-1);j++)
            B[j] = retzero();
        for(j=0;j<activecount;j++)
        {
            if(j==i)
            {
                int n1 = act[j].net1-1;
                int n2 = act[j].net2-1;
                if(act[j].type==1)
                {
                    if(n1!=-1)
                    B[n1] = sub(B[n1],act[j].val);
                    if(n2!=-1)
                    B[n2] = add(B[n2],act[j].val);
                }
                else
                {
                    int kk = act[j].active_count - 1;
                    B[netcount+kk] = act[j].val;
                }
            }
            
        }
        int k;
        // temp = Ainv*B
        for(j=1;j<=(netcount+voltcount-1);j++)
        {
            for(k=0;k<(netcount+voltcount-1);k++)
            {
                temp[j] = add(temp[j],mult(Ainv->start[j-1][k],B[k]));
            }
        }
        fprintf(res,"VOLTAGES\n");
        for(j=0;j<passivecount;j++)
        {
            complex* valll = sub(temp[pass[j].net1],temp[pass[j].net2]);
            fprintf(res,"%s %lf %lf \n",pass[j].name,ab(valll),angle(valll));
            fflush(res);
        }
        fflush(res);
        for(j=0;j<activecount;j++)
        {
            complex* valll = sub(temp[act[j].net1],temp[act[j].net2]);
            fprintf(res,"%s %lf %lf \n",act[j].name,ab(valll),angle(valll));
        }
        fflush(res);
        fprintf(res,"\nCURRENTS\n");
        for(j=0;j<passivecount;j++)
        {
            complex* valll = sub(temp[pass[j].net1],temp[pass[j].net2]);
            complex* vallll = retzero();
            if(pass[j].type==0)
            {
                vallll->r = 1/(pass[j].val);
            }
            else if(pass[j].type==1)
            {
                vallll->c = -1/(w*pass[j].val);
            }
            else{
                vallll->c = 1*w*pass[j].val;
            }
            valll = mult(valll,vallll);
            fprintf(res,"%s %lf %lf \n",pass[j].name,ab(valll),angle(valll));
        }
        for(j=0;j<activecount;j++)
        {
            if(act[j].type==1)
            {
                complex* valll = act[j].val;
                fprintf(res,"%s %lf %lf \n",act[j].name,ab(valll),angle(valll));
            }
            else
            {
                int kkk = act[j].active_count - 1;
                complex* valll = temp[netcount+kkk+1];
                fprintf(res,"%s %lf %lf \n",act[j].name,ab(valll),angle(valll));
            }
        }
        fprintf(res,"\n");
    }
    
}

yyerror(char *s)
{
	//printf("reachyyerr\n");
	//yyparse();
	//fprintf(write_file1,"error reached at %s \n",s);
	SynErr = 1;
	throw_err1();
}
