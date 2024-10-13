#include "pch.h"
#include "framework.h"
#include "GRIM.h"
#include "GRIMDlg.h"
#include "afxdialogex.h"
#include <atlimage.h>  // 이미지를 다루기 위해 추가

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CGRIMDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_DRAW, &CGRIMDlg::OnBnClickedButtonDraw)
    ON_BN_CLICKED(IDC_BUTTON_ACTION, &CGRIMDlg::OnBnClickedButtonAction)
    ON_BN_CLICKED(IDC_BUTTON_OPEN, &CGRIMDlg::OnBnClickedButtonOpen)
    ON_STN_CLICKED(IDC_STATIC_PICTURE, &CGRIMDlg::OnStnClickedStaticPicture)
END_MESSAGE_MAP()

// CGRIMDlg 생성자
CGRIMDlg::CGRIMDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_GRIM_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// CGRIMDlg 초기화
BOOL CGRIMDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();  // 부모 클래스의 OnInitDialog 호출

    // 아이콘 설정
    SetIcon(m_hIcon, TRUE);    // 큰 아이콘 설정
    SetIcon(m_hIcon, FALSE);   // 작은 아이콘 설정

    // Static Picture 컨트롤 서브클래스화
    if (!m_pictureControl.SubclassDlgItem(IDC_STATIC_PICTURE, this)) {
        AfxMessageBox(_T("Failed to subclass static picture control!"));
        return FALSE;  // 서브클래싱 실패 시 FALSE 반환
    }

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// DoDataExchange 함수 정의
void CGRIMDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // 컨트롤과 멤버 변수를 연결
    DDX_Control(pDX, IDC_STATIC_PICTURE, m_pictureControl);  // Static Picture 컨트롤
    DDX_Control(pDX, IDC_EDIT1_X1, m_editX1);  // X1 좌표 입력 컨트롤
    DDX_Control(pDX, IDC_EDIT2_Y1, m_editY1);  // Y1 좌표 입력 컨트롤
    DDX_Control(pDX, IDC_EDIT3_X2, m_editX2);  // X2 좌표 입력 컨트롤
    DDX_Control(pDX, IDC_EDIT4_Y2, m_editY2);  // Y2 좌표 입력 컨트롤
}

// 배경 지우기 함수 (잔상 제거)
void ClearBackground(CDC& dc, CRect rect)
{
    CBrush brush(RGB(255, 255, 255));  // 흰색 배경으로 지우기
    dc.FillRect(rect, &brush);
}

// Draw 버튼 클릭 이벤트 핸들러
void CGRIMDlg::OnBnClickedButtonDraw()
{
    // X1, Y1 좌표를 받아옵니다.
    CString strX1, strY1;
    m_editX1.GetWindowText(strX1);
    m_editY1.GetWindowText(strY1);

    int x1 = _ttoi(strX1);
    int y1 = _ttoi(strY1);

    // 랜덤한 반지름
    int radius = rand() % 100 + 10; // 10 ~ 100 사이의 랜덤 반지름

    // Picture Control에 원 그리기
    CClientDC dc(&m_pictureControl);
    CRect rect;
    m_pictureControl.GetClientRect(&rect);

    ClearBackground(dc, rect);  // 배경 지우기

    dc.Ellipse(x1 - radius, y1 - radius, x1 + radius, y1 + radius);

    // 원의 중심에 X자를 표시
    dc.MoveTo(x1 - 10, y1);
    dc.LineTo(x1 + 10, y1);
    dc.MoveTo(x1, y1 - 10);
    dc.LineTo(x1, y1 + 10);

    // 좌표 출력
    CString coordText;
    coordText.Format(_T("(%d, %d)"), x1, y1);
    dc.TextOutW(x1 + 15, y1 + 15, coordText);
}

