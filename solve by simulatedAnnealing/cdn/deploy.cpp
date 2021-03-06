#include <string.h>
#include "cdn.h"

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num, char * filename)
{
    //构建一个稀疏图对象
    SparseGraph g;     
    //读取txt中数据并一一赋值给g对象
    ReadGraph(g,topo); 
    //针对不同规模设置初始的收敛度
    float RANDX, RANDY, RANDZ; //产生随机数，用于选择三个情况选择
    int time1, time2;//初始和二次退火迭代次数
    float  at;       //退火速率
    if(g.networkNodeNum < 200)
    {
	time1 = -1;
	time2 = 15000;
	RANDX = 0.05;
	RANDY = 0.33;
	RANDZ = 0.67;
	at    = 0.98;
    }
    else if(g.networkNodeNum < 500)
    {
	time1 = -1;
	time2 = 15000;
	RANDX = 0.05;
	RANDY = 0.33;
	RANDZ = 0.67;
	at    = 0.98;
    }
    else
    {
	time1 = 1000;
	time2 = 30000;
	RANDX = 0.13;
	RANDY = 0.5;
	RANDZ = 0.5;
	at    = 0.9;
    }
    //启发式搜索
    simulatedAnneal(g,time1,time2,RANDX, RANDY, RANDZ, at); 
    //保存结果
    g.saveUsedpath();
    //将结果写入保存输出变量里面
    char topo_file[100000];
    sprintf(topo_file, "%d\n\n", g.pathCount);
    for (int i = 0; i < g.pathCount; i++){
	for (int j = 0; j < g.path[i].size(); j++){
	    char str[5];
	    sprintf(str, "%d ", g.path[i][j]);
	    strcat(topo_file, str);
	}
	strcat(topo_file, "\n");
    }
    write_result(topo_file, filename);
}

