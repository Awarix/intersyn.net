#include <stdio.h>
#include <string.h>  
// #include <dos.h>
/* #include <windows.h> */
// #include <alloc.h>
// #include <conio.h>
#include <stdlib.h>
#include <math.h>

#define PASSIVE struct pas
#define SOURCE struct ist
#define GRAPH struct graph
#define DIAG struct diag

PASSIVE /* ������� ���寮����� ��⢥� �奬� */
{
 char v1; /* ���� 㧥� */
 char v2; /* ��ன 㧥� */
 int kol; /* ��⭮��� - ������⢮ ��⢥�, �室��� � ���寮��᭨� */
          /* ��� y-���寮��᭨�� - �� ������⢮ ��ࠫ������ ��⢥� */
          /* ��� z-���寮��᭨�� - �� ������⢮ ��᫥����⥫��� ��⢥� */
 char *reb; /* �����䨪��� ���寮��᭨�� (�⤥��� ᮯ�⨢����� ��� */
            /* �஢��������, �㬬� �஢������⥩ ��� ᮯ�⨢�����)     */
};
SOURCE /* ������� �ࠢ�塞�� ���筨��� �奬� */
{
 char v1; /* ��砫�� 㧥� ��⢨ ������� �� */
 char v2; /* ������ 㧥� ��⢨ ������� �� */
 char v3; /* ��砫�� 㧥� ��⢨ �ਥ����� �� */
 char v4; /* ������ 㧥� ��⢨ �ਥ����� �� */
 int kol; /* ��⭮��� - ������⢮ �⤥���� ��, ��ࠧ���� ����� �� */
          /* ��� ���� - �� ������⢮ ��ࠫ������ ����               */
          /* ��� ���� - � ������⢮ ��᫥����⥫��� ����            */
 char *reb;
};
GRAPH /* ������� ���, ������䭮�� �奬� */
{
 char v1; /* ���� 㧥� */
 char v2; /* ��ன 㧥� */
 int ess; /* ⨯ ��⢨ ��� �� ����� (��� ��� �奬� � ��) */
};
DIAG
{
 int nu; /* ����� ���������㥬�� ��⢨ */
 char *reb; /* �����䨪��� ���������㥬�� ��⢨ */
};

// void main(int argc, char* argv[]);

int yesgm(int n,PASSIVE *matr,int act_n,SOURCE *act,
	   int *first,int *fm,char *sm1,char *sm2, int *smf);
int yesgq(int n,PASSIVE *matr,int act_n,SOURCE *act,
	   int *first,int *fm);
int yesgt(int n,PASSIVE *matr,int act_n,SOURCE *act,
	   int *first,int *fm);
int yesm(int act_n,SOURCE *act,int *first);
int yesq(int act_n,SOURCE *act,int *first);
int yest(int act_n,SOURCE *act,int *first);


void parse_command_line(int argc, char* argv[]);

/* �������� ����� �奬�, �஢�ન ���४⭮��, ������� � �������⨪� */
void input(int *n,PASSIVE *matr,int *act_n,SOURCE *act,
           int *nmd,PASSIVE *matrd,int *actd_n,SOURCE *actd,
           int *dui,PASSIVE *diagui,int *dact,int *diag,
	   int *dva,DIAG *diagva,int *nej,int *da,int *ncL);
void detan(int n,PASSIVE *matr,int act_n,SOURCE *act);
void detanp(int n,PASSIVE *matr,int act_n,SOURCE *act,int ncL);
void cirfunpa(int n,PASSIVE *matr,int act_n,SOURCE *act,
	      PASSIVE *diagui,int ncL);
void cirfunst(int n,PASSIVE *matr,int act_n,SOURCE *act,
	      PASSIVE *diagui);
void analys(int n,PASSIVE *matr,int act_n,SOURCE *act,
	    int dui,PASSIVE *diagui);
void analysp(int n,PASSIVE *matri,int act_n,SOURCE *acti,
	     int dui,PASSIVE *diagui,int ncL);

/* �����ᨢ��� �������᪮� ࠧ������� �奬���� ��।���⥫� */
void gggf(int n,PASSIVE *matr,int act_n,SOURCE *act);
void multipl(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl);
void gggp(int n,PASSIVE *matr,int act_n,SOURCE *act,int pln);
void multiplp(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl,int pln);
void newtral(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl);
void allntr999(int *act_n,SOURCE *act);
void autontr999(int k,int *act_n,SOURCE *act,int *fl);
void ntr999(int *act_n,SOURCE *act);
int is999(int j,int act_n,SOURCE *act);
void one999(int act_n,SOURCE *act);

/* �㭪樨 � ����ணࠬ�� ��� �஢�ન ��஦����� � ��饭�� �奬� */
void simply(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *flag);
void triangl(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *flag);
void redu(int *n,PASSIVE *matr,int *act_n,SOURCE *act);
void reallf(int *n,PASSIVE *a1);
void parall(int *act_n,SOURCE *act);
void seqzz0(int *n,PASSIVE *a1,int act_n,SOURCE *act);
int pasloop(int *n,PASSIVE *matr,
	     char *s1,char *s2,char **t,int *numb,int *sign);
