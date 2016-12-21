#include <stdio.h>
#include <malloc.h>
#include <time.h>

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
	struct edge *adj; //邻接表的形式
};

struct vertex v[729]; //原图
struct vertex vt[729]; //转置后的图
int ordered_vertex[729]; //in order of decreasing u.f
int ordered_vertex_p;
int sccs[729*2]; //用于保存强连通分量的结果
int sccs_p;

int time_;

void dfs_visit(int i,int n){
	struct edge *p;
	time_++;
	v[i].d=time_;
	v[i].color=GRAY;
	for(p=v[i].adj;p;p=p->next){
		if(v[p->v].color==WHITE){
			v[p->v].pi=i;
			dfs_visit(p->v,n);
		}
	}
	v[i].color=BLACK;
	time_++;
	v[i].f=time_;
	ordered_vertex[ordered_vertex_p]=i; //结束时按序保存在ordered_vertex以供下次DFS使用。否则对u.f排序，时间复杂度就会变大
	ordered_vertex_p--;
}

void dfs(int n){
	int i;
	for(i=0;i<n;i++){
		v[i].color=WHITE;
		v[i].pi=NIL;
	}
	time_=0;
	ordered_vertex_p=n-1;
	for(i=0;i<n;i++){
		if(v[i].color==WHITE){
			dfs_visit(i,n);
		}
	}
}

void dfs_visit2(int i,int n){ //对转置图的DFS
	struct edge *p;
	sccs[sccs_p++]=i; //保存结果
	time_++;
	vt[i].d=time_;
	vt[i].color=GRAY;
	for(p=vt[i].adj;p;p=p->next){
		if(vt[p->v].color==WHITE){
			vt[p->v].pi=i;
			dfs_visit2(p->v,n);
		}
	}
	vt[i].color=BLACK;
	time_++;
	vt[i].f=time_;
}

void dfs2(int n){
	int i;
	for(i=0;i<n;i++){
		vt[i].color=WHITE;
		vt[i].pi=NIL;
	}
	time_=0;
	sccs_p=0;
	for(i=0;i<n;i++){
		if(vt[ordered_vertex[i]].color==WHITE){
			dfs_visit2(ordered_vertex[i],n);
			sccs[sccs_p++]=-1; //分隔DFS森林中不同的树
		}
	}
}

void scc(int n){
	struct edge *p,*q;
	int i;
	dfs(n);
	for(i=0;i<n;i++){ //求转置图
		for(p=v[i].adj;p;p=p->next){
			q=malloc(sizeof(struct edge));
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
	double diff;
    struct timespec start,stop;
	for(i=0;i<5;i++){
		n*=3;
		for(j=0;j<n;j++){ //初始化
			v[j].d=vt[j].d=-1;
			v[j].pi=vt[j].pi=-1;
			v[j].color=vt[j].color=-1;
			v[j].f=vt[j].f=-1;
			v[j].adj=vt[j].adj=NULL;
		}
		sprintf(filename,"../Input/size%d/input.txt",i+1);
		fp=fopen(filename,"r");
		while(fscanf(fp,"%d,%d",&a,&b)>0){ //读入数据
			struct edge *p=malloc(sizeof(struct edge));
			p->v=b;
			p->next=v[a].adj;
			v[a].adj=p;
		}
		fclose(fp);

		clock_gettime(CLOCK_MONOTONIC,&start); //开始计时
		scc(n);
		clock_gettime(CLOCK_MONOTONIC,&stop); //结束计时
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec)/1000000000L;
		sprintf(filename,"../Output/size%d/time1.txt",i+1);
		fp=fopen(filename,"w");
        fprintf(fp,"%.9fs\n",diff);
        fclose(fp);

        sprintf(filename,"../Output/size%d/output1.txt",i+1);
        fp=fopen(filename,"w");
		fprintf(fp,"(");
		for(j=0;j<sccs_p-1;j++){ //写入结果
			if(sccs[j]<0){
				fprintf(fp,")\n(");
			}else{
				fprintf(fp,"%d",sccs[j]);
				if(sccs[j+1]>=0){
					fprintf(fp,",");
				}
			}
		}
		fprintf(fp,")\n\n");
		fclose(fp);
	}
	return 0;
}