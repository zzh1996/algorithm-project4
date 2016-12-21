#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define INF 10000000
#define NIL -1

int w[729][729]; //原矩阵
int d[729][729]; //FW算法使用的D矩阵
int pi[729][729]; //FW算法使用的π矩阵
int test[5][5]={ //书上的测试数据
	{0,3,8,INF,-4},
	{INF,0,INF,1,7},
	{INF,4,0,INF,INF},
	{2,INF,-5,0,INF},
	{INF,INF,INF,6,0},
};

void generate(int n,int n_edge){ //随机生成数据
	int i,j,a,b,weight;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(i==j)
				w[i][j]=0;
			else
				w[i][j]=INF;
		}
	}
	for(i=0;i<n_edge;i++){
		do{
			a=rand()%n;
			b=rand()%n;
			weight=rand()%39-9; //(-10,30)
		}while(w[a][b]!=INF);
		w[a][b]=weight;
	}
}

void regenerate(int n){ //对负的权值重新生成
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(w[i][j]<0)
				w[i][j]=rand()%39-9;
		}
	}
}

void fw(int n){ //FW算法
	int i,j,k;
	for(i=0;i<n;i++){ //初始化π矩阵
		for(j=0;j<n;j++){
			if(i==j||w[i][j]==INF)
				pi[i][j]=NIL;
			else
				pi[i][j]=i;
		}
	}
	memcpy(d,w,sizeof(d)); //初始化D矩阵
	for(k=0;k<n;k++){
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(d[i][k]!=INF&&d[k][j]!=INF&&d[i][k]+d[k][j]<d[i][j]){ //算法核心部分
					d[i][j]=d[i][k]+d[k][j]; //更新D矩阵
					pi[i][j]=pi[k][j]; //更新π矩阵
				}
			}
		}
	}
}

void print(int g[729][729],int n){ //输出矩阵，用于调试
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(g[i][j]!=INF)
				printf("%d\t",g[i][j]);
			else
				printf("INF\t");
		}
		printf("\n");
	}
}

int is_invalid(int n){ //判断D矩阵对角线是否有负数
	int i;
	for(i=0;i<n;i++){
		if(d[i][i]<0)
			return 1;
	}
	return 0;
}

void print_shortest_path(FILE *fp,int i,int j){ //输出最短路径
	if(i==j)
		fprintf(fp,"%d,",i);
	else if(pi[i][j]==NIL)
		fprintf(fp,"No path from %d to %d exists\n",i,j);
	else{
		print_shortest_path(fp,i,pi[i][j]);
		fprintf(fp,"%d,",j);
	}
}

int main(){
	int i,j,k,n=3,nlog3n;
	FILE *fp;
	char filename[100];
	double diff;
	struct timespec start,stop;
	srand(time(0));
	for(i=0;i<5;i++){
		n*=3;
		nlog3n=n*(i+2);
		generate(n,nlog3n); //生成数据
		//printf("Graph n=%d\n",n);
		//print(w,n);
		fw(n);
		//printf("D=\n");
		//print(d,n);
		while(is_invalid(n)){ //如果有负权重的环路则重新生成
			printf("Regenerating n=%d\n",n);
			regenerate(n);
			//printf("Graph n=%d\n",n);
			//print(w,n);
			fw(n);
			//printf("D=\n");
			//print(d,n);
		}
		/*n=5;
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				w[j][k]=test[j][k];
			}
		}*/
		sprintf(filename,"../Input/size%d/input.txt",i+1);
		fp=fopen(filename,"w");
		for(j=0;j<n;j++){ //保存原始数据作为输入
			for(k=0;k<n;k++){
				if(j!=k&&w[j][k]!=INF){
					fprintf(fp,"%d,%d %d\n",j,k,w[j][k]);
				}
			}
		}
		fclose(fp);

		clock_gettime(CLOCK_MONOTONIC,&start); //开始计时
		fw(n);
		clock_gettime(CLOCK_MONOTONIC,&stop); //结束计时
		//print(pi,n);
		//print(d,n);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec)/1000000000L;
		sprintf(filename,"../Output/size%d/time2.txt",i+1);
		fp=fopen(filename,"w");
		fprintf(fp,"%.9fs\n",diff);
		fclose(fp);

		sprintf(filename,"../Output/size%d/output2.txt",i+1);
		fp=fopen(filename,"w");
		for(j=0;j<n;j++){ //输出结果
			for(k=0;k<n;k++){
				fprintf(fp,"From %d to %d: ",j,k);
				if(d[j][k]==INF){
					fprintf(fp,"d=INF\n");
				}else{
					fprintf(fp,"d=%d\n",d[j][k]);
					fprintf(fp,"(");
					print_shortest_path(fp,j,k);
					fseek(fp,-1,SEEK_CUR); //回退掉最后一个逗号
					fprintf(fp,")\n");
				}
			}
		}
		fclose(fp);
	}
	return 0;
}