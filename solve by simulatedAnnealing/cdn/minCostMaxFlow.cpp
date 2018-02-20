#include <iostream>
#include <set>
#include <iterator>
#include "cdn.h"

using namespace std;

/*
@���������ghͼ�ṹ
server �������ڵ�
*/
long minCostMaxFlow(SparseGraph &gh, set<int> &server)
{
    long sumTotal = 0;
    ford_fulkerson d(gh, gh.V() - 1, gh.V() - 2, gh.totalRequire);
    sumTotal = d.sumCost();
    sumTotal += gh.serverCost * server.size();
    if(!d.reachOK()){
	sumTotal = MAXCOST;        //
    }
    return sumTotal;
}
