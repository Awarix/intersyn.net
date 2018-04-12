/* ê†ß´Æ¶•≠®• ·Â•¨≠Æ£Æ ÆØ‡•§•´®‚•´Ô ØÆ Á†·‚Ô¨ - °®·•™Ê®Ô ·Â•¨Î */

#include "cirsym.h"

extern FILE *out,*inpa,*set;

extern char *b,*c,sp;
extern unsigned long leng,lengl,lepr;
extern int flag_fi3,flag_fi4,flag_fi5,flag_fsn,extract,
       flag_e,cirf,noequ,flag_pln,nuz,flag_sp,flag_cL,st_pln,flag_mat;
extern float range2,range3,range4,range5;

/*************************************************************/
void transfor(int n,PASSIVE *matr,
	      int act_n,SOURCE *act,int *num,GRAPH *ac)
{
 int i;

 *num=-1;
 for (i=0;i<n;i++)
    {
     ++(*num);
     ac[*num].v1=matr[*num].v1;
     ac[*num].v2=matr[*num].v2;
     ac[*num].ess=0;
    }
 for (i=0;i<act_n;i++)
    {
     ++(*num);
     ac[*num].v1=act[i].v1;
     ac[*num].v2=act[i].v2;
     ac[*num].ess=i+1;
     ++(*num);
     ac[*num].v1=act[i].v3;
     ac[*num].v2=act[i].v4;
     ac[*num].ess=-(i+1);
    }
 (*num)++;
}
/***************************************************************/
void fortrans(int num,GRAPH *ac,int n,PASSIVE *matr,SOURCE *act,
	      int *n1,PASSIVE *a1,int *act1_n,SOURCE *act1,
	      int key,char *ac0)
{
 int i,j,k;

 *n1=-1;
 *act1_n=-1;
 for (i=0;i<num;i++)
    {
     if (key==1 && ac[i].v1 == 'Ô' || key==2 && ac[i].v1 != 'Ô')
       {
	k=ac[i].ess;
	if (!k)
	  {
	   (*n1)++;
	   for (j=0;j<n;j++)
	      {
	       if (   ac0[i]==matr[j].v1 && ac[i].v2==matr[j].v2
		   || ac0[i]==matr[j].v2 && ac[i].v2==matr[j].v1)
		 {
                  if (matr[j].kol < 0) continue;
		  a1[*n1].v1=matr[j].v1;
		  a1[*n1].v2=matr[j].v2;
		  a1[*n1].kol=matr[j].kol;
		  a1[*n1].reb=strdup(matr[j].reb);
                  matr[j].kol=-matr[j].kol;
		  break;
		 }
	      }
	  }
	else
	  {
	   if (k > 0)
	     {
	      (*act1_n)++;
	      act1[*act1_n].v1=act[k-1].v1;
	      act1[*act1_n].v2=act[k-1].v2;
	      act1[*act1_n].v3=act[k-1].v3;
	      act1[*act1_n].v4=act[k-1].v4;
	      act1[*act1_n].kol=act[k-1].kol;
	      act1[*act1_n].reb=strdup(act[k-1].reb);
	     }
	  }
       }
    }
 (*n1)++;
 (*act1_n)++;
}
/**********************************************************/
void fortest(int num,GRAPH *ac,int *n1,int *act1_n,int key)
{
 int i,k;

 *n1=-1;
 *act1_n=-1;
 for (i=0;i<num;i++)
    {
     if (key==1 && ac[i].v1=='Ô' || key==2 && ac[i].v1!='Ô')
       {
	k=ac[i].ess;
	if (!k) (*n1)++;
	else
	if (k > 0) (*act1_n)++;
       }
    }
 (*n1)++;
 (*act1_n)++;
}
/******************************************************/
void bond1(int n,GRAPH *a1,char s1,char s2,int *k)
{
 int i;
 char s4;

 for (i=0;i<n;i++)
    {
     if (*k ==n) break;
     if (a1[i].v1==s1)
       {
	if (a1[i].v2==s2)
	  {
	   a1[i].v1='Ô'; (*k)++;
	   continue;
	  }
	s4=a1[i].v2;
	a1[i].v1='Ô'; (*k)++;
	bond1(n,a1,s4,s2,&*k);
	if (s1==s2) break;
       }
     if (a1[i].v1!='Ô' && a1[i].v2==s1)
       {
	if (a1[i].v1==s2)
	  {
	   a1[i].v1='Ô'; (*k)++;
	   continue;
	  }
	s4=a1[i].v1;
	a1[i].v1='Ô'; (*k)++;
	bond1(n,a1,s4,s2,&*k);
	if (s1==s2) break;
       }
    }
}
/*********************************************************/
int indep1(int nc,GRAPH *ac)
{
 int i,j;

 for (i=0;i<nc;i++)
    {
     if (!ac[i].ess || ac[i].v1 == 'Ô') continue;
     for (j=0;j<nc;j++)
	{
	 if (!ac[j].ess || ac[j].v1 == 'Ô') continue;
	 if (ac[i].ess==-ac[j].ess) goto eq1;
	}
     return(0);
eq1: continue;
    }
 return(1);
}
/*****************************************************************/
void form1(int num,GRAPH *ac,char *ac0,int n,PASSIVE *matr,
	   int act_n,SOURCE *act)
{
 int n1,act1_n,i,j,p,key;
 unsigned long dl,dl1;
 PASSIVE *a1;
 SOURCE *act1;

 dl=leng+strlen(c);
 for (i=1;i<3;i++)
    {
     if (i==2) key=2;
     else key=1;
     if (i==2) strcat(b++,"*");
     fortest(num,ac,&n1,&act1_n,key);
     if (n1)
       {
	a1=( PASSIVE *) malloc (n1*sizeof(PASSIVE));
       }
     if (act1_n)
       {
	act1=( SOURCE *) malloc (act1_n*sizeof(SOURCE));
       }
     if (i==2) for (j=0;j<n;j++)
                  if (matr[j].kol < 0) matr[j].kol=-matr[j].kol;
     fortrans(num,ac,n,matr,act,&n1,a1,&act1_n,act1,key,ac0);
     node_n(n1,a1,act1_n,act1,&p);
     if (n1+act1_n > 1) strcat(b++,"(");
     dl1=leng+strlen(c);
     gggf(n1,a1,act1_n,act1);
     if (n1) free(a1); if (act1_n) free(act1);
     if (dl1>=leng+strlen(c))
       {
	ster(dl); break;
       }
     else if (n1+act1_n > 1) strcat(b++,")");
    }
 for (i=0;i<n;i++) free(matr[i].reb);
 for (i=0;i<act_n;i++) free(act[i].reb);
}
/*  ********* èéÑèêéÉêÄååÄ ìóÖíÄ òÄêçàêéÇ **********
    ************************************************ */
