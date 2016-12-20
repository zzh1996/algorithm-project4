#include <stdio.h>
#include <malloc.h>

#define NIL -1

enum {WHITE,GRAY,BLACK};

struct edge{
	int v;
	struct edge *next;
};

struct vertex{
	int d;
	int pi;
	int color;
	int f;
	struct edge *adj;
};

struct vertex v[729];
struct vertex vt[729];
int ordered_vertex[729]; //in order of decreasing u.f
int ordered_vertex_p;
int sccs[729*2];
int sccs_p;

int time;

void dfs_visit(int i,int n){
	struct edge *p;
	time++;
	v[i].d=time;
	v[i].color=GRAY;
	for(p=v[i].adj;p;p=p->next){
		if(v[p->v].color==WHITE){
			v[p->v].pi=i;
			dfs_visit(p->v,n);
		}
	}
	v[i].color=BLACK;
	time++;
	v[i].f=time;
	ordered_vertex[ordered_vertex_p]=i;
	ordered_vertex_p--;
}

void dfs(int n){
	int i;
	for(i=0;i<n;i++){
		v[i].color=WHITE;
		v[i].pi=NIL;
	}
	time=0;
	ordered_vertex_p=n-1;
	for(i=0;i<n;i++){
		if(v[i].color==WHITE){
			dfs_visit(i,n);
		}
	}
}

void dfs_visit2(int i,int n){
	struct edge *p;
	sccs[sccs_p++]=i;
	time++;
	vt[i].d=time;
	vt[i].color=GRAY;
	for(p=vt[i].adj;p;p=p->next){
		if(vt[p->v].color==WHITE){
			vt[p->v].pi=i;
			dfs_visit2(p->v,n);
		}
	}
	vt[i].color=BLACK;
	time++;
	vt[i].f=time;
}

void dfs2(int n){
	int i;
	for(i=0;i<n;i++){
		vt[i].color=WHITE;
		vt[i].pi=NIL;
	}
	time=0;
	sccs_p=0;
	for(i=0;i<n;i++){
		if(vt[ordered_vertex[i]].color==WHITE){
			dfs_visit2(ordered_vertex[i],n);
			sccs[sccs_p++]=-1;
		}
	}
}

void scc(int n){
	int n;
	struct edge *p,*q;
	dfs(n);
	for(i=0;i<n;i++){
		for(p=v[i].adj;p;p=p->next){
			struct edge *q=malloc(sizeof(edge));
			q->v=i;
			q->next=vt[p->v].adj;
			vt[p->v].adj=q;
		}
	}
	dfs2(n);
}

int main(){
	int i,j,n=3,a,b;
	FILE *fp;
	char filename[100];
	for(i=0;i<5;i++){
		n*=3;
		for(j=0;j<n;j++){
			v[i].d=vt[i].d=-1;
			v[i].pi=vt[i].pi=-1;
			v[i].color=vt[i].color=-1;
			v[i].f=vt[i].f=-1;
			v[i].adj=vt[i].adj=NULL;
		}
		sprintf(filename,"../Input/size%d/input.txt",i+1);
		fp=fopen(filename,"r");
		while(fscanf(fp,"%d,%d",&a,&b)>0){
			struct edge *p=malloc(sizeof(edge));
			p->v=b;
			p->next=v[a].adj;
			v[a].adj=p;
		}
		fclose(fp);

		scc(n);

		printf("(")
		for(j=0;j<sccs_p-1;j++){
			if(sccs[sccs_p]<0){
				printf(")\n(");
			}else{
				printf("%d",sccs[sccs_p]);
			}
		}
		printf(")\n\n");
		
	}
	return 0;
}