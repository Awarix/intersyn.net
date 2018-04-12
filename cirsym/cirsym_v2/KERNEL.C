/* Рекурсивное динамическое разложение схемного определителя */

#include "cirsym.h"



extern FILE *out,*inpa,*set;

extern char *b,*c,sp;
extern unsigned long leng,lengl,lepr;
extern int flag_fi3,flag_fi4,flag_fi5,flag_fsn,extract,
       flag_e,cirf,noequ,flag_pln,nuz,flag_sp,flag_cL,
       flag_mir,flag_opt,flag_two,flag_thr,st_pln;
extern float range2,range3,range4,range5;

/* Функция проверки существования многомерного НУИ */
 int is999(int j,int act_n,SOURCE *act)
{
 int i;

 for (i=0;i<act_n;i++)
    {
     if (i==j) continue;
     if (abs(act[i].kol) == 999) return(1);
    }
 return(0);
}
/* Нейтрализация многомерного НУИ */
 void allntr999(int *act_n,SOURCE *act)
{
 int i,act0_n;

 for (i=0;i<*act_n;i++)
    {
     act0_n=*act_n;
     if (act[i].kol==999)
       {
        if (act[i].reb[0] == 'E' || act[i].reb[0] == 'U')
          nuintr(act[i].v1,act[i].v2,i,act);
        else
          if (act[i].reb[0] == 'J' || act[i].reb[0] == 'I')
            {
	     fract(i,act_n,act);
	     if (*act_n != act0_n) i--;
	    }
       }
    }
}
/* Нейтрализация k-го элемента многомерного НУИ */
 void autontr999(int k,int *act_n,SOURCE *act,int *fl)
{
 *fl=2;
 if (act[k].kol < 0) return;
 if (act[k].kol != 999) {act[k].kol=0; *fl=3; return;}
 if (is999(k,*act_n,act))
   {
    *fl=1;
    if (act[k].reb[0] == 'E' || act[k].reb[0] == 'U')
      nuintr(act[k].v1,act[k].v2,k,act);
    else
    if (act[k].reb[0] == 'J' || act[k].reb[0] == 'I')
      fract(k,act_n,act);
   }
 one999(*act_n,act);
}
/* Нейтрализация оставшихся элементов многомерного НУИ */
 void ntr999(int *act_n,SOURCE *act)
{
 int i;

 for (i=0;i<*act_n;i++)
    {
     if (act[i].kol != 999) continue;
     if (act[i].reb[0] == 'E' || act[i].reb[0] == 'U')
       nuintr(act[i].v1,act[i].v2,i,act);
     else
     if (act[i].reb[0] == 'J' || act[i].reb[0] == 'I')
       fract(i,act_n,act);
    }
}
/* Преобразование многомерного НУИ (с одним элементом) в обычный НУИ*/
 void one999(int act_n,SOURCE *act)
{
 int i,j,numb=0;

 for (i=0;i<act_n;i++)
    {
     if (act[i].kol == 999) numb++, j=i;
    }
 if (numb==1)
   {
    act[j].kol=-999;
   }
}
/* Нейтрализация УИ (act[j].kol = 0) с проверкой на вырождение схемы */
 void newtral(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl)
{
 int j,p1,p2;
 char s1,s2,s3,s4,type;

nwtrl:
 for (j=0;j<*act_n;j++)
    {
     if (act[j].kol) continue;
     node_n(*n,matr,*act_n,act,&p1);
     type=act[j].reb[0];
     s1=act[j].v1; s2=act[j].v2; s3=act[j].v3; s4=act[j].v4;
     if (type == 'K')
       {
	nuintr(s1,s2,j,act);
	node_n(*n,matr,*act_n,act,&p2);
	if (p1 != p2) {*fl=2; goto ret;}
	else break;
       }
     else
     if (type == 'B')
       {
	nuintr(s3,s4,j,act);
	node_n(*n,matr,*act_n,act,&p2);
	if (p1 != p2) {*fl=2; goto ret;}
	else break;
       }
     else
       if (type == 'H')
	 {
                    if (s3=='0') {s3=s4; s4='0';}
	  nuintr(s1,s2,j,act);
	  unipas(s3,s4,n,matr);
	  uniact(s3,s4,act_n,act);
	 }
       else
       if (type == 'G')
	 {
	  fract(j,act_n,act);
	  node_n(*n,matr,*act_n,act,&p2);
	  if (p1 != p2) {*fl=2; goto ret;}
	  else break;
	 }
    }
 simply(n,matr,act_n,act,fl);
 if (*fl == 3) goto nwtrl;
 else return; /* *fl == 0 - det!=0 или *fl == 2 - det=0 */
ret:
 frematr(*n,matr); freact(*act_n,act);
}
/* Выделение элементов с максимальным участием (общих множителей) */
/*                  в полиномиальном определителе                 */