void bisec1(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag)
{
 int i,k,p,num;
 char *str,*ac0;
 GRAPH *ac=( GRAPH *) malloc ((n+2*act_n)*sizeof(GRAPH));

 str=(char*) malloc(2*n+4*act_n+1);
 ac0=(char*) malloc(n+2*act_n);
 nodestr(n,matr,act_n,act,&p,str);
 transfor(n,matr,act_n,act,&num,ac);
 for (k=0;k<num;k++) ac0[k]=ac[k].v1;
 *flag=0;
 for (i=0;i<p;i++)
    {
  //   if (str[i] != '0') continue; 
     k=0;
     bond1(num,ac,str[i],str[i],&k);
     if (k == num || !indep1(num,ac)) goto rep;
     printf("*");
     *flag=1;
     form1(num,ac,ac0,n,matr,act_n,act);
     break;
rep:
     for (k=0;k<num;k++) ac[k].v1=ac0[k];
    }
 free(ac0); free(ac); free(str);
}
/***************************************************************/
void form2(int num,GRAPH *ac,char *ac0,char s1,char s2,
	   int n,PASSIVE *matr,int act_n,SOURCE *act)
{
 int i,j,n1,act1_n,p,key;
 unsigned long dl,dl1,dl2;
 char *t,t0[2];
 PASSIVE *a1;
 SOURCE *act1;

 t0[0]='\0'; t=t0;
 strcat(t++,"1");
 dl2=leng+strlen(c);
 for (i=1;i<5;i++)
    {
     if (i%2)
       {
	dl=leng+strlen(c); key=1;
	if (dl != dl2) strcat(b++,"+");
       }
     else key=2;
     if (!(i%2)) strcat(b++,"*");
     fortest(num,ac,&n1,&act1_n,key);
     if (n1)
       {
	a1=( PASSIVE *) malloc (n1*sizeof(PASSIVE));
       }
     if (i==1 || i==4) act1_n++;
     if (act1_n)
       {
	act1=( SOURCE *) malloc (act1_n*sizeof(SOURCE));
       }
     if (i==3) for (j=0;j<n;j++) if (matr[j].kol < 0) matr[j].kol=-matr[j].kol;
     fortrans(num,ac,n,matr,act,&n1,a1,&act1_n,act1,key,ac0);
     if (i==1 || i==4) nui(s1,s2,&act1_n,act1);
     node_n(n1,a1,act1_n,act1,&p);
     if (n1 || act1_n)
       {
	if (n1+act1_n > 1) strcat(b++,"(");
	dl1=leng+strlen(c);
	gggf(n1,a1,act1_n,act1);
	if (n1) free(a1); if (act1_n) free(act1);
	if (dl1>=leng+strlen(c))
	  {
	   ster(dl); if (i%2) i++; continue;
	  }
	else if (n1+act1_n > 1) strcat(b++,")");
       }
     else
       {
	if (n1)
	  {
	   for (j=0;j<n1;j++) free(a1[j].reb); free(a1);
	  }
	if (act1_n)
	  {
	   for (j=0;j<act1_n;j++) free(act1[j].reb); free(act1);
	  }
	ster(dl); if (i%2) i++; continue;
       }
    }
 for (j=0;j<n;j++) free(matr[j].reb);
 for (j=0;j<act_n;j++) free(act[j].reb);
}
/***********************************************************/
void bond2(int n,GRAPH *a1,char s1,char s2,char s3,int *k)
{
 int i;
 char s4;

 for (i=0;i<n;i++)
    {
     if (*k > n-2)
       {
	*k=n;
	break;
       }
     if (a1[i].v1==s1)
       {
	if (a1[i].v2==s2 || a1[i].v2==s3)
	  {
	   a1[i].v1='Ô'; (*k)++;
	   continue;
	  }
	s4=a1[i].v2;
	a1[i].v1='Ô'; (*k)++;
	bond2(n,a1,s4,s2,s3,&*k);
	if(s1==s2) break;
       }
     if (a1[i].v1!='Ô' && a1[i].v2==s1)
       {
	if (a1[i].v1==s2 || a1[i].v1==s3)
	  {
	   a1[i].v1='Ô'; (*k)++;
	   continue;
	  }
	s4=a1[i].v1;
	a1[i].v1='Ô'; (*k)++;
	bond2(n,a1,s4,s2,s3,&*k);
	if(s1==s2) break;
       }
    }
}
/*********************************************************/
int indep2(int nc,GRAPH *ac,
	   char *ac0,char s1,char s2,int *k)
{
 int i,j;

 for (i=0;i<nc;i++)
    {
     if (!ac[i].ess) goto eq1;
     if (ac[i].v1 == 'Ô') continue;
     for (j=0;j<nc;j++)
	{
	 if (!ac[j].ess || ac[j].v1 == 'Ô') continue;
	 if (ac[i].ess==-ac[j].ess) goto eq1;
	}
     if (ac0[i]==s1 && ac[i].v2==s2 || ac[i].v2==s1 && ac0[i]==s2)
       {
	ac[i].v1='Ô';
	(*k)++;
       }
eq1: continue;
    }
 for (i=0;i<nc;i++)
    {
     if (!ac[i].ess) goto eq2;
     if (ac[i].v1 != 'Ô') continue;
     for (j=0;j<nc;j++)
	{
	 if (!ac[j].ess || ac[j].v1 != 'Ô') continue;
	 if (ac[i].ess==-ac[j].ess) goto eq2;
	}
     if (ac0[i]==s1 && ac[i].v2==s2 || ac[i].v2==s1 && ac0[i]==s2)
       {
	ac[i].v1=ac0[i];
	(*k)--;
       }
     else return(0);
eq2: continue;
    }
 return(1);
}
/*************************************************************/
 void bisec2(int n,PASSIVE *matr,int act_n,SOURCE *act,
             int *flag,float range)
{
 int i,j,l,i3,j3,h,m,k,p,num,numa,fl11,fl12,fl21,fl22,mn,mk;
 char s1,s2,s1o,s2o,*str,*ac0,o1,o2;
 float pr1,rel,nrel,drel;

 GRAPH *ac=( GRAPH *) malloc ((n+2*act_n+100)*sizeof(GRAPH));
 GRAPH *ace=( GRAPH *) malloc ((n+2*act_n+100)*sizeof(GRAPH));
 ac0=(char*) malloc(n+2*act_n+100);
 str=(char*) malloc(2*n+4*act_n+1);
 nodestr(n,matr,act_n,act,&p,str);
 pr1 = 1.;
 transfor(n,matr,act_n,act,&num,ac);
 i=0;
 for (k=num;k<num+p-1;k++)
    {
     ac[k].ess=0;
     ac[k].v1=str[i];
     ac[k].v2=str[i+1];
     i++;
    }
 if (flag_mat) numa=num+p-1; else numa=num;
 for (k=0;k<numa;k++) ac0[k]=ac[k].v1;
 *flag=0;
 for (i=0;i<p;i++)
    {
     s1=str[i];
     for (j=i+1;j<p;j++)
        {
         s2=str[j];
   //     if (s1 != '0' && s2 != '0') continue; 
	 for (k=0;k<act_n;k++)
	    {
	     if (act[k].kol != 999) continue;
	     o1=act[k].v3;
	     o2=act[k].v4;
	     if (s1==o1 && s2==o2 || s1==o2 && s2==o1) goto cnt;
	    }
         k=0;
         bond2(numa,ac,s1,s1,s2,&k);
         if (   k==num || k==1
             || !indep2(num,ac,ac0,s1,s2,&k)) goto rep;
         nrel=num-k; drel=num; rel=nrel/drel;
         if (k==num-1) goto rep;
         if (rel <= 0.5-range || rel >= 0.5+range) goto rep;
         for (i3=0;i3<n;i3++)
            {
	     if (   ac0[i3]==s1 && ac[i3].v2==s2
	         || ac0[i3]==s2 && ac[i3].v2==s1)
	       {
	        mn=0; mk=0;
	        for (j3=0;j3<n;j3++)
	           {
	            if (j3==i3) continue;
                    mn=mn+matr[j3].kol;
	            if (ac[j3].v1=='Ô') mk=mk+matr[j3].kol;
	           }
	        h=0;
	        for (j3=n;j3<num;j3++)
	           {
	            if (ac[j3].ess < 0) continue;
	            mn=mn+abs(act[ac[j3].ess-1].kol);
	            if (ac[j3].v1=='Ô')
		      {h++; mk=mk+abs(act[ac[j3].ess-1].kol);}
	           }
	        if (k-h<n+act_n-k+h || k-h==n+act_n-k+h && mk<mn-mk)
	          {if (ac[i3].v1 != 'Ô') {ac[i3].v1='Ô'; k++;}}
                else
	          if (ac[i3].v1 == 'Ô') {ac[i3].v1=ac0[i3]; k--;}
	       }
            }
         if (k==1) goto rep;
         fl11=0;fl12=0;fl21=0;fl22=0;
         for (m=0;m<num;m++)
            {
	     if (ac[m].v1 =='Ô')
	       {
	        if (ac0[m]==s1 || ac[m].v2==s1) fl11=1;
	        if (ac0[m]==s2 || ac[m].v2==s2) fl12=1;
	       }
	     else
	       {
	        if (ac0[m]==s1 || ac[m].v2==s1) fl21=1;
	        if (ac0[m]==s2 || ac[m].v2==s2) fl22=1;
	       }
             if (fl11 && fl12 && fl21 && fl22) goto net;
            }
         goto rep;
     net:
         if (pr1 > fabs(rel-0.5))
	   {
	    *flag=1;
            for (l=0;l<numa;l++) ace[l]=ac[l];
	    s1o=s1; s2o=s2;
            pr1=fabs(rel-0.5);
           }
    rep:
	 for (l=0;l<numa;l++) ac[l].v1=ac0[l];
    cnt: ;
        }
     }
 free(str); free(ac);
 if (*flag)
   {
    s1=s1o; s2=s2o;
    printf(" . ");
    form2(num,ace,ac0,s1,s2,n,matr,act_n,act);
   }
 free(ace); free(ac0);
}
/***************************************************************/
void form(int num,GRAPH *ac,char *ac0,int next,char *ext,
	  int n,PASSIVE *matr,int act_n,SOURCE *act)
{
 int i,j,jr,k,l,m=0,n1,act1_n,p,key,
     u=1,str,col,one1,one2,ii,jj,sign,fl;
 unsigned long dl,dl1,dl2;
 char *t,t0[2];
 PASSIVE *a1;
 SOURCE *act1;

 t0[0]='\0'; t=t0;
 strcat(t++,"1");
 dl2=leng+strlen(c);
 for (i=1;i<next;i++) u*=2;
 for (i=0;i<u;i++)
    {
     k=i;
     one1=0;
     while(k)
	{
	 if (k & 1) one1++;
	 k=k>>1;
	}
     for (j=0;j<u;j++)
	{
	 sign=0;
	 k=j;
	 one2=0;
	 while(k)
	    {
	     if (k & 1) one2++;
	     k=k>>1;
	    }
	 if (one1!=one2) continue;
    even:
	 m++;
	 if (m%2)
	   {
	    dl=leng+strlen(c); key=2;
	    if (dl != dl2) strcat(b++,"+");
	   }
	 else key=1;
	 if (!(m%2))
	   {
	    strcat(b++,"*");
	    ii=u-1-i; jj=u-1-j;
	   }
	 fortest(num,ac,&n1,&act1_n,key);
	 if (n1)
	   {
	    a1=( PASSIVE *) malloc (n1*sizeof(PASSIVE));
	   }
	 if (m%2) {act1_n+=one1; fl=0;}
	 else {act1_n+=next-1-one1; fl=1;}
	 if (act1_n)
	   {
	    act1=( SOURCE *) malloc (act1_n*sizeof(SOURCE));
	   }
         if (m%2) for (jr=0;jr<n;jr++)
                     if (matr[jr].kol < 0) matr[jr].kol=-matr[jr].kol;
	 fortrans(num,ac,n,matr,act,&n1,a1,&act1_n,act1,key,ac0);
	 str=-1; col=-1;
	 if (!(m%2)) {k=ii; l=jj;}
 	 else {k=i; l=j;}
scan:
	 while(k)
	    {
	     str++;
	     if (k & 1) {k=k>>1; goto tw;}
	     k=k>>1;
	    }
      tw:
	 while(l)
	    {
	     col++;
	     if (l & 1) {l=l>>1; goto suc;}
	     l=l>>1;
	    }
	 if (!k && !l) goto con;
     suc:
	 if (m%2) sign+=str+col;
         nuiall(ext[next-1],ext[str],ext[next-1],ext[col],&act1_n,act1);
         act1_n--;
	 if (fl && !(m%2) && sign%2)
	   {
	    fl=0;
	    act1[act1_n].v3=ext[col];
	    act1[act1_n].v4=ext[next-1];
	   }
	 act1_n++;
	 goto scan;
     con:
	 node_n(n1,a1,act1_n,act1,&p);
	 if (n1 || act1_n)
	   {
	    if (n1+act1_n > 1) strcat(b++,"(");
	    dl1=leng+strlen(c);
 	    gggf(n1,a1,act1_n,act1);
	    if (n1) free(a1); if (act1_n) free(act1);
	    if (dl1>=leng+strlen(c))
	      {
	       ster(dl); if (m%2) m++; continue;
	      }
	    else if (n1+act1_n > 1) strcat(b++,")");
	   }
	 else
	   {
	    ster(dl); if (m%2) m++; continue;
	   }
         if (   *(b-4) == '*' && *(b-3) == '('
             && *(b-2) == '1' && *(b-1) == ')')
           {
            b-=4; b[0]='\0';
           }
         if (   *(b-2) == '*' && *(b-1) == '1')
           {
            b-=2; b[0]='\0';
           }
	 if (m%2) goto even;
	}
    }
 for (j=0;j<n;j++) free(matr[j].reb);
 for (j=0;j<act_n;j++) free(act[j].reb);
}
/*****************************************************************/
void bond3(int n,GRAPH *a1,char s1,char s2,char s3,char s4,int *k)
{
 int i;
 char s5;

 for(i=0;i<n;i++)
  {
   if (a1[i].v1==s1)
     {
      if (a1[i].v2==s2 || a1[i].v2==s3 || a1[i].v2==s4)
	{
	 a1[i].v1='Ô'; (*k)++;
	 continue;
	}
      s5=a1[i].v2;
      a1[i].v1='Ô'; (*k)++;
      bond3(n,a1,s5,s2,s3,s4,k);
      if(s1==s2) break;
     }
   if (a1[i].v1 != 'Ô' && a1[i].v2==s1)
     {
      if (a1[i].v1==s2 || a1[i].v1==s3 || a1[i].v1==s4)
	{
	 a1[i].v1='Ô'; (*k)++;
	 continue;
	}
      s5=a1[i].v1;
      a1[i].v1='Ô'; (*k)++;
      bond3(n,a1,s5,s2,s3,s4,k);
      if (s1==s2) break;
     }
  }
}
/*********************************************************/
int indep3(int nc,GRAPH *ac,
	   char *ac0,char s1,char s2,char s3,int *k)
{
 int i,j;

 for (i=0;i<nc;i++)
    {
     if (!ac[i].ess || ac[i].v1 == 'Ô') continue;
     for (j=0;j<nc;j++)
	{
	 if (!ac[j].ess || ac[j].v1 == 'Ô') continue;
	 if (ac[i].ess==-ac[j].ess) goto eq1;
	}
     if (   ac0[i]==s1 && ac[i].v2==s2 || ac[i].v2==s1 && ac0[i]==s2
	 || ac0[i]==s1 && ac[i].v2==s3 || ac[i].v2==s1 && ac0[i]==s3
	 || ac0[i]==s2 && ac[i].v2==s3 || ac[i].v2==s2 && ac0[i]==s3)
       {
	ac[i].v1='Ô';
	(*k)++;
       }
eq1: continue;
    }
 for (i=0;i<nc;i++)
    {
     if (!ac[i].ess || ac[i].v1 != 'Ô') continue;
     for (j=0;j<nc;j++)
	{
	 if (!ac[j].ess || ac[j].v1 != 'Ô') continue;
	 if (ac[i].ess==-ac[j].ess) goto eq2;
	}
     if (   ac0[i]==s1 && ac[i].v2==s2 || ac[i].v2==s1 && ac0[i]==s2
	 || ac0[i]==s1 && ac[i].v2==s3 || ac[i].v2==s1 && ac0[i]==s3
	 || ac0[i]==s2 && ac[i].v2==s3 || ac[i].v2==s2 && ac0[i]==s3)
       {
	ac[i].v1=ac0[i];
	(*k)--;
       }
     else return(0);
eq2: continue;
    }
 return(1);
}
/************************************************************/
void bisec3(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range)
{
 int i,j,h=0,k,k1,k2,k3,p,num,numa,next,r1,r2,r3,
     fl11,fl12,fl13,fl21,fl22,fl23;
 char s1,s2,s3,s1o,s2o,*str,*ac0,ext[3],o1,o2;
 float pr1=1.,rel,nrel,drel;
 GRAPH *ac=( GRAPH *) malloc ((n+2*act_n+100)*sizeof(GRAPH));
 GRAPH *ace=( GRAPH *) malloc ((n+2*act_n+100)*sizeof(GRAPH));

 ac0=(char*) malloc(n+2*act_n+100);
 str=(char*) malloc(2*n+4*act_n+1);
 *flag=0;
 nodestr(n,matr,act_n,act,&p,str);
 transfor(n,matr,act_n,act,&num,ac);
 for (i=0;i<p;i++)
    {
  //   if (str[i] != '0') continue;
     k=0;
     for (j=0;j<num;j++)
	{
	 if (ac[j].v1 == str[i] || ac[j].v2 == str[i]) k++;
	}
     if (k > h) {h=k; s3=str[i];}
    }
 for (i=0;i<p;i++)
    {
     if (str[i]==s3) {for (j=i;j<p-1;j++) str[j]=str[j+1]; break;}
    }
i=0;
 for (k=num;k<num+p-1;k++)
    {
     ac[k].ess=0;
     ac[k].v1=str[i];
     ac[k].v2=str[i+1];
     i++;
    }
 if (flag_mat) numa=num+p-1; else numa=num;
 for (k=0;k<numa;k++) ac0[k]=ac[k].v1;
 for (i=0;i<p-1;i++)
    {
     s1=str[i];
     for (j=i+1;j<p-1;j++)
	{
           s2=str[j];
	     for (k=0;k<act_n;k++)
		{
		 if (act[k].kol != 999) continue;
		 o1=act[k].v3; o2=act[k].v4;
		 if (   s1==o1 && s2==o2 || s1==o2 && s2==o1
		     || s1==o1 && s3==o2 || s1==o2 && s3==o1
		     || s2==o1 && s3==o2 || s2==o2 && s3==o1) goto cnt;
		}
	 k1=0;
	 bond3(numa,ac,s1,s1,s2,s3,&k1);
         for (h=0;h<numa;h++) ac[h].v1=ac0[h];
         k2=0;
         bond3(numa,ac,s2,s2,s1,s3,&k2);
         for (h=0;h<numa;h++) ac[h].v1=ac0[h];
         k3=0;
         bond3(numa,ac,s3,s3,s1,s2,&k3);
         r1=abs(num-2*k1);
         r2=abs(num-2*k2);
         r3=abs(num-2*k3);
         k=0;
         for (h=0;h<numa;h++) ac[h].v1=ac0[h];
         if (r1<=r2 && r1<=r3) bond3(numa,ac,s1,s1,s2,s3,&k);
         else
           if (r2<=r1 && r2<=r3) bond3(numa,ac,s2,s2,s1,s3,&k);
           else
             if (r3<=r1 && r3<=r2) bond3(numa,ac,s3,s3,s1,s2,&k);
         if (!indep3(num,ac,ac0,s1,s2,s3,&k)) goto rep;
         nrel=num-k; drel=num; rel=nrel/drel;
         if (rel < 0.5-range || rel > 0.5+range) goto rep;
         fl11=0;fl12=0;fl13=0;fl21=0;fl22=0;fl23=0;
         for (h=0;h<num;h++)
            {
	     if (ac[h].v1 =='Ô')
	       {
	        if (ac0[h]==s1 || ac[h].v2==s1) fl11=1;
	        if (ac0[h]==s2 || ac[h].v2==s2) fl12=1;
                if (ac0[h]==s3 || ac[h].v2==s3) fl13=1;
	       }
	     else
	       {
	        if (ac0[h]==s1 || ac[h].v2==s1) fl21=1;
	        if (ac0[h]==s2 || ac[h].v2==s2) fl22=1;
	        if (ac0[h]==s3 || ac[h].v2==s3) fl23=1;
	       }
             if (   fl11 && fl12 && fl13
                 && fl21 && fl22 && fl23) goto net;
            }
         goto rep;
     net:
         if (pr1 > fabs(rel-0.5))
           {
            s1o=s1; s2o=s2;
	    pr1=fabs(rel-0.5);
            for (h=0;h<numa;h++) ace[h]=ac[h];
            *flag=1;
           }
     rep:
	 for (h=0;h<numa;h++) ac[h].v1=ac0[h];
     cnt: ;
	}
    }
 free(str); free(ac);
 if (*flag)
   {
    ext[0]=s1o; ext[1]=s2o; ext[2]=s3; next=3;
    printf(" ' ");
    form(num,ace,ac0,next,ext,n,matr,act_n,act);
   }
 free(ace); free(ac0);
}
/*****************************************************************/
void bond4(int n,GRAPH *a1,
	   char s1,char s2,char s3,char s4,char s5,int *k)
{
 int i;
 char s6;

 for(i=0;i<n;i++)
  {
   if (a1[i].v1==s1)
     {
      if (a1[i].v2==s2 || a1[i].v2==s3 || a1[i].v2==s4 || a1[i].v2==s5)
	{
	 a1[i].v1='Ô'; (*k)++;
	 continue;
	}
      s6=a1[i].v2;
      a1[i].v1='Ô'; (*k)++;
      bond4(n,a1,s6,s2,s3,s4,s5,&*k);
      if(s1==s2) break;
     }
   if (a1[i].v1 != 'Ô' && a1[i].v2==s1)
     {
      if (a1[i].v1==s2 || a1[i].v1==s3 || a1[i].v1==s4 || a1[i].v1==s5)
	{
	 a1[i].v1='Ô'; (*k)++;
	 continue;
	}
      s6=a1[i].v1;
      a1[i].v1='Ô'; (*k)++;
      bond4(n,a1,s6,s2,s3,s4,s5,&*k);
      if (s1==s2) break;
     }
  }
}
/*********************************************************/
int indep4(int nc,GRAPH *ac,
	   char *ac0,char s1,char s2,char s3,char s4,int *k)
{
 int i,j;

 for (i=0;i<nc;i++)
    {
     if (!ac[i].ess || ac[i].v1 == 'Ô') continue;
     for (j=0;j<nc;j++)
	{
	 if (!ac[j].ess || ac[j].v1 == 'Ô') continue;
	 if (ac[i].ess==-ac[j].ess) goto eq1;
	}
     if (   ac0[i]==s1 && ac[i].v2==s2 || ac[i].v2==s1 && ac0[i]==s2
	 || ac0[i]==s1 && ac[i].v2==s3 || ac[i].v2==s1 && ac0[i]==s3
	 || ac0[i]==s1 && ac[i].v2==s4 || ac[i].v2==s1 && ac0[i]==s4
	 || ac0[i]==s2 && ac[i].v2==s3 || ac[i].v2==s2 && ac0[i]==s3
	 || ac0[i]==s2 && ac[i].v2==s4 || ac[i].v2==s2 && ac0[i]==s4
	 || ac0[i]==s3 && ac[i].v2==s4 || ac[i].v2==s3 && ac0[i]==s4)
       {
	ac[i].v1='Ô';
	(*k)++;
       }
eq1: continue;
    }
 for (i=0;i<nc;i++)
    {
     if (!ac[i].ess || ac[i].v1 != 'Ô') continue;
     for (j=0;j<nc;j++)
	{
	 if (!ac[j].ess || ac[j].v1 != 'Ô') continue;
	 if (ac[i].ess==-ac[j].ess) goto eq2;
	}
     if (   ac0[i]==s1 && ac[i].v2==s2 || ac[i].v2==s1 && ac0[i]==s2
	 || ac0[i]==s1 && ac[i].v2==s3 || ac[i].v2==s1 && ac0[i]==s3
	 || ac0[i]==s1 && ac[i].v2==s4 || ac[i].v2==s1 && ac0[i]==s4
	 || ac0[i]==s2 && ac[i].v2==s3 || ac[i].v2==s2 && ac0[i]==s3
	 || ac0[i]==s2 && ac[i].v2==s4 || ac[i].v2==s2 && ac0[i]==s4
	 || ac0[i]==s3 && ac[i].v2==s4 || ac[i].v2==s3 && ac0[i]==s4)
       {
	ac[i].v1=ac0[i];
	(*k)--;
       }
     else return(0);
eq2: continue;
    }
 return(1);
}
/**************************************************************************/
void bisec4(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range)
{

 int i,j,l,h=0,k,k1,k2,k3,k4,p,num,numa,next,r1,r2,r3,r4;
 int fl11,fl12,fl13,fl14,fl21,fl22,fl23,fl24;
 char s1,s2,s3,s4,s1o,s2o,s3o,*str,*ac0,ext[4],o1,o2;
 float pr1=1.,rel,nrel,drel;
 GRAPH *ac=( GRAPH *) malloc ((n+2*act_n+100)*sizeof(GRAPH));
 GRAPH *ace=( GRAPH *) malloc ((n+2*act_n+100)*sizeof(GRAPH));

 ac0=(char*) malloc(n+2*act_n+100);
 str=(char*) malloc(2*n+4*act_n+1);
 *flag=0;
 nodestr(n,matr,act_n,act,&p,str);
 transfor(n,matr,act_n,act,&num,ac);
 for (i=0;i<p;i++)
    {
  //   if (str[i] != '0') continue;
     k=0;
     for (j=0;j<num;j++)
	{
	 if (ac[j].v1 == str[i] || ac[j].v2 == str[i]) k++;
	}
     if (k > h) {h=k; s4=str[i];}
    }
 for (i=0;i<p;i++)
    {
     if (str[i]==s4) {for (j=i;j<p-1;j++) str[j]=str[j+1]; break;}
    }
 i=0;
 for (k=num;k<num+p-1;k++)
    {
     ac[k].ess=0;
     ac[k].v1=str[i];
     ac[k].v2=str[i+1];
     i++;
    }
  if (flag_mat) numa=num+p-1; else numa=num;
  for (k=0;k<numa;k++) ac0[k]=ac[k].v1;
  for (l=0;l<p-1;l++)
    {
     s1=str[l];
     for (i=l+1;i<p-1;i++)
	{
         s2=str[i];
	 for (j=i+1;j<p-1;j++)
	    {
             s3=str[j];
	     for (k=0;k<act_n;k++)
		{
		 if (act[k].kol != 999) continue;
		 o1=act[k].v3;
		 o2=act[k].v4;
		 if (   s1==o1 && s2==o2 || s1==o2 && s2==o1
		     || s1==o1 && s3==o2 || s1==o2 && s3==o1
		     || s1==o1 && s4==o2 || s1==o2 && s4==o1
		     || s2==o1 && s3==o2 || s2==o2 && s3==o1
		     || s2==o1 && s4==o2 || s2==o2 && s4==o1
		     || s3==o1 && s4==o2 || s3==o2 && s4==o1) goto cnt;
		}
	     k1=0;
	     bond4(numa,ac,s1,s1,s2,s3,s4,&k1);
             for (h=0;h<numa;h++) ac[h].v1=ac0[h];
             k2=0;
             bond4(numa,ac,s2,s2,s1,s3,s4,&k2);
             for (h=0;h<numa;h++) ac[h].v1=ac0[h];
             k3=0;
             bond4(numa,ac,s3,s3,s1,s2,s4,&k3);
             k4=0;
             bond4(numa,ac,s4,s4,s1,s2,s3,&k4);
             r1=abs(num-2*k1);
             r2=abs(num-2*k2);
             r3=abs(num-2*k3);
             r4=abs(num-2*k4);
             k=0;
             for (h=0;h<numa;h++) ac[h].v1=ac0[h];
             if (r1<=r2 && r1<=r3 && r1<=r4) bond4(numa,ac,s1,s1,s2,s3,s4,&k);
             else
             if (r2<=r1 && r2<=r3 && r2<=r4) bond4(numa,ac,s2,s2,s1,s3,s4,&k);
             else
             if (r3<=r1 && r3<=r2 && r3<=r4) bond4(numa,ac,s3,s3,s1,s2,s4,&k);
             else
             if (r4<=r1 && r4<=r2 && r4<=r3) bond4(numa,ac,s4,s4,s1,s2,s3,&k);
             if (!indep4(num,ac,ac0,s1,s2,s3,s4,&k)) goto rep;
             nrel=num-k; drel=num; rel=nrel/drel;
             if (rel > 0.5-range && rel < 0.5+range) 
{
             fl11=0;fl12=0;fl13=0;fl14=0;fl21=0;fl22=0;fl23=0;fl24=0;
             for (h=0;h<num;h++)
                {
	         if (ac[h].v1 =='Ô')
		   {
	            if (ac0[h]==s1 || ac[h].v2==s1) fl11=1;
	            if (ac0[h]==s2 || ac[h].v2==s2) fl12=1;
	            if (ac0[h]==s3 || ac[h].v2==s3) fl13=1;
	            if (ac0[h]==s4 || ac[h].v2==s4) fl14=1;
	           }
	         else
	           {
	            if (ac0[h]==s1 || ac[h].v2==s1) fl21=1;
	            if (ac0[h]==s2 || ac[h].v2==s2) fl22=1;
	            if (ac0[h]==s3 || ac[h].v2==s3) fl23=1;
	            if (ac0[h]==s4 || ac[h].v2==s4) fl24=1;
	           }
                 if (   fl11 && fl12 && fl13 && fl14
                     && fl21 && fl22 && fl23 && fl24) goto net;
                }
}
             goto rep;
         net:
             if (pr1 > fabs(rel-0.5))
   
            {
                s1o=s1; s2o=s2; s3o=s3;
		pr1=fabs(rel-0.5);
                for (h=0;h<numa;h++) ace[h]=ac[h];
                *flag=1;
               }
	 rep:
	     for (h=0;h<numa;h++) ac[h].v1=ac0[h];
         cnt: ;
	    }
	}
    }
 free(str);  free(ac);
 if (*flag)
   {
    printf(" : ");
    ext[0]=s1o; ext[1]=s2o; ext[2]=s3o; ext[3]=s4; next=4;
    form(num,ace,ac0,next,ext,n,matr,act_n,act);
   }
 free(ace); free(ac0);
}
/*****************************************************************/
void bond5(int n,GRAPH *a1,
	   char s1,char s2,char s3,char s4,char s5,char s6,int *k)
{
 int i;
 char s7;

 for(i=0;i<n;i++)
  {
   if (a1[i].v1==s1)
     {
      if (   a1[i].v2==s2 || a1[i].v2==s3
	  || a1[i].v2==s4 || a1[i].v2==s5 || a1[i].v2==s6)
	{
	 a1[i].v1='Ô'; (*k)++;
	 continue;
	}
      s7=a1[i].v2;
      a1[i].v1='Ô'; (*k)++;
      bond5(n,a1,s7,s2,s3,s4,s5,s6,k);
      if (s1==s2) break;
     }
   if (a1[i].v1 != 'Ô' && a1[i].v2==s1)
     {
      if (   a1[i].v1==s2 || a1[i].v1==s3
	  || a1[i].v1==s4 || a1[i].v1==s5 || a1[i].v1==s6)
	{
	 a1[i].v1='Ô'; (*k)++;
	 continue;
	}
      s7=a1[i].v1;
      a1[i].v1='Ô'; (*k)++;
      bond5(n,a1,s7,s2,s3,s4,s5,s6,k);
      if (s1==s2)  break;
     }
  }
}
/*********************************************************/
int indep5(int nc,GRAPH *ac,
	    char *ac0,char s1,char s2,char s3,char s4,char s5,int *k)
{
 int i,j;

 for (i=0;i<nc;i++)
    {
     if (!ac[i].ess || ac[i].v1 == 'Ô') continue;
     for (j=0;j<nc;j++)
	{
	 if (!ac[j].ess || ac[j].v1 == 'Ô') continue;
	 if (ac[i].ess==-ac[j].ess) goto eq1;
	}
     if (   ac0[i]==s1 && ac[i].v2==s2 || ac[i].v2==s1 && ac0[i]==s2
	 || ac0[i]==s1 && ac[i].v2==s3 || ac[i].v2==s1 && ac0[i]==s3
	 || ac0[i]==s1 && ac[i].v2==s4 || ac[i].v2==s1 && ac0[i]==s4
	 || ac0[i]==s1 && ac[i].v2==s5 || ac[i].v2==s1 && ac0[i]==s5
	 || ac0[i]==s2 && ac[i].v2==s3 || ac[i].v2==s2 && ac0[i]==s3
	 || ac0[i]==s2 && ac[i].v2==s4 || ac[i].v2==s2 && ac0[i]==s4
	 || ac0[i]==s2 && ac[i].v2==s5 || ac[i].v2==s2 && ac0[i]==s5
	 || ac0[i]==s3 && ac[i].v2==s4 || ac[i].v2==s3 && ac0[i]==s4
	 || ac0[i]==s3 && ac[i].v2==s5 || ac[i].v2==s3 && ac0[i]==s5
	 || ac0[i]==s4 && ac[i].v2==s5 || ac[i].v2==s4 && ac0[i]==s5)
       {
	ac[i].v1='Ô';
	(*k)++;
       }
eq1: continue;
    }
 for (i=0;i<nc;i++)
    {
     if (!ac[i].ess || ac[i].v1 != 'Ô') continue;
     for (j=0;j<nc;j++)
	{
	 if (!ac[j].ess || ac[j].v1 != 'Ô') continue;
	 if (ac[i].ess==-ac[j].ess) goto eq2;
	}
     if (   ac0[i]==s1 && ac[i].v2==s2 || ac[i].v2==s1 && ac0[i]==s2
	 || ac0[i]==s1 && ac[i].v2==s3 || ac[i].v2==s1 && ac0[i]==s3
	 || ac0[i]==s1 && ac[i].v2==s4 || ac[i].v2==s1 && ac0[i]==s4
	 || ac0[i]==s1 && ac[i].v2==s5 || ac[i].v2==s1 && ac0[i]==s5
	 || ac0[i]==s2 && ac[i].v2==s3 || ac[i].v2==s2 && ac0[i]==s3
	 || ac0[i]==s2 && ac[i].v2==s4 || ac[i].v2==s2 && ac0[i]==s4
	 || ac0[i]==s2 && ac[i].v2==s5 || ac[i].v2==s2 && ac0[i]==s5
	 || ac0[i]==s3 && ac[i].v2==s4 || ac[i].v2==s3 && ac0[i]==s4
	 || ac0[i]==s3 && ac[i].v2==s5 || ac[i].v2==s3 && ac0[i]==s5
	 || ac0[i]==s4 && ac[i].v2==s5 || ac[i].v2==s4 && ac0[i]==s5)
       {
	ac[i].v1=ac0[i];
	(*k)--;
       }
     else return(0);
eq2: continue;
    }
 return(1);
}
/***********************************************************************/
void bisec5(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range)
{
 int i,j,l,h=0,k,k1,k2,k3,k4,k5,m,p,num,numa,next,r1,r2,r3,r4,r5;
 int fl11,fl12,fl13,fl14,fl15,fl21,fl22,fl23,fl24,fl25;
 char c,s1,s2,s3,s4,s5,s1o,s2o,s3o,s4o,*str,*ac0,ext[5],o1,o2;
 float pr1=1.,rel,nrel,drel;
 GRAPH *ac=( GRAPH *) malloc ((n+2*act_n+100)*sizeof(GRAPH));
 GRAPH *ace=( GRAPH *) malloc ((n+2*act_n+100)*sizeof(GRAPH));

 ac0=(char*) malloc(n+2*act_n+100);
 str=(char*) malloc(2*n+4*act_n+1);
 nodestr(n,matr,act_n,act,&p,str);
 transfor(n,matr,act_n,act,&num,ac);
 *flag=0;
 for (i=0;i<p;i++)
    {
     if (str[i] != '0') {
     k=0;
     for (j=0;j<num;j++)
	{
	 if (ac[j].v1 == str[i] || ac[j].v2 == str[i]) k++;
	}
     if (k > h) {h=k; s5=str[i];}  } else {s5=str[i]; break;}
    }
 for (i=0;i<p;i++)
    {
     if (str[i]==s5) {for (j=i;j<p-1;j++) str[j]=str[j+1]; break;}
    }
 i=0;
 for (k=num;k<num+p-2;k++)
    {
     ac[k].ess=0;
     ac[k].v1=str[i];
     ac[k].v2=str[i+1];
     i++;
    }
 if (flag_mat) numa=num+p-1; else numa=num;
 for (k=0;k<numa;k++) ac0[k]=ac[k].v1;
 for (l=0;l<p-1;l++)
    {
     s1=str[l];
     for (i=l+1;i<p-1;i++)
	{
	 s2=str[i];
	 for (j=i+1;j<p-1;j++)
	    {
	     s3=str[j];
	 for (m=j+1;m<p-1;m++)
	    {
	     s4=str[m];
	     for (k=0;k<act_n;k++)
		{
		 if (act[k].kol != 999) continue;
		 o1=act[k].v3;
		 o2=act[k].v4;
		 if (   s1==o1 && s2==o2 || s1==o2 && s2==o1
		     || s1==o1 && s3==o2 || s1==o2 && s3==o1
		     || s1==o1 && s4==o2 || s1==o2 && s4==o1
		     || s1==o1 && s5==o2 || s1==o2 && s5==o1
		     || s2==o1 && s3==o2 || s2==o2 && s3==o1
		     || s2==o1 && s4==o2 || s2==o2 && s4==o1
		     || s2==o1 && s5==o2 || s2==o2 && s5==o1
		     || s3==o1 && s4==o2 || s3==o2 && s4==o1
		     || s3==o1 && s5==o2 || s3==o2 && s5==o1
		     || s4==o1 && s5==o2 || s4==o2 && s5==o1) goto cnt;
		}
	     k1=0;
	     bond5(numa,ac,s1,s1,s2,s3,s4,s5,&k1);
             for (h=0;h<numa;h++) ac[h].v1=ac0[h];
             k2=0;
             bond5(numa,ac,s2,s2,s1,s3,s4,s5,&k2);
             for (h=0;h<num;h++) ac[h].v1=ac0[h];
             k3=0;
             bond5(numa,ac,s3,s3,s1,s2,s4,s5,&k3);
             for (h=0;h<numa;h++) ac[h].v1=ac0[h];
             k4=0;
             bond5(numa,ac,s4,s4,s1,s2,s3,s5,&k4);
             for (h=0;h<numa;h++) ac[h].v1=ac0[h];
             k5=0;
             bond5(numa,ac,s5,s5,s1,s2,s3,s4,&k5);
             r1=abs(num-2*k1);
             r2=abs(num-2*k2);
             r3=abs(num-2*k3);
             r4=abs(num-2*k4);
             r5=abs(num-2*k5);
             k=0;
             for (h=0;h<numa;h++) ac[h].v1=ac0[h];
           if (r1<=r2 && r1<=r3 && r1<=r4 && r1<=r5)
               bond5(numa,ac,s1,s1,s2,s3,s4,s5,&k);
             else
             if (r2<=r1 && r2<=r3 && r2<=r4 && r2<=r5)
               bond5(numa,ac,s2,s2,s1,s3,s4,s5,&k);
             else
             if (r3<=r1 && r3<=r2 && r3<=r4 && r3<=r5)
               bond5(numa,ac,s3,s3,s1,s2,s4,s5,&k);
             else
             if (r4<=r1 && r4<=r2 && r4<=r3 && r4<=r5)
               bond5(numa,ac,s4,s4,s1,s2,s3,s5,&k);
             else
             if (r5<=r1 && r5<=r2 && r5<=r3 && r5<=r4) 
               bond5(numa,ac,s5,s5,s1,s2,s3,s4,&k);
             if (!indep5(num,ac,ac0,s1,s2,s3,s4,s5,&k)) goto rep;
	     nrel=num-k; drel=num; rel=nrel/drel;
	     if (rel < 0.5-range || rel > 0.5+range) goto rep;
             fl11=0;fl12=0;fl13=0;fl14=0;fl15=0;
             fl21=0;fl22=0;fl23=0;fl24=0;fl25=0;
             for (h=0;h<num;h++)
                {
	         if (ac[h].v1 =='Ô')
	           {
	            if (ac0[h]==s1 || ac[h].v2==s1) fl11=1;
	            if (ac0[h]==s2 || ac[h].v2==s2) fl12=1;
	            if (ac0[h]==s3 || ac[h].v2==s3) fl13=1;
	            if (ac0[h]==s4 || ac[h].v2==s4) fl14=1;
	            if (ac0[h]==s5 || ac[h].v2==s5) fl15=1;
	           }
	         else
	           {
	            if (ac0[h]==s1 || ac[h].v2==s1) fl21=1;
	            if (ac0[h]==s2 || ac[h].v2==s2) fl22=1;
	            if (ac0[h]==s3 || ac[h].v2==s3) fl23=1;
	            if (ac0[h]==s4 || ac[h].v2==s4) fl24=1;
	            if (ac0[h]==s5 || ac[h].v2==s5) fl25=1;
	           }
                 if (   fl11 && fl12 && fl13 && fl14 && fl15
                     && fl21 && fl22 && fl23 && fl24 && fl25) goto net;
                }
             goto rep;
         net:
             if (pr1 > fabs(rel-0.5))
               {
                s1o=s1; s2o=s2; s3o=s3; s4o=s4;
		pr1=fabs(rel-0.5);
                for (h=0;h<numa;h++) ace[h]=ac[h];
                *flag=1; goto fl;
               }
	 rep:
	     for (h=0;h<numa;h++) ac[h].v1=ac0[h];
         cnt: ;
	    }
	    }
        }
    }
fl:
 free(str); free(ac);
 if (*flag)
   {
    printf(" ^ ");
    ext[0]=s1o; ext[1]=s2o; ext[2]=s3o; ext[3]=s4o; ext[4]='0';//s5; 
    next=5;
    form(num,ace,ac0,next,ext,n,matr,act_n,act);
   }
 free(ace); free(ac0);
}
