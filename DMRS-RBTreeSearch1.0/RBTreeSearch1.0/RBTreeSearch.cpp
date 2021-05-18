#include "stdafx.h"
#include "RBTreeSearch.h"
#include <map>
#include<queue>
using namespace std;
map<string,int> Dictionary;
map<string,double> IDFVALUE;
RBTreeNode* RBTreeRoot;
RBTreeNode* RBTreeRoot_Enc;
double K_TH_SCORE;
int KEYWORD_NUM;
int num_k;    //the value of k
vector<double> queryVector;

priority_queue<RBTreeNode*,vector<RBTreeNode*>,cmp> Top_K_Queue;
priority_queue<RBTreeNode*,vector<RBTreeNode*>,cmp> Whole_Queue;//queue without cutting
vector<bool> ARRAY_S;
//vector<vector<double>> Matrix1;
//vector<vector<double>> Matrix2;
//vector<vector<double>> Matrix1_T;
//vector<vector<double>> Matrix2_T;
//vector<vector<double>> Matrix1_INV;
//vector<vector<double>> Matrix2_INV;


RBTreeSearch::RBTreeSearch()
{

}
RBTreeSearch::~RBTreeSearch()
{

}
void RBTreeSearch::GetAllKeywords(vector <CString> strPaths)
{
	Dictionary.clear();
	int wordIndex = 0;//关键词在向量中的位置
	for (int fi = 0; fi < strPaths.size(); fi++)
	{
		CStdioFile File;
		CString str;
		
		CString strsplite(' ');
		map<string,int>::iterator it;

		File.Open(strPaths[fi],CFile::modeRead | CFile::typeText);
		while(File.ReadString(str) == TRUE)
		{

				int pos = str.Find(strsplite,0);//定位分割符
				CString strleft;
				if(pos!=-1)
				{
					//定位成功
					strleft = str.Left(pos);//前面的字符串为新的分割单元
					string sleft(strleft.GetBuffer());
					strleft.ReleaseBuffer();
					it=Dictionary.find(sleft);
					if (it==Dictionary.end())
					{
						Dictionary.insert(map<string,int>::value_type(sleft,wordIndex));
						wordIndex++;
					}
				}		
		}
		File.Close();

	}
}

