/* Функции и подпрограммы для проверки вырождения и упрощения схемы */

#include "cirsym.h"

extern FILE *out,*inpa,*set;

extern char *b,*c,sp;
extern unsigned long leng,lengl,lepr;
extern int flag_fi3,flag_fi4,flag_fi5,flag_fsn,extract,
       flag_e,cirf,noequ,flag_pln,nuz,flag_sp,flag_cL,flag_nul,st_pln;
extern float range2,range3,range4,range5;

/*      Вывод подформулы СВЕРТЫВАНИЕ ПОСЛЕДОВАТЕЛЬНЫХ ПРОВОДИМОСТЕЙ      */
void red2yy(int *n,PASSIVE *a1,int act_n,SOURCE *act)
{
 int i,j,k,p,send,rec,red[2];
 char *t,*t0,*y,*y0,*str,ext[2];
 GRAPH *a2=( GRAPH *) malloc((*n)*sizeof(GRAPH));

 str = (char*) malloc(2*(*n)+8*act_n);
beg:
 copypas(*n,a1,a2);
 verstr(*n,a2,&p,str);
 for (i=0;i<p;i++)
    {
     for (j=0,k=0;j<*n;j++)
	if (a1[j].v1==str[i] || a1[j].v2==str[i])
	  {
	   if (   a1[j].reb[0]=='r' || a1[j].reb[0]=='R' || a1[j].reb[0]=='z'
	       || a1[j].reb[0]=='Z' || a1[j].reb[0]=='L'
	       || a1[j].reb[0]=='c' && flag_pln) goto con;
	   else k++;
	  }
     if (k==2)
       {
	send=0; rec=0;
	for (j=0;j<act_n;j++)
	   {
	    if (str[i]==act[j].v1 || str[i]==act[j].v2) send=1;
	    if (str[i]==act[j].v3 || str[i]==act[j].v4) rec=1;
	   }
	if (send || rec) continue;
	for (j=0,k=0;j<*n;j++)
	   if (a1[j].v1==str[i] || a1[j].v2==str[i])
	     {
	      if (a1[j].v1 != str[i]) ext[k]=a1[j].v1;
	      if (a1[j].v2 != str[i]) ext[k]=a1[j].v2;
	      red[k]=j;
	      k++;
	     }
	t0=(char*) malloc(strlen(a1[red[0]].reb)+strlen(a1[red[1]].reb)+30);
	t0[0]='\0'; t=t0;
	y0=(char*) malloc(10);
	y0[0]='\0'; y=y0;
	strcat(y,"Ym"); y+=2;
	if (nuz <=8) sprintf(y++,"%d",++nuz);
	else
	  if (nuz > 8) sprintf(y,"%d",++nuz);
	strcat(t,y0); t+=strlen(y0);
	strcat(t++,"=");
	strcat(t,a1[red[0]].reb); t+=strlen(a1[red[0]].reb);
	strcat(t++,"*");
	strcat(t,a1[red[1]].reb); t+=strlen(a1[red[1]].reb);
	strcat(t,"/("); t+=2;
	strcat(t,a1[red[0]].reb); t+=strlen(a1[red[0]].reb);
	strcat(t++,"+");
	strcat(t,a1[red[1]].reb); t+=strlen(a1[red[1]].reb);
	free(a1[red[0]].reb);
	strcat(t,");");
	strcat(b,t0); b+=strlen(t0);
	a1[red[0]].reb=strdup(y0);
	a1[red[0]].v1=ext[0];
	a1[red[0]].v2=ext[1];
	a1[red[0]].kol=1;
	free(t0);
	free(y0);
	k=red[1];
	frmatr(k,n,a1);
	reallf(n,a1);
	kontrol();
	goto beg;
       }
con: ;
    }
 free(a2);
 free(str);
}
/*       Вывод подформулы СВЕРТЫВАНИЕ ПАРАЛЛЕЛЬНЫХ СОПРОТИВЛЕНИЙ        */
void red2zz(int *n,PASSIVE *a1)
{
 int i,j;
 char *t,*t0,*y,*y0;

 for (j=0;j<*n;j++)
    {
     if (   a1[j].reb[0] == 'g' || a1[j].reb[0] == 'G'
	 || a1[j].reb[0]=='y' || a1[j].reb[0]=='Y'
	 || a1[j].reb[0]=='c'
	 || a1[j].reb[0]=='L' && flag_pln
	 || a1[j].reb[0]=='-') continue;
     for (i=0;i<*n;i++)
	{
	 if (i == j) continue;
	 if (   a1[i].reb[0] == 'g' || a1[i].reb[0] == 'G'
	     || a1[i].reb[0]=='y' || a1[i].reb[0]=='Y'
	     || a1[i].reb[0]=='c'
	     || a1[i].reb[0]=='L' && flag_pln
	     || a1[i].reb[0]=='-') continue;

	 if (   a1[j].v1==a1[i].v1 && a1[j].v2==a1[i].v2
	     || a1[j].v1==a1[i].v2 && a1[j].v2==a1[i].v1)
	   {
	    t0=(char*) malloc(strlen(a1[i].reb)+strlen(a1[j].reb)+30);
	    t0[0]='\0'; t=t0;
	    y0=(char*) malloc(10);
	    y0[0]='\0'; y=y0;
	    strcat(y,"Zm"); y+=2;
	    if (nuz <=8) sprintf(y++,"%d",++nuz);
	    else
	      if (nuz > 8) sprintf(y,"%d",++nuz);
	    strcat(t,y0); t+=strlen(y0);
	    strcat(t++,"=");
	    if (a1[i].kol == 1)
	      {
	       strcat(t,a1[i].reb); t+=strlen(a1[i].reb);
	      }
	    else
	      {
	       strcat(t++,"(");
	       strcat(t,a1[i].reb); t+=strlen(a1[i].reb);
	       strcat(t++,")");
	      }
	    strcat(t++,"*");
	    if (a1[j].kol == 1)
	      {
	       strcat(t,a1[j].reb); t+=strlen(a1[j].reb);
	      }
	    else
	      {
	       strcat(t++,"(");
	       strcat(t,a1[j].reb); t+=strlen(a1[j].reb);
	       strcat(t++,")");
	      }
	    strcat(t,"/("); t+=2;
	    strcat(t,a1[i].reb); t+=strlen(a1[i].reb);
	    strcat(t++,"+");
	    strcat(t,a1[j].reb); t+=strlen(a1[j].reb);
	    strcat(t,");");
	    free(a1[i].reb);
	    strcat(b,t0); b+=strlen(t0);
	    a1[i].reb=strdup(y0);
	    a1[i].kol=1;
	    free(t0);
	    free(y0);
	    frmatr(j,n,a1);
	    kontrol();
	    goto con;
	   }
	}
con: ;
    }
}
/*   Вывод подформулы ОБЪЕДИНЕНИЕ ПОСЛЕДОВАТЕЛЬНЫХ СОПРОТИВЛЕНИЙ   */
void seqzz(int *n,PASSIVE *a1,int act_n,SOURCE *act)
{
 int i,j,k,p,send,rec,red[2];
 char *t,*t0,*str,ext[2];
 GRAPH *a2=( GRAPH *) malloc((*n)*sizeof(GRAPH));

 str = (char*) malloc(2*(*n)+8*act_n);
beg:
 copypas(*n,a1,a2);
 verstr(*n,a2,&p,str);
 for (i=0;i<p;i++)
    {
     for (j=0,k=0;j<*n;j++)
	if (a1[j].v1==str[i] || a1[j].v2==str[i])
	if (a1[j].v1==str[i] || a1[j].v2==str[i])
	  {
	   if (   a1[j].reb[0]=='g' || a1[j].reb[0]=='G' || a1[j].reb[0]=='L'
	       || a1[j].reb[0]=='y' || a1[j].reb[0]=='Y' || a1[j].reb[0]=='c'
	       || a1[j].reb[0]=='L' && flag_pln
	       || a1[j].reb[0]=='-'
	       || a1[j].v1 == a1[j].v2) goto con;
	   else k++;
	  }
     if (k==2)
       {
	send=0; rec=0;
	for (j=0;j<act_n;j++)
	   {
	    if (str[i]==act[j].v1 || str[i]==act[j].v2) send=1;
	    if (str[i]==act[j].v3 || str[i]==act[j].v4) rec=1;
	   }
	if (send || rec) continue;
	for (j=0,k=0;j<*n;j++)
	   if (a1[j].v1==str[i] || a1[j].v2==str[i])
	     {
	      if (a1[j].v1 != str[i]) ext[k]=a1[j].v1;
	      if (a1[j].v2 != str[i]) ext[k]=a1[j].v2;
	      red[k]=j;
	      k++;
	     }
	t0=(char*) malloc(strlen(a1[red[0]].reb)+strlen(a1[red[1]].reb)+30);
	t0[0]='\0'; t=t0;
	strcat(t,a1[red[0]].reb); t+=strlen(a1[red[0]].reb);
	strcat(t++,"+");
	strcat(t,a1[red[1]].reb);
	free(a1[red[0]].reb);
	a1[red[0]].reb=strdup(t0);
	a1[red[0]].v1=ext[0];
	a1[red[0]].v2=ext[1];
	a1[red[0]].kol+=1;
	free(t0);
	k=red[1];
	frmatr(k,n,a1);
	kontrol();
	goto beg;
       }
con: ;
    }
 free(a2);
 free(str);
}
/*     Вывод подформулы ОБЪЕДИНЕНИЕ ПАРАЛЛЕЛЬНЫХ ПРОВОДИМОСТЕЙ     */
void reallyy(int *n,PASSIVE *a1)
{
 int i,j;
 char *t,*t0,*y,*y0;

 for (j=0;j<*n;j++)
    {
     if (   a1[j].reb[0]=='r' || a1[j].reb[0]=='R' || a1[j].reb[0]=='z'
	 || a1[j].reb[0]=='Z' || a1[j].reb[0]=='L'
	 || a1[j].reb[0]=='c' && flag_pln
	) continue;
     for (i=0;i<*n;i++)
	{
	 if (i == j) continue;
	 if (   a1[i].reb[0]=='r' || a1[i].reb[0]=='R' || a1[i].reb[0]=='z'
	     || a1[i].reb[0]=='Z' || a1[i].reb[0]=='L'
	     || a1[i].reb[0]=='c' && flag_pln) continue;

	 if (   a1[j].v1==a1[i].v1 && a1[j].v2==a1[i].v2
	     || a1[j].v1==a1[i].v2 && a1[j].v2==a1[i].v1)
	   {
	    t0=(char*) malloc(strlen(a1[i].reb)+strlen(a1[j].reb)+30);
	    t0[0]='\0'; t=t0;
	    y0=(char*) malloc(10);
	    y0[0]='\0'; y=y0;
	    strcat(y,"Ym"); y+=2;
	    if (nuz <=8) sprintf(y++,"%d",++nuz);
	    else
	      if (nuz > 8) sprintf(y,"%d",++nuz);
	    strcat(t,y0); t+=strlen(y0);
	    strcat(t++,"=");
	    strcat(t,a1[i].reb); t+=strlen(a1[i].reb); free(a1[i].reb);
	    strcat(t++,"+");
	    strcat(t,a1[j].reb); t+=strlen(a1[j].reb);
	    strcat(t++,";");
	    strcat(b,t0); b+=strlen(t0);
	    a1[i].reb=strdup(y0);
	    a1[i].kol=1;
	    free(t0);
	    free(y0);
	    frmatr(j,n,a1);
	    kontrol();
	    goto con;
	   }
	}
con: ;
    }
}
/* Вывод подформулы СВЕРТЫВАНИЕ ПАРАЛЛЕЛЬНЫХ ПРОВОДИМОСТИ И СОПРОТИВЛЕНИЯ */
void reallyz(int *n,PASSIVE *a1)
{
 int i,j;
 char *t,*t0,*y,*y0;

beg:
 for (i=0;i<*n;i++)
    {
     for (j=0;j<*n;j++)
	{
	 if (i == j) continue;
	 if (   a1[j].v1==a1[i].v1 && a1[j].v2==a1[i].v2
	     || a1[j].v1==a1[i].v2 && a1[j].v2==a1[i].v1)
	   {
	    if (  (a1[i].reb[0]=='g' || a1[i].reb[0]=='y'
		|| a1[i].reb[0]=='Y' || a1[i].reb[0]=='G'
		|| a1[i].reb[0]=='c' && !flag_pln
		|| a1[i].reb[0]=='-')
	     &&    (a1[j].reb[0]=='r' || a1[j].reb[0]=='z'
		 || a1[j].reb[0]=='Z'
		 || a1[j].reb[0]=='L' && !flag_pln))
	   {
	    t0=(char*) malloc(strlen(a1[i].reb)+strlen(a1[j].reb)+30);
	    t0[0]='\0'; t=t0;
	    y0=(char*) malloc(10);
	    y0[0]='\0'; y=y0;
	    strcat(y,"Ym"); y+=2;
	    if (nuz <=8) sprintf(y++,"%d",++nuz);
	    else
	      if (nuz > 8) sprintf(y,"%d",++nuz);
	    strcat(t,y0); t+=strlen(y0);
	    strcat(t++,"=");
	    strcat(t,a1[i].reb); t+=strlen(a1[i].reb); free(a1[i].reb);
	    if (a1[j].reb[0] != '-') strcat(t++,"+");
	    strcat(t,"1/("); t+=3;
	    strcat(t,a1[j].reb); t+=strlen(a1[j].reb);
	    strcat(t,");");
	    strcat(b,t0); b+=strlen(t0);
	    a1[i].reb=strdup(y0);
	    a1[i].kol=1;
	    free(t0);
	    free(y0);
	    frmatr(j,n,a1);
	    kontrol();
	    goto beg;
	   }
	  }
	}
    }
}
/* Вывод подформулы СВЕРТЫВАНИЕ ПОСЛЕДОВАТЕЛЬНЫХ СОПРОТИВЛЕНИЯ И ПРОВОДИМОСТИ */
void seqzy(int *n,PASSIVE *a1,int act_n,SOURCE *act)
{
 int i,j,k,p,send,rec,red[2];
 char *t,*t0,*y,*y0,*str,ext[2];
 GRAPH *a2=( GRAPH *) malloc((*n)*sizeof(GRAPH));

 str = (char*) malloc(2*(*n)+8*act_n+1);
beg:
 copypas(*n,a1,a2);
 verstr(*n,a2,&p,str);
 for (i=0;i<p;i++)
    {
     for (j=0,k=0;j<*n;j++)
	if (a1[j].v1==str[i] || a1[j].v2==str[i])
	  {
	   if (a1[j].v1 == a1[j].v2) goto con;
	   else k++;
	  }
     if (k==2)
       {
	send=0; rec=0;
	for (j=0;j<act_n;j++)
	   {
	    if (str[i]==act[j].v1 || str[i]==act[j].v2) send=1;
	    if (str[i]==act[j].v3 || str[i]==act[j].v4) rec=1;
	   }
	if (send || rec) continue;
	for (j=0,k=0;j<*n;j++)
	if (a1[j].v1==str[i] || a1[j].v2==str[i])
	  {
	   if (a1[j].v1 != str[i]) ext[k]=a1[j].v1;
	   if (a1[j].v2 != str[i]) ext[k]=a1[j].v2;
	   red[k]=j;
	   k++;
	  }
	if (   ((a1[red[0]].reb[0]=='g' || a1[red[0]].reb[0]=='y'
	    || a1[red[0]].reb[0]=='Y'
	    || a1[red[0]].reb[0]=='c' && !flag_pln
	    || a1[red[0]].reb[0]=='G' || a1[red[0]].reb[0]=='-')
	    &&  (   a1[red[1]].reb[0]=='r' || a1[red[1]].reb[0]=='z'
		 || a1[red[1]].reb[0]=='Z'
		 || a1[red[1]].reb[0]=='L' && !flag_pln))
	    ||
	       ((a1[red[1]].reb[0]=='g' || a1[red[1]].reb[0]=='y'
	    || a1[red[1]].reb[0]=='Y'
	    || a1[red[1]].reb[0]=='c' && !flag_pln
	    || a1[red[1]].reb[0]=='G' || a1[red[1]].reb[0]=='-')
	    &&  (   a1[red[0]].reb[0]=='r' || a1[red[0]].reb[0]=='z'
		 || a1[red[0]].reb[0]=='Z'
		 || a1[red[0]].reb[0]=='L' && !flag_pln)))
       {
	if (a1[red[0]].reb[0]!='r' && a1[red[0]].reb[0]!='z'
	    && a1[red[0]].reb[0]!='Z' && a1[red[0]].reb[0]!='L')
	  {k=red[0]; red[0]=red[1]; red[1]=k;}

	t0=(char*) malloc(strlen(a1[red[0]].reb)+strlen(a1[red[1]].reb)+30);
	t0[0]='\0'; t=t0;
	y0=(char*) malloc(10);
	y0[0]='\0'; y=y0;
	strcat(y,"Zm"); y+=2;
	if (nuz <=8) sprintf(y++,"%d",++nuz);
	else
	  if (nuz > 8) sprintf(y,"%d",++nuz);
	strcat(t,y0); t+=strlen(y0);
	strcat(t++,"=");
	strcat(t,a1[red[0]].reb); t+=strlen(a1[red[0]].reb);
	free(a1[red[0]].reb);
	strcat(t,"+1/("); t+=4;
	strcat(t,a1[red[1]].reb); t+=strlen(a1[red[1]].reb);
	strcat(t++,");");
	strcat(b,t0); b+=strlen(t0);
	a1[red[0]].reb=strdup(y0);
	a1[red[0]].v1=ext[0];
	a1[red[0]].v2=ext[1];
	a1[red[0]].kol=1;
	free(t0);
	free(y0);
	k=red[1];
	frmatr(k,n,a1);
	kontrol();
	goto beg;
/*        seqzy(n,a1,act_n,act);*/
       }
       }
con: ;
    }
 free(a2);
 free(str);
}
/**************  Сведение параллельных y-ветвей  ************** */
 void reallf(int *n,PASSIVE *a1)
{
 int i,j;

 if (!yesg(*n,a1)) return;
 for (j=0;j<*n;j++)
    {
     if (   a1[j].reb[0] == 'R' || a1[j].reb[0]=='z'
	 || a1[j].reb[0]=='Z' || a1[j].reb[0]=='L'
//	 || a1[j].reb[0]=='c' && flag_pln
	) continue;
     for (i=0;i<*n;i++)
	{
	 if (i == j) continue;
	 if (a1[j].reb[0] == 'c' && a1[i].reb[0]!='c') continue;
	 if (a1[j].reb[0] == 'g' && a1[i].reb[0]!='g') continue;
	 if (   a1[i].reb[0] == 'R' || a1[i].reb[0]=='z'
	     || a1[i].reb[0]=='Z' || a1[i].reb[0]=='L'
//	     || a1[i].reb[0]=='c' && flag_pln
	    ) continue;

	 if (   a1[j].v1==a1[i].v1 && a1[j].v2==a1[i].v2
	     || a1[j].v1==a1[i].v2 && a1[j].v2==a1[i].v1)
	   {
	    char *t,*t0;

	    t0=(char*) malloc(strlen(a1[i].reb)+strlen(a1[j].reb)+130);
	    t0[0]='\0'; t=t0;
	    strcat(t,a1[i].reb); t+=strlen(a1[i].reb); free(a1[i].reb);
	    if (a1[j].reb[0] != '-') strcat(t++,"+");
	    strcat(t,a1[j].reb);
	    a1[i].reb=strdup(t0);
	    free(t0);
	    a1[i].kol+=a1[j].kol;
	    frmatr(j,n,a1);
	    kontrol();
	    goto con;
	   }
	}
con: ;
    }
}
/*************** Объединение параллельных ИТУН  ****************/
 void parall(int *act_n,SOURCE *act)
{
 int i,j,pos,neg;
 char *t,*t0;

 if (!yess(*act_n,act)) return;
 for (j=0;j<*act_n;j++)
    {
     if (act[j].kol > 0 && act[j].reb[0] == 'G')
       {
	for (i=0;i<*act_n;i++)
	   {
	    if (i != j && act[i].kol > 0 && act[i].reb[0] == 'G')
	      {
	       pos=0; neg=0;
	       if (   act[j].v1==act[i].v1 && act[j].v2==act[i].v2
		   && act[j].v3==act[i].v3 && act[j].v4==act[i].v4
		   ||
		      act[j].v1==act[i].v2 && act[j].v2==act[i].v1
		   && act[j].v3==act[i].v4 && act[j].v4==act[i].v3) pos=1;
	       if (   act[j].v1==act[i].v1 && act[j].v2==act[i].v2
		   && act[j].v4==act[i].v3 && act[j].v3==act[i].v4
		   ||
		      act[j].v2==act[i].v1 && act[j].v1==act[i].v2
		   && act[j].v3==act[i].v3 && act[j].v4==act[i].v4) neg=1;
	       if (pos || neg)
		 {
		  t0=(char*)malloc(strlen(act[i].reb)+strlen(act[j].reb)+30);
		  t0[0]='\0'; t=t0;
		  strcat(t,act[i].reb); t+=strlen(act[i].reb);
		  free(act[i].reb);
		  if (neg)
		    {
		     if (act[j].reb[0] == '-' && act[j].kol == 1)
		       act[j].reb[0] = '+';
		     else strcat(t++,"-");
		     if (act[j].kol==1) strcat(t,act[j].reb);
		     else
		       {
			strcat(t++,"(");
			strcat(t,act[j].reb); t+=strlen(act[j].reb);
			strcat(t++,")");
		       }
		    }
		  else
		    {
		     strcat(t++,"+"); strcat(t,act[j].reb);
		    }
		  act[i].reb=strdup(t0); free(t0);
		  act[i].kol+=act[j].kol;
		  fract(j,act_n,act);
		  kontrol();
		  goto con;
		 }
	      }
	   }
       }
con: ;
    }
}
/**************  Сведение последовательных z-ветвей  **************/
 void seqzz0(int *n,PASSIVE *a1,int act_n,SOURCE *act)
{
 int i,j,k,p,send,rec,red[2];
 char *t,*t0,*str,ext[2];
 GRAPH *a2;

 if (!yesr(*n,a1)) return;
 a2=( GRAPH *) malloc((*n)*sizeof(GRAPH));
 str = (char*) malloc(2*(*n)+8*act_n+1);
beg:
 copypas(*n,a1,a2);
 verstr(*n,a2,&p,str);
 for (i=0;i<p;i++)
    {
     for (j=0,k=0;j<*n;j++)
	if (a1[j].v1==str[i] || a1[j].v2==str[i])
	  {
	   if (   a1[j].reb[0]=='g' || a1[j].reb[0]=='c'
	       || a1[j].reb[0]=='y' || a1[j].reb[0]=='Y'
//	       || a1[j].reb[0]=='L' && flag_pln
	       || a1[j].v1 == a1[j].v2) goto con;
	   else k++;
	  }
     if (k==2)
       {
	send=0; rec=0;
	for (j=0;j<act_n;j++)
	   {
	    if (str[i]==act[j].v1 || str[i]==act[j].v2) send=1;
	    if (str[i]==act[j].v3 || str[i]==act[j].v4) rec=1;
	   }
	if (send || rec) continue;
	for (j=0,k=0;j<*n;j++)
	   if (a1[j].v1==str[i] || a1[j].v2==str[i])
	     {
	      if (a1[j].v1 != str[i]) ext[k]=a1[j].v1;
	      if (a1[j].v2 != str[i]) ext[k]=a1[j].v2;
	      red[k]=j;
	      k++;
	     }
	if (a1[red[0]].reb[0]=='L' && a1[red[1]].reb[0]!='L') continue;
	if (a1[red[0]].reb[0]=='R' && a1[red[1]].reb[0]!='R') continue;
	t0=(char*) malloc(strlen(a1[red[0]].reb)+strlen(a1[red[1]].reb)+30);
	t0[0]='\0'; t=t0;
	strcat(t,a1[red[0]].reb); t+=strlen(a1[red[0]].reb);
	strcat(t++,"+");
	strcat(t,a1[red[1]].reb);
	free(a1[red[0]].reb);
	a1[red[0]].reb=strdup(t0);
	a1[red[0]].v1=ext[0];
	a1[red[0]].v2=ext[1];
	a1[red[0]].kol+=1;
	free(t0);
	k=red[1];
	frmatr(k,n,a1);
	kontrol();
	goto beg;
       }
con: ;
    }
 free(a2); free(str);
}
 void redact(int act_n, SOURCE *act, int *fl)
{
 int i;

 for (i=0; i<act_n; i++)
    { // юўхЁхфэющ ¤ыхьхэЄ ёяшёър pat
	if (   act[i].v1 == '0' && act[i].v2 == '0'
            || act[i].v3 == '0' && act[i].v4 == '0') 
       {
	*fl=2; return;
       }
    }
}
/******************* Комплекс схемных упрощений *******************/
 void simply(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl)
{
 int p1,p2;

 *fl=0; /* *fl=2 - схема вырождена, *fl=3 - схема содержит УИ с act[].kol==0 */
 if (!*n && !*act_n) return;
 one999(*act_n,act);
 redact(*act_n,act,fl);
 if (*fl==2) goto ret;
 reallf(n,matr);
 seqzz0(n,matr,*act_n,act);
 parall(act_n,act);
 hangr(n,matr,act_n,act,fl);
 if (*fl==2) goto ret;
 redu(n,matr,act_n,act);
 ejuihang(n,matr,act_n,act,fl);
 if (*fl==2) goto ret;
 if (*fl==3) return;
 node_n(*n,matr,*act_n,act,&p1);
 triangl(n,matr,act_n,act,fl);
 if (*fl==2) goto ret;
 if (*fl==3) return;
 mtria(n,matr,*act_n,act,fl);
 actloop(act_n,act,fl);
 if (*fl==2) goto ret;
 mloop(act_n,act,fl);
 if (*fl==2) goto ret;
 redu(n,matr,act_n,act);
 redseq(*n,matr,act_n,act,fl);
 if (*fl==2) goto ret;
 if (*fl==3) return;
 redu(n,matr,act_n,act);
 redpar(act_n,act,fl);
 if (*fl==2) goto ret;
 if (*fl==3) return;
 redu(n,matr,act_n,act);
 node_n(*n,matr,*act_n,act,&p2);
 if (   p1 != p2 || !hanged(*n,matr,*act_n,act)
     || !connec(*n,matr,*act_n,act)) {*fl=2; goto ret;}
 else return;
ret:
 frematr(*n,matr); freact(*act_n,act);
}
/* Упрощения при разомкнутых генераторах или приемниках УИ */
 void ejuihang(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl)
{
 int j,i,k,p,send,rec,na;
 char *str;
 GRAPH *a3;

 if (!*act_n) return;
 a3=(GRAPH *) malloc ((*n+8*(*act_n))*sizeof(GRAPH));
 copypas(*n,matr,a3);
 copyact(*n,*act_n,act,&na,a3);
 str=(char*) malloc(2*na+1);
 verstr(na,a3,&p,str); free(a3);
 for (i=0;i<p;i++)
    {
     for (j=0,k=0;j<*n;j++)
	if (matr[j].v1==str[i] || matr[j].v2==str[i]) k++;
     if (k==0)
       {
	send=0; rec=0;
	for (j=0;j<*act_n;j++)
	   {
	    if (str[i]==act[j].v1 || str[i]==act[j].v2) {send++; k=j;}
	    if (str[i]==act[j].v3 || str[i]==act[j].v4) {rec++; k=j;}
	   }
	if (act[k].kol <= 0) continue;
	if (send==1 && !rec)
	  {
	   if (   act[k].reb[0] == 'K'
	       || act[k].reb[0] == 'E' || act[k].reb[0] == 'U'
	       || act[k].reb[0] == 'H')
	     {
	      autontr999(k,act_n,act,fl);
	      goto ret;
	     }
	   else
	     if (   act[k].reb[0] != 'M' && act[k].reb[0] != 'Q'
		 && act[k].reb[0] != 'T')
	       {
		*fl=2; goto ret;
	       }
	     else
	       if (str[i]!='0')
		 {
		  *fl=2; goto ret;
		 }
	  }
	if (!send && rec==1)
	  {
	   if (act[k].reb[0] == 'B' || act[k].reb[0] == 'H')
	     {
	      act[k].kol=0; *fl=3; goto ret;
	     }
	   else
	     if (   act[k].reb[0] != 'M' && act[k].reb[0] != 'Q'
		 && act[k].reb[0] != 'T')
	       {
		*fl=2; goto ret;
	       }
	     else
	       if (str[i]!='0')
		 {
		  *fl=2; goto ret;
		 }
	  }
       }
    }
ret:
 free(str);
}
/* Упрощения при замкнутых генераторах или приемниках УИ */
 void actloop(int *act_n, SOURCE *act,int *fl)
{
 int i;

 one999(*act_n,act);
 for (i=0;i<*act_n;i++)
    {
     if (act[i].v1 == act[i].v2)
       {
	if (   act[i].kol < 0 || act[i].reb[0] == 'H'
	    || act[i].reb[0] == 'K' || act[i].reb[0] == 'E'
	    || act[i].reb[0] == 'U' || act[i].reb[0] == 'Q')
	  {*fl=2; return;}
	else
	  if (   act[i].reb[0]=='G'
	      || act[i].reb[0]=='J' || act[i].reb[0]=='I') fract(i,act_n,act);
	  else
	    if (act[i].reb[0]=='B')
	      {
	       free(act[i].reb);
	       nui(act[i].v3,act[i].v4,&i,act);
	       i--;
	      }
       }
     else
       if (act[i].v3 == act[i].v4)
	 {
	  if (   act[i].kol < 0 || act[i].kol == 999
	      || act[i].reb[0] == 'H' || act[i].reb[0] == 'B'
	      || act[i].reb[0] == 'T')
	    {*fl=2; return;}
	  else
	    if (act[i].reb[0]=='G') fract(i,act_n,act);
	    else
	      if (act[i].reb[0]=='K')
		{
		 free(act[i].reb);
		 nui(act[i].v1,act[i].v2,&i,act);
		 i--;
		}
	 }
    }
}
/* Упрощения при замкнутых генераторах или приемниках УИ */
 void mloop(int *act_n, SOURCE *act,int *fl)
{
 int i,j;

 for (i=0;i<*act_n;i++)
    {
     if (   act[i].reb[0] != 'M' && act[i].reb[0] != 'Q'
	 && act[i].reb[0] != 'T') continue;
     for (j=0;j<*act_n;j++)
	{
	 if (i == j) continue;
	 if (   act[j].reb[0] != 'M' && act[j].reb[0] != 'Q'
	     && act[j].reb[0] != 'T') continue;
	 if (act[i].reb[0] == 'M' && act[j].reb[0] == 'M')
	   {
	    if (   act[i].v1 == act[j].v1 && act[i].v2 == act[j].v2
		|| act[i].v1 == act[j].v2 && act[i].v2 == act[j].v1
		|| act[i].v3 == act[j].v3 && act[i].v4 == act[j].v4
		|| act[i].v3 == act[j].v4 && act[i].v4 == act[j].v3)
	      {*fl=2; return;}
	   }
	 else
	   if (   act[i].reb[0] == 'M' && act[j].reb[0] == 'Q'
	       || act[i].reb[0] == 'Q' && act[j].reb[0] == 'M')
	     {
	      if (   act[i].v3 == act[j].v3 && act[i].v4 == act[j].v4
		  || act[i].v3 == act[j].v4 && act[i].v4 == act[j].v3)
	      {*fl=2; return;}
	     }
	 else
	   if (   act[i].reb[0] == 'M' && act[j].reb[0] == 'T'
	       || act[i].reb[0] == 'T' && act[j].reb[0] == 'M')
	     {
	      if (   act[i].v1 == act[j].v1 && act[i].v2 == act[j].v2
		  || act[i].v1 == act[j].v2 && act[i].v2 == act[j].v1)
		{*fl=2; return;}
	     }
	 else
	   if (   act[i].reb[0] == 'Q' && act[j].reb[0] == 'Q')
	     {
	      if (   act[i].v3 == act[j].v3 && act[i].v4 == act[j].v4
		  || act[i].v3 == act[j].v4 && act[i].v4 == act[j].v3)
	      {*fl=2; return;}
	     }
	 else
	   if (   act[i].reb[0] == 'T' && act[j].reb[0] == 'T')
	     {
	      if (   act[i].v1 == act[j].v1 && act[i].v2 == act[j].v2
		  || act[i].v1 == act[j].v2 && act[i].v2 == act[j].v1)
		{*fl=2; return;}
	     }
	}
    }
}
/* Удаление y-ветвей, параллельных генераторам E или приемникам I */
 void redu(int *n,PASSIVE *matr,int *act_n,SOURCE *act)
{
 int i,m;
 char s1,s2,t1,t2;

 if (!*act_n) return;
 for (i=0;i<*act_n;i++)
    {
     if (   act[i].reb[0] =='1'
	 || act[i].reb[0] == 'K' || act[i].reb[0] == 'H'
	 || act[i].reb[0] == 'E' || act[i].reb[0] == 'I'
	 || act[i].reb[0] == 'Q')
       {
	s1=act[i].v1; s2=act[i].v2;
	for (m=0;m<*n;m++)
	   {
	    if (   matr[m].reb[0] == 'R' || matr[m].reb[0] == 'z'
		|| matr[m].reb[0] == 'Z' || matr[m].reb[0] == 'L') continue;
	    t1=matr[m].v1; t2=matr[m].v2;
	    if (t1==s1 && t2==s2 || t1==s2 && t2==s1)
	      {
	       frmatr(m,n,matr);
	      }
	   }
       }
    }
 for (i=0;i<*act_n;i++)
    {
     if (   act[i].reb[0] == '1'
	 || act[i].reb[0] == 'B' || act[i].reb[0] == 'H'
//	 || act[i].reb[0] == 'J' || act[i].reb[0] == 'I'
	 || act[i].reb[0] == 'T')
       {
	s1=act[i].v3; s2=act[i].v4;
	for (m=0;m<*n;m++)
	   {
	    if (   matr[m].reb[0] == 'R' || matr[m].reb[0] == 'z'
		|| matr[m].reb[0] == 'Z' || matr[m].reb[0] == 'L') continue;
	    t1=matr[m].v1; t2=matr[m].v2;
	    if (t1==s1 && t2==s2 || t1==s2 && t2==s1)
	      {
	       frmatr(m,n,matr);
	      }
	   }
       }
    }
}
/* Упрощения при параллельном соединении генераторов и приемников УИ */
 void redpar(int *act_n,SOURCE *act, int *fl)
{
 int i,j;
 char typi,typj;

rep:
 for (i=0;i<*act_n;i++)
    {
     if (!act[i].kol) continue;
     for (j=0;j<*act_n;j++)
	{
	 if (i == j || !act[j].kol) continue;
	 typi=act[i].reb[0]; typj=act[j].reb[0];
	 if (   act[j].v1==act[i].v1 && act[j].v2==act[i].v2
	     || act[j].v1==act[i].v2 && act[j].v2==act[i].v1)
	   {
	    if (   (   typi == 'K' || typi == 'E' || typi == 'U'
		    || typi == 'H' || act[i].kol < 0)
		&& (   typj == 'K' || typj == 'E' || typj == 'U'
		    || typj == 'H' || act[j].kol < 0))
	      {
	       *fl=2; return;
	      }
	    else
	    if (   (   typi == 'K' || typi == 'E' || typi == 'U'
		    || typi == 'H' || act[i].kol < 0)
		&& ((   typj == 'G' || typj == 'B'
		     || typj == 'J' || typj == 'I') && act[j].kol > 0))
	     {
	      autontr999(j,act_n,act,fl);
	      if (*fl == 2 || *fl == 3) return;
	      else goto rep;
	     }
	   }
	 if (   act[j].v3==act[i].v3 && act[j].v4==act[i].v4
	     || act[j].v3==act[i].v4 && act[j].v4==act[i].v3)
	   {
	    if (   (typi == 'B' || typi == 'H' || act[i].kol < 0)
		&& (typj == 'B' || typj == 'H' || act[j].kol < 0))
	      {
	       *fl=2; return;
	      }
	    else
	      if (   (  (typi == 'G' || typi == 'K') && act[i].kol > 0)
		  && (  typj == 'B' || typj == 'H'
		   || 
act[j].kol < 0 || act[j].kol == 999))
		{
		 autontr999(i,act_n,act,fl);
		 if (*fl == 2 || *fl == 3) return;
		 else goto rep;
		}
	   }
	 if (   act[i].v1==act[j].v3 && act[i].v2==act[j].v4
	     || act[i].v1==act[j].v4 && act[i].v2==act[j].v3)
	   {
	    if (   act[i].kol < 0
		&& (   //typj == 'B'
		 //   || 
typj == 'H') && act[j].kol > 0
		&& act[j].kol != 999)
	      {
	       act[j].kol = -act[j].kol;
	       goto rep;
	      }
	   }
	 if (   act[i].v3==act[j].v1 && act[i].v4==act[j].v2
	     || act[i].v3==act[j].v2 && act[i].v4==act[j].v1)
	   {
	    if (   act[i].kol < 0
		&& (   typj == 'H' || typj == 'K' 
		    || typj == 'E' || typj == 'U') && act[j].kol > 0)
	      {
	       act[j].kol = -act[j].kol;
	       if (act[j].kol==-999)
		 {
		  ntr999(act_n,act);
		  goto rep;
		 }
	      }
	   }
	}
    }
}
/* Стягивание z-ветвей, последовательных генераторам J или приемникам U */
 void hangr(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl)
{
 int j,i,k,x,p,send,rec,p1,p2;
 char *str,s1,s2;
 GRAPH *a2;

 *fl=0;
 if (!yesr(*n,matr)) return;
 a2=(GRAPH*) malloc((*n)*sizeof(GRAPH));
 str = (char*) malloc(2*(*n)+8*(*act_n)+1);
 copypas(*n,matr,a2);
 verstr(*n,a2,&p,str); free(a2);
 for (i=0;i<p;i++)
    {
     for (j=0,k=0;j<*n;j++)
	if (   (matr[j].v1==str[i] || matr[j].v2==str[i])
	    && matr[j].v1 != matr[j].v2) {x=j; k++;}

     if (k!=1) goto con;
     else
       {
	for (j=0;j<*act_n;j++)
	   if (    (str[i]==act[j].v1 || str[i]==act[j].v2
		|| str[i]==act[j].v3 || str[i]==act[j].v4)
		&& (act[j].reb[0]=='M' || act[j].reb[0]=='Q'
		|| act[j].reb[0]=='T')) goto con;
       }
     if (   matr[x].reb[0]=='R' || matr[x].reb[0] == 'z'
	  || matr[x].reb[0] == 'Z' || matr[x].reb[0] == 'L')
       {
	send=0; rec=0;
	for (j=0;j<*act_n;j++)
	   {
	    if (   str[i]!=act[j].v1 && str[i]!=act[j].v2
		&& str[i]!=act[j].v3 && str[i]!=act[j].v4) continue;
	    if (str[i]==act[j].v1 || str[i]==act[j].v2)
	      {
	       if (   act[j].kol < 0
		   || act[j].reb[0] == 'B' || act[j].reb[0]=='G'
		   || act[j].reb[0] == 'J' || act[j].reb[0] == 'I') send++;
	       else
		 if (   act[j].reb[0] == 'K' || act[j].reb[0] == 'H'
		     || act[j].reb[0] == 'E' || act[j].reb[0] == 'U') goto con;
	      }
	    if (str[i]==act[j].v3 || str[i]==act[j].v4)
	      {
	       if (   act[j].kol < 0 || act[j].kol == 999
		   || act[j].reb[0] == 'K' || act[j].reb[0]=='G') rec++;
	       else
		 if (act[j].reb[0] == 'B' || act[j].reb[0] == 'H') goto con;
	      }
	   }
	if (!send && !rec || send && !rec || !send && rec)
	  {
	   s1=matr[x].v1; s2=matr[x].v2;
	   node_n(*n,matr,*act_n,act,&p1);
	   frmatr(x,n,matr);
                     if (s1=='0') {s1=s2; s2='0';}
	   unipas(s1,s2,n,matr);
	   uniact(s1,s2,act_n,act);
	   node_n(*n,matr,*act_n,act,&p2);
	   if (   p1 != p2+1 && p2 != 0 || !hanged(*n,matr,*act_n,act)
	       || !connec(*n,matr,*act_n,act))
	     {*fl=2; goto ret;}
                     // act[j].kol=0;
	   actloop(act_n,act,fl);
	   if (*fl == 2) goto ret;
	   continue;
	  }
       }
con: ;
    }
ret:
 free(str);
}
/* Упрощения при последовательном соединении генераторов и приемников УИ */
 void redseq(int n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl)
{
 int j,i,k,p,send,rec,*se,*re,na;
 char *str,typ1,typ2;
 GRAPH *a3;

 if (!*act_n) return;
 se=(int*) malloc((*act_n+2)*sizeof(int));
 re=(int*) malloc((*act_n+2)*sizeof(int));
 a3=(GRAPH *) malloc ((n+4*(*act_n))*sizeof(GRAPH));
 copypas(n,matr,a3);
 copyact(n,*act_n,act,&na,a3);
 str=(char*) malloc(2*na+1);
 verstr(na,a3,&p,str); free(a3);
 for (i=0;i<p;i++)
    {
     for (j=0,k=0;j<n;j++)
	if (matr[j].v1==str[i] || matr[j].v2==str[i]) k++;
     if (k==0)
       {
	send=0; rec=0;
	for (j=0;j<*act_n;j++)
	   {
	    if (str[i]==act[j].v1 || str[i]==act[j].v2) se[++send]=j;
	    if (str[i]==act[j].v3 || str[i]==act[j].v4) re[++rec]=j;
	   }
	if (send==1 && rec==1)
	  {
	   typ1=act[se[1]].reb[0]; typ2=act[re[1]].reb[0];
	   if (   (   typ1 == 'B' || typ1 == 'G'
		   || typ1 == 'J' || typ1 == 'I' || act[se[1]].kol < 0)
	       && (   typ2 == 'G' || typ2 == 'K'
		   || act[re[1]].kol < 0))
	     {
	      if (act[se[1]].kol > 0)
		{
		 act[se[1]].kol=-act[se[1]].kol;
		 if (act[se[1]].kol==-999)
		   {
		    ntr999(act_n,act);
		    goto ret;
		   }
		}
	      else
	      if (act[re[1]].kol > 0)
		{
		 act[re[1]].kol=-act[re[1]].kol;
		 goto ret;
		}
	     }
	  }
	if (send==2 && !rec)
	  {
	   typ1=act[se[1]].reb[0]; typ2=act[se[2]].reb[0];
	   if (   ((   typ1 == 'K' || typ1 == 'H'
		    || typ1 == 'E' || typ1 == 'U') && act[se[1]].kol > 0)
	       && (   typ2 == 'G' || typ2 == 'B'
		   || typ2 == 'J' || typ2 == 'I' || act[se[2]].kol < 0))
	     {
	      autontr999(se[1],act_n,act,fl);
	      goto ret;
	     }
	   else
	   if (   ((   typ2 == 'K' || typ2 == 'H'
		    || typ2 == 'E' || typ2 == 'U') && act[se[2]].kol > 0)
	       && (   typ1 == 'G' || typ1 == 'B'
		   || typ1 == 'J' || typ1 == 'I' || act[se[1]].kol < 0))
	     {
	      autontr999(se[2],act_n,act,fl);
	      goto ret;
	     }
	   else
	     if (   (   typ1 == 'G' || typ1 == 'B'
		     || typ1 == 'J' || typ1 == 'I' || act[se[1]].kol < 0)
		 && (   typ2 == 'G' || typ2 == 'B'
		     || typ2 == 'J' || typ2 == 'I' || act[se[2]].kol < 0))
	       {
		*fl=2; goto ret;
	       }
	  }
	if (!send && rec==2)
	  {
	   typ1=act[re[1]].reb[0]; typ2=act[re[2]].reb[0];
	   if (   ((typ1 == 'B' || typ1 == 'H') && act[re[1]].kol > 0)
	       && (typ2 == 'G' || typ2 == 'K' || act[re[2]].kol < 0))
	     {
	      autontr999(re[1],act_n,act,fl);
	      goto ret;
	     }
	   else
	   if (   ((typ2 == 'B' || typ2 == 'H') && act[re[2]].kol > 0)
	       && (typ1 == 'G' || typ1 == 'K' || act[re[1]].kol < 0))
	     {
	      autontr999(re[2],act_n,act,fl);
	      goto ret;
	     }
	   else
	   if (   (   typ1 == 'G' || typ1 == 'K'
		   || act[re[1]].kol == 999 || act[re[1]].kol < 0)
	       && (   typ2 == 'G' || typ2 == 'K'
		   || act[re[2]].kol == 999 || act[re[2]].kol < 0))
	     {
	      *fl=2; goto ret;
	     }
	  }
       }
    }
ret:
 free(se); free(re); free(str);
}
/*  Упрощение третьего в треугольнике c двумя генераторами или приемниками */
 void triangl(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl)
{
 int i,j,k,m,pr=0;
 char s1,s2,t1,t2,x1,x2;
 *fl=1;

beg:
 for (i=0;i<*act_n;i++)
    {
     if (act[i].kol >= 0) continue;
     if (!pr) {s1=act[i].v1; s2=act[i].v2;}
     else {s1=act[i].v3; s2=act[i].v4;}
     for (j=0;j<*act_n;j++)
	{
	 if (j != i && act[j].kol < 0)
	   {
	    if (!pr) {t1=act[j].v1; t2=act[j].v2;}
	    else {t1=act[j].v3; t2=act[j].v4;}
	    if (t1==s1) {x1=t2; x2=s2; goto sear;}
	    if (t1==s2) {x1=t2; x2=s1; goto sear;}
	    if (t2==s1) {x1=t1; x2=s2; goto sear;}
	    if (t2==s2) {x1=t1; x2=s1; goto sear;}
	   }
	 continue;
sear:
	 for (k=0;k<*n;k++)
	    {
	     if (   (matr[k].reb[0] == 'g' || matr[k].reb[0] == 'y'
		 || matr[k].reb[0] == 'Y' || matr[k].reb[0] == 'c')
		 && (   matr[k].v1==x1 && matr[k].v2==x2
		     || matr[k].v1==x2 && matr[k].v2==x1))
	       {
		frmatr(k,n,matr);
	       }
	    }
	 for (m=0;m<*act_n;m++)
	    {
	     if (m != i && m != j && act[m].reb[0] == 'G')
	       {
		if (!pr) {t1=act[m].v1; t2=act[m].v2;}
		else {t1=act[m].v3; t2=act[m].v4;}
		if (t1==x1 && t2==x2 || t1==x2 && t2==x1)
		  {
		   if (act[m].kol < 0)
		     {
		      *fl=2; return;
		     }
		   else {act[m].kol=0; *fl=3; return;}
		  }
	       }
	    }
	}
    }
 pr++;
 if (pr==1) goto beg;
}
/*  Упрощение третьего в треугольнике c двумя генераторами или приемниками */
 int mtria(int *n,PASSIVE *matr,int act_n,SOURCE *act,int *fl)
{
 int i,j,k,m,pr=0;
 char s1,s2,t1,t2,x1,x2;
 *fl=1;

beg:
 for (i=0;i<act_n;i++)
    {
     if (   !pr && act[i].reb[0] == 'T'
	 ||  pr && act[i].reb[0] == 'Q'
	 ||  act[i].reb[0] == 'M')
       {
	if (!pr) {s1=act[i].v1; s2=act[i].v2;}
	else {s1=act[i].v3; s2=act[i].v4;}
	for (j=0;j<act_n;j++)
	   {
	    if (i == j) continue;
	    if (   !pr && act[j].reb[0] == 'T'
		||  pr && act[j].reb[0] == 'Q'
		||  act[j].reb[0] == 'M')
	      {
	       if (!pr) {t1=act[j].v1; t2=act[j].v2;}
	       else {t1=act[j].v3; t2=act[j].v4;}
	       if (t1==s1) {x1=t2; x2=s2; goto sear;}
	       if (t1==s2) {x1=t2; x2=s1; goto sear;}
	       if (t2==s1) {x1=t1; x2=s2; goto sear;}
	       if (t2==s2) {x1=t1; x2=s1; goto sear;}
	      }
	    continue;
sear:
	    for (k=0;k<*n;k++)
	       {
		if (   (matr[k].reb[0] == 'g' || matr[k].reb[0] == 'y'
		    || matr[k].reb[0] == 'Y' || matr[k].reb[0] == 'c')
		    && (   matr[k].v1==x1 && matr[k].v2==x2
			|| matr[k].v1==x2 && matr[k].v2==x1))
		  {
		   frmatr(k,n,matr);
		  }
	       }
	   }
	}
    }
 pr++;
 if (pr==1) goto beg;
 return(0);
}

