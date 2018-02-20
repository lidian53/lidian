/*****************
FX 2017.3.26  ����ʱ��Ĳ���

�ڼ��㵱ǰ����������С���������֮ǰ�����Ѽ���Ϊ0
sumCur = minCostMaxFlow(gh, existNode);
��֮�����¼����ļ�����Ҳ����������ʱ��
ReadGraph(gh, "case6.txt");
Ŀǰ��ʼ�������ű���1000��
********************/
#include<set>
#include<iterator>
#include"cdn.h"

#define SIMULATE 10000
using namespace std;


/*
@ ģ���˻��㷨����
@ gh������ͼģ�ͣ�tģ���˻�ĳ�ʼ�¶ȣ�
@ at�˻�����ʣ�N ��С�ɽ��ܷ���������
*/
void simulatedAnneal(SparseGraph &gh, int iterTime1, int iterTime2, float RANDX, float RANDY, float RANDZ, float at)
{
    float  randX;
    long   sumCur;         //��ǰ������ܼ۸�
    set<int> existNode;    //��ǰѡ��ķ�����
    set<int> notWantedNode;//��������֮����������ڵ�
    set<int> copyExistNode;//������ǰ���������˻�ʧ�ܷ�����һ�η�����״̬
    int    count = 0;      //����ͳ��
    double totalTime=0;    //����ʱ��
    bool   isInitial = true;//�Ƿ������һ״̬�ı�־
    int    consumeNodeNum = gh.consumeNodeNum;
    //��ʼ������ܼ۸񣬷��������������ѽڵ������ӵĵط�
    long sumPre = consumeNodeNum*gh.serverCost;
    long sumChange = MAXCOST;
    long sumMin    = MAXCOST;
    double t = 50;     //0.5*consumeNodeNum*gh.serverCost; //��ʼ�¶�0.5
    //��������ڵ����
    int networkNodeNum = gh.networkNodeNum-2;
    gh.GetmaxLinkedNode(); //����·���Ľڵ�
    //���ݳ������÷������ڵ㱻ѡ�еĸ���
    float weight[1000]={0.0};
    for (int i = 0; i < consumeNodeNum; i++)
    {
	existNode.insert(gh.f[i].linkedNodeID);
	weight[gh.f[i].linkedNodeID] = 0.7; //�����ѽڵ������ӵ�����ڵ㱻��ȥ��Ȩ�غܴ�
    }
    for (int i = 0;i<networkNodeNum;i++)
    {
 	if(weight[i]<0.00001)
	{
	    if(gh.g[i].size()<=1)
	 	weight[i] = 1;
	    else //���ݳ��ȣ��������ѽڵ������ӵ�����ڵ��Ȩ�����õ�0-0.7֮��,����Խ�󱻼���ĸ���Խ��
		weight[i] = 0.3*((float)(gh.maxLinkedNode - gh.g[i].size()))/ (gh.maxLinkedNode-2.0)+ 0.3; 
	}
    } 
  //��ʼ�����������������ѽڵ������ӵ�����ڵ�
    for (int i = 0; i < consumeNodeNum; i++)
	existNode.insert(gh.f[i].linkedNodeID);
    srand((unsigned)time(NULL));	
    while (1)
    {	
    	if(isInitial && count >= iterTime1)
   	{
        	count = 0;
        	isInitial = false;	//�����趨ֵ�ͽ�����һ��״̬���������¼�ʱ
   	}
	//���ӻ��Ǽ���ǰ�ȱ�����ǰ�ڵ㣬��ֹ���ǳ��˻��������
	copyExistNode = existNode;  
        //���������
	randX = (float) rand()/ RAND_MAX;
	//������ʼ״̬
	if(isInitial && count <= iterTime1){
		//�����һ���������ڵ�
		if(randX < RANDX)  
	    		reduceOneServer(existNode);	
		//�滻һ���������ڵ�
		else                
		{
	    		getRemainNode(gh.f, existNode, notWantedNode);	 
	    		addOneServer(existNode, notWantedNode);
	    		reduceOneServer(existNode);

		}
	} 
	else if(!isInitial && count < iterTime2) //  
	{
//printf("****second****\n");
		t = t*at; //�����¶�
		if(randX < RANDY)
		{
			reduceOneServer(existNode,weight);
		} 
		else if(randX< RANDZ)
		{
			getRemainNode(networkNodeNum, existNode, notWantedNode);	 
	    		addOneServer(existNode, notWantedNode,weight);
		} 
		else
		{
			getRemainNode(networkNodeNum, existNode, notWantedNode);	 
	    		addOneServer(existNode, notWantedNode,weight);
			reduceOneServer(existNode,weight);
	 	} 
	}
	else
	    break;
	//��С������
	for (set<int>::iterator it = existNode.begin(); it != existNode.end();it++)
    		gh.g[gh.V() - 1].push_back(new Edge(gh.V()-1, *it, MAXVALUE, 0));	
	sumCur = minCostMaxFlow(gh, existNode);	
	double df = sumCur - sumPre;
	//���ܵ�ǰ���
	if (df < 0)
	{
    		sumPre = sumCur;          //������С���
		if(sumMin > sumPre)
		{
//printf("totalTime:%f,sumPre:%d\n",totalTime,sumPre);
			sumMin = sumPre;
	    		gh.serverRel.clear();
	    		gh.serverRel = existNode; //���ܵ�ǰ���������ֽ��
	    		for (size_t i = 0; i < gh.gRel.size(); i++)
				gh.gRel[i].clear();
	    		for (size_t i = 0; i < networkNodeNum; i++)
	    		{
	      			for (size_t j = 0; j < gh.g[i].size(); j++)
		  			gh.gRel[i].push_back({gh.g[i][j]->w(),gh.g[i][j]->rem_f,gh.g[i][j]->cur_cost});
	    		}
		}
	}
	//��һ�����ʽ��ܵ�ǰ״̬ 
	else if(!isInitial&&(exp(-df/t) > ((double) rand()/ RAND_MAX)) && sumCur != MAXCOST) //
		sumPre = sumCur;
	//��������ε�״̬
	else 
    		existNode = copyExistNode;
        //����ʱ��
	totalTime = ((double)(clock() - start) / CLOCKS_PER_SEC);
	if (totalTime > 88)
	    break;
        //�������ս�����仯���ٴ�����
	if(sumChange == sumMin)
	    count++;
	else
	    count = 0;
	sumChange = sumMin;
        //�ָ�ͼ�ṹ��������
	gh.recover();
    }
   printf("\ntime: %lf\n",totalTime);
   printf("\ntotal cost: %ld\n",sumMin);
}
