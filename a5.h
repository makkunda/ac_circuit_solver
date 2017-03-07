#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define write_file stdout
typedef struct
	{
		double r;
		double c;
	} complex;
typedef struct
	{
		int row;
		int col;
		complex*** start;
	} matrix;
double ab(complex* a)
{
	double ar,ac;
	ar = a->r;
	ac = a->c;
	return sqrt(ar*ar + ac*ac);
}
double sqrabs(complex* a)
{
	double ar,ac;
	ar = a->r;
	ac = a->c;
	return (ar*ar + ac*ac);
}
double angle(complex* a)
 {
  double real = a->r;
  double comp = a->c;
  double ans = atan(comp/real);
  double pi = 4*atan(1);
  ans = ans*180/pi;
  return ans;
 }
complex* retzero()            //returns a complex number 0
{
	complex* comp = (complex*) calloc(1,sizeof(complex));
	comp->r = 0;
	comp->c = 0;
	return comp;
}
complex* retone()            //returns a complex number 0
{
	complex* comp = (complex*) calloc(1,sizeof(complex));
	comp->r = 1;
	comp->c = 0;
	return comp;
}

complex* retnegone()            //returns a complex number 0
{
	complex* comp = (complex*) calloc(1,sizeof(complex));
	comp->r = -1;
	comp->c = 0;
	return comp;
}

complex* construct(double a,double b)
{
	complex* comp = (complex*) calloc(1,sizeof(complex));
	comp->r = a;
	comp->c = b;
	return comp;
}

complex *getcopy(complex *a)
{
	complex *b=retzero();
	b->r=a->r;
	b->c=a->c;
	return b;
}

matrix* construct_matrix(int row,int col)
{
	matrix* comp = (matrix*) calloc(1,sizeof(matrix));
	comp->row = row;
	comp->col = col;

	comp->start=(complex ***)malloc(sizeof(complex **) * row);
int i;
//printf("reached\n" );
  for(i=0;i<row;i++)
  	{
  		//printf("reached1\n" );
  		comp->start[i]=(complex **)malloc(sizeof(complex *)*col);

  		int j;

  		for(j=0;j<col;j++)
  			{comp->start[i][j]=retzero();}
  	}
  	return comp;
}

matrix *construct_matrix2(int row,int col,matrix *a)
{
	matrix* comp = construct_matrix(row,col);

	int i,j;

	for(i=0;i<(a->row);i++)
	{
		for(j=0;j<(a->col);j++)
		{
			comp->start[i][j]=getcopy(a->start[i][j]);
		}
	}
	return comp;
}
complex* add(complex* a,complex* b)
{
	complex* comp = (complex*) calloc(1,sizeof(complex));
	comp->r = a->r + b->r;
	comp->c = a->c + b->c;
	return comp;
}

complex* scalarmult(complex* a,double b)
{
	complex* comp = (complex*) calloc(1,sizeof(complex));
	comp->r = b*(a->r);
	comp->c = b*(a->c);
	return comp;
}

complex* conjugate(complex* a)
{
	complex* comp = (complex*) calloc(1,sizeof(complex));
	comp->r = a->r;
	comp->c = -1*(a->c);
	return comp;
}

complex* sub(complex* a,complex* b)
{
	complex* comp = (complex*) calloc(1,sizeof(complex));
	comp->r = a->r - b->r;
	comp->c = a->c - b->c;
	return comp;
}

complex* mult(complex* a,complex* b)
{
	complex* comp = (complex*) calloc(1,sizeof(complex));
	double ar,ac,br,bc;
	ar = a->r; br = b->r; ac = a->c; bc = b->c;
	comp->r = ar*br - ac*bc;
	comp->c = ar*bc + ac*br;
	return comp;
}
complex* divide(complex* a,complex* b)         // a divided by b
{
	complex* ans = mult(a,conjugate(b));
	double div = sqrabs(b);
	div = 1/div;
	return scalarmult(ans,div);
}
matrix* creatematrix(int row,int col)
 {
 	matrix* m = (matrix*) calloc(1,sizeof(matrix));
 	m->row = row;
 	m->col = col;
 	int i,j;
 	for(i=0;i<row;i++)
 	{
 		for(j=0;j<col;j++)
 			  m->start[i][j] = retzero();
 	}
 	return m;
 }
