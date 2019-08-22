// CPicture.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "CPicture.h"
#include "afxdialogex.h"


// CPicture 对话框

IMPLEMENT_DYNAMIC(CPicture, CDialogEx)

CPicture::CPicture(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PICTURE, pParent)
{
	m_CurrentTime = 100;
	// 将数组m_nzValues的元素都初始化为0   
	memset(m_nzValues, 0, sizeof(int) * POINT_COUNT);
}

CPicture::~CPicture()
{
	
}

void CPicture::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIMER_STATIC, m_CurrentTime);
	DDX_Control(pDX, IDC_DRAW, m_picDraw);
}

BEGIN_MESSAGE_MAP(CPicture, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START_BUTTON, &CPicture::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_END_BUTTON, &CPicture::OnBnClickedEndButton)
END_MESSAGE_MAP()


// CPicture 消息处理程序


void CPicture::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData(TRUE);  //将控件数据保存到变量
	m_CurrentTime++;
	UpdateData(FALSE); //将变量数据保存到控件
	CRect rectPicture;
	// 获取绘图控件的客户区坐标   
	// （客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标）   
	m_picDraw.GetClientRect(&rectPicture);
	m_map->update();
	//m_map->drawTest(m_picDraw.GetDC(), rectPicture, 1);
	m_map->draw(m_picDraw.GetDC(), rectPicture);
	if (m_map->isGameFinished())
	{
		KillTimer(1);
		if (m_map != NULL)
		{
			m_map->remove();
			delete m_map;
			m_map = nullptr;
		}
		MessageBox(_T("Game Over!"), _T("Game Over!"), MB_OK);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CPicture::OnBnClickedStartButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//定时器编号为1, 间隔1000ms, 调用WM_TIMER消息处理函数(OnTimer)处理事件
	// 以时间为种子来构造随机数生成器   
	srand((unsigned)time(NULL));
	SetTimer(1, UPDATE_INTERVAL, NULL);	
	if (m_map == nullptr)
	{
		m_map = new Map();
		m_map->init();
	}
	else
	{
		;
	}
	CRect rectPicture;
	// 获取绘图控件的客户区坐标   
	// （客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标）   
	m_picDraw.GetClientRect(&rectPicture);
	m_map->setMaxMinXY(0, 0, rectPicture.Width(), rectPicture.Height());
}

void CPicture::OnBnClickedEndButton()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	if (m_map != NULL)
	{
		m_map->remove();
		delete m_map;
		m_map = nullptr;
	}
}

BOOL CPicture::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_DRAW); //获取控件指针，IDC_DRAW为控件ID号  
	//pWnd->SetWindowPos(NULL, 50, 80, 0, 0, SWP_NOZORDER | SWP_NOSIZE); //把按钮移到窗口的(50,80)处  
	//pWnd = GetDlgItem(IDC_DRAW);
	//pWnd->SetWindowPos(NULL, 0, 0, 100, 80, SWP_NOZORDER | SWP_NOMOVE); //把编辑控件的大小设为(100,80)，位置不变  
	//pWnd = GetDlgItem(IDC_DRAW);
	pWnd->SetWindowPos(NULL, 100, 100, 500, 400, SWP_NOZORDER); //编辑控件的大小和位置都改变  

	// 设置图片控件为焦点，以便响应键盘事件
	m_picDraw.SetFocus();

	// 返回TRUE则使用默认的焦点（按钮），因此必须返回FALSE
	return FALSE;  // return TRUE  unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CPicture::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//判断游戏是否进行
	if (m_map != NULL)
	{
		//判断是否为键盘消息
		if (pMsg->message == WM_KEYDOWN)    //捕捉到键盘
		{
			switch (pMsg->wParam)
			{
			case VK_UP: //方向键↑
				m_map->addBirdY();
				break;

			case VK_DOWN:  //方向键↓
				m_map->minusBirdY();
				break;

			case VK_RIGHT:  //方向键→
				;
				break;

			case VK_LEFT:  //方向键←
				;
				break;
			default:
				break;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