void RBTreeSearch::GetAllFilesInFolder(CString path, vector <CString> &strPaths)
{
	//vector <CString> strPaths;
	//---------寻找文件---------
	CFileFind finder;
	BOOL bWorking = finder.FindFile(path+"\\*.*");

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!finder.IsDots()) 
		{
			CString strFileName,strFileNameExt;
			strFileName=finder.GetFilePath();//.GetFileName();
			int pos=strFileName.ReverseFind('.');//(".",0);
			strFileNameExt=strFileName.Mid(pos,strFileName.GetLength()-pos);
			strFileNameExt.MakeLower();

			if( strFileNameExt==".txt"	)
			{
				strPaths.push_back(strFileName);
			}
		}
	}

}
bool RBTreeSearch::ConstructRBTree(CString path,CString &keywordNum,CString &docNum,CString &contime)
{
	
	vector<RBTreeNode*> tempNodes;
	//vector<vector<CString>> leafnodesInTxt;
	//CString strMsgALL = "D:\\leafnodes.txt";

	vector <CString> strPaths;
	GetAllFilesInFolder(path, strPaths);//////////////////获取文件夹中所有文件

	GetAllKeywords(strPaths);////////////////////统计文件集对应的关键词集
	

	docNum.Format("%d",strPaths.size());
	//leafnodesInTxt.resize(strPaths.size());

	if (Dictionary.size()==0)
	{
		AfxMessageBox("Caution:Dictionary is empty!",MB_OK,0);
		return false;
	}
	KEYWORD_NUM=Dictionary.size()+KEY_EXTEND;
	keywordNum.Format("%d",KEYWORD_NUM);
	
	///////////////////////////////////////////////////////////
	//CvMat *M1=cvCreateMat(KEYWORD_NUM,KEYWORD_NUM,CV_64FC1);
	//GenRand(M1,1);	
	//MatrixToVector(M1,Matrix1);
	////CvMat *M1_INV=cvCreateMat(KEYWORD_NUM,KEYWORD_NUM,CV_64FC1);
	////cvInvert(M1,M1_INV,CV_LU);
	////cvReleaseMat(&M1);
	////MatrixToVector(M1_INV,Matrix1_INV);
	////cvReleaseMat(&M1_INV);

	//CvMat *M2=cvCreateMat(KEYWORD_NUM,KEYWORD_NUM,CV_64FC1);
	//GenRand(M2,2);
	//MatrixToVector(M2,Matrix2);
	////CvMat *M2_INV=cvCreateMat(KEYWORD_NUM,KEYWORD_NUM,CV_64FC1);	
	////cvInvert(M2,M2_INV,CV_LU);
	////cvReleaseMat(&M2);
	////MatrixToVector(M2_INV,Matrix2_INV);
	////cvReleaseMat(&M2_INV);


	//

	///////////////////////////////////////////////////////////
	///////////////////////对于每个文件生成叶子节点
	//int start=GetTickCount();////////计时开始！
	LARGE_INTEGER m_liPerfFreq={0},m_liPerfStart={0},liPerfNow={0};
	QueryPerformanceFrequency(&m_liPerfFreq);
	QueryPerformanceCounter(&m_liPerfStart);

	for (int fi = 0; fi < strPaths.size(); fi++)
	{   
		CString leafname=strPaths[fi];
		vector<double> vecData(KEYWORD_NUM,0);//初始化文件索引向量,所有维的值初始化为0
		//读取文件的关键词及权值
		CStdioFile File;
		CString str;
		CString strsplite=" ";
		map<string,int>::iterator it;
		File.Open(strPaths[fi],CFile::modeRead | CFile::typeText);
		while(File.ReadString(str) == TRUE)
		{
				int pos = str.Find(strsplite,0);//定位分割符
				CString strleft,strright;
				if(pos!=-1)
				{
					//定位成功
					strleft = str.Left(pos);//前面的字符串为新的分割单元
					string keystr(strleft.GetBuffer());
					strleft.ReleaseBuffer();
					//str = str.Right(str.GetLength()-pos-strsplite.GetLength());//指定新的分割对象					
					strright=str.Right(str.GetLength()-pos-strsplite.GetLength());
					double weight=atof(strright);	
					it=Dictionary.find(keystr);
					if (it!=Dictionary.end())
					{
						vecData[Dictionary[keystr]]=weight;
					}
					else
					{
						AfxMessageBox("Caution:Keyword Set is not completed!",MB_OK,0);
						return false;
					}	
				}
				str.Empty();
		}
		//创建文件对应的叶子节点
		RBTreeNode *leafNode=new RBTreeNode;
		//CString dir('\\');
		//int pp=strPaths[fi].Find(dir,0);
		//CString strleft = str.Left(pos);//前面的字符串为新的分割单元
		//string keystr(strleft.GetBuffer());
		//strleft.ReleaseBuffer();
		//
		//if (pp!=-1)
		//{
			CString filename=strPaths[fi];//.Right(strPaths[fi].GetLength()-pp-dir.GetLength());
			leafNode->InitialLeaf(vecData,filename);
			tempNodes.push_back(leafNode); 
		//}
		//delete leafNode;
		             
	}
	//////////////////////////////////////////////////////////

	vector<RBTreeNode*> sec_tempnodes;
	while (tempNodes.size()>0)
	{
		if (tempNodes.size()%2==0)
		{
			for (int i=0;i<tempNodes.size();i+=2)
			{
				RBTreeNode* interNode=new RBTreeNode;
				RBTreeNode* leftNode=tempNodes[i];
				RBTreeNode* rightNode=tempNodes[i+1];

				interNode->InitialInternalNode(leftNode,rightNode);
				leftNode->parent=interNode;
				rightNode->parent=interNode;
				sec_tempnodes.push_back(interNode);

			}
			tempNodes.clear();
			tempNodes=sec_tempnodes;
			sec_tempnodes.clear();
			if (tempNodes.size()==1)
			{
				RBTreeRoot=tempNodes[0];
				break;
			}
		}
		else
		{
			if (tempNodes.size()>4)
			{
				for (int n=0;n<tempNodes.size()-1;n+=2)
				{
					RBTreeNode* interNode=new RBTreeNode;
					RBTreeNode* leftNode=tempNodes[n];
					RBTreeNode* rightNode=tempNodes[n+1];
					
					interNode->InitialInternalNode(leftNode,rightNode);
					leftNode->parent=interNode;
					rightNode->parent=interNode;
					sec_tempnodes.push_back(interNode);
				}
				RBTreeNode* sec_lastNode=sec_tempnodes[sec_tempnodes.size()-1];
				RBTreeNode* lastNode=tempNodes[tempNodes.size()-1];
				RBTreeNode* last_merge=new RBTreeNode;
				last_merge->InitialInternalNode(sec_lastNode,lastNode);
				sec_lastNode->parent=last_merge;
				lastNode->parent=last_merge;
				sec_tempnodes.pop_back();
				sec_tempnodes.push_back(last_merge);
				tempNodes.clear();
				tempNodes=sec_tempnodes;
				sec_tempnodes.clear();
			}
			else if (tempNodes.size()==3)
			{
				RBTreeNode* firstNode=tempNodes[0];
				RBTreeNode* secNode=tempNodes[1];
				RBTreeNode* thirdNode=tempNodes[2];
				RBTreeNode* tempNode=new RBTreeNode;
				RBTreeNode* finalNode=new RBTreeNode;
				tempNode->InitialInternalNode(firstNode,secNode);
				firstNode->parent=tempNode;
				secNode->parent=tempNode;
				finalNode->InitialInternalNode(tempNode,thirdNode);
				tempNode->parent=finalNode;
				thirdNode->parent=finalNode;
				RBTreeRoot=finalNode;
				break;
			}
									
		}
	}

	vector<RBTreeNode*>().swap(tempNodes);
	vector<RBTreeNode*>().swap(sec_tempnodes);

	ARRAY_S.clear();
	int seed=GetTickCount();
	CvRNG rng= cvRNG(seed);
	for (int i=0;i<KEYWORD_NUM;i++)
	{
		//int ran=cvRandInt(&rng)%2;
		ARRAY_S.push_back(cvRandInt(&rng)%2);
	}

	GenEncTree_dataSplit(RBTreeRoot);

	QueryPerformanceCounter(&liPerfNow);
	double run_time= (double)(liPerfNow.QuadPart - m_liPerfStart.QuadPart)*1000 /(double)m_liPerfFreq.QuadPart;
	contime.Format("%.4lf",run_time);

	
	return true;
	
	
}