matrix* multmatrix(matrix* a,matrix* b)  //AB
 {
 	if(a->col!=b->row)
 		return NULL;
 	matrix* ans = construct_matrix(a->row,b->col);
 	int i,j,k;
 	int rr = a->row;int cc = b->col;int loop = a->col;
 	for(i=0;i<rr;i++)
 	{
 		for(j=0;j<cc;j++)
 		{
 			for(k=0;k<loop;k++)
 				  ans->start[i][j] = add(ans->start[i][j],mult(a->start[i][k],b->start[k][j]));
 		}
 	}
 	return ans;
 }



matrix *inverse(matrix *b)
{
	    int i, j, k, n;

	    n=b->row;
	    matrix *a=construct_matrix2(n,2*n,b);
    	double d; 
    	complex *aa;

    for (i = 0; i < n; i++)
                a->start[i][i+n]= construct(1,0);

 

      for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(i!=j){
                complex *ratio = divide(a->start[j][i],a->start[i][i]);
                for(k = 0; k < 2*n; k++){
                    a->start[j][k] = sub(a->start[j][k],mult(ratio,a->start[i][k]));
                }
            }
        }
    }

    for(i = 0; i < n; i++){
        aa = getcopy(a->start[i][i]);

        for(j = 0; j < 2*n; j++){
            a->start[i][j] =divide( a->start[i][j],aa);
        }
    }

    matrix *inverse=construct_matrix(n,n);

    for(i=0;i<n;i++)
    	for(j=n;j<2*n;j++)
    		inverse->start[i][j-n]=a->start[i][j];
    	return inverse;
}

double extract_value(char *s)
{
	int x=strlen(s);
	char *temp=malloc(sizeof (char)*x);

	int i;

	for(i=0;i<x;i++)
	{
		if(s[i]!='.' && (s[i]>57 || s[i]<48))
			break;
		temp[i]=s[i];
	}
	temp[i]='\0';
	double val;
	sscanf(temp,"%lf",&val);

	for(;i<x;i++)
	{
		if(s[i]=='F')
		{
			if(i!=x-1)
				val/=(1e15);
		}
		else if(s[i]=='P')
		{
			val/=1e12;
		}
		else if(s[i]=='N')
		{
			val/=1e9;
		}
		else if(s[i]=='U')
		{
			val/=1e6;
		}else if(s[i]=='M')
		{
			if(s[i+1]=='E')
				val*=1e6;
			else
			val/=1e3;
		}
		else if(s[i]=='K')
		{
			val*=1e3;
		}
		break;
	}
	//printf("%.15lf",1/(1e15));


	return val;
}



typedef struct
	{
		char* name;
		int net1;
		int net2;
		char* value;
		char* total;
		int type;
		char* n1;
		char* n2;
		double val;
	} passive;
typedef struct
	{
		char* name;
		int net1;
		int net2;
		char* offset;
		char* amplitude;
		char* frequency;
		char* delay;
		char* damping; 
		char* n1;
		char* n2;
		char* total;
		int type;
		complex* val;
		double freq;
	} active;
void dfs_cycle_detect(int i);
void print_svg();
//void draw_inductor(int sx,int sy,int rotate,char *s);
//void draw_capacitor(int sx,int sy,int rotate,char *s);
//void draw_resistor(int sx,int sy,int rotate,char *s);
//void draw_voltmeter(int sx,int sy,int rotate,char *s);
//void draw_ammemeter(int sx,int sy,int rotate,char *s);
//void draw_line(int sx,int sy,int ex,int ey);
void create_passive(char* name,char* n1,char* n2,char* val,int type);
void create_active(char* name,char* n1,char* n2,char* off,char* amp,char* freq,char* del,char* damp,int type);
void throw_err();
extern passive pass[10000];
extern active  act[10000];
extern int passivecount;
extern int activecount ;
extern int netposition[10000];
extern char* netp[10000];
extern int exzero ;
extern int netcount;
extern int coun_net[10000];
FILE *write_file1;
void throw_err1()
{
	fprintf(write_file1,"syntax error \n");
	exit(-1);
}
int search(char* x,int np)
{
 int i;
 for(i=1;i<np;i++)
  {
    if(strcmp(netp[i],x)==0)
	return i;	
  }
 return 0;
}
void create_passive(char* name,char* n1,char* n2,char* val,int type){
	passive* new1 = (passive*) calloc(1,sizeof(passive)*100);
	new1->name = name;
	int number = 0;int i;
	new1->n1 = n1;new1->n2 = n2;
	if(strcmp(n1,"0")==0)
		{ new1->net1 = 0; exzero = 1;}
	else
	 {
	 	/*for(i=3;n1[i]!='\0';i++)
	 	{
	 		int x = (int)n1[i] - (int)'0';
	 		number = 10*number + x;
	 	}
	 	new1->net1 = number+1;
		if(search(number+1,netcount)==0)
                    {
			netposition[netcount] = number+1;
			netcount++;
		    }	*/
	 	if(search(n1,netcount)==0)
	 	{
	 		new1->net1 = netcount;
	 		//netp[netcount] = n1;
            netp[netcount]= (char*)calloc(1,sizeof(char)*100);
	 		strcpy(netp[netcount],n1);
            coun_net[netcount]=0;
	 		netcount++;
	 	}
	 	else
	 		new1->net1 = search(n1,netcount);
	 }
	 if(strcmp(n2,"0")==0)
		 { new1->net2 = 0; exzero = 1;}
	else
	 {
	 	/*
	 	number = 0;
	 	for(i=3;n2[i]!='\0';i++)
	 	{
	 		int x = (int)n2[i] - (int)'0';
	 		number = 10*number + x;
	 	}
	 	new1->net2 = number+1;
		if(search(number+1,netcount)==0)
                    {
			netposition[netcount] = number+1;
			netcount++;
		    }
		*/ 
		if(search(n2,netcount)==0)
	 	{
	 		new1->net2 = netcount;
	 		//netp[netcount] = n2;
            netp[netcount]= (char*)calloc(1,sizeof(char)*100);
            coun_net[netcount]=0;
	 		strcpy(netp[netcount],n2);
	 		netcount++;
	 	}
	 	else
	 		new1->net2 = search(n2,netcount);   
	 }		
	new1->value = val;
	new1->type = type;
	new1->total = (char*)calloc(1,sizeof(char)*100);
	strcat(new1->total,name);
	strcat(new1->total," ");
	strcat(new1->total,val);
	new1->val = 0;
	pass[passivecount] = *new1;
	passivecount++;
	//printf("%s \n",new1->total);
//fprintf(write_file1,"%s %d %d %s %s\n",new1->total,new1->net1,new1->net2,new1->n1,new1->n2);
} 
void create_active(char* name,char* n1,char* n2,char* off,char* amp,char* freq,char* del,char* damp,int type){
	active* new1 = (active*) calloc(1,sizeof(active)*100);
		new1->name = name;
	int number = 0;int i;
	new1->n1 = n1;new1->n2 = n2;
	if(strcmp(n1,"0")==0)
		{ new1->net1 = 0; exzero = 1;}
	else
	 {
	 	/*for(i=3;n1[i]!='\0';i++)
	 	{
	 		int x = (int)n1[i] - (int)'0';
	 		number = 10*number + x;
	 	}
	 	new1->net1 = number+1;
		if(search(number+1,netcount)==0)
                    {
			netposition[netcount] = number+1;
			netcount++;
		    }	*/
	 	if(search(n1,netcount)==0)
	 	{
	 		new1->net1 = netcount;
	 		//netp[netcount] = n1;
            netp[netcount]= (char*)calloc(1,sizeof(char)*100);
            coun_net[netcount]=0;
	 		strcpy(netp[netcount],n1);
	 		netcount++;
	 	}
	 	else
	 		new1->net1 = search(n1,netcount);
	 }
	 if(strcmp(n2,"0")==0)
		 { new1->net2 = 0; exzero = 1;}
	else
	 {
	 	/*
	 	number = 0;
	 	for(i=3;n2[i]!='\0';i++)
	 	{
	 		int x = (int)n2[i] - (int)'0';
	 		number = 10*number + x;
	 	}
	 	new1->net2 = number+1;
		if(search(number+1,netcount)==0)
                    {
			netposition[netcount] = number+1;
			netcount++;
		    }
		*/ 
		if(search(n2,netcount)==0)
	 	{
	 		new1->net2 = netcount;
	 		//netp[netcount] = n2;
            netp[netcount]= (char*)calloc(1,sizeof(char)*100);
	 		strcpy(netp[netcount],n2);
            coun_net[netcount]=0;
	 		netcount++;
	 	}
	 	else
	 		new1->net2 = search(n2,netcount);   
	 }		
	new1->offset = off;
	new1->amplitude = amp;
	new1->frequency = freq;
	new1->delay = del;
	new1->damping = damp;
	new1->type = type;
	new1->total = (char*)calloc(1,sizeof(char)*100);
	strcat(new1->total,name);
	strcat(new1->total," SINE (");
	strcat(new1->total,off);
	strcat(new1->total," ");
	strcat(new1->total,amp);
	strcat(new1->total," ");
	strcat(new1->total,freq);
	strcat(new1->total," ");
	strcat(new1->total,del);
	strcat(new1->total," ");
	strcat(new1->total,damp);
	strcat(new1->total," )");
	new1->val = retzero();
	new1->freq = 0;
	act[activecount] = *new1;
	activecount++;	
	//printf("%s \n",new1->total);
	//fprintf(write_file1,"%s %d %d %s %s\n",new1->total,new1->net1,new1->net2,new1->n1,new1->n2);
}
//FILE *write_file;