// Action 버튼 클릭 이벤트 핸들러 (이미지 이동 및 저장 기능 추가)
void CGRIMDlg::OnBnClickedButtonAction()
{
    // X1, Y1, X2, Y2 좌표를 받아옵니다.
    CString strX1, strY1, strX2, strY2;
    m_editX1.GetWindowText(strX1);
    m_editY1.GetWindowText(strY1);
    m_editX2.GetWindowText(strX2);
    m_editY2.GetWindowText(strY2);

    int x1 = _ttoi(strX1);
    int y1 = _ttoi(strY1);
    int x2 = _ttoi(strX2);
    int y2 = _ttoi(strY2);

    int step = 10; // 이동 간격

    for (int i = 0; i <= 100; i += step)
    {
        int x = x1 + (x2 - x1) * i / 100;
        int y = y1 + (y2 - y1) * i / 100;

        // Picture Control에 원 그리기
        CClientDC dc(&m_pictureControl);
        CRect rect;
        m_pictureControl.GetClientRect(&rect);

        ClearBackground(dc, rect);  // 배경 지우기

        dc.Ellipse(x - 20, y - 20, x + 20, y + 20);

        // 원의 중심에 X자를 표시
        dc.MoveTo(x - 10, y);
        dc.LineTo(x + 10, y);
        dc.MoveTo(x, y - 10);
        dc.LineTo(x, y + 10);

        // 좌표 출력
        CString coordText;
        coordText.Format(_T("(%d, %d)"), x, y);
        dc.TextOutW(x + 15, y + 15, coordText);

        // 이미지 저장
        CImage image;
        image.Create(rect.Width(), rect.Height(), 32);
        BitBlt(image.GetDC(), 0, 0, rect.Width(), rect.Height(), dc, 0, 0, SRCCOPY);
        image.ReleaseDC();

        // 저장 경로와 파일 이름 설정
        CString filename;
        filename.Format(_T("image/step%d.jpg"), i);

        // 디렉토리가 없으면 생성
        if (!PathIsDirectory(_T("image"))) {
            CreateDirectory(_T("image"), NULL);
        }

        // 파일 저장
        HRESULT hr = image.Save(filename);
        if (FAILED(hr)) {
            AfxMessageBox(_T("Failed to save image!"));
        }
    }
}

// Open 버튼 클릭 이벤트 핸들러
void CGRIMDlg::OnBnClickedButtonOpen()
{
    // 파일 열기 다이얼로그를 통해 이미지 파일 선택
    CFileDialog dlg(TRUE, _T("jpg"), NULL, OFN_FILEMUSTEXIST, _T("Image Files (*.jpg;*.bmp)|*.jpg;*.bmp|All Files (*.*)|*.*||"));
    if (dlg.DoModal() == IDOK)
    {
        CString filePath = dlg.GetPathName();

        // 이미지 로드
        CImage image;
        image.Load(filePath);

        // Picture 컨트롤에 이미지 표시
        CClientDC dc(&m_pictureControl);
        CRect rect;
        m_pictureControl.GetClientRect(&rect);

        ClearBackground(dc, rect);  // 배경 지우기

        image.StretchBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

        // 선택한 이미지의 중심에 X 표시
        dc.MoveTo(rect.Width() / 2 - 10, rect.Height() / 2);
        dc.LineTo(rect.Width() / 2 + 10, rect.Height() / 2);
        dc.MoveTo(rect.Width() / 2, rect.Height() / 2 - 10);
        dc.LineTo(rect.Width() / 2, rect.Height() / 2 + 10);

        // 좌표 출력
        CString coordText;
        coordText.Format(_T("(%d, %d)"), rect.Width() / 2, rect.Height() / 2);
        dc.TextOutW(rect.Width() / 2 + 15, rect.Height() / 2 + 15, coordText);
    }
}

void CGRIMDlg::OnStnClickedStaticPicture()
{
    // Static 컨트롤이 클릭되었을 때 처리할 내용 (현재는 필요 없음)
}
