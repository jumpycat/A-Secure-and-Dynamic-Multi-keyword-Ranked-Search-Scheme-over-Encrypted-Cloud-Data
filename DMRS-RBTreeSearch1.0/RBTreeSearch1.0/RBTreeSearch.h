#pragma once

using namespace std;
#include "RBTreeNode.h"
#include "math.h"


struct cmp
{
	bool operator()(RBTreeNode* a,RBTreeNode* b)

	{
		return a->score > b->score;//∞¥…˝–Ú≈≈–Ú
	}
};

class RBTreeSearch
{
public:
	RBTreeSearch(void);
	~RBTreeSearch(void);
	/////////////////////////////////////////
	void GetAllKeywords(vector <CString> strPaths);
	void GetAllFilesInFolder(CString path, vector <CString> &strPaths);
	bool ConstructRBTree(CString path,CString &keywordNum,CString &docNum,CString &contime);
	void SearchRBTree(RBTreeNode* node);
	void Updata_Top_K_List(RBTreeNode* node);
	void startSearch(vector<string> keywords_list,CString &time,vector<CString> &result_list,int k,CString &w_time);
	
	///////////////////////////////////20130705
	void GenEncryptedQuery(vector<vector<double>> Matrix1_inv,vector<vector<double>> Matrix2_inv);
	void GenRand(CvMat* arr, int seed);
	void MatrixToVector(CvMat* M,vector<vector<double>> &Matrix);
	void GenEncTree_dataSplit(RBTreeNode* node);
	

};