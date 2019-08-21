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

	/*
	// 将数组中的所有元素前移一个单位，第一个元素丢弃   
	for (int i = 0; i < POINT_COUNT - 1; i++)
	{
		m_nzValues[i] = m_nzValues[i + 1];
	}
	// 为最后一个元素赋一个80以内的随机数值（整型）   
	m_nzValues[POINT_COUNT - 1] = rand() % 80;

	// 获取绘图控件的客户区坐标   
	// （客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标）   
	m_picDraw.GetClientRect(&rectPicture);

	// 绘制波形图   
	DrawWave(m_picDraw.GetDC(), rectPicture);
	*/

	CRect rectPicture;
	// 获取绘图控件的客户区坐标   
	// （客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标）   
	m_picDraw.GetClientRect(&rectPicture);
	m_map->update();
	m_map->draw(m_picDraw.GetDC(), rectPicture);
	if (m_map->isGameFinished())
	{
		KillTimer(1);
		m_map->remove();
		delete m_map;
		m_map = nullptr;
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
	SetTimer(1, 1000, NULL);
	if (m_map == nullptr)
	{
		m_map = new Map();
		m_map->init();
	}
	else
	{
		;
	}
}


void CPicture::OnBnClickedEndButton()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	m_map->remove();
	delete m_map;
	m_map = nullptr;
}

void CPicture::DrawWave(CDC *pDC, CRect &rectPicture)
{
	float fDeltaX;     // x轴相邻两个绘图点的坐标距离   
	float fDeltaY;     // y轴每个逻辑单位对应的坐标值   
	int nX;      // 在连线时用于存储绘图点的横坐标   
	int nY;      // 在连线时用于存储绘图点的纵坐标   
	CPen newPen;       // 用于创建新画笔   
	CPen *pOldPen;     // 用于存放旧画笔   
	CBrush newBrush;   // 用于创建新画刷   
	CBrush *pOldBrush; // 用于存放旧画刷   

	// 计算fDeltaX和fDeltaY   
	fDeltaX = (float)rectPicture.Width() / (POINT_COUNT - 1);
	fDeltaY = (float)rectPicture.Height() / 80;

	// 创建黑色新画刷   
	newBrush.CreateSolidBrush(RGB(0, 0, 0));
	// 选择新画刷，并将旧画刷的指针保存到pOldBrush   
	pOldBrush = pDC->SelectObject(&newBrush);
	// 以黑色画刷为绘图控件填充黑色，形成黑色背景   
	pDC->Rectangle(rectPicture);
	// 恢复旧画刷   
	pDC->SelectObject(pOldBrush);
	// 删除新画刷   
	newBrush.DeleteObject();

	// 创建实心画笔，粗度为1，颜色为绿色   
	newPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	// 选择新画笔，并将旧画笔的指针保存到pOldPen   
	pOldPen = pDC->SelectObject(&newPen);

	// 将当前点移动到绘图控件窗口的左下角，以此为波形的起始点   
	pDC->MoveTo(rectPicture.left, rectPicture.bottom);
	// 计算m_nzValues数组中每个点对应的坐标位置，并依次连接，最终形成曲线   
	for (int i = 0; i < POINT_COUNT; i++)
	{
		nX = rectPicture.left + (int)(i * fDeltaX);
		nY = rectPicture.bottom - (int)(m_nzValues[i] * fDeltaY);
		pDC->LineTo(nX, nY);
	}

	// 恢复旧画笔   
	pDC->SelectObject(pOldPen);
	// 删除新画笔   
	newPen.DeleteObject();
}


BOOL CPicture::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_DRAW); //获取控件指针，IDC_BUTTON1为控件ID号  
	//pWnd->SetWindowPos(NULL, 50, 80, 0, 0, SWP_NOZORDER | SWP_NOSIZE); //把按钮移到窗口的(50,80)处  
	//pWnd = GetDlgItem(IDC_DRAW);
	//pWnd->SetWindowPos(NULL, 0, 0, 100, 80, SWP_NOZORDER | SWP_NOMOVE); //把编辑控件的大小设为(100,80)，位置不变  
	//pWnd = GetDlgItem(IDC_DRAW);
	pWnd->SetWindowPos(NULL, 100, 100, 500, 400, SWP_NOZORDER); //编辑控件的大小和位置都改变  

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