void RBTreeSearch::GenEncTree_dataSplit(RBTreeNode* node)//,vector<bool> arry_S
{
	if(node)
	{
		node->enc_dataSplit(ARRAY_S);
		GenEncTree_dataSplit(node->leftChild);
		GenEncTree_dataSplit(node->rightChild);	//,arry_S	
	}
	return;
}

void RBTreeSearch:: startSearch(vector<string> keywords_list,CString &time,vector<CString> &result_list,int k,CString &w_time)
{
	///////////////////////////////////////////生成关键词对应idf
	CStdioFile File;
	CString str;
	CString path="allWordsIDF.txt";
	//CString path="idf.txt";
	num_k=k;

	CString strsplite(' ');
	map<string,double>::iterator dou_it;

	File.Open(path,CFile::modeRead | CFile::typeText);
	while(File.ReadString(str) == TRUE)
	{

		int pos = str.Find(strsplite,0);//定位分割符
		CString strleft,strright;
		if(pos!=-1)
		{
			//定位成功
			strleft = str.Left(pos);//前面的字符串为新的分割单元
			string sleft(strleft.GetBuffer());
			strleft.ReleaseBuffer();
			strright=str.Right(str.GetLength()-pos-strsplite.GetLength());
			double weight=atof(strright);	
			dou_it=IDFVALUE.find(sleft);
			if (dou_it==IDFVALUE.end())
			{
				IDFVALUE.insert(map<string,double>::value_type(sleft,weight));				
			}
		}		
	}
	File.Close();

	///////////////////////////////////////////////////初始化查询向量和top-k队列

	
	
	////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////初始化一些变量
	LARGE_INTEGER m_Freq={0},m_Start={0},m_Now={0};
	QueryPerformanceFrequency(&m_Freq);
	QueryPerformanceCounter(&m_Start);

	queryVector.clear();
	queryVector.resize(KEYWORD_NUM,0);
	
	//priority_queue<RBTreeNode*,vector<RBTreeNode*>,cmp> Original_Top_K;


	vector<int> loc;
	double sum=0;
	map<string,int>::iterator int_it;
	for (int n=0;n<keywords_list.size();n++)
	{
		dou_it=IDFVALUE.find(keywords_list[n]);
		int_it=Dictionary.find(keywords_list[n]);
		if (dou_it!=IDFVALUE.end() && int_it!=Dictionary.end())
		{
			loc.push_back(Dictionary[keywords_list[n]]);
			sum+=double(IDFVALUE[keywords_list[n]]*IDFVALUE[keywords_list[n]]);
			queryVector[Dictionary[keywords_list[n]]]=IDFVALUE[keywords_list[n]];
		}
		else
		{
			AfxMessageBox("Caution:Keyword  is not in dictionary!",MB_OK,0);
			continue;
		}	
	}
	if (loc.size()==0)
	{
		AfxMessageBox("Caution:No Keyword you have input  is in dictionary!",MB_OK,0);
		return;
	}
	for (int c=0;c<loc.size();c++)
	{
		queryVector[loc[c]]/=sqrt(abs(sum));
	}

	vector<double> queryVector_later=queryVector;
	int seed=GetTickCount();
	CvRNG rng= cvRNG(seed);
	for (int i=0;i<KEYWORD_NUM;i++)
	{
		if (!ARRAY_S[i])
		{
			double ran=cvRandReal(&rng);
			queryVector[i]=ran;
			queryVector_later[i]-=ran;
		}
	}

	queryVector.resize(KEYWORD_NUM*2);
	for (int j=0;j<KEYWORD_NUM;j++)
	{
		queryVector[j+KEYWORD_NUM]=queryVector_later[j];
	}

	vector<double>().swap(queryVector_later);
		

	//GenEncryptedQuery(Matrix1,Matrix2);

	QueryPerformanceCounter(&m_Now);
	double run_time2= (double)(m_Now.QuadPart - m_Start.QuadPart)*1000 /(double)m_Freq.QuadPart;
	w_time.Format("%.4lf",run_time2);

	
	

	while (!Top_K_Queue.empty())
	{
		Top_K_Queue.pop();
	}
	
    //RBTreeRoot->node_scoreCal(queryVector);
	K_TH_SCORE=0;
	////////////////////////////////////////////////////////正式开始搜索
	LARGE_INTEGER m_liPerfFreq={0},m_liPerfStart={0},liPerfNow={0};
	QueryPerformanceFrequency(&m_liPerfFreq);
	QueryPerformanceCounter(&m_liPerfStart);

	RBTreeRoot->node_scoreCal(queryVector);
	SearchRBTree(RBTreeRoot);/////////////////search function

	QueryPerformanceCounter(&liPerfNow);
	double run_time= (double)(liPerfNow.QuadPart - m_liPerfStart.QuadPart)*1000 /(double)m_liPerfFreq.QuadPart;
	time.Format("%.4lf",run_time);
	///////////////////////////////////////////计时结束
	priority_queue<RBTreeNode*,vector<RBTreeNode*>,cmp> temp=Top_K_Queue;
	vector<CString> listResult;
	while (!temp.empty())
	{
		RBTreeNode* node=temp.top();
		CString filename=node->fileName;
		CString score;
		score.Format("%.6lf",node->score);
		listResult.push_back(filename);
		listResult.push_back(score);
		temp.pop();
	}

	result_list.clear();
	for (int i=listResult.size()-1;i>=0;i-=2)
	{
		result_list.push_back(listResult[i-1]);
		result_list.push_back(listResult[i]);
	}
	vector<CString> Result=result_list;
	////////////////////////////////////////



}