/*
cont:
       dl2=leng+strlen(c);
       if (!strlen(m0)) strcat(m++,"1"), m[0]='\0'; 
       else {m--; m[0]='\0';} 
       printa(m0,1,signs); 
       if (!p2 || p2==1) goto ret;
       if (strlen(m0)) strcat(b++,"*");
       if (   (*(b-3) == '(' || *(b-3) == '+'
	   || *(b-3) == '-' || strlen(c) == 2)
	   && *(b-2) == '1' && *(b-1) == '*')
	 {
	  b-=2; b[0]='\0'; 
	 }
       if (*(b-1)=='-') sens=1; else sens=0;
       if (*n + *act_n > 0 && (m0[0] != '1' || sens)) strcat(b++,"(");
       dl1=leng+strlen(c);
       gggf(*n,matr,*act_n,act);
       if (dl1>=leng+strlen(c))
	 {
	  ster(dl2); 
	  *fl=2;
	  goto ret;
	 }
       else
	 {
	  if (*n + *act_n > 0 && (m0[0] != '1' || sens)) strcat(b++,")");
	 }
      }
    else
      { 
       if (!strlen(m0)) {strcat(m++,"1"); m[0]='\0';}
       else {m--; m[0]='\0';} 
       printa(m0,1,signs); 
      }
    *fl=1; 
    goto ret;
   }
 *fl=3;
 return;
ret: ; // free(m0);
} */
void multiplp(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl,int pln)
{
 int p2,sign,signs,numb,tnum,tn,sens,k=0,flm;
 char s1,s2,*t,*m,m0[500];
 unsigned long dl1,dl2;

 newtral(n,matr,act_n,act,fl);
 if (*fl == 2) return;
 node_n(*n,matr,*act_n,act,&p2);
 if (!p2 && *(b-1)=='*') {strcat(b++,"1"); *fl=1; return;}
/* выявление общих множителей */
 flm=0;
 if (!p2 && pln==st_pln)
   {
    strcat(b++,"1");
    *fl=1;
    return;
   }
 else if (!p2 && pln != st_pln)
	{
	 *fl=2;
	 return;
	}
   if (   mqtloop(*act_n,act,&s1,&s2,&t,&numb,&sign)
   || pasloop(n,matr,&s1,&s2,&t,&numb,&sign)
   || pashang(n,matr,*act_n,act,&s1,&s2,&t,&numb,&sign)
 || redur(n,matr,*act_n,act,&s1,&s2,&t,&numb,&sign)
 || degener(act_n,act,&s1,&s2,&t,&numb,&sign)	
 || nodred(*n,matr,act_n,act,&s1,&s2,&t,&numb,&sign)
 || nodreda(*n,act_n,act,&s1,&s2,&t,&numb,&sign,&flm)
    )
    { /* t - очередной множитель; numb, sign - кратность и знак множителя */
    signs=0; /* знак строки сомножителей */
    tnum=0;
//    m0=(char*) malloc(500); /*  строка сомножителей */
    m0[0]='\0'; m=m0;
rep:
    if (flm==1) {*fl=2; goto ret;}
    if (sign && signs || !sign && !signs) signs=0; else signs=1;
    if (t[0]=='1') goto nopr;
    tn=tnum;
    if (abs(numb) == 1) tnum+=strlen(t)+1;
    else tnum+=strlen(t)+3;
//    m0=(char*) realloc(m0,(tnum+1)*sizeof(char));
    m0[tn]='\0'; m=m0+tn;
    if (   !strlen(m0) && !*n && !*act_n  && !signs
	&& ((*(b-1) == '(' || *(b-1) == '+' || *(b-1) == '-' /* || *(b-1) == '*' */)
	&& strlen(c)  || !lengl ))
      {
       numb=1;
      }
    if (strchr(t,'c')!=NULL || strchr(t,'L')!=NULL) k++;
    printi(&m,t,numb); /* вывод в строку t очередного сомножителя */
nopr:
    free(t);
    if (*n || *act_n)
      {
       if (s1 != ' ' && s1 != s2)
	 {
	  if (s1=='0') {s1=s2; s2='0';}
	  unipas(s1,s2,n,matr);
	  uniact(s1,s2,act_n,act);
	 }
       mirror(*act_n,act);
       newtral(n,matr,act_n,act,fl);
       if (*fl == 2) goto ret;
       node_n(*n,matr,*act_n,act,&p2);
       if (!p2 || p2==1 && !n) goto cont;
/* повторный проход */
       flm=0;
       if (   mqtloop(*act_n,act,&s1,&s2,&t,&numb,&sign)
   || pasloop(n,matr,&s1,&s2,&t,&numb,&sign)
   || pashang(n,matr,*act_n,act,&s1,&s2,&t,&numb,&sign)
 || redur(n,matr,*act_n,act,&s1,&s2,&t,&numb,&sign)
 || degener(act_n,act,&s1,&s2,&t,&numb,&sign)	
 || nodred(*n,matr,act_n,act,&s1,&s2,&t,&numb,&sign)
 || nodreda(*n,act_n,act,&s1,&s2,&t,&numb,&sign,&flm)	  
	  ) goto rep;
cont:
       dl2=leng+strlen(c);
       if (!strlen(m0)) strcat(m++,"1"), m[0]='\0'; /* множитель-единица */
       else {m--; m[0]='\0';} /* удаление лишнего знака "*" */
       if (k + pln > st_pln) {*fl=2; goto ret;}
       else
       printa(m0,1,signs); /* вывод строки сомножителей в выходную строку с */
       if (strlen(m0)) strcat(b++,"*");
       if (   (*(b-3) == '(' || *(b-3) == '+'
	   || *(b-3) == '-' || strlen(c) == 2)
	   && *(b-2) == '1' && *(b-1) == '*')
	 {
	  b-=2; b[0]='\0'; /* удаление избыточного множителя "1*" */
	 }
       if (*(b-1)=='-') sens=1; else sens=0;
       if (*n + *act_n > 0 && (m0[0] != '1' || sens)) strcat(b++,"(");
       dl1=leng+strlen(c);
       gggp(*n,matr,*act_n,act,pln+k);
       if (dl1>=leng+strlen(c))
	 {
	  ster(dl2); /* определитель схемы равен нулю */
	  *fl=2;
	  goto ret;
	 }
       else
	 {
	  if (*n + *act_n > 0 && (m0[0] != '1' || sens)) strcat(b++,")");
	 }
      }
    else
      { /* определитель схемы является произведением сомножителей */
       if (k + pln != st_pln) {*fl=2; goto ret;}
       if (!strlen(m0)) {strcat(m++,"1"); m[0]='\0';}
       else {m--; m[0]='\0';} /* удаление лишнего знака "*" */
       printa(m0,1,signs); /* вывод строки сомножителей в выходную строку с */
      }
    *fl=1; /* определитель раскрыт */
    goto ret;
   }
 *fl=3; /* нет общих множителей */
 return;
 ret: ;//free(m0);
}
/* Генератор формулы полиномиального схемного определителя */
 void gggp(int n,PASSIVE *matr,int act_n,SOURCE *act,int pln)
{
 int fl,p1,p2,first,fm,smf,n1,act1_n,extract0,q,k;
 char s1,s2,sm1,sm2;
 unsigned long dl,dl1,dl2;
 PASSIVE *a1;
 SOURCE *act1;

/* коррекция знака подвыражения */
 if (*(b-1)=='-')
   {
    b--; b[0]='\0';
    strcat(b++,"+");
   }
/* деление на две подсхемы по одному узлу */
 mirror(act_n,act);
 multiplp(&n,matr,&act_n,act,&fl,pln);
 if (fl == 1 || fl == 2) return; /* det схемы != 0 || det = 0 */
 node_n(n,matr,act_n,act,&p1);
 if (p1 && pln==st_pln && !yescL(n,matr))
   {
    bisec1(n,matr,act_n,act,&fl);
    if (fl) return;
/* деление на две подсхемы по двум - пяти узлам */
    if (flag_fsn && p1 > flag_fsn)
      {
       bisec2(n,matr,act_n,act,&fl,range2);
       if (fl) return;
      }
    if (flag_fi3 && p1 > flag_fi3)
      {
       bisec3(n,matr,act_n,act,&fl,range3);
       if (fl) return;
      }
    if (flag_fi4  &&  p1 > flag_fi4)
      {
       bisec4(n,matr,act_n,act,&fl,range4);
       if (fl) return;
      }
    if (flag_fi5 && p1 > flag_fi5)
      {
       bisec5(n,matr,act_n,act,&fl,range5);
       if (fl) return;
      }
   }
 extract0=extract;
beg:
/* выделение элементов с образованием двух производных схем */
 if (fl == 1 || fl == 2) return; /* det схемы != 0 || det = 0 */
 if (!extract0 && yesgc(n,matr))
   { /* выделение проводимости */
    hangpas(n,matr,act_n,act,&first);
    if (first==-1)
      choiceg(n,matr,act_n,act,&first);
    if (strchr(matr[first].reb,'c')!=NULL) k=1; else k=0;
    s1=matr[first].v1; s2=matr[first].v2;
    dl=leng+strlen(c);
    dl2=leng+strlen(c);
    printa(matr[first].reb,matr[first].kol,0);
    frmatr(first,&n,matr);
    if (k + pln > st_pln) {ster(dl2); goto two1;}
    strcat(b++,"*");
    n1=n; act1_n=act_n;
    if (n1)
      {
       a1=(PASSIVE *) malloc(n1*sizeof(PASSIVE));
       pascopy(n1,matr,a1);
      }
    act1=(SOURCE *) malloc ((act1_n+1)*sizeof(SOURCE));
    if (act1_n) actcopy(act1_n,act,act1);
    nui(s1,s2,&act1_n,act1);
    if (n1+act1_n > 1) strcat(b++,"(");
    dl1=leng+strlen(c);
    gggp(n1,a1,act1_n,act1,pln+k);
    if (n1) free(a1); if (act1_n) free(act1);
    if (dl1>=leng+strlen(c)) ster(dl2);
    else
      {
       if (n1+act1_n > 1) strcat(b++,")");
       dl=leng+strlen(c);
       strcat(b++,"+");
      }
two1:
    node_n(n,matr,act_n,act,&p2);
    dl1=leng+strlen(c);
    if (p2 == p1)
      {
       gggp(n,matr,act_n,act,pln);
       if (dl1 >= leng+strlen(c)) ster(dl);
      }
    else
      {
       ster(dl);
       frematr(n,matr); freact(act_n,act);
      }
    return;
   } /* end g-extraction */
 else
 if (!extract0 && yesrL(n,matr))
   { /* выделение сопротивления */
    choicer(n,matr,act_n,act,&first);
    dl=leng+strlen(c);
    dl2=leng+strlen(c);
    if (strchr(matr[first].reb,'L')!=NULL) k=1; else k=0;
    s1=matr[first].v1; s2=matr[first].v2;
    printa(matr[first].reb,matr[first].kol,0);
    frmatr(first,&n,matr);
    if (k + pln > st_pln) {ster(dl2); goto two2;}
    strcat(b++,"*");
    n1=n; act1_n=act_n;
    node_n(n1,matr,act1_n,act,&p2);
    if ((n1 || act1_n) && p2 == p1)
      {
       if (n1)
	 {
	  a1=(PASSIVE *) malloc(n1*sizeof(PASSIVE));
	  pascopy(n1,matr,a1);
	 }
       if (act1_n)
	 {
	  act1=(SOURCE *) malloc (act1_n*sizeof(SOURCE));
	  actcopy(act1_n,act,act1);
	 }
       if (n1+act1_n > 1) strcat(b++,"(");
       dl1=leng+strlen(c);
       gggp(n1,a1,act1_n,act1,pln+k);
       if (n1) free(a1); if (act1_n) free(act1);
       if (dl1>=leng+strlen(c)) ster(dl2);
       else
	 {
	  if (n1+act1_n > 1) strcat(b++,")");
	  dl=leng+strlen(c);
	  strcat(b++,"+");
	 }
      }
    else ster(dl2);
two2:
    act1_n=act_n;
    act1=(SOURCE*) malloc((act1_n+1)*sizeof(SOURCE));
    if (act1_n) actcopy(act1_n,act,act1);
    nui(s1,s2,&act1_n,act1);
    dl1=leng+strlen(c);
    gggp(n,matr,act1_n,act1,pln);
    if (dl1 >= leng+strlen(c)) ster(dl);
    free(act1);
    freact(act_n,act);
    return;
   }  /* end r-extraction */
 else
 if (!extract0 && yesg(n,matr))
   { /* выделение проводимости */
    hangpas(n,matr,act_n,act,&first);
    if (first==-1)
      choiceg(n,matr,act_n,act,&first);
    if (strchr(matr[first].reb,'c')!=NULL) k=1; else k=0;
    s1=matr[first].v1; s2=matr[first].v2;
    dl=leng+strlen(c);
    dl2=leng+strlen(c);
    printa(matr[first].reb,matr[first].kol,0);
    frmatr(first,&n,matr);
    if (k + pln > st_pln) {ster(dl2); goto two3;}
    strcat(b++,"*");
    n1=n; act1_n=act_n;
    if (n1)
      {
       a1=(PASSIVE *) malloc(n1*sizeof(PASSIVE));
       pascopy(n1,matr,a1);
      }
    act1=(SOURCE *) malloc ((act1_n+1)*sizeof(SOURCE));
    if (act1_n) actcopy(act1_n,act,act1);
    nui(s1,s2,&act1_n,act1);
    if (n1+act1_n > 1) strcat(b++,"(");
    dl1=leng+strlen(c);
    gggp(n1,a1,act1_n,act1,pln+k);
    if (n1) free(a1); if (act1_n) free(act1);
    if (dl1>=leng+strlen(c)) ster(dl2);
    else
      {
       if (n1+act1_n > 1) strcat(b++,")");
       dl=leng+strlen(c);
       strcat(b++,"+");
      }
two3:
    node_n(n,matr,act_n,act,&p2);
    dl1=leng+strlen(c);
    if (p2 == p1)
      {
       gggp(n,matr,act_n,act,pln);
       if (dl1 >= leng+strlen(c)) ster(dl);
      }
    else
      {
       ster(dl);
       frematr(n,matr); freact(act_n,act);
      }
    return;
   } /* end g-extraction */
 else
 if (!extract0 && yesr(n,matr))
   { /* выделение сопротивления */
    choicer(n,matr,act_n,act,&first);
    dl=leng+strlen(c);
    dl2=leng+strlen(c);
    if (strchr(matr[first].reb,'L')!=NULL) k=1; else k=0;
    s1=matr[first].v1; s2=matr[first].v2;
    printa(matr[first].reb,matr[first].kol,0);
    frmatr(first,&n,matr);
    if (k + pln > st_pln) {ster(dl2); goto two4;}
    strcat(b++,"*");
    n1=n; act1_n=act_n;
    node_n(n1,matr,act1_n,act,&p2);
    if ((n1 || act1_n) && p2 == p1)
      {
       if (n1)
	 {
	  a1=(PASSIVE *) malloc(n1*sizeof(PASSIVE));
	  pascopy(n1,matr,a1);
	 }
       if (act1_n)
	 {
	  act1=(SOURCE *) malloc (act1_n*sizeof(SOURCE));
	  actcopy(act1_n,act,act1);
	 }
       if (n1+act1_n > 1) strcat(b++,"(");
       dl1=leng+strlen(c);
       gggp(n1,a1,act1_n,act1,pln+k);
       if (n1) free(a1); if (act1_n) free(act1);
       if (dl1>=leng+strlen(c)) ster(dl2);
       else
	 {
	  if (n1+act1_n > 1) strcat(b++,")");
	  dl=leng+strlen(c);
	  strcat(b++,"+");
	 }
      }
    else ster(dl2);
two4:
    act1_n=act_n;
    act1=(SOURCE*) malloc((act1_n+1)*sizeof(SOURCE));
    if (act1_n) actcopy(act1_n,act,act1);
    nui(s1,s2,&act1_n,act1);
    dl1=leng+strlen(c);
    gggp(n,matr,act1_n,act1,pln);
    if (dl1 >= leng+strlen(c)) ster(dl);
    free(act1);
    freact(act_n,act);
    return;
   }  /* end r-extraction */
 else
 if (extract0 && noideal(act_n,act,&first))
   { /* выделение управляемых источников */
    char type;

    type=act[first].reb[0];
    if (n)
      {
       a1=(PASSIVE *) malloc(n*sizeof(PASSIVE));
       pascopy(n,matr,a1);
      }
    act1_n=act_n;
    act1=(SOURCE *) malloc (act1_n*sizeof(SOURCE));
    actcopy(act1_n,act,act1);
    act[first].kol=-act[first].kol;
    if (act[first].kol==-999) ntr999(&act_n,act);
    dl=leng+strlen(c);
    if (type == 'K' || type == 'B')
      {
       fl=0;
       if (!n && act_n==1) {strcat(b++,"("); fl=1;}
      }
    dl1=leng+strlen(c);
    gggp(n,matr,act_n,act,pln);
//    frematr(n,matr);
//    freact(act_n,act);
    if (dl1 >= leng+strlen(c)) ster(dl);
    dl=leng+strlen(c);
    if (dl1 < leng+strlen(c)) strcat(b++,"+");
    if (type == 'K' || type == 'B')
      {
       if (fl)
	 {
	  strcat(b,"1)"); b+=2;
	  freematr(n,a1); freeact(act1_n,act1);
	  return;
	 }
      }
    autontr999(first,&act1_n,act1,&fl);
    if (fl == 1 || fl == 3)
      {
       dl1=leng+strlen(c);
       gggp(n,a1,act1_n,act1,pln);
       if (n) free(a1); if (act1_n) free(act1);
       if (dl1>=leng+strlen(c)) ster(dl);
      }
    else
      {
       ster(dl);
       freematr(n,a1); freeact(act1_n,act1);
      }
    return;
   }   /* end active */
 else
   {
    if (extract0 == 0) extract0=1;
    else if (extract0 == 1) extract0=0;
    goto beg;
   }
}
/* Выделение элементов с максимальным участием (общих множителей) */
 void multipl(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl)
{
 int p2,sign,signs,numb,tnum,tn,sens,flm;
 char s1,s2,*t,*m,m0[500];
 unsigned long dl1,dl2;

 newtral(n,matr,act_n,act,fl);
 if (*fl == 2) return;
 node_n(*n,matr,*act_n,act,&p2);
 if (!p2 && *(b-1)=='*') {strcat(b++,"1"); *fl=1; return;}
/* выявление общих множителей */
 flm=0;
    if (   mqtloop(*act_n,act,&s1,&s2,&t,&numb,&sign)
   || pasloop(n,matr,&s1,&s2,&t,&numb,&sign)
   || pashang(n,matr,*act_n,act,&s1,&s2,&t,&numb,&sign)
 || redur(n,matr,*act_n,act,&s1,&s2,&t,&numb,&sign)
 || degener(act_n,act,&s1,&s2,&t,&numb,&sign)	
 || nodred(*n,matr,act_n,act,&s1,&s2,&t,&numb,&sign)
 || nodreda(*n,act_n,act,&s1,&s2,&t,&numb,&sign,&flm)
    )
   { /* t - очередной множитель; numb, sign - кратность и знак множителя */
    signs=0; /* знак строки сомножителей */
    tnum=0;
//    m0=(char*) malloc(500); /*  строка сомножителей */
    m0[0]='\0'; m=m0;
rep:
    if (flm==1) {*fl=2; goto ret;}
    if (sign && signs || !sign && !signs) signs=0; else signs=1;
    if (t[0]=='1') goto nopr;
    tn=tnum;
    if (abs(numb) == 1) tnum+=strlen(t)+1;
    else tnum+=strlen(t)+3;
//    m0=(char*) realloc(m0,(tnum+1)*sizeof(char));
    m0[tn]='\0'; m=m0+tn;
/*    if (   !strlen(m0) && !*n && !*act_n  && !signs
	&& ((*(b-1) == '(' || *(b-1) == '+' || *(b-1) == '-')
	&& strlen(c)  || !lengl ))
      {
       numb=1;
      }*/
    printi(&m,t,numb); /* вывод в строку t очередного сомножителя */
nopr:
    free(t);
    if (*n || *act_n)
      {
       if (s1 != ' ' && s1 != s2)
	 {
	  if (s1=='0') {s1=s2; s2='0';}
	  unipas(s1,s2,n,matr);
	  uniact(s1,s2,act_n,act);
	 }
       mirror(*act_n,act);
       newtral(n,matr,act_n,act,fl);
       if (*fl == 2) goto ret;
       node_n(*n,matr,*act_n,act,&p2);
       if (!p2 || p2==1 && !n) goto cont;
/* повторный проход */
       flm=0;
       if (   mqtloop(*act_n,act,&s1,&s2,&t,&numb,&sign)
   || pasloop(n,matr,&s1,&s2,&t,&numb,&sign)
   || pashang(n,matr,*act_n,act,&s1,&s2,&t,&numb,&sign)
 || redur(n,matr,*act_n,act,&s1,&s2,&t,&numb,&sign)
 || degener(act_n,act,&s1,&s2,&t,&numb,&sign)	
 || nodred(*n,matr,act_n,act,&s1,&s2,&t,&numb,&sign)
 || nodreda(*n,act_n,act,&s1,&s2,&t,&numb,&sign,&flm)	  
	  ) goto rep;
cont:
       dl2=leng+strlen(c);
       if (!strlen(m0)) strcat(m++,"1"), m[0]='\0'; /* множитель-единица */
       else {m--; m[0]='\0';} /* удаление лишнего знака "*" */
       printa(m0,1,signs); /* вывод строки сомножителей в выходную строку с */
       if (!p2 || p2==1) goto ret;
       if (strlen(m0)) strcat(b++,"*");
       if (   (*(b-3) == '(' || *(b-3) == '+'
	   || *(b-3) == '-' || strlen(c) == 2)
	   && *(b-2) == '1' && *(b-1) == '*')
	 {
	  b-=2; b[0]='\0'; /* удаление избыточного множителя "1*" */
	 }
       if (*(b-1)=='-') sens=1; else sens=0;
       if (*n + *act_n > 0 && (m0[0] != '1' || sens)) strcat(b++,"(");
       dl1=leng+strlen(c);
       gggf(*n,matr,*act_n,act);
       if (dl1>=leng+strlen(c))
	 {
	  ster(dl2); /* определитель схемы равен нулю */
	  *fl=2;
	  goto ret;
	 }
       else
	 {
	  if (*n + *act_n > 0 && (m0[0] != '1' || sens)) strcat(b++,")");
	 }
      }
    else
      { /* определитель схемы является произведением сомножителей */
       if (!strlen(m0)) {strcat(m++,"1"); m[0]='\0';}
       else {m--; m[0]='\0';} /* удаление лишнего знака "*" */
       printa(m0,1,signs); /* вывод строки сомножителей в выходную строку с */
      }
    *fl=1; /* определитель раскрыт */
    goto ret;
   }
 *fl=3; /* нет общих множителей */
 return;
ret: ; // free(m0);
}
/* Генератор формулы схемного определителя */
 void gggf(int n,PASSIVE *matr,int act_n,SOURCE *act)
{
 int fl,p1,p2,first,fm,smf,n1,act1_n,extract0,q;
 char s1,s2,sm1,sm2;
 unsigned long dl,dl1,dl2;
 PASSIVE *a1;
 SOURCE *act1;

/* коррекция знака подвыражения */
 if (*(b-1)=='-')
   {
    b--; b[0]='\0';
    strcat(b++,"+");
   }
 mirror(act_n,act);
 multipl(&n,matr,&act_n,act,&fl);
 if (fl == 1 || fl == 2 || !n && !act_n) return; /* det схемы != 0 || det = 0 */
/* выделение общих множителей */
/* деление на две подсхемы по одному узлу */
 bisec1(n,matr,act_n,act,&fl);
 if (fl) return;
/* деление на две подсхемы по двум - пяти узлам */
 node_n(n,matr,act_n,act,&p1);
 if (flag_fsn && p1 > flag_fsn)
   {
    bisec2(n,matr,act_n,act,&fl,range2);
    if (fl) return;
   }
 if (flag_fi3 && p1 > flag_fi3)
   {
    bisec3(n,matr,act_n,act,&fl,range3);
    if (fl) return;
   }
 if (flag_fi4  &&  p1 > flag_fi4)
   {
    bisec4(n,matr,act_n,act,&fl,range4);
    if (fl) return;
   }
 if (flag_fi5 && p1 > flag_fi5)
   {
    bisec5(n,matr,act_n,act,&fl,range5);
    if (fl) return;
   }
 extract0=extract;
beg:
 if (fl == 1 || fl == 2) return; /* det схемы != 0 || det = 0 */
/* выделение элементов с образованием двух производных схем */
 if (!extract0 && yesg(n,matr))
   { /* выделение проводимости */
    hangpas(n,matr,act_n,act,&first);
    if (first==-1)  
    {if (!flag_opt) first=0; else choiceg(n,matr,act_n,act,&first);}
    dl2=leng+strlen(c);
    printa(matr[first].reb,matr[first].kol,0);
    dl=leng+strlen(c);
    strcat(b++,"*");
 //   dl1=leng+strlen(c);
    s1=matr[first].v1; s2=matr[first].v2;
    frmatr(first,&n,matr);
    n1=n; act1_n=act_n;
    if (n1)
      {
       a1=(PASSIVE *) malloc(n1*sizeof(PASSIVE));
       pascopy(n1,matr,a1);
      }
    act1=(SOURCE *) malloc ((act1_n+1)*sizeof(SOURCE));
    if (act1_n) actcopy(act1_n,act,act1);
    nui(s1,s2,&act1_n,act1);
    if (n1+act1_n > 1) strcat(b++,"(");
    dl1=leng+strlen(c);
    gggf(n1,a1,act1_n,act1);
    if (n1) free(a1); free(act1);
    if (dl1>=leng+strlen(c)) ster(dl2);
    else
      {
       if (n1+act1_n > 1) strcat(b++,")");
       dl=leng+strlen(c);
       strcat(b++,"+");
      }
    node_n(n,matr,act_n,act,&p2);
    dl1=leng+strlen(c);
    if (p2 == p1)
      {
       gggf(n,matr,act_n,act);
       if (dl1 >= leng+strlen(c)) ster(dl);
      }
    else
      {
       ster(dl);
       frematr(n,matr); freact(act_n,act);
      }
    return;
   } /* end g-extraction */
 else
 if (!extract0 && yesr(n,matr))
   { /* выделение сопротивления */
    choicer(n,matr,act_n,act,&first);
    dl2=leng+strlen(c);
    printa(matr[first].reb,matr[first].kol,0);
    dl=leng+strlen(c);
    strcat(b++,"*");
  //  dl1=leng+strlen(c);
    s1=matr[first].v1; s2=matr[first].v2;
    frmatr(first,&n,matr);
    n1=n; act1_n=act_n;
       if (n1)
	 {
	  a1=(PASSIVE *) malloc(n1*sizeof(PASSIVE));
	  pascopy(n1,matr,a1);
	 }
       if (act1_n)
	 {
	  act1=(SOURCE *) malloc (act1_n*sizeof(SOURCE));
	  actcopy(act1_n,act,act1);
	 }
    node_n(n1,a1,act1_n,act1,&p2);
    if ((n1 || act1_n) && p2 == p1)
      {

       if (n1+act1_n > 1) strcat(b++,"(");
       dl1=leng+strlen(c);
       gggf(n1,a1,act1_n,act1);
       if (n1) free(a1); if (act1_n) free(act1);
       if (dl1>=leng+strlen(c)) ster(dl2);
       else
	 {
	  if (n1+act1_n > 1) strcat(b++,")");
	  dl=leng+strlen(c);
	  strcat(b++,"+");
	 }
      }
    else
      {
       ster(dl2);
//       freematr(n1,a1); freeact(act1_n,act1);
      }
    act1=(SOURCE*) malloc((act_n+1)*sizeof(SOURCE));
    actcopy(act_n,act,act1);
    nui(s1,s2,&act_n,act1);
    dl1=leng+strlen(c);
    gggf(n,matr,act_n,act1);
    free(act1);
    if (dl1 >= leng+strlen(c)) ster(dl);
    freact(act1_n,act);
    return;
   }  /* end r-extraction */
 else
 if (extract0 && noideal(act_n,act,&first))
   { /* выделение управляемых источников */
   char type;

   type=act[first].reb[0];
    if (n)
      {
       a1=(PASSIVE *) malloc(n*sizeof(PASSIVE));
       pascopy(n,matr,a1);
      }
    act1_n=act_n;
    act1=(SOURCE *) malloc (act1_n*sizeof(SOURCE));
    actcopy(act1_n,act,act1);
    act[first].kol=-act[first].kol;
    if (act[first].kol==-999) ntr999(&act_n,act);
    dl=leng+strlen(c);
    if (type == 'K' || type == 'B')
      {
       fl=0;
       if (!n && act_n==1) {strcat(b++,"("); fl=1;}
      }
    dl1=leng+strlen(c);
    gggf(n,matr,act_n,act);
    if (dl1 >= leng+strlen(c)) ster(dl);
    dl=leng+strlen(c);
    if (dl1 < leng+strlen(c)) strcat(b++,"+");
    if (type == 'K' || type == 'B')
      {
       if (fl)
	 {
	  strcat(b,"1)"); b+=2;
	  freematr(n,a1); freeact(act1_n,act1);
	  return;
	 }
      }
    autontr999(first,&act1_n,act1,&fl);
    if (fl == 1 || fl == 3)
      {
       dl1=leng+strlen(c);
       gggf(n,a1,act1_n,act1);
       if (n) free(a1); if (act1_n) free(act1);
       if (dl1>=leng+strlen(c)) ster(dl);
      }
    else
      {
       ster(dl);
       freematr(n,a1); freeact(act1_n,act1);
      }
    return;
   } /* end active */
 else
   {
    if (extract0 == 0) extract0=1;
    else if (extract0 == 1) extract0=0;
    { mirror(act_n,act);
      multipl(&n,matr,&act_n,act,&fl);
      goto beg;}
   }
}
 void mirror(int act_n,SOURCE *act)
{
 int i;
 char s1,s2,s3,s4,s0;

 for (i=0;i<act_n;i++)
    {
     if (act[i].reb[0]=='M')
       {
	if (act[i].v3!='0' && act[i].v4!='0')
	  {
	   if (   act[i].v1=='0' && act[i].v2!='0'
	       || act[i].v1!='0' && act[i].v2=='0' )
	     {
	      act[i].reb[0]='Q';
	      if (act[i].v1=='0') {act[i].v1=act[i].v2; act[i].v2='0';}
	     }
	  }
	else
	if (act[i].v1!='0' && act[i].v2!='0')
	  {
	   if (   act[i].v3=='0' && act[i].v4!='0'
	       || act[i].v3!='0' && act[i].v4=='0' )
	     {
	      act[i].reb[0]='T';
	      if (act[i].v3=='0') {act[i].v3=act[i].v4; act[i].v4='0';}
	     }
	  }
	else
	if (   act[i].v1=='0' && act[i].v2!='0'
	    && act[i].v3=='0' && act[i].v4!='0'
	    ||
	       act[i].v1!='0' && act[i].v2=='0'
	    && act[i].v3!='0' && act[i].v4=='0'
	    ||
	       act[i].v1=='0' && act[i].v2!='0'
	    && act[i].v3!='0' && act[i].v4=='0'
	    ||
	       act[i].v1!='0' && act[i].v2=='0'
	    && act[i].v3=='0' && act[i].v4!='0')
	     {
	      act[i].reb[0]='1';
	      if (act[i].v1=='0') {act[i].v1=act[i].v2; act[i].v2='0';}
	      if (act[i].v3=='0') {act[i].v3=act[i].v4; act[i].v4='0';}
	     }
       }
     else
     if (act[i].reb[0]=='Q')
       {
	if (   act[i].v3!='0' && act[i].v4=='0'
	    || act[i].v3=='0' && act[i].v4!='0')
	  {
	   act[i].reb[0]='1';
	   if (act[i].v3=='0') {act[i].v3=act[i].v4; act[i].v4='0';}
	  }
       }
     else
     if (act[i].reb[0]=='T')
       {
	if (   act[i].v1!='0' && act[i].v2=='0'
	    || act[i].v1=='0' && act[i].v2!='0')
	  {
	   act[i].reb[0]='1';
	   if (act[i].v1=='0') {act[i].v1=act[i].v2; act[i].v2='0';}
	  }
       }
    }
}
/****************************************************/
void redum(int act_n,SOURCE *act)
{
 int i,j;
 char s1,s2,s3,s4,s0;

 for (i=0;i<act_n;i++)
    {
     if (   act[i].reb[0]=='M' || act[i].reb[0]=='Q')
       {
	for (j=0;j<act_n;j++)
	   {
	    if (i==j) continue;
	    if (   act[j].reb[0] == '1' || act[j].reb[0] == 'T')
	      {
	       if (   act[i].v3==act[j].v3 && act[j].v4=='0'
		   || act[i].v3==act[j].v4 && act[j].v3=='0')
		 {
		  if (act[i].reb[0]=='M') act[i].reb[0]='T';
		  else act[i].reb[0]='1';
		  act[i].v3=act[i].v4;
		  act[i].v4='0';
		  break;
		 }
	       else
		 if (   act[i].v4==act[j].v4 && act[j].v3=='0'
		     || act[i].v4==act[j].v3 && act[j].v4=='0')
		   {
		    if (act[i].reb[0]=='M') act[i].reb[0]='T';
		    else act[i].reb[0]='1';
		    act[i].v4='0';
		    break;
		   }
	      }
	   }
       }
    }
 for (i=0;i<act_n;i++)
    {
     if (   act[i].reb[0]=='M' || act[i].reb[0]=='T')
       {
	for (j=0;j<act_n;j++)
	   {
	    if (i==j) continue;
	    if (   act[j].reb[0] == '1' || act[j].reb[0] == 'Q')
	      {
	       if (   act[i].v1==act[j].v1 && act[j].v2=='0'
		   || act[i].v1==act[j].v2 && act[j].v1=='0')
		 {
		  if (act[i].reb[0]=='M') act[i].reb[0]='Q';
		  else act[i].reb[0]='1';
		  act[i].v1=act[i].v2;
		  act[i].v2='0';
		  break;
		 }
	       else
		 if (   act[i].v2==act[j].v2 && act[j].v1=='0'
		     || act[i].v2==act[j].v1 && act[j].v2=='0')
		   {
		    if (act[i].reb[0]=='M') act[i].reb[0]='Q';
		    else act[i].reb[0]='1';
		    act[i].v2='0';
		    break;
		   }
	      }
	   }
       }
    }
}
/******************** Выделение z-петли ********************/
 int mqtloop(int act_n,SOURCE *act,
	     char *s1,char *s2,char **t,int *numb,int *sign)
{
 int i,ind=0,ipr=0;
 char *w,w0[20];

 *sign=0;
 w0[0]='\0'; w=w0;
rep:
 ipr++;
 for (i=0;i<act_n;i++)
    {
     if (   (act[i].reb[0]=='M' && act[i].kol < 0
	 || act[i].reb[0]=='T' && act[i].kol < 0)
	 && act[i].v1 == act[i].v2)
       {
	printf(" r_l2");
	strcat(w++,"2");
	*t=strdup(w0);
	*numb=1;
	*s1=act[i].v1; *s2=act[i].v2;
	*sign=0;
	if (act[i].reb[0]=='M') act[i].reb[0]='Q';
	else act[i].reb[0]='N';
	act[i].v2='0';
	ind=1;
	goto con;
       }
    }
con:
 for (i=0;i<act_n;i++)
    {
     if (  (act[i].reb[0]=='M' && act[i].kol < 0
	 || act[i].reb[0]=='Q' && act[i].kol < 0)
	 && act[i].v3 == act[i].v4)
       {
	if (ind)
	  {
	   printf(" r_l4");
	   w0[0]='4';
	  }
	else
	  {
	   printf(" r_l2"); strcat(w++,"2");
	  }
	*t=strdup(w0);
	*numb=1;
	*s1=act[i].v3; *s2=act[i].v4;
	*sign=0;
	if (act[i].reb[0]=='M') act[i].reb[0]='T';
	else act[i].reb[0]='N';
	act[i].v4='0';
	ind=1;
       }
    }
 if (ipr<5) goto rep;
 if (ind) return(1); else return(0);
}



