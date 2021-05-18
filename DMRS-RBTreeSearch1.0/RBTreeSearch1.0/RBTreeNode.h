#pragma once



#include <vector>
#include <cv.h>
#include <highgui.h>
using namespace std;
const int KEY_EXTEND=0;




class RBTreeNode  
{

public:
	RBTreeNode*	leftChild;
	RBTreeNode*	rightChild;
	RBTreeNode*  parent;
	vector<double>		m_Data;
	//vector<double>		enc_m_Data;
	CString fileName;//ÎÄ¼þ±àºÅ
	double score;
	bool tag;//internal node:0;leaf node:1
public:
	RBTreeNode();
	~RBTreeNode();

	void InitialLeaf(vector<double> vectorData,CString fileNumber);
	void InitialInternalNode( RBTreeNode *left, RBTreeNode *right);
	void node_scoreCal(vector<double> queryVector);

	void enc_dataSplit(vector<bool> arry_S);//,vector<vector<double>> Matrix1,vector<vector<double>> Matrix2

	
	
};