void print_to_file(char *a)
{
	int i=0;
		while(a[i]!='\0')
		{
			if(a[i]=='\"')
				fprintf(write_file,"\"");
			else if(a[i]=='\\')
				fprintf(write_file,"\\");
			else if(a[i]=='\?')
				fprintf(write_file,"\?");
			else if(a[i]=='\'')
				fprintf(write_file,"\'");
			else fprintf(write_file,"%c",a[i]);
			i++;
		}
}

void draw_inductor(int sx,int sy,int rotate,char *s)
{
	char *a="	<path  \n d=\"M 0,32 L 22,32 C 22,32 22,22 32,22 C 42,22 42,32 42,32 C 42,32 42,22 52,22 C 62,22 62,32 62,32 C 62,32 62,22 72,22 C 82,22 82,32 82,32 C 82,32 82,22 92,22 C 102,22 102,32 102,32 L 128,32\" 	\n	style=\"fill:none;fill-opacity:0.75;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\" id=\"path3124\" \0";

		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'translate(%d %d) rotate(%d 0 32)\'/> ",(sx),sy,rotate);

		a="<text x=\"35\" y=\"20\" font-size=\"20\" fill=\"black\"\0";
		print_to_file(a);
		
		fprintf(write_file, " transform=\'translate(%d %d) rotate(%d 32 20)\'> ",(sx),sy,rotate);
		fprintf(write_file,"%s",s);
		fprintf(write_file, "</text>\n");

}