void RBTreeSearch:: SearchRBTree(RBTreeNode* node)
{

	if(!node->tag)
	{
		RBTreeNode *leftNode=node->leftChild;
		RBTreeNode *rightNode=node->rightChild;

		if (node->score>K_TH_SCORE)
		{
			leftNode->node_scoreCal(queryVector);
			rightNode->node_scoreCal(queryVector);
			if (leftNode->score>=rightNode->score)
			{
				SearchRBTree(node->leftChild);
				SearchRBTree(node->rightChild);
			}
			else 
			{
				SearchRBTree(node->rightChild);
				SearchRBTree(node->leftChild);
			}
		}
		return;
	}
	else
	{
		if (node->score>0)
		{
			Updata_Top_K_List(node);
		}		
		return;
	}
	
}


void RBTreeSearch::Updata_Top_K_List(RBTreeNode* node)
{

		if (Top_K_Queue.size()<num_k-1 )
		{

			Top_K_Queue.push(node);	
			K_TH_SCORE=0;
		}
		else if (Top_K_Queue.size()==num_k-1)
		{

				Top_K_Queue.push(node);
				RBTreeNode* least_node=Top_K_Queue.top();
				K_TH_SCORE=least_node->score;		
		}
		else  if (Top_K_Queue.size()==num_k)
		{
			RBTreeNode* least_node=Top_K_Queue.top();
			if (node->score > least_node->score)
			{
				Top_K_Queue.pop();
				Top_K_Queue.push(node);
				RBTreeNode* new_least=Top_K_Queue.top();
				K_TH_SCORE=new_least->score;
			}	
		}

}


void RBTreeSearch::GenRand(CvMat* arr, int seed)
{
	// let's noisy_screen be the floating-point 2d array that is to be "crapped" 
	CvRandState rng;

	// initialize random generator
	rng.state = cvRNG(0xffffffff);
	cvRandInit( &rng,
		0, 1,      // use dummy parameters now and adjust them further 
		seed, // use input seed here 
		CV_RAND_UNI // specify uniform type  
		);

	cvRandArr( &rng.state, arr, CV_RAND_UNI, cvRealScalar(0), cvRealScalar(1) );
	// RNG state does not need to be deallocated 
}

void RBTreeSearch::MatrixToVector(CvMat* M,vector<vector<double>> &Matrix)
{


	double *data = (double *)(M->data.db);
	//int step = M->step;
	for(int  i = 0; i<  M->rows; i++)
	{
		vector <double> temp;
		for(int j = 0; j <  M->cols; j++)
		{
			temp.push_back( data[i*M->cols + j]);
		}
		Matrix.push_back(temp);
	}


}