int redur(int *n,PASSIVE *matr,int act_n,SOURCE *act,
	  char *ss1,char *ss2,char **t,int *numb,int *sign);
int degener(int *act_n,SOURCE *act,
	  char *s1,char *s2,char **t,int *numb,int *sign);
int pashang(int *n,PASSIVE *matr,int act_n,SOURCE *act,
	    char *s1,char *s2,char **t,int *numb,int *sign);
int nodred(int n,PASSIVE *matr,int *act_n,SOURCE *act,
	   char  *s1,char *s2, char **t,int *numb,int *sign);
int nodreda(int n,int *act_n,SOURCE *act,
	   char  *s1,char *s2, char **t,int *numb,int *sign,int *flm);
void ejuihang(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl);
void actloop(int *act_n, SOURCE *act,int *fl);
void redpar(int *act_n,SOURCE *act, int *fl);
void redseq(int n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl);
int hanged(int n,PASSIVE *matr,int act_n,SOURCE *act);
int connec(int n,PASSIVE *matr,int act_n,SOURCE *act);
void hangr(int *n,PASSIVE *matr,int *act_n,SOURCE *act,int *fl);

/* �㭪樨 �뢮�� ������ */
void red2yy(int *n,PASSIVE *a1,int act_n,SOURCE *act);
void red2zz(int *n,PASSIVE *a1);
void seqzz(int *n,PASSIVE *a1,int act_n,SOURCE *act);
void reallyy(int *n,PASSIVE *a1);
void reallyz(int *n,PASSIVE *a1);
void seqzy(int *n,PASSIVE *a1,int act_n,SOURCE *act);

/* �ᯮ����⥫�� �㭪樨 � ����ணࠬ�� */
void extra(int el_n,char *el,int be,int *ee,char *sr,int *k);
void ster(unsigned long k);
void verstr(int n,GRAPH *matr,int *p,char *str);
void ver_n(int n,GRAPH *matr,int *p);
void nodestr(int n,PASSIVE *matr,int act_n,SOURCE *act,int *p,char *str);
void node_n(int n,PASSIVE *matr,int act_n,SOURCE *act,int *p);
void nodes(int act_n,SOURCE *act,int *p,char *str);
void node(int act_n,SOURCE *act,int *p);
void printa(char *t,int numb,int sign);
void printi(char **t,char *a,int numb);
int noideal(int act_n,SOURCE *act,int *first);
void copypas(int n,PASSIVE *matr,GRAPH *a1);
void actcopy(int act_n,SOURCE *act,SOURCE *act1);
void pascopy(int n,PASSIVE *matr,PASSIVE *matr1);
void copyact(int n,int act_n,SOURCE *act,int *na,GRAPH *a2);
void uniact(char s1,char s2,int *act_n,SOURCE *act);
void unipas(char s1,char s2,int *n,PASSIVE *matr);
void kontrol(void);
int minimum(int pow1,int pow2);
int maximum(int activ1,int activ2);
void hangpas(int n,PASSIVE *matr,int act_n,SOURCE *act,int *first);
void choiceg(int n,PASSIVE *matr,int act_n,SOURCE *act,int *first);
void choicer(int n,PASSIVE *matr,int act_n,SOURCE *act,int *first);
void nui(char s1,char s2,int *act_n,SOURCE *act);
void nuiall(char s1,char s2,char s3,char s4,int *act_n,SOURCE *act);
void nuintr(char s1,char s2,int k,SOURCE *act);
void actall(char s1,char s2,char s3,char s4,
	    int kol,char *str,int *act_n,SOURCE *act);
void matrall(char s1,char s2,int kol,char *str,int *n,PASSIVE *matr);
void freematr(int n, PASSIVE *matr);
void freeact(int act_n, SOURCE *act);
void frematr(int n, PASSIVE *matr);
void freact(int act_n, SOURCE *act);
void fract(int j,int *act_n,SOURCE *act);
void frmatr(int j,int *n,PASSIVE *matr);
 int yesgc(int n,PASSIVE *matr);
 int yesrL(int n,PASSIVE *matr);
 int yescL(int n,PASSIVE *matr);
int yesg(int n,PASSIVE *matr);
int yesr(int n,PASSIVE *matr);
int yess(int act_n,SOURCE *act);
int yesn(int act_n,SOURCE *act);
// float fabs(float x);
void bond1f(int n,GRAPH *a1,char s1,char s2,int *k);

/* ���������� �奬���� ��।���⥫� �� ���� - ��ᥪ�� �奬� */
void bond1(int n,GRAPH *a1,char s1,char s2,int *k);
void bond2(int n,GRAPH *a1,char s1,char s2,char s3,int *k);
void bond3(int n,GRAPH *a1,char s1,char s2,char s3,char s4,int *k);
void bond4(int n,GRAPH *a1,
	   char s1,char s2,char s3,char s4,char s5,int *k);