void draw_resistor(int sx,int sy,int rotate,char *s)
{
	char *a="<path	\n	d=\"M 0,32 0,32 L 16,32 L 24,24 L 32,32 L 40,40 L 48,32 L 56,24 L 64,32 L 72,40 L 80,32 L 88,24 L 96,32 L 104,40 L 112,32 L 128,32\" \nstyle=\"fill:none;fill-opacity:0.75;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\" id=\"path3124\" \0";

		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'translate(%d %d) rotate(%d 0 32)\'/> ",(sx),sy,rotate);

		a="<text x=\"35\" y=\"20\" font-size=\"20\" fill=\"black\"\0";
		print_to_file(a);
		
		fprintf(write_file, " transform=\'translate(%d %d) rotate(%d 32 20)\'> ",(sx),sy,rotate);
		fprintf(write_file,"%s",s);
		fprintf(write_file, "</text>\n");
}

void draw_capacitor(int sx,int sy,int rotate,char *s)
{
	char *a="<path d=\"M 64,10 L 64,40\" style=\"fill:none;fill-opacity:0.75;fill-rule:evenodd;stroke:#000000;stroke-width:1.5;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1\" id=\"path17478\" \0";  
			print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'translate(%d %d) rotate(%d 0 25)\'/> ",(sx),sy+7,rotate);

	a="<path d=\"M 70,40 L 70,10\" style=\"fill:none;fill-opacity:0.75;fill-rule:evenodd;stroke:#000000;stroke-width:1.5;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1\" id=\"path17480\" \0";
		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'translate(%d %d) rotate(%d 0 25)\'/> ",(sx),sy+7,rotate);
	a="<path d=\"M 0,25 L 64,25\" style=\"fill:none;fill-opacity:0.75;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\" id=\"path17482\" \0";
		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'translate(%d %d) rotate(%d 0 25)\'/> ",(sx),sy+7,rotate);
	a="<path d=\"M 70,25 L 128,25\" style=\"fill:none;fill-opacity:0.75;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\" id=\"path17484\" \0";
		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'translate(%d %d) rotate(%d 0 25)\'/> ",(sx),sy+7,rotate);

		a="<text x=\"35\" y=\"17\" font-size=\"20\" fill=\"black\"\0";
		print_to_file(a);
		
		fprintf(write_file, " transform=\'translate(%d %d) rotate(%d 32 20)\'> ",(sx),sy,rotate);
		fprintf(write_file,"%s",s);
		fprintf(write_file, "</text>\n");
}

void draw_voltmeter(int sx,int sy,char *s)
{

	char *a="<path d=\"M 40,25 C 40,33.284271 33.284271,40 25,40 C 16.715729,40 10,33.284271 10,25 C 10,16.715729 16.715729,10 25,10 C 33.284271,10 40,16.715729 40,25 L 40,25 z \" style=\"opacity:1;fill:none;fill-opacity:1;stroke:#000000;stroke-width:1;stroke-linecap:square;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\" id=\"path1319\" \0";  
			print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'translate(%d %d)\'/> ",(sx),sy+7);

	a="<path d=\"M 16,25.000005 C 16,25.000005 17.5,20.000004 20.5,20.000004 C 23.5,20.000004 26.5,30.000006 29.5,30.000006 C 32.5,30.000006 34,25.000005 34,25.000005\" style=\"fill:none;fill-opacity:0.75;fill-rule:evenodd;stroke:#000000;stroke-width:1.00000012px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\" id=\"path4056\" \0";
		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'          translate(%d %d)   \'/> ",(sx),sy+7);
	a="<path d=\"M 10 25 L 0 25\" style=\"opacity:1;fill:none;fill-opacity:1;stroke:#000000;stroke-width:1;stroke-linecap:square;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\" id=\"path1319\" \0";
		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'          translate(%d %d)   \'/> ",(sx),sy+7);
	a="<path d=\"M 40 25 L 50 25\" style=\"opacity:1;fill:none;fill-opacity:1;stroke:#000000;stroke-width:1;stroke-linecap:square;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\" id=\"path1319\" \0";
		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'          translate(%d %d)   \'/> ",(sx),sy+7);

		a="<text x=\"0\" y=\"0\" font-size=\"15\" fill=\"black\"\0";
		print_to_file(a);
	
		fprintf(write_file, " transform=\'        translate(%d %d) \'> ",(sx-25),sy+12);
		fprintf(write_file,"%s",s);
		fprintf(write_file, "</text>\n");
}

void draw_ammeter(int sx,int sy,char *s)
{
	char *a="<path d=\"M 40,25 C 40,33.284271 33.284271,40 25,40 C 16.715729,40 10,33.284271 10,25 C 10,16.715729 16.715729,10 25,10 C 33.284271,10 40,16.715729 40,25 L 40,25 z \" style=\"opacity:1;fill:none;fill-opacity:1;stroke:#000000;stroke-width:1;stroke-linecap:square;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\" id=\"path1319\" \0";  
			print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'translate(%d %d)\'/> ",(sx),sy+7);

	a="<path d=\"M 35 25 L 15 25 L 20 20 L 15 25 L 20 30\" style=\"opacity:1;fill:none;fill-opacity:1;stroke:#000000;stroke-width:1;stroke-linecap:square;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\" id=\"path1319\" \0";
		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'          translate(%d %d)   \'/> ",(sx),sy+7);
	a="<path d=\"M 10 25 L 0 25\" style=\"opacity:1;fill:none;fill-opacity:1;stroke:#000000;stroke-width:1;stroke-linecap:square;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\" id=\"path1319\" \0";
		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'          translate(%d %d)   \'/> ",(sx),sy+7);
	a="<path d=\"M 40 25 L 50 25\" style=\"opacity:1;fill:none;fill-opacity:1;stroke:#000000;stroke-width:1;stroke-linecap:square;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\" id=\"path1319\" \0";
		print_to_file(a);
		//fprintf(write_file,"%s",a);
		fprintf(write_file, "transform=\'          translate(%d %d)   \'/> ",(sx),sy+7);

		a="<text x=\"0\" y=\"0\" font-size=\"15\" fill=\"black\"\0";
		print_to_file(a);
	
		fprintf(write_file, " transform=\'        translate(%d %d) \'> ",(sx-25),sy+8);
		fprintf(write_file,"%s",s);
		fprintf(write_file, "</text>\n");
}

void draw_line(int sx,int sy,int ex,int ey)
{
	fprintf(write_file,"<path d=\"M %d %d L %d %d \" style=\"opacity:1;fill:none;fill-opacity:1;stroke:#000000;stroke-width:1;stroke-linecap:square;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\" id=\"path1319\"/>",sx,sy,ex,ey);  
}

void extend_resistor(int sx,int sy,int ex,int ey,char *s)
{
	int remain=(ex-sx)-128;
	int to_draw=remain/2;
	int trial=32;
	sy+=trial;

	draw_line(sx,sy,sx+to_draw,sy);
	//remain=remain-to_draw;
	draw_line(ex-(remain-to_draw),sy,ex,sy);
	draw_resistor(sx+to_draw,sy-trial,0,s);
}