/*******************  Проверка на связность  ********************/
 int connec(int n,PASSIVE *matr,int act_n,SOURCE *act)
{
 int pr,na;
 char kr;
 GRAPH *a2;

 if (!n && !act_n) return(1);
 a2=( GRAPH *) malloc ((n+4*act_n)*sizeof(GRAPH));
 copypas(n,matr,a2);
 copyact(n,act_n,act,&na,a2);
 pr=0; kr=a2[0].v1; bond1f(na,a2,kr,kr,&pr); free(a2);
 if (na>pr) return(0); else return(1);
}
/* Выявление сечений из генераторов НУИ (J) или приемников НУИ (U) */
 int hanged(int n,PASSIVE *matr,int act_n,SOURCE *act)
{
 int j,i,k,p,send,rec,na;
 char *str,kr;
 GRAPH *a3;

 if (!act_n) return(1);
 a3=(GRAPH *) malloc ((n+4*act_n)*sizeof(GRAPH));
 copypas(n,matr,a3);
 copyact(n,act_n,act,&na,a3);
 str=(char*) malloc(2*na+1);
 verstr(na,a3,&p,str); free(a3);
 for (i=0;i<p;i++)
    {
     k=0;
     kr=str[i];
     for (j=0;j<n;j++)
	if (matr[j].v1==kr || matr[j].v2==kr) k++;
     for (j=0;j<act_n;j++)
	if ((act[j].reb[0]=='M' || act[j].reb[0]=='Q' || act[j].reb[0]=='T')
	    &&
	     (kr==act[j].v1 || kr==act[j].v2
	    || kr==act[j].v3 || kr==act[j].v4)) k++;
     if (k==0)
       {
	send=0; rec=0;
	for (j=0;j<act_n;j++)
	   {
	    if (   str[i]!=act[j].v1 && str[i]!=act[j].v2
		&& str[i]!=act[j].v3 && str[i]!=act[j].v4) continue;
	    if (str[i]==act[j].v1 || str[i]==act[j].v2)
	      {
	       if (   act[j].reb[0] == '1'
		   || act[j].reb[0] == 'B' || act[j].reb[0] == 'G'
		   || act[j].reb[0] == 'J' || act[j].reb[0] == 'I') send++;
	       else
		 if (   act[j].reb[0] == 'K' || act[j].reb[0] == 'H'
		     || act[j].reb[0] == 'E' || act[j].reb[0] == 'U') goto con;
	      }
	    if (str[i]==act[j].v3 || str[i]==act[j].v4)
	      {
	       if ( act[j].reb[0] == '1' || act[j].kol == 999
		   || act[j].reb[0] == 'K' || act[j].reb[0] == 'G') rec++;
	       else
		 if (act[j].reb[0] == 'B' || act[j].reb[0] == 'H') goto con;
	      }
	   }
	if (send && !rec || !send && rec)
	  {
	   free(str); return(0);
	  }
       }
con: ;
    }
 free(str);
 return(1);
}
/******************** Выделение z-петли ********************/
 int pasloop(int *n,PASSIVE *matr,
	     char *s1,char *s2,char **t,int *numb,int *sign)
{
 int i;
 char *w,w0[2];

// if (!yesr(*n,matr)) return(0);
 for (i=0;i<*n;i++)
    {
     if (   (matr[i].reb[0]=='R' || matr[i].reb[0]=='z'
	 || matr[i].reb[0]=='Z' || matr[i].reb[0]=='L')
	 && matr[i].v1 == matr[i].v2)
       {
	*t=strdup(matr[i].reb);
	*numb=matr[i].kol;
	*s1=matr[i].v1; *s2=matr[i].v2;
	frmatr(i,n,matr);
	*sign=0;
	return(1);
       }
     else
       if (   (matr[i].reb[0]=='g' || matr[i].reb[0]=='y'
	   || matr[i].reb[0]=='Y' || matr[i].reb[0]=='C')
	   && matr[i].v1 == matr[i].v2)
       {
	w0[0]='\0'; w=w0;
	strcat(w++,"1");
	*t=strdup(w0);
	*numb=1;
	*s1=matr[i].v1; *s2=matr[i].v2;
	frmatr(i,n,matr);
	*sign=0;
	return(1);
       }
    }
 return(0);
}
/* Выделение z-ветви, параллельной генератору напряжения или приемнику тока */
 int redur(int *n,PASSIVE *matr,int act_n,SOURCE *act,
	  char *ss1,char *ss2,char **t,int *numb,int *sign)
{
 int i,m;
 char s1,s2,t1,t2;

 if (!yesr(*n,matr) || !act_n) return(0);
 if (!act_n) return(0);
 for (i=0;i<act_n;i++)
    {
     if (   act[i].reb[0] == 'K' || act[i].reb[0] == 'E'
	 || act[i].reb[0] == 'U' || act[i].reb[0] == 'H'
	 || act[i].kol < 0)
       {
	s1=act[i].v1; s2=act[i].v2;
	for (m=0;m<*n;m++)
	   {
	    if (   matr[m].reb[0]=='R' || matr[m].reb[0]=='z'
		|| matr[m].reb[0]=='Z' || matr[m].reb[0]=='L')
	      {
	       t1=matr[m].v1; t2=matr[m].v2;
	       if (t1==s1 && t2==s2 || t1==s2 && t2==s1)
		 {
		  *t=strdup(matr[m].reb);
		  *numb=matr[m].kol;
		  *ss1=matr[m].v1; *ss2=matr[m].v1;
		  frmatr(m,n,matr);
		  *sign=0;
		  return(1);
		 }
	      }
	   }
       }
    }
 for (i=0;i<act_n;i++)
    {
     if (   act[i].reb[0] == 'B' || act[i].reb[0] == 'H'
	 || act[i].kol < 0 || act[i].kol == 999)
       {
	s1=act[i].v3; s2=act[i].v4;
	for (m=0;m<*n;m++)
	   {
	    if (   matr[m].reb[0]=='R' || matr[m].reb[0]=='z'
		|| matr[m].reb[0]=='Z' || matr[m].reb[0]=='L')
	      {
	       t1=matr[m].v1; t2=matr[m].v2;
	       if (t1==s1 && t2==s2 || t1==s2 && t2==s1)
		 {
		  *t=strdup(matr[m].reb);
		  *numb=matr[m].kol;
		  *ss1=matr[m].v1; *ss2=matr[m].v1;
		  frmatr(m,n,matr);
		  *sign=0;
		  return(1);
		 }
	      }
	   }
	}
    }
 return(0);
}
/* Выделение у-ветви, последовательной генератору J или приемнику U */
 int pashang(int *n,PASSIVE *matr,int act_n,SOURCE *act,
	    char *s1,char *s2,char **t,int *numb,int *sign)
{
 int j,i,k,x,p,send,rec,*re,ind,pr;
 char *str;
 GRAPH *a2;

 if (!yesg(*n,matr)) return(0);
 a2=( GRAPH *) malloc((*n)*sizeof(GRAPH));
 re=(int*) malloc((act_n+2)*sizeof(int));
 str = (char*) malloc(2*(*n)+8*act_n+1);
 copypas(*n,matr,a2);
 verstr(*n,a2,&p,str); free(a2);
 if (yesmqt(act_n,act)) pr=1; else pr=0;
 for (i=0;i<p;i++)
    {
     if (pr && str[i]=='0') continue;
     for (j=0,k=0;j<*n;j++)
	{
	 if (matr[j].v1==str[i] || matr[j].v2==str[i]) {x=j; k++;}
	}
     if (   k==1
	 && (   matr[x].reb[0]=='g' || matr[x].reb[0]=='y'
	     || matr[x].reb[0]=='Y' || matr[x].reb[0]=='c'))
       {
	send=0; rec=0; ind=0;
	for (j=0;j<act_n;j++)
	   {
	    if (str[i]==act[j].v1 || str[i]==act[j].v2) {send++; k=j;}
	    if (str[i]==act[j].v3 || str[i]==act[j].v4)
	      {
	       if (!ind)
		 {
		  if (act[j].kol == 999)
		    {
		     ind=1;
		     re[++rec]=j; k=j;
		    }
		  else {re[++rec]=j; k=j;}
		 }
	       else
		 if (act[j].kol < 0) {re[++rec]=j; k=j;}
	      }
	   }
	if (!send && !rec ||
	       send == 1 && !rec
	    && (   act[k].reb[0] == 'B' || act[k].reb[0] == 'J'
		|| act[k].reb[0] == 'I' || act[k].reb[0] == 'G'
		|| act[k].kol < 0)
	    || rec == 1 && !send
	    && (   act[k].reb[0] == 'K' || act[k].reb[0] == 'G'
		|| act[k].kol < 0 || act[k].kol == 999))
	  {
	   *t=strdup(matr[x].reb);
	   *numb=matr[x].kol;
	   *s1=matr[x].v1; *s2=matr[x].v2;
	   frmatr(x,n,matr);
	   free(str); free(re);
	   *sign=0;
	   return(1);
	  }
	else continue;
       }
    }
 free(str); free(re);
 return(0);
}
/* Выделение параллельных генератора НУИ и приемника НУИ */
 int degener(int *act_n,SOURCE *act,
	     char *s1,char *s2,char **t,int *numb,int *sign)
{
 int i,j,ind=0;

 if (!yesn(*act_n,act)) return(0);
rep:
 for (i=0;i<*act_n;i++)
    {
     if (   !ind && act[i].kol != -999
	 ||  ind && act[i].kol >= 0 ) continue;
//     if (   act[i].reb[0] == 'M' || act[i].reb[0] == 'Q'
//	 || act[i].reb[0] == 'T') continue;
     for (j=0;j<*act_n;j++)
	{
	 if (   !ind && act[j].kol != -999
	     ||  ind && act[j].kol >= 0 ) continue;
	 *sign=-1;
	 if (act[i].v1 == act[j].v3 && act[i].v2 == act[j].v4) *sign=0;
	 if (act[i].v1 == act[j].v4 && act[i].v2 == act[j].v3) *sign=1;
	 if (*sign != -1)
	   {
	    if (i != j)
	      {
	       if (!*sign)
		 {
		  act[j].v3=act[i].v4;
		  act[j].v4=act[i].v3;
		 }
	       else
		 {
		  act[j].v3=act[i].v3;
		  act[j].v4=act[i].v4;
		  *sign=0;
		 }
	      }
	    *t=strdup(act[i].reb);
	    *numb=act[i].kol;
	    *s1=act[i].v1; *s2=act[i].v2;
	    fract(i,act_n,act);
	    return(1);
	   }
	}
    }
 ind++;
 if (ind == 1) goto rep;
 else return(0);
}
/* Редукция узла, инцидентного генераторам НУИ (J) и приемникам НУИ (U) */
 int nodred(int n,PASSIVE *matr,int *act_n,SOURCE *act,
	    char  *s1,char *s2, char **t,int *numb,int *sign)
{
 char *str,kr,akr;
 GRAPH *a3;
 int i,j,k,na,p,sn1,sn2,ret=0,sou1,sou2;

 a3=(GRAPH *) malloc ((n+4*(*act_n))*sizeof(GRAPH));
 copypas(n,matr,a3);
 copyact(n,*act_n,act,&na,a3);
 str=(char*) malloc(na+1);
 verstr(na,a3,&p,str);
 free(a3);
 for (i=0;i<p;i++)
    {
     kr=str[i];
     if (kr=='0') continue;
     for (j=0,k=0;j<n;j++) if (matr[j].v1==kr || matr[j].v2==kr) k++;
     for (j=0;j<*act_n;j++)
	if ((act[j].reb[0]=='M')
	    &&
	     (kr==act[j].v1 || kr==act[j].v2
	    || kr==act[j].v3 || kr==act[j].v4)) k++;
 /* for (j=0;j<*act_n;j++)
	if (   act[j].reb[0]=='B'
	    && (kr==act[j].v3 || kr==act[j].v4)) k++;*/
     for (j=0;j<*act_n;j++)
	if (   act[j].reb[0]=='Q'
	    && (kr==act[j].v3 || kr==act[j].v4)) k++;
     for (j=0;j<*act_n;j++)
	if (   act[j].reb[0]=='T'
	    && (kr==act[j].v1 || kr==act[j].v2)) k++;
     if (k) continue;
     sou1 = -1; sou2 = -1;
     for (j=0;j<*act_n;j++)
	{
	 if (act[j].kol >= 0) continue;
	 if (kr==act[j].v1) {sou1=j; sn1=1;}
	 if (kr==act[j].v2) {sou1=j; sn1=0;}
	 if (kr==act[j].v3) {sou2=j; sn2=1;}
	 if (kr==act[j].v4) {sou2=j; sn2=0;}
	}
     if (sou1 == -1) continue;
     if (sou2 == -1) continue;
     for (j=0;j<*act_n;j++)
	{
	 if (act[j].kol <= 0 || act[j].reb[0] == 'G') continue;
	 if (      (kr==act[j].v1 || kr==act[j].v2)
		&& (   act[j].reb[0]=='K' || act[j].reb[0]=='E'
		    || act[j].reb[0]=='U' || act[j].reb[0]=='H')
	     ||
		   (kr==act[j].v3 || kr==act[j].v4)
		&& (   act[j].reb[0]=='B'
		    || act[j].reb[0]=='J' || act[j].reb[0]=='I'
		    || act[j].reb[0]=='H')) goto con;
	}
     if (sn1==sn2) *sign=0; else *sign=1;
     if (sou1!=sou2)
       {
	if (*sign) *sign=0; else *sign=1;
       }
     ret=1;
     break;
con: ;
    }
 free(str);
 if (!ret) return(0);
 if (act[sou1].v1 == kr) akr=act[sou1].v2; else akr=act[sou1].v1;
 for (k=0;k<*act_n;k++)
    {
     if (k==sou1) continue;
     if (act[k].v1==kr) act[k].v1=akr;
     if (act[k].v2==kr) act[k].v2=akr;
    }
 if (act[sou2].v3 == kr) akr=act[sou2].v4; else akr=act[sou2].v3;
 for (k=0;k<*act_n;k++)
    {
     if (k==sou2) continue;
     if (act[k].v3==kr) act[k].v3=akr;
     if (act[k].v4==kr) act[k].v4=akr;
    }
 if (sou1!=sou2)
   {
    if (!*sign)
      {
       act[sou2].v3=act[sou1].v3;
       act[sou2].v4=act[sou1].v4;
      }
    else
      {
       act[sou2].v3=act[sou1].v4;
       act[sou2].v4=act[sou1].v3;
       *sign=0;
      }
   }
 *t=strdup(act[sou1].reb);
 *numb=act[sou1].kol;
 fract(sou1,act_n,act);
 *s1=' '; *s2=' ';
 return(1);
}
   /* єяЁю∙хэш  тшфр: (р-0)->(a+0); (a-a)->(a+0), sign=2*sign; (a+a)->0 */
