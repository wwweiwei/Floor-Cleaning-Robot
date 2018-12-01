#include <stdio.h>
#include <stdlib.h>
#define INF 9999

// global variable
int m, n, length, map[102][102];
unsigned char pre[102][102];
void FindMax(int *,int *,int *);

// for queue
#define MaxQueueSize 400
int queue[MaxQueueSize][3], list[MaxQueueSize][2];
void AddQue(int,int,int);
void DelQue(int *, int *, int *);
int EmptyQue(void);
int front=-1, rear=-1;

int main(void)
{
    FILE *fp1, *fp2;   
    int i, j, x, y, step, max;
    long int TotalLine=0;
    char str[10];
    
    fp1=fopen("map.txt","r");
    fp2=fopen("output.txt","w");
    
    fscanf(fp1,"%d %d %d",&m,&n,&length);
    fprintf(fp2,"%5ld\n",99999); // when finished, rewrite this data into file
    
    // read map from data file
    for (j=0;j<=n+1;j++){ map[0][j]=-1; map[m+1][j]=-1; } // set up/down border
    for (i=0;i<=m+1;i++){ map[i][0]=-1; map[i][n+1]=-1; } // set left/right border
    for (i=1;i<=m;i++) 
    for (j=1;j<=n;j++){
            fscanf(fp1,"%s",&str);
            if (str[0]=='0') map[i][j]=INF;
            else if (str[0]=='1') map[i][j]=-1;
            else if (str[0]=='R'){ map[i][j]=INF; x=i; y=j; }
            else printf("read error\n");
    } 
      
    // find shortest path  
    AddQue(x,y,1); map[x][y]=1; fprintf(fp2,"%d %d\n",x,y); TotalLine++;
    while (EmptyQue()==0) {
       DelQue(&x,&y,&step);
       if (map[x][y-1]==INF){ AddQue(x, y-1, step+1); map[x][y-1]=step+1; pre[x][y-1]=0; }
       if (map[x-1][y]==INF){ AddQue(x-1, y, step+1); map[x-1][y]=step+1; pre[x-1][y]=1; }
       if (map[x][y+1]==INF){ AddQue(x, y+1, step+1); map[x][y+1]=step+1; pre[x][y+1]=2; }
       if (map[x+1][y]==INF){ AddQue(x+1, y, step+1); map[x+1][y]=step+1; pre[x+1][y]=3; }
    }  
    
    FindMax(&max,&x,&y);
    while (max!=1){
          step=map[x][y];
          for (i=1;i<=step;i++) {
              list[i][0]=x; list[i][1]=y;  
              if (pre[x][y]==0) y++;
              else if (pre[x][y]==1) x++;
              else if (pre[x][y]==2) y--;
              else x--;
          }
          // walk forward
          for (i=step-1;i>=1;i--) { 
              x=list[i][0]; y=list[i][1];
              fprintf(fp2,"%d %d\n",x,y); TotalLine++;
              map[x][y]=0;
          }
          // walk backward
          for (i=2;i<=step;i++) { 
              x=list[i][0]; y=list[i][1];
              fprintf(fp2,"%d %d\n",x,y); TotalLine++;
          }
          FindMax(&max,&x,&y);
    }
    rewind(fp2); fprintf(fp2,"%5ld\n",TotalLine);
    
    fclose(fp1); fclose(fp2); //getchar();
    return(0);
}

void FindMax(int *pmax,int *px,int *py)
{
    *pmax=1;
    for(int i=1;i<=m;i++) 
    for(int j=1;j<=n;j++) 
       if (map[i][j]>(*pmax)){
          *pmax=map[i][j];
          *px=i; *py=j;
       }
}

void AddQue(int x, int y, int step)
{
     rear=(rear+1+MaxQueueSize)%MaxQueueSize;
     if (front==rear) printf("Queue Full.\n");
     else {
        queue[rear][0]=x; 
        queue[rear][1]=y;
        queue[rear][2]=step;
     }
}
void DelQue(int *px, int *py, int *pstep)
{
     if (front==rear) printf("Queue Empty.\n");
     else {
        front=(front+1+MaxQueueSize)%MaxQueueSize;
        (*px)=queue[front][0];
        (*py)=queue[front][1];
        (*pstep)=queue[front][2];
     }
}

int EmptyQue(void){
    if (front==rear) return 1; else return 0;
}
