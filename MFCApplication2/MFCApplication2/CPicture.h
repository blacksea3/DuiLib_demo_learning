#pragma once

#include "GameClasses.h"

// CPicture 对话框

class CPicture : public CDialogEx
{
	DECLARE_DYNAMIC(CPicture)

public:
	CPicture(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPicture();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PICTURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_CurrentTime;
//	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedEndButton();

private:
#define POINT_COUNT 100
	int m_nzValues[POINT_COUNT];
	Map* m_map = nullptr;
	void DrawWave(CDC*, CRect&);
public:
	CStatic m_picDraw;
	virtual BOOL OnInitDialog();
};
