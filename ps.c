#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define N 1536
int M; int (*C)[5]; int *occ[2*N+2]; int nocc[2*N+2]; int *ns; int *fl; int nf=0; int *pos; char a[N+1];
int lit_true(int l){return l>0? a[l]:!a[-l];}
int idx(int l){return l>0? 2*l: 2*(-l)+1;}
int main(int argc,char**argv){ int seed=atoi(argv[1]); srand(seed); int lim=atoi(argv[3]); time_t t0=time(0); FILE*f=fopen("cnf.txt","r"); int nv; fscanf(f,"p cnf %d %d",&nv,&M); C=malloc(sizeof(int[5])*M);
 for(int i=0;i<M;i++){for(int j=0;j<5;j++)fscanf(f,"%d",&C[i][j]); int z; fscanf(f,"%d",&z); for(int j=0;j<5;j++)nocc[idx(C[i][j])]++;}
 for(int k=0;k<2*N+2;k++){occ[k]=malloc(sizeof(int)*(nocc[k]+1)); nocc[k]=0;}
 for(int i=0;i<M;i++)for(int j=0;j<5;j++){int k=idx(C[i][j]);occ[k][nocc[k]++]=i;}
 ns=calloc(M,4); fl=malloc(4*M); pos=malloc(4*M);
 /* majority init */
 for(int v=1;v<=N;v++) a[v]= (seed%2==0)? (nocc[2*v]>=nocc[2*v+1]) : (rand()&1);
 for(int i=0;i<M;i++){int s=0;for(int j=0;j<5;j++)s+=lit_true(C[i][j]); ns[i]=s; if(!s){pos[i]=nf;fl[nf++]=i;}}
 double cb=3.4+0.1*(seed%8); double pr[64]; for(int b=0;b<64;b++)pr[b]=pow(cb,-b);
 long steps=0; int best=nf;
 while(nf>0){ int c=fl[rand()%nf]; double p[5],sum=0;
  for(int j=0;j<5;j++){int v=abs(C[c][j]); int fal= a[v]? 2*v:2*v+1; /* lit currently true for var: becomes false */
   int brk=0; int k=fal; for(int t=0;t<nocc[k];t++) if(ns[occ[k][t]]==1) brk++; p[j]=pr[brk<63?brk:63]; sum+=p[j];}
  double r=(double)rand()/RAND_MAX*sum; int j=0; for(;j<4;j++){r-=p[j]; if(r<=0)break;}
  int v=abs(C[c][j]); int was= a[v]? 2*v:2*v+1; int now= a[v]? 2*v+1:2*v; a[v]=!a[v];
  for(int t=0;t<nocc[was];t++){int q=occ[was][t]; if(--ns[q]==0){pos[q]=nf;fl[nf++]=q;}}
  for(int t=0;t<nocc[now];t++){int q=occ[now][t]; if(ns[q]++==0){int pp=pos[q]; int last=fl[--nf]; fl[pp]=last; pos[last]=pp;}}
  steps++; if(nf<best){best=nf;} if((steps&1048575)==0 && time(0)-t0>lim){printf("BEST %d seed %d cb %.1f steps %ld\n",best,seed,cb,steps);return 1;}
 }
 FILE*o=fopen(argv[2],"w"); for(int v=1;v<=N;v++)fputc(a[v]?'1':'0',o); fclose(o); printf("SOLVED %ld\n",steps); return 0;}