// 1000 - ьр°шээюх яЁхфёЄртыхэшх эюьхЁр "0",
//        ўЄюс√ ьюцэю с√ыю яЁшётрштрЄ№ чэръ
void redpat(int nr,int n, int pat[4][100], int *sign)
// n - ўшёыю ¤ыхьхэЄют ╠╤└─; pat[0] - яхЁтр  ёЄЁюър;
// pat[1] - яхЁт√щ ёЄюысхЎ; pat[2] - тЄюЁр  ёЄЁюър;
// pat[3] - тЄюЁющ ёЄюысхЎ; *sign - чэрўхэшх ╠╤└─
{
 int i,O=255;
 for (i = nr; i<n; i++)
    {// юўхЁхфэющ ¤ыхьхэЄ ёяшёър pat
     // ьюфшЇшърЎш  ёяшёър тшфр (0-a)->(a+0) ш (0-c)->(c+0)
     // (0+a)-> -(a+0) ш (0+c)-> -(c+0)
     if (pat[0][i] == O)
	if (pat[1][i] < 0)
	  {pat[0][i] = abs(pat[1][i]); pat[1][i] = O;}
	else
	  {pat[0][i] = pat[1][i]; pat[1][i] = O; *sign = -(*sign);}
     if (pat[2][i] == O)
	if (pat[3][i] < 0)
	  {pat[2][i] = abs(pat[3][i]); pat[3][i] = O;}
	else
	  {pat[2][i] = pat[3][i]; pat[3][i] = O; *sign = -(*sign);}
     // ьюфшЇшърЎш  ёяшёър тшфр (р-0)->(a+0) ш (c-0)->(c+0)
     if (pat[1][i] == -O) pat[1][i] = O;
     if (pat[3][i] == -O) pat[3][i] = O;
     // ьюфшЇшърЎш  ёяшёър тшфр (a-a)->(a+0), sign=2*sign
     // ш (c-c)->(c+0), sign=2*sign ё єфтюхэшхь чэрър
     if (pat[0][i] == abs(pat[1][i]) && pat[1][i]<0)
       {pat[1][i] = O; *sign = 2*(*sign);}
     if (pat[2][i] == abs(pat[3][i]) && pat[3][i]<0)
       {
	pat[3][i] = O; *sign = 2 * (*sign);
       }
     // (a+a) шыш (ё+ё) - юяЁхфхышЄхы№ Ёртхэ эєы■
     if (   pat[1][i]>0 && pat[0][i]==pat[1][i]
	 || pat[3][i]>0 && pat[2][i]==pat[3][i]
	 || pat[0][i] == O && pat[0][i]==pat[1][i]
	 || pat[2][i] == O && pat[2][i]==pat[3][i]
	)
       {*sign=0; return;}
    }
}
   // ьюфшЇшърЎш  ёЄЁюъ юёЄрт°шїё  ¤ыхьхэЄют ёяшёър - ыєў°р  тхЁёш 
