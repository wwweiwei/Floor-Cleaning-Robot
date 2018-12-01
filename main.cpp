#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#define INF 20000000

using namespace std;

int m, n, length;
vector<vector<int>> pre;
vector<vector<int>> array_2D;
vector<vector<int>> map1,map2; //map2 for temporary memory
queue<vector<int>>  queue1;
vector<vector<int>> list1;

void FindMax(int *,int *,int *);
void AddQue(int,int,int);
void DelQue(int *, int *, int *);
int EmptyQue(void);

void FindMax(int *pmax,int *px,int *py)
{
    *pmax=1;
    for(int i=1;i<=m;i++)
        for(int j=1;j<=n;j++)
           if (map1[i][j]>(*pmax)){
              *pmax=map1[i][j];
              *px=i; *py=j;
           }
}

void AddQue(int x, int y, int step)
{
        vector<int> q(3);
        q[0]=x;
        q[1]=y;
        q[2]=step;
        queue1.push(q);
}

void DelQue(int *px, int *py, int *pstep)
{
        vector<int> q;
        q=queue1.front();
        queue1.pop();
        (*px)=q[0];
        (*py)=q[1];
        (*pstep)=q[2];
}

std::vector<std::vector<int>> make_one(int rows, int cols) {
    return std::vector<std::vector<int>>(rows, std::vector<int>(cols));
}
int main(int argc, char *argv[])
{
    string path,filename;
    ifstream fp1; //input file stream
    ofstream fp2; //output file stream
    int i, j, x, y, step, max;
    long int TotalLine=0;
    char str[10];

    //check input arguments
    if( argc == 2)
    {
      path.assign( ".\\test_case\\");
      path += argv[1];
    }
    else
      path=".";
    filename = path + "\\floor.data";
    fp1.open(filename);
    filename = path + "\\final.path";
    fp2.open(filename);

    fp1>>m>>n>>length;

    map1 = make_one(m+2,n+2);
    pre.assign(m+2, std::vector<int>(n+2, 5)); // initially fill pre with 5
    // read map from data file
    for (j=0;j<=n+1;j++){ map1[0][j]=-1; map1[m+1][j]=-1; } // set up/down border
    for (i=0;i<=m+1;i++){ map1[i][0]=-1; map1[i][n+1]=-1; } // set left/right border
    for (i=1;i<=m;i++)
        for (j=1;j<=n;j++){
            fp1>>str;
            if (str[0]=='0') map1[i][j]=INF;
            else if (str[0]=='1') map1[i][j]=-1;
            else if (str[0]=='R'){ map1[i][j]=INF; x=i; y=j; }
            else cout <<"read error\n";
    }
    map2.assign(map1.begin(), map1.end()); //copy map1 to map2

    int org_x=x; int org_y=y;

    max=length*2; //enough large max
    x=org_x; y=org_y;
    map1.assign(map2.begin(), map2.end()); //copy map2 to map1

    int ok_mask=15; // 1+2+4+8
    max=length*2; //enough large max
    int count1=0;
    while ((max-1)*2>length && count1<4)
    {
        count1++;
        x=org_x; y=org_y;
        map1.assign(map2.begin(), map2.end()); //copy map2 to map1

        // find shortest path
        AddQue(x,y,1); map1[x][y]=1;
        while (!queue1.empty()) {
           DelQue(&x,&y,&step);
           if (step==1) {
               if (map1[x][y-1]==INF && (ok_mask&1)){ AddQue(x, y-1, step+1); map1[x][y-1]=step+1; pre[x][y-1]=0; ok_mask&=14;}
               else if (map1[x-1][y]==INF && (ok_mask&2)){ AddQue(x-1, y, step+1); map1[x-1][y]=step+1; pre[x-1][y]=1; ok_mask&=13;}
               else if (map1[x][y+1]==INF && (ok_mask&4)){ AddQue(x, y+1, step+1); map1[x][y+1]=step+1; pre[x][y+1]=2; ok_mask&=11;}
               else if (map1[x+1][y]==INF && (ok_mask&8)){ AddQue(x+1, y, step+1); map1[x+1][y]=step+1; pre[x+1][y]=3; ok_mask&=7;}
           }
           else {
               if (map1[x][y-1]==INF){ AddQue(x, y-1, step+1); map1[x][y-1]=step+1; pre[x][y-1]=0; }
               if (map1[x-1][y]==INF){ AddQue(x-1, y, step+1); map1[x-1][y]=step+1; pre[x-1][y]=1; }
               if (map1[x][y+1]==INF){ AddQue(x, y+1, step+1); map1[x][y+1]=step+1; pre[x][y+1]=2; }
               if (map1[x+1][y]==INF){ AddQue(x+1, y, step+1); map1[x+1][y]=step+1; pre[x+1][y]=3; }
           }
        }
        FindMax(&max,&x,&y);
    cout << "find max:"<<max <<" length="<<length<<endl;

    }

/*
   for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=n;j++)
            cout << map1[i][j] <<"\t";
        cout << endl;
    }
*/

    while (max!=1){
        list1.clear();
        step=map1[x][y];
        for (i=1;i<=step;i++) {
            vector<int> temp2(2);
            temp2[0]=x;
            temp2[1]=y;
            list1.push_back(temp2); //farest ....R
            if (pre[x][y]==0) y++;
            else if (pre[x][y]==1) x++;
            else if (pre[x][y]==2) y--;
            else x--;
        }

          // walk from 1....farest
          for (i=step-2;i>=0;i--) {
                x=list1[i][0];
                y=list1[i][1];
                vector<int> temp(2);
                temp[0]=x-1;
                temp[1]=y-1;
                array_2D.push_back(temp);
                TotalLine++;
                map1[x][y]=0;
          }

          // walk from farest...R
          for (i=1;i<=step-1;i++) {
                x=list1[i][0];
                y=list1[i][1];
                vector<int> temp(2);
                temp[0]=x-1;
                temp[1]=y-1;
                array_2D.push_back(temp);
                TotalLine++;
          }
          FindMax(&max,&x,&y);
    }

    fp2<<TotalLine<<endl;
    for(i=0;i<array_2D.size();i++){
        fp2<<array_2D[i][0]<<" ";
        fp2<<array_2D[i][1]<<endl;
    }

    fp1.close();
    fp2.close();

    return 0;
}