void bond5(int n,GRAPH *a1,
	   char s1,char s2,char s3,char s4,char s5,char s6,int *k);
int indep1(int nc,GRAPH *ac);
int indep2(int nc,GRAPH *ac,char *ac0,char s1,char s2,int *k);
int indep3(int nc,GRAPH *ac,
	   char *ac0,char s1,char s2,char s3,int *k);
int indep4(int nc,GRAPH *ac,
	   char *ac0,char s1,char s2,char s3,char s4,int *k);
int indep5(int nc,GRAPH *ac,
	    char *ac0,char s1,char s2,char s3,char s4,char s5,int *k);
void fortrans(int num,GRAPH *ac,int n,PASSIVE *matr,SOURCE *act,
	      int *n1,PASSIVE *a1,int *act1_n,SOURCE *act1,
	      int key,char *ac0);
void transfor(int n,PASSIVE *matr,
	      int act_n,SOURCE *act,int *num,GRAPH *ac);
void fortest(int num,GRAPH *ac,int *n1,int *act1_n,int key);

void form1(int num,GRAPH *ac,char *ac0,int n,PASSIVE *matr,
	   int act_n,SOURCE *act);
void form2(int num,GRAPH *ac,char *ac0,char s1,char s2,
	   int n,PASSIVE *matr,int act_n,SOURCE *act);
void form(int num,GRAPH *ac,char *ac0,int next,char *ext,
	   int n,PASSIVE *matr,int act_n,SOURCE *act);
void bisec1(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag);
void bisec2(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range);
void bisec1m(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag);
void bisec2m(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range);
void bisec3(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range);
void bisec4(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range);
void bisec5(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range);

/* ������ ��ᥪ�� �᫨⥫� � �������⥫� */
void formbeg(int num,GRAPH *ac,char *ac0,int next,char *ext,
	  int n,PASSIVE *matr,int act_n,SOURCE *act);
void bis2(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range,
	   int *num,GRAPH *ace,char *ac0,int *next,char *ext,int *kopt);
void bis3(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range,
	    int *num,GRAPH *ace,char *ac0,int *next,char *ext,int *kopt);
void bis4(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range,
	    int *num,GRAPH *ace,char *ac0,int *next,char *ext,int *kopt);
void bis5(int n,PASSIVE *matr,int act_n,SOURCE *act,int *flag,float range,
	    int *num,GRAPH *ace,char *ac0,int *next,char *ext,int *kopt);
int indep2m(int nc,GRAPH *ac,
	   char *ac0,char s1,char s2,int *k);
int indep3m(int nc,GRAPH *ac,
	    char *ac0,char s1,char s2,char s3,int *k);
int indep4m(int nc,GRAPH *ac,
	   char *ac0,char s1,char s2,char s3,char s4,int *k);
int indep5m(int nc,GRAPH *ac,
	    char *ac0,char s1,char s2,char s3,char s4,char s5,int *k);

void mirnul(char s1,char s2,char s3,char s4,int *act_n,SOURCE *act,char type);
void mirall(char s1,char s2,char s3,char s4,int *p,char *str,
	    int *n,PASSIVE *matr,int *act_n,SOURCE *act);
void qirall(char s1,char s2,char s3,char s4,int *p,char *str,
	    int *n,PASSIVE *matr,int *act_n,SOURCE *act);
void tirall(char s1,char s2,char s3,char s4,int *p,char *str,
	    int *n,PASSIVE *matr,int *act_n,SOURCE *act);
void mloop(int *act_n, SOURCE *act,int *fl);
int yeslm(int n,PASSIVE *matr,int act_n,SOURCE *act,
	    int *first,int *fm,char *sm1,char *sm2, int *smf);
int yeslq(int n,PASSIVE *matr,int act_n,SOURCE *act,
	    int *first,int *fm,char *sm1,char *sm2, int *smf);
int yeslt(int n,PASSIVE *matr,int act_n,SOURCE *act,
	    int *first,int *fm,char *sm1,char *sm2, int *smf);
int mtria(int *n,PASSIVE *matr,int act_n,SOURCE *act,int *fl);
void mirror(int act_n,SOURCE *act);
int mqtloop(int act_n,SOURCE *act,
	     char *s1,char *s2,char **t,int *numb,int *sign);
int nodredm(int n,PASSIVE *matr,int *act_n,SOURCE *act,
	    char  *s1,char *s2, char **t,int *numb,int *sign,int *flm);
int nodredg(int n,PASSIVE *matr,int *act_n,SOURCE *act,
	    char  *s1,char *s2, char **t,int *numb,int *sign,int *flm);
int yesmqt(int act_n,SOURCE *act);
void redum(int act_n,SOURCE *act);