void modification(int nr,int n, int pat[4][100],
		  int ind,int n1,int n2,int *sign)
{
 int j,t1,t2;

 if (ind==0) {n1=0; n2=1; t1 = pat[0][nr]; t2 = pat[1][nr];}
 else {n1=2; n2=3; t1 = pat[2][nr]; t2 = pat[3][nr];}
 for (j = nr + 1; j < n; j++)
    {
     if (t2 < 0)
       {// хёыш эюьхЁ тЄюЁющ ёЄЁюъш (ёЄюысЎр) юЄЁшЎрЄхы№э√щ
	if (pat[n1][j] == t1)
	  if (pat[n2][j] < 0)
	    {pat[n1][j] = -pat[n2][j]; pat[n2][j] = -t1;}
	  else
	    {pat[n1][j] = pat[n2][j]; pat[n2][j] = t1; *sign = -(*sign);}
	if (abs(pat[n2][j]) == t1)
	  if (pat[n2][j] < 0) pat[n2][j] = -t2;
	  else pat[n2][j] = t2;
       }
     else
       {// чрьхэр эюьхЁют ёЄЁюъ (ёЄюысЎют) ё ёюїЁрэхэшхь чэрър
	if (abs(pat[n1][j]) == t1)
	  if (pat[n1][j] < 0) pat[n1][j] = -t2;
	  else pat[n1][j] = t2;
	if (abs(pat[n2][j]) == t1)
	  if (pat[n2][j] < 0) pat[n2][j] = -t2;
	  else pat[n2][j] = t2;
       }
    }
 }
  /* ═рїюцфхэшх юяЁхфхышЄхы  *sign */
 void det(int n, int pat[4][100],int *sign)
{
 int i,j,k,a,b,c,d,nl=0,list[400];

 *sign=1;
 if (!flag_nul)
   {
    redpat(0,n,pat,sign);
    if (!(*sign)) return;
    for (i = 0; i<n; i++)
       for (j = 0; j<4; j++)
	  {
	   for (k = 0; k<nl; k++)
	      if (abs(pat[j][i]) == list[k]) goto br;
	   list[nl]=abs(pat[j][i]); nl++;
       br: ;
	  }
   }
 for (i = 0; i < n; i++)
    {
     redpat(i,n,pat,sign);
     if (!(*sign)) return;
     // юўхЁхфэющ ¤ыхьхэЄ ёяшёър pat
     a = pat[0][i]; b = pat[1][i];
     c = pat[2][i]; d = pat[3][i];
     if (!flag_nul)
       for (k = 0; k<nl; k++)
	  if (a == list[k])
	    {
	     for (j = k; j<nl-1; j++) list[j]=list[j+1];
	     nl--; break;
	    }
     if (a != c) *sign=-(*sign); // эюьхЁр ёЄЁюъ Ёрчэ√х
	// ьюфшЇшърЎш  ёЄЁюъ юёЄрт°шїё  ¤ыхьхэЄют ёяшёър
     modification(i,n,pat,0,a,b,sign);
	// ьюфшЇшърЎш  ёЄюысЎют юёЄрт°шїё  ¤ыхьхэЄют ёяшёър
     modification(i,n,pat,1,c,d,sign);
     // чрьхэр эюьхЁют ёЄЁюъ ш ёЄюысЎют ё эюьхЁюь t1 эр эюьхЁ t3
     // ё ёюїЁрэхэшхь чэрър
     for (j = i + 1; j < n; j++)
	{
	 if (abs(pat[0][j]) == a)
	   if (pat[0][j] < 0) pat[0][j] = -c;
	   else pat[0][j] = c;
	 if (abs(pat[1][j]) == a)
	   if (pat[1][j] < 0) pat[1][j] = -c;
	   else pat[1][j] = c;
	 if (abs(pat[2][j]) == a)
	   if (pat[2][j] < 0) pat[2][j] = -c;
	   else pat[2][j] = c;
	 if (abs(pat[3][j]) == a)
	   if (pat[3][j] < 0) pat[3][j] = -c;
	   else pat[3][j] = c;
	}
    }
  if (!flag_nul && nl > 1) *sign=0;
  i=n-1;
  if (   (pat[0][i] == pat[2][i] && abs(pat[1][i]) == abs(pat[3][i])
       || pat[0][i] == abs(pat[3][i]) && abs(pat[1][i]) == pat[2][i])
	 && (   pat[0][i] == 255 || pat[1][i] == 255
	     || pat[2][i] == 255 || pat[3][i] == 255)) return;
  else *sign=0;
}
    /* ═рїюцфхэшх юяЁхфхышЄхы  ёїхь√ ё шфхры№э√ьш N-, M-, Q-, T-¤ыхьхэЄрьш */
