/* Управляющий модуль: ввод схемы, анализ и диагностика */

// extern unsigned _stklen = 26000U;

#include "cirsym.h"

#include <sys/time.h>

char input_file_name[500];
char output_file_name[500];
char settings_file_name[500];



FILE *out,*inpa,*set;

char *b,*c,sp;
unsigned long leng=0,lengl,lepr=0;
int flag_fi3,flag_fi4,flag_fi5,flag_fsn,extract,flag_mat,
    flag_e,cirf,noequ,flag_pln,nuz=0,flag_sp,flag_cL,flag_num,flag_nul,
    flag_two,flag_thr,flag_out,flag_opt,st_pln,bi3=0,bi4=0,bi5=0;
float range2,range3,range4,range5;


/*-------------------------------------------------------*/
void parse_command_line(int argc, char* argv[])
{
	int i,i2,k;

	/* setup default file names */
//	strcpy( input_file_name,    "cir" );
//	strcpy( output_file_name,   "out" );
	strcpy( settings_file_name, "setup.sym" );
	flag_num=99;

	/* parse command line arguments */
	for (i=1; i<argc; i++)
	   {
	    flag_num=atoi(argv[i]);
	    printf("flag_num=%d\n",flag_num);
	    printf("%s",argv[i]);
	   }
}
void mirnul(char s1,char s2,char s3,char s4,int *act_n,SOURCE *act,char type)
{
 char *w,w0[2];

 w0[0]='\0'; w=w0;
 if (type=='M') strcat(w++,"M");
 else
   if (type=='Q') strcat(w++,"Q");
   else
     if (type=='T') strcat(w++,"T");
 act[*act_n].v1=s1;
 act[*act_n].v2=s2;
 act[*act_n].v3=s3;
 act[*act_n].v4=s4;
 act[*act_n].kol=1;
 act[*act_n].reb=strdup(w0);
 (*act_n)++;
}
 void mirall (char s1,char s2,char s3,char s4,int *p,char *str,
	      int *n,PASSIVE *matr,int *act_n,SOURCE *act)
{
 char *w,w0[2],sp0,sp1,sp2,sp3;
 int i,np;

 // printf("p=%d\n",*p);
 // for (i=0;i<*p;i++) printf("%c",str[i]);

 np=*p;
beg0:
 for (i=0;i<*p;i++) if (np+48==str[i]) {np++;goto beg0;}
 sp0=np+48;
 str[*p]=sp0;
 (*p)++;
 np=*p;
beg1:
 for (i=0;i<*p;i++) if (np+48==str[i]) {np++;goto beg1;}
 sp1=np+48;
 str[*p]=sp1;
 (*p)++;
 np=*p;
beg2:
 for (i=0;i<*p;i++) if (np+48==str[i]) {np++;goto beg2;}
 sp2=np+48;
 str[*p]=sp2;
 (*p)++;
 np=*p;
beg3:
 for (i=0;i<*p;i++) if (np+48==str[i]) {np++;goto beg3;}
 sp3=np+48;
 str[*p]=sp3;
 (*p)++;
 // np=*p;

// printf("pm=%d\n",*p);
// for (i=0;i<*p;i++) printf("%c",str[i]);

 w0[0]='\0'; w=w0;
 strcat(w++,"1");
 act[*act_n].v1=s1;
 act[*act_n].v2=sp0;
 act[*act_n].v3=sp1; //sp0; 
 act[*act_n].v4=sp0; //sp1; 
 act[*act_n].kol=-1;
 act[*act_n].reb=strdup(w0);
 (*act_n)++;
 act[*act_n].v1=sp3; //sp2;
 act[*act_n].v2=sp2; //sp3;
 act[*act_n].v3=s3;
 act[*act_n].v4=sp2;
 act[*act_n].kol=-1;
 act[*act_n].reb=strdup(w0);
 (*act_n)++;
 act[*act_n].v1=s2;
 act[*act_n].v2=sp1;
 act[*act_n].v3=s4;
 act[*act_n].v4=sp3;
 act[*act_n].kol=-1;
 act[*act_n].reb=strdup(w0);
 (*act_n)++;
 w0[0]='\0'; w=w0;
 strcat(w++,"g");
 matr[*n].v1='0';
 matr[*n].v2=sp0;
 matr[*n].kol=1;
 matr[*n].reb=strdup(w0);
 (*n)++;
 matr[*n].v1='0';
 matr[*n].v2=sp1;
 matr[*n].kol=1;
 matr[*n].reb=strdup(w0);
 (*n)++;
 matr[*n].v1='0';
 matr[*n].v2=sp2;
 matr[*n].kol=1;
 matr[*n].reb=strdup(w0);
 (*n)++;
 matr[*n].v1='0';
 matr[*n].v2=sp3;
 matr[*n].kol=1;
 matr[*n].reb=strdup(w0);
 (*n)++;
 fprintf(out,"g=1;\n");
}
 void qirall (char s1,char s2,char s3,char s4,int *p,char *str,
	      int *n,PASSIVE *matr,int *act_n,SOURCE *act)
{
 char *w,w0[2],sp0,sp1;
 int i,np;

 np=*p;
beg0:
 for (i=0;i<*p;i++) if (np+48==str[i]) {np++;goto beg0;}
 sp0=np+48;
 str[*p]=sp0;
 (*p)++;
 np=*p;
beg1:
 for (i=0;i<*p;i++) if (np+48==str[i]) {np++;goto beg1;}
 sp1=np+48;
 str[*p]=sp1;
 (*p)++;

 w0[0]='\0'; w=w0;
 strcat(w++,"1");
 act[*act_n].v1=s1;
 act[*act_n].v2=s2;
 act[*act_n].v3=s4;
 act[*act_n].v4=sp1;
 act[*act_n].kol=-1;
 act[*act_n].reb=strdup(w0);
 (*act_n)++;
 act[*act_n].v1=sp1; //sp0;
 act[*act_n].v2=sp0; //sp1;
 act[*act_n].v3=s3;
 act[*act_n].v4=sp0;
 act[*act_n].kol=-1;
 act[*act_n].reb=strdup(w0);
 (*act_n)++;
 w0[0]='\0'; w=w0;
 strcat(w++,"g");
 matr[*n].v1='0';
 matr[*n].v2=sp0;
 matr[*n].kol=1;
 matr[*n].reb=strdup(w0);
 (*n)++;
 matr[*n].v1='0';
 matr[*n].v2=sp1;
 matr[*n].kol=1;
 matr[*n].reb=strdup(w0);
 (*n)++;
 fprintf(out,"g=1;\n");
}
 void tirall(char s1,char s2,char s3,char s4,int *p,char *str,
	     int *n,PASSIVE *matr,int *act_n,SOURCE *act)
{
 char *w,w0[2],sp0,sp1;
 int i,np;

 np=*p;
beg0:
 for (i=0;i<*p;i++) if (np+48==str[i]) {np++;goto beg0;}
 sp0=np+48;
 str[*p]=sp0;
 (*p)++;
 np=*p;
beg1:
 for (i=0;i<*p;i++) if (np+48==str[i]) {np++;goto beg1;}
 sp1=np+48;
 str[*p]=sp1;
 (*p)++;

 w0[0]='\0'; w=w0;
 strcat(w++,"1");
 act[*act_n].v1=s1;
 act[*act_n].v2=sp0;
 act[*act_n].v3=sp1; //sp0;
 act[*act_n].v4=sp0; //sp1;
 act[*act_n].kol=-1;
 act[*act_n].reb=strdup(w0);
 (*act_n)++;
 act[*act_n].v1=s2;
 act[*act_n].v2=sp1;
 act[*act_n].v3=s3;
 act[*act_n].v4=s4;
 act[*act_n].kol=-1;
 act[*act_n].reb=strdup(w0);
 (*act_n)++;
 w0[0]='\0'; w=w0;
 strcat(w++,"g");
 matr[*n].v1='0';
 matr[*n].v2=sp0;
 matr[*n].kol=1;
 matr[*n].reb=strdup(w0);
 (*n)++;
 matr[*n].v1='0';
 matr[*n].v2=sp1;
 matr[*n].kol=1;
 matr[*n].reb=strdup(w0);
 (*n)++;
 fprintf(out,"g=1;\n");
}
/* преобразование cir-файла в рабочие структуры matr и act */
int main(int argc, char** argv)
{
 int k,i0,i1,i2;
 int nr,nc,nl,ng,nn,nm,nq,nt,ns,nk,nf,nh,
     nsub,da,ne,nj,nu,ni,dui,nej;

 long double vl,fbeg;
 char st[30],nbr[20],el[100],str[80],type,
      n1,n2,n3,n4,*t,t0[20];
 SOURCE *act;
 PASSIVE *matr,*diagui;

 // struct time timen;

 int i,j,n,m,act_n,ncL;
 long ft,ftb,fte,jl;
 char *y,*y0;

 // gettime(&timen);
 parse_command_line( argc, argv );

 el[0]='\0';
 t0[0]='\0';

 if ((set=fopen(settings_file_name,"r"))!=NULL)
  {
   fgets(el,99,set); /* задание режимов бисекции */
   fscanf(set,"%d %f\n",&flag_fi5,&range5);
   fgets(el,99,set);
   fscanf(set,"%d %f\n",&flag_fi4,&range4);
   fgets(el,99,set);
   fscanf(set,"%d %f\n",&flag_fi3,&range3);
   fgets(el,99,set);
   fscanf(set,"%d %f\n",&flag_fsn,&range2);
   fgets(el,99,set);
   fscanf(set,"%d\n",&extract);
   fgets(el,99,set);
   fscanf(set,"%d\n",&noequ);
   fgets(el,99,set);
   fscanf(set,"%d\n",&flag_sp);
   fgets(el,99,set);
   fscanf(set,"%d\n",&flag_e);
   fgets(el,99,set);
   fscanf(set,"%d\n",&flag_cL);
   fgets(el,99,set);
   fscanf(set,"%d\n",&flag_pln);
   fgets(el,99,set);
   fscanf(set,"%d\n",&flag_nul);
   fgets(el,99,set);
   fscanf(set,"%d\n",&flag_opt);
   fgets(el,99,set);
   fscanf(set,"%d\n",&flag_mat);
   fclose(set);
  }
 else {printf("\n   Cannot open setup.sym file!"); exit(1);}


 /* printf("Version 01.2005 (C) Ulyanovsk, USTU, Filaretov V."); */

// if (flag_pln) flag_mir=1;
 flag_out=1;
 if (flag_sp) sp = 's'; else sp = 'p';
 st[0]='\0'; y=st;
 st[0]='*'; y++; st[1]=sp; y++; st[2]='\0';


  c = (char*) malloc(500);
  y0=(char*) malloc(100);

     j=0;
beg:
     j++;
     if (j > flag_num) goto exi;
     y0[0]='\0'; y=y0;
     sprintf(y,"%d",j);

     strcpy(input_file_name,y);
     if ((inpa=fopen(input_file_name,"r"))==NULL) goto beg;
     else
       {
	strcpy(output_file_name,input_file_name);
	strcat(output_file_name,".out");
	if ((out=fopen(output_file_name,"w"))!=NULL)
	  {
	   fclose(out);
	   out=fopen(output_file_name,"r+");
	  }



 for (m=0;; m++)
    {
//    el[0]='\0';
//    y0[0]='\0'; y=y0;
    fgets(el,99,inpa);
    fputs("\n",out);
    fputs("% ",out);
    fputs(el,out);

     if (el[0]=='.' && (el[1]=='t' || el[1]=='T'))
       {
	fclose(inpa);
	break;
       }
     leng=0; lepr=0; nuz=0;

     nr=0,nc=0,nl=0,ng=0,nn=0,nm=0,nq=0,nt=0,ns=0,nk=0,nf=0,nh=0,
     nsub=0,da=0,ne=0,nj=0,nu=0,ni=0,dui=0;


     matr=(PASSIVE *) malloc (300*sizeof(PASSIVE));
     act=(SOURCE *) malloc (300*sizeof(SOURCE));
     diagui=(PASSIVE *) malloc (50*sizeof(PASSIVE));

     n=0;  act_n=0;


 for (i0=0;i0<1000;i0++)
    {
     fgets(el,99,inpa);
     if (el[0]=='*' || el[0]=='\n' || el[0]=='\t') continue;
     else
     if (el[0]=='.' && (el[1]=='x' || el[1]=='X')) continue;
     else
     if (el[0]=='.' && (el[1]=='e' || el[1]=='E')) goto new_is_a_bad_label_since_it_is_also_a_keyword;
     else
     if (el[0]=='.' && (el[1]=='a' || el[1]=='A')) type='w';
     else
     if (el[0]=='.' && (el[1]=='d' || el[1]=='D')) {da=1; continue;}
     else
     if (el[0]=='r') {type='r'; nr++;}
     else
     if (el[0]=='R') {type='R'; nr++;}
     else
     if (el[0]=='c') {type='c'; nc++;}
     else
     if (el[0]=='C') {type='C'; nc++;}
     else
     if (el[0]=='l') {type='l'; nl++;}
     else
     if (el[0]=='L') {type='L'; nl++;}
     else
     if (el[0]=='g') {type='g'; ng++;}
     else
     if (el[0]=='G') {type='G'; ns++;}
     else
     if (el[0]=='k' || el[0]=='K') {type='K'; nk++;}
     else
     if (el[0]=='b' || el[0]=='B') {type='B'; nf++;}
     else
     if (el[0]=='h' || el[0]=='H') {type='H'; nh++;}
     else
     if (el[0]=='n' || el[0]=='N') {type='N'; nn++;}
     else
     if (el[0]=='m' || el[0]=='M') {type='M'; nm++;}
     else
     if (el[0]=='q' || el[0]=='Q') {type='Q'; nq++;}
     else
     if (el[0]=='t' || el[0]=='T') {type='T'; nt++;}
     else
     if (el[0]=='j' || el[0]=='J') {type='J'; nj++;}
     else
     if (el[0]=='e' || el[0]=='E') {type='E'; ne++;}
     else
     if (el[0]=='i' || el[0]=='I') {type='I'; ni++;}
     else
     if (el[0]=='u' || el[0]=='U') {type='U'; nu++;}
     else
     if (el[0]=='x' || el[0]=='X') {type='X'; nsub++;}
     else
     if (el[0]=='.' && (el[1]=='t' || el[1]=='T')) goto beg;
     else
       {
	printf("\n Cannot execute %s file!",el);
	fprintf(out,"\n%% Cannot execute %s file!",el); exit(0);
       }
     if (   type=='R' || type=='r' || type=='C' || type=='c'
	 || type=='L' || type=='l' || type=='g'
	 || type=='E' || type=='J' || type=='U' || type=='I')
       {
	extra(99,el,0,&i1,nbr,&k);
	extra(99,el,i1,&i2,str,&k);
	if (k) n1=atoi(str)+48;
	extra(99,el,i2,&i1,str,&k);
	if (k) n2=atoi(str)+48;
	extra(99,el,i1,&i2,str,&k);
	if (k) vl=atof(str);
       }
     if (   type=='G' || type=='K' || type=='B' || type=='H'
	 || type=='N' || type=='M' || type=='Q' || type=='T')
       {
	extra(99,el,0,&i1,nbr,&k);
	extra(99,el,i1,&i2,str,&k);
	if (k) n1=atoi(str)+48;
	extra(99,el,i2,&i1,str,&k);
	if (k) n2=atoi(str)+48;
	extra(99,el,i1,&i2,str,&k);
	if (k) n3=atoi(str)+48;
	extra(99,el,i2,&i1,str,&k);
	if (k) n4=atoi(str)+48;
	extra(99,el,i1,&i2,str,&k);
	if (k) vl=atof(str);
       }
     if (type=='R' || type=='r' || type=='L' || type=='l'
		   || type=='C' || type=='c' || type=='g')
       {
	str[0]=type; str[1]='\0'; t=str;
	strcat(t,nbr);
	if (type == 'R')
	  {
	   str[0]='R';
	   if (flag_out) fprintf(out,"%s=%Lg;\n",str,vl);
	  }
	if (type == 'r')
	  {
	   str[0]='g';
	   if (flag_out) fprintf(out,"%s=1/%Lg;\n",str,vl);
	  }
	if (type == 'g')
	  {
	   str[0]='g';
	   if (flag_out) fprintf(out,"%s=%Lg;\n",str,vl);
	  }
	if (!flag_pln)
	  {
	   if (type == 'C')
	     {
	      str[0]='z';
	      if (flag_out)
		{
		 fprintf(out,"C%s=%Lg;\n",nbr,vl);
		 fprintf(out,"%s=1/(%c*C%s);\n",str,sp,nbr);
		}
	     }
	   if (type == 'c')
	     {
	      if (!flag_cL)
		{
		 str[0]='y';
		 if (flag_out)
		   {
		    fprintf(out,"c%s=%Lg;\n",nbr,vl);
		    fprintf(out,"%s=%c*c%s;\n",str,sp,nbr);
		   }
		}
	      else
		{
		 if (flag_out) fprintf(out,"c%s=%Lg;\n",nbr,vl);
		 if (!flag_pln) strcat(str,st);
		}
	     }
	   if (type == 'L')
	     {
	      if (!flag_cL)
		{
		 str[0]='Z';
		 if (flag_out)
		   {
		    fprintf(out,"L%s=%Lg;\n",nbr,vl);
		    fprintf(out,"%s=%c*L%s;\n",str,sp,nbr);
		   }
		}
	      else
		{
		 if (flag_out) fprintf(out,"L%s=%Lg;\n",nbr,vl);
		 if (!flag_pln) strcat(str,st);
		}
	     }
	   if (type == 'l')
	     {
	      str[0]='Y';
	      if (flag_out)
		{
		 fprintf(out,"l%s=%Lg;\n",nbr,vl);
		 fprintf(out,"%s=1/(%c*l%s);\n",str,sp,nbr);
		}
	     }
	  }
	else
	  {
	   if (type == 'C' || type == 'c')
	     {
	      if (flag_out) fprintf(out,"c%s=%Lg;\n",nbr,vl);
	      str[0]='c';
	     }
	   if (type == 'L' || type == 'l')
	     {
	      if (flag_out) fprintf(out,"L%s=%Lg;\n",nbr,vl);
	      str[0]='L';
	     }
	  }
	matrall(n1,n2,1,str,&n,matr);
       }
     if (type=='G' || type=='K' || type=='B' || type=='H')
       {
	str[0]=type;
	str[1]='\0'; t=str;
	strcat(t,nbr);
	if (flag_out) fprintf(out,"%s=%Lg;\n",str,vl);
	actall(n1,n2,n3,n4,1,str,&act_n,act);
       }
     if (type=='E' || type=='J')
       {
	str[0]=type;
	str[1]='\0'; t=str;
	strcat(t,nbr);
	if (flag_out) fprintf(out,"%s=%Lg;\n",str,vl);
	actall(n1,n2,n1,n2,999,str,&act_n,act);
       }
     if (type=='U' || type=='I')
       {
	str[0]=type;
	str[1]='\0'; t=str;
	strcat(t,nbr);
	matrall(n1,n2,1,str,&dui,diagui);
       }
    if (type=='N')
       {
	nuiall(n1,n2,n3,n4,&act_n,act);
       }
    if (type=='M' || type=='Q'|| type=='T')
      mirnul(n1,n2,n3,n4,&act_n,act,type);
     if (flag_out && type=='w')
       {
	extra(99,el,0,&i1,nbr,&k);
/* закон изменения частоты */
	extra(99,el,i1,&i2,str,&k);
	extra(99,el,i2,&i1,str,&k);
/*	if (k) nw=atoi(str)+48;  */
	extra(99,el,i1,&i2,str,&k);
	if (k) fbeg=atof(str);
	extra(99,el,i2,&i1,str,&k);
/*	if (k) fend=atof(str);  */
	fprintf(out,"f=%Lf;\n",fbeg);
	fprintf(out,"%c=2*3.14159265358979323j*f;\n",sp);
	fprintf(out,"%% Values:\n");
       }
    }
//     fclose(inpa);
new_is_a_bad_label_since_it_is_also_a_keyword:
if (flag_nul && nm+nq+nt)
  {
   int p,na,act1_n=0;
   char *strn,s1,s2,s3,s4;
   GRAPH *a3;

   SOURCE *act1;
   act1 = (SOURCE *) malloc (200*sizeof(SOURCE));

   a3=(GRAPH *) malloc ((n+8*(act_n))*sizeof(GRAPH));
   copypas(n,matr,a3);
   copyact(n,act_n,act,&na,a3);
   strn=(char*) malloc(2*na+1);
   verstr(na,a3,&p,strn); free(a3);
   for (k=0;k<act_n;k++)
      {
       if (act[k].kol < 0 || act[k].kol == 999)
	 {
	  act1[act1_n].v1=act[k].v1;
	  act1[act1_n].v2=act[k].v2;
	  act1[act1_n].v3=act[k].v3;
	  act1[act1_n].v4=act[k].v4;
	  act1[act1_n].kol=act[k].kol;
	  act1[act1_n].reb=strdup(act[k].reb);
	  act1_n++;
	 }
      }
   for (k=0;k<act_n;k++)
      {
       if (act[k].kol < 0 || act[k].kol == 999) continue;
       s1=act[k].v1; s2=act[k].v2;
       s3=act[k].v3; s4=act[k].v4;
       type=act[k].reb[0];
       if (type=='M') mirall(s1,s2,s3,s4,&p,strn,&n,matr,&act1_n,act1);
       else
       if (type=='Q') qirall(s1,s2,s3,s4,&p,strn,&n,matr,&act1_n,act1);
       else
       if (type=='T') tirall(s1,s2,s3,s4,&p,strn,&n,matr,&act1_n,act1);
      }
   free(strn);

   act1=(SOURCE *) realloc (act1,act1_n*sizeof(SOURCE));
   actcopy(act1_n,act1,act);
   freeact(act1_n,act1);
   act_n=act1_n;
  }
 // for (k=0;k<act_n;k++)
    // fprintf(out,"%d %d %d %d\n",act[k].v1-48,act[k].v2-48,act[k].v3-48,act[k].v4-48);
 ncL=nc+nl;
 nej=ne+nj;

 if (flag_out) 
{
 printf(
 "\n/R/=%d,/g/=%d,/C/=%d,/L/=%d,/G/=%d,/K/=%d,/B/=%d,/H/=%d,/N/=%d,/M/=%d,/Q/=%d,/T/=%d\n",
 nr,ng,nc,nl,ns,nk,nf,nh,nn,nm,nq,nt);

 fprintf(out,
 "\n%%/R/=%d,/g/=%d,/C/=%d,/L/=%d,/G/=%d,/K/=%d,/B/=%d,/H/=%d,/N/=%d,/M/=%d,/Q/=%d,/T/=%d\n",
 nr,ng,nc,nl,ns,nk,nf,nh,nn,nm,nq,nt);
}

	matr=(PASSIVE *) realloc (matr,n*sizeof(PASSIVE));
	act=(SOURCE *) realloc (act,act_n*sizeof(SOURCE));
	diagui=(PASSIVE *) realloc (diagui,dui*sizeof(PASSIVE));


	  if (!dui || da)
	    {
	     if (!flag_pln) detan(n,matr,act_n,act);
	     else detanp(n,matr,act_n,act,ncL);
	    }
	  else
	    {
	     if (dui == 1 && nej == 1)
	       {
		if (!flag_pln) cirfunst(n,matr,act_n,act,diagui);
		else cirfunpa(n,matr,act_n,act,diagui,ncL);
	       }
	     else
	       {
		if (!flag_pln) analys(n,matr,act_n,act,dui,diagui);
		else analysp(n,matr,act_n,act,dui,diagui,ncL);
	       }
	    }

	if (dui) {for (i=0;i<dui;i++) free(diagui[i].reb); free(diagui);}

        if (flag_out) 
          {
	   printf("\nLength of expression = %ld characters",leng);
	   fprintf(out,"\n\n%% Length of expression = %ld characters",leng);
	  }
        fprintf(out,"\n\n%% END");
          
    } // end for m

     if (flag_out) 
       {    
	/* ftb=ftell(out);
	fseek(out,-1,SEEK_END);
	fte=ftell(out);
	ft=fte-ftb;
	fseek(out,-ft-1,SEEK_END);
	ft=ft/70 + 1;
	for (jl=0;jl<ft;jl++)
	   {
	    for (i=0;i<70;i++) fputc(' ',out);
	    fputs("\n",out);
	   }
	   */
	{
	  /*	 struct time timek;

	 gettime(&timek);
	 if (timen.ti_hund>timek.ti_hund)
	   {
	    timen.ti_sec++;
	    timek.ti_hund+=99;
	   }
	 if (timen.ti_sec>timek.ti_sec)
	   {
	    timen.ti_min++;
	    timek.ti_sec+=60;
	   }
	 if (timen.ti_min>timek.ti_min)
	   {
	    timen.ti_hour++;
	    timek.ti_min+=60;
	   }
	 if (timen.ti_hour>timek.ti_hour) timek.ti_hour+=24;
	 printf(
	 "\nExecution time: %02d hour %02d min. %02d.%02d sec.                    \n",
	 timek.ti_hour-timen.ti_hour,timek.ti_min-timen.ti_min,
	 timek.ti_sec-timen.ti_sec,timek.ti_hund-timen.ti_hund);
	 fprintf(out,
	 "\n\n%% Execution time: %02d hour %02d min. %02d.%02d sec.                    \n",
	 timek.ti_hour-timen.ti_hour,timek.ti_min-timen.ti_min,
	 timek.ti_sec-timen.ti_sec,timek.ti_hund-timen.ti_hund);
	  */
	}
       }
      fclose(out);
     }
 goto beg;
exi:
 free(c); free(y0);
}
