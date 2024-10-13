#pragma once
#include "afxwin.h"

class CGRIMDlg : public CDialogEx
{
public:
    CGRIMDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_GRIM_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
    HICON m_hIcon;

    virtual BOOL OnInitDialog();  // OnInitDialog 함수

    // 컨트롤 변수 선언
    CStatic m_pictureControl;  // Static Picture 컨트롤
    CEdit m_editX1, m_editY1, m_editX2, m_editY2;  // 좌표 입력 컨트롤

    // 버튼 핸들러
    afx_msg void OnBnClickedButtonDraw();
    afx_msg void OnBnClickedButtonAction();
    afx_msg void OnBnClickedButtonOpen();

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnStnClickedStaticPicture();
};