int nodreda(int n,int *act_n, SOURCE *act,
	char  *s1, char *s2, char **t, int *numb, int *sign,int *flm)
{
 int i, j, si;
 int pat[4][100];
 char *w, w0[3], c0;

 *flm=0;
 if (n || !(*act_n)) return(0);
 for (i=0;i<*act_n;i++)
    if (   act[i].reb[0] != '1' && act[i].reb[0] != 'M'
	&& act[i].reb[0] != 'Q' && act[i].reb[0] != 'T')
       return(0);
 // printf(" r_a\n");
for (i = 0; i < *act_n; i++)
    {
     if (act[i].v1!= '0' && act[i].v2!= '0' && act[i].v3!= '0' && act[i].v4!= '0') 
       continue;
goto con;
      }
c0=act[0].v1;
for (i = 0; i < *act_n; i++)
    {
     if (act[i].v1==c0) act[i].v1 = '0';
     if (act[i].v2==c0) act[i].v2 = '0';
     if (act[i].v3==c0) act[i].v3 = '0';
     if (act[i].v4==c0) act[i].v4 = '0';
      }
 // яхЁхїюф юЄ юяшёрэшщ act_n шфхры№э√ї ¤ыхьхэЄют ъ ёєььрЁэ√ь
 // ш ЁрчэюёЄэ√ь └─ т тшфх ёЄЁєъЄєЁ√ *pat ЁрчьхЁэюёЄш act_n
con: for (i = 0; i < *act_n; i++)
    {
     // (a+b)(c+d)
     if (act[i].reb[0] == '1')
       {
	pat[0][i] = act[i].v1-48; pat[1][i] = act[i].v2-48;
	pat[2][i] = act[i].v3-48; pat[3][i] = act[i].v4-48;
       }
     else // (a-b)(c-d)
     if (act[i].reb[0] == 'M')
       {
	pat[0][i] = act[i].v1-48; pat[1][i] = -(act[i].v2-48);
	pat[2][i] = act[i].v3-48; pat[3][i] = -(act[i].v4-48);
       }
     else // (a+b)(c-d)
     if (act[i].reb[0] == 'Q')
       {
	pat[0][i] = act[i].v1-48; pat[1][i] = act[i].v2-48;
	pat[2][i] = act[i].v3-48; pat[3][i] = -(act[i].v4-48);
       }
     else // (a-b)(c+d)
     if (act[i].reb[0] == 'T')
       {
	pat[0][i] = act[i].v1-48; pat[1][i] = -(act[i].v2-48);
	pat[2][i] = act[i].v3-48; pat[3][i] = act[i].v4-48;
       }
    }
 for (i = 0; i < *act_n; i++)
    {
     if (!pat[0][i]) pat[0][i]=255;
     if (!pat[1][i]) pat[1][i]=255;
     if (!pat[2][i]) pat[2][i]=255;
     if (!pat[3][i]) pat[3][i]=255;
    }
 det(*act_n, pat,sign);
 if (*sign)
   { // яЁхюсЁрчютрэшх фтєчэрўэюую ўшёыр sign т ёЄЁюъє шч фтєї ЎшЇЁ
    si = abs(*sign);
    w0[0] = '\0'; w = w0;
rep: if (si == 0) strcat(w++, "0");
    if (si == 1) strcat(w++, "1");
    if (si == 2) strcat(w++, "2");
    if (si == 3) strcat(w++, "3");
    if (si == 4) strcat(w++, "4");
    if (si == 5) strcat(w++, "5");
    if (si == 6) strcat(w++, "6");
    if (si == 7) strcat(w++, "7");
    if (si == 8) strcat(w++, "8");
    if (si == 9) strcat(w++, "9");
    si = si / 10;
    if (si != 0 ) goto rep;
    *t = strdup(w0);
/*(*t)+=strlen(w0);
if (abs(*sign)) for (i = 0; i < *act_n; i++)
    {
     // (a+b)(c+d)
   if (act[i].kol == -1 && act[i].reb[0] != '1')
       {
strcat((*t)++,"*");
	    strcat(*t,act[i].reb); (*t)+=strlen(act[i].reb); // free(act[i].reb);
       }
}*/
    *numb = 1;
    *s1 = ' '; *s2 = ' ';
    if (*sign > 0) *sign = 0;
    else *sign = 1;
   }
 else *flm=1;
 freact(*act_n,act);
 *act_n=0;
 return(1);
}