// RBTreeSearch1.0Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CRBTreeSearch10Dlg 对话框
class CRBTreeSearch10Dlg : public CDialog
{
// 构造
public:
	CRBTreeSearch10Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RBTREESEARCH10_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTestbutton();
	CString m_keywords;


	//CString m_topk_result;
	CString m_time;
	CListCtrl m_ListCtrl;
	CString m_path;
	afx_msg void OnBnClickedBrowse();
	CString m_keywordNum;
	CString m_docNum;
	CString m_indexTime;
	int m_numK;
	CString m_WSTime;
	CString m_path_state;
};