void extend_capacitor(int sx,int sy,int ex,int ey,char *s)
{
	int remain=(ex-sx)-128;
	int to_draw=remain/2;
	int trial=32;
	sy+=trial;

	if(to_draw!=0)to_draw+=10;

	draw_line(sx,sy,sx+to_draw,sy);
	//remain=remain-to_draw;
	draw_line(ex-(remain-to_draw),sy,ex,sy);
	draw_capacitor(sx+to_draw,sy-trial,0,s);
}
void extend_inductor(int sx,int sy,int ex,int ey,char *s)
{
	int remain=(ex-sx)-128;
	int to_draw=remain/2;
	int trial=32;
	sy+=trial;


	draw_line(sx,sy,sx+to_draw,sy);
	//remain=remain-to_draw;
	draw_line(ex-(remain-to_draw),sy,ex,sy);
	draw_inductor(sx+to_draw,sy-trial,0,s);
}
void extend_voltmeter(int sx,int sy,int ex,int ey,char *s)
{
	// 	FILE *pp=fopen("dub.txt","a");
	// fprintf(pp, "%s44\n",s);
	int remain=(ex-sx)-50;
	int to_draw=remain/2;
	int trial=32;
	sy+=trial;
	if(to_draw!=0)to_draw+=20;

	draw_line(sx,sy,sx+to_draw,sy);
	//remain=remain-to_draw;
	draw_line(ex-(remain-to_draw),sy,ex,sy);
	draw_voltmeter(sx+to_draw,sy-trial,s);
}
void extend_ammeter(int sx,int sy,int ex,int ey,char *s)
{
	int remain=(ex-sx)-50;
	int to_draw=remain/2;
	int trial=32;
	sy+=trial;
	if(to_draw!=0)to_draw+=10;

	draw_line(sx,sy,sx+to_draw,sy);
	//remain=remain-to_draw;
	draw_line(ex-(remain-to_draw),sy,ex,sy);
	draw_ammeter(sx+to_draw,sy-trial,s);
}
void draw_net(int x,int sy,int ey,char *s)
{
    draw_line(x,sy,x,ey);

    

    char *a="<text x=\"0\" y=\"0\" font-size=\"20\" fill=\"black\"\0";
        print_to_file(a);
   
        fprintf(write_file, " transform=\'        translate(%d %d) \'> ",(x-15),ey+14);
        fprintf(write_file,"%s",s);
        fprintf(write_file, "</text>\n");

}
void draw_ground(int x,int sy,int ey,char *s)
{
    draw_line(x,sy,x,ey);

    //int mid=(sy+ey)/2;

    char *a="<text x=\"0\" y=\"0\" font-size=\"20\" fill=\"black\"\0";
        print_to_file(a);

        fprintf(write_file, " transform=\'        translate(%d %d) \'> ",(x+2),ey-2);
        fprintf(write_file,"%s",s);
        fprintf(write_file, "</text>\n");

        fprintf(write_file, "<circle cx=\"%d\" cy=\"%d\" r=\"2\" fill=\"black\" /> \n ",x,ey);

        draw_line(x-10,ey,x+10,ey);
        draw_line(x-5,ey+4,x+5,ey+4);
        draw_line(x-2,ey+8,x+2,ey+8);
}
void draw_passive(int sx,int sy,int ex , int ey,char* s,int type)
 {
   if(type==0)
       extend_resistor(sx,sy,ex,ey,s);
   else if(type==1)
       extend_inductor(sx,sy,ex,ey,s);
   else
      extend_capacitor(sx,sy,ex,ey,s);
  fprintf(write_file, "<circle cx=\"%d\" cy=\"%d\" r=\"2\" fill=\"black\" /> \n ",sx,sy+32);
  fprintf(write_file, "<circle cx=\"%d\" cy=\"%d\" r=\"2\" fill=\"black\" /> \n ",ex,ey+32);
 }

void draw_active(int sx,int sy,int ex , int ey,char* s,int type)
 {
   if(type==0)
       extend_voltmeter(sx,sy,ex,ey,s);
   else
      extend_ammeter(sx,sy,ex,ey,s);
  fprintf(write_file, "<circle cx=\"%d\" cy=\"%d\" r=\"2\" fill=\"black\" /> \n ",sx,sy+32);
  fprintf(write_file, "<circle cx=\"%d\" cy=\"%d\" r=\"2\" fill=\"black\" /> \n ",ex,ey+32);
 //  	FILE *pp=fopen("dub.txt","a");
	// fprintf(pp, "%s33\n",s);
 }




