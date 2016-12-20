#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int g[729][729];

int main(){
	int i,j,k,n=3,nlog3n,a,b;
	FILE *fp;
	char filename[100];
	srand(time(0));
	for(i=0;i<5;i++){
		n*=3;
		nlog3n=n*(i+2);
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				g[j][k]=0;
			}
		}
		sprintf(filename,"size%d/input.txt",i+1);
		fp=fopen(filename,"w");
		for(j=0;j<nlog3n;j++){
			do{
				a=rand()%n;
				b=rand()%n;
			}while(g[a][b]);
			g[a][b]=1;
			fprintf(fp,"%d,%d\n",a,b);
		}
		fclose(fp);
	}
	return 0;
}