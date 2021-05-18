#include "stdafx.h"
#include "RBTreeNode.h"
#include <vector>

using namespace std;


RBTreeNode :: RBTreeNode()
{

}
RBTreeNode::~RBTreeNode()
{

}

void RBTreeNode::InitialLeaf(vector<double>	 vectorData,CString fileNumber)
{
	m_Data= vectorData;
	int vecSize=vectorData.size();
	srand(GetTickCount());
	int r=0;
	while (r<KEY_EXTEND/2)
	{
		int loc=(int)(rand()%KEY_EXTEND)+vecSize-KEY_EXTEND;
		if (m_Data[loc]!=1)
		{
			m_Data[loc]=1;
			r++;
		}		
	}

	leftChild = NULL;
	rightChild = NULL;
	parent = NULL;
	//height = 1;
	fileName = fileNumber;
	score=0;
	tag=TRUE;
}

void RBTreeNode::InitialInternalNode( RBTreeNode *left, RBTreeNode *right)
{
	m_Data=left->m_Data;
	int vecSize=m_Data.size();
	vector<double> rightWeight=right->m_Data;	
	for (int ki=0;ki<vecSize;ki++)
	{
		if (m_Data[ki]<rightWeight[ki])
			m_Data[ki]=rightWeight[ki];	
	}

	leftChild = left;
	rightChild = right;
	parent = NULL;

	fileName="";
	score=0;
	tag=FALSE;
}

void RBTreeNode:: node_scoreCal(vector<double> queryVector)
{
	double temp=0;
	if (!queryVector.empty() && ! m_Data.empty())
	{
		for (int ki=0;ki<queryVector.size();ki++)
		{
			temp+=double(queryVector[ki]*m_Data[ki]);
		}
		score=temp;
	}
	else 
		return ;
}

//void RBTreeNode:: enc_node_scoreCal(vector<double> queryVector)
//{
//	double temp=0;
//	if (!queryVector.empty() && ! enc_m_Data.empty())
//	{
//		for (int ki=0;ki<queryVector.size();ki++)
//		{
//			temp+=double(queryVector[ki]*enc_m_Data[ki]);
//		}
//		score=temp;
//	}
//	else 
//		return ;
//}

void RBTreeNode:: enc_dataSplit(vector<bool> arry_S)//,vector<vector<double>> Matrix1,vector<vector<double>> Matrix2
{
	vector<double> m1_Data=m_Data;
	vector<double> m2_Data=m_Data;

	int seed=GetTickCount();
	CvRNG rng= cvRNG(seed);

	int dimen= arry_S.size();
	int i;


	for ( i=0;i<dimen;i++)
	{
		if (arry_S[i])
		{
			double ran=cvRandReal(&rng);
			m1_Data[i]=ran;
			m2_Data[i]-=ran;
		}
	}

	for (i=0;i<dimen;i++)
	{
		m1_Data.push_back(m2_Data[i]);
	}
	m_Data=m1_Data;

}