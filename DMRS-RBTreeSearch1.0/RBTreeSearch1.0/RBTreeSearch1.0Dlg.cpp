// RBTreeSearch1.0Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RBTreeSearch1.0.h"
#include "RBTreeSearch1.0Dlg.h"

#include "RBTreeSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRBTreeSearch10Dlg 对话框




CRBTreeSearch10Dlg::CRBTreeSearch10Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRBTreeSearch10Dlg::IDD, pParent)
	, m_keywords(_T(""))
	/*, m_result_list(_T(""))*/
	//, m_topk_result(_T(""))
	, m_time(_T(""))
	, m_path(_T(""))
	, m_keywordNum(_T(""))
	, m_docNum(_T(""))
	, m_indexTime(_T(""))
	, m_numK(0)
	, m_WSTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRBTreeSearch10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KEYWORDS, m_keywords);
	//DDX_LBString(pDX, IDC_RESULT_LIST, m_result_list);
	//DDX_Text(pDX, IDC_TOPK_RESULT, m_topk_result);
	DDX_Text(pDX, IDC_TIME, m_time);
	DDX_Control(pDX, IDC_RESULT_LIST, m_ListCtrl);
	DDX_Text(pDX, IDC_FILE_PATH, m_path);
	DDX_Text(pDX, IDC_KEYWORD_NUM, m_keywordNum);
	DDX_Text(pDX, IDC_DOC_NUM, m_docNum);
	DDX_Text(pDX, IDC_INDEX_TIME, m_indexTime);
	DDX_Text(pDX, IDC_NUM_K, m_numK);
	DDX_Text(pDX, IDC_WS_TIME, m_WSTime);
}

BEGIN_MESSAGE_MAP(CRBTreeSearch10Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TestButton, &CRBTreeSearch10Dlg::OnBnClickedTestbutton)
	ON_BN_CLICKED(IDC_BROWSE, &CRBTreeSearch10Dlg::OnBnClickedBrowse)
END_MESSAGE_MAP()


// CRBTreeSearch10Dlg 消息处理程序

BOOL CRBTreeSearch10Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//获取当前扩展样式
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();
	//选中某行使整行高亮（report风格）
	dwStyle = LVS_EX_FULLROWSELECT;
	//网格线（report风格）
	dwStyle |= LVS_EX_GRIDLINES;
	//在item前产生checkout控件(可选)
	//       dwStyle |= LVS_EX_CHECKBOXED;
	m_ListCtrl.SetExtendedStyle(dwStyle);
	CRect rect;
	m_ListCtrl.GetWindowRect(&rect);

	int listwidth = rect.Width();

	m_ListCtrl.InsertColumn(0, "NO", LVCFMT_LEFT, listwidth/5, -1);
	m_ListCtrl.InsertColumn(1, "FileName", LVCFMT_LEFT, 2*listwidth/5, -1);
	m_ListCtrl.InsertColumn(2,"Score",LVCFMT_LEFT,2*listwidth/5, -1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRBTreeSearch10Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRBTreeSearch10Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRBTreeSearch10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRBTreeSearch10Dlg::OnBnClickedTestbutton()
{
	// TODO: 在此添加控件通知处理程序代码

	m_keywords.Empty();
	UpdateData(TRUE);
	//m_topk_result.Empty();
	m_ListCtrl.DeleteAllItems();
	
	if (m_keywords=="")
	{
		MessageBox("please input keywords","提示");
		return;
	}
	if (m_numK==0)
	{
		MessageBox("please input number","提示");
		return;
	}
	vector<string> keywords_list;
	//keywords_list.clear();
	CString strsplit=" ";
	CString temp=m_keywords;
	CString strleft;
	while (!temp.IsEmpty())
	{
		int pp=temp.Find(strsplit,0);
		if (pp!=-1)
		{
			strleft=temp.Left(pp);
			string keyword(strleft.GetBuffer());
			strleft.ReleaseBuffer();
			keywords_list.push_back(keyword);
			temp=temp.Right(temp.GetLength()-pp-strsplit.GetLength());
		}
		else if (temp.GetLength()>0)
		{
			string keyword(temp.GetBuffer());
			temp.ReleaseBuffer();
			keywords_list.push_back(keyword);
			temp.Empty();
		}
	}

	if (m_path.IsEmpty())
	{
		AfxMessageBox("Caution: Dataset is empty!",MB_OK,0);
		return;
	}
	
	RBTreeSearch Test_One;
	vector<CString> result_list;
	if (m_path.CompareNoCase(m_path_state)==0)
	{
		Test_One.startSearch(keywords_list,m_time,result_list,m_numK,m_WSTime);
		for (int ti=0;ti<result_list.size();ti+=2)
		{
			CString s;
			s.Format("%d",ti/2+1);
			m_ListCtrl.InsertItem(ti/2,s);
			m_ListCtrl.SetItemText(ti/2,1,result_list[ti]);
			m_ListCtrl.SetItemText(ti/2,2,result_list[ti+1]);		
		}
	}
	else
	{
		if(Test_One.ConstructRBTree(m_path,m_keywordNum,m_docNum,m_indexTime))
		{
			Test_One.startSearch(keywords_list,m_time,result_list,m_numK,m_WSTime);
			for (int ti=0;ti<result_list.size();ti+=2)
			{
				CString s;
				s.Format("%d",ti/2+1);
				m_ListCtrl.InsertItem(ti/2,s);
				m_ListCtrl.SetItemText(ti/2,1,result_list[ti]);
				m_ListCtrl.SetItemText(ti/2,2,result_list[ti+1]);		
			}
		}
		else AfxMessageBox("Caution!",MB_OK,0);
	}
	
	m_path_state=m_path;
	/*m_keywords_state=m_keywords;*/
	UpdateData(FALSE);
}

void CRBTreeSearch10Dlg::OnBnClickedBrowse()
{
	// TODO: 在此添加控件通知处理程序代码

	m_time.Empty();
	m_WSTime.Empty();
	m_indexTime.Empty();	
	m_keywordNum.Empty();
	m_docNum.Empty();
	
	
	m_path ="";
	m_path_state="";
	UpdateData(FALSE);



	//m_numK=0;


	char szTempDir[MAX_PATH];  //-----MAX_PATH==260
	CString szTitle="选择你要检测的文件夹";
	BROWSEINFO bi; 
	ZeroMemory(&bi, sizeof(bi));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();  //父窗口句柄
	bi.pidlRoot = NULL;              //LPITEMIDLIST类型，指明开始的根目录，可以为空
	bi.pszDisplayName = NULL; 
	bi.lpszTitle = szTitle;          //对话框标题
	bi.ulFlags = BIF_RETURNONLYFSDIRS; 
	bi.lpfn = NULL; 

	//CString strFoldName;
	//vector <CString > strFilePaths;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi); 
	if (!pidl) 
	{ 
		return; 	
	}  
	BOOL bSuccess = SHGetPathFromIDList(pidl,szTempDir);  
	LPMALLOC pMalloc;
	if(SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		pMalloc->Free(pidl);
		pMalloc->Release();
	}
	if (!bSuccess) 
	{ 
		return;
	}
	else
	{
		m_path = szTempDir;       //获取目录路径
	}

	
	UpdateData(FALSE);
}
