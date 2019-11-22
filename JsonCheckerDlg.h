
// JsonCheckerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CJsonCheckerDlg �Ի���
class CJsonCheckerDlg : public CDialogEx
{
// ����
public:
	CString m_sJsonFile; // JSON �ļ�
	CJsonCheckerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JSONCHECKER_DIALOG, IDH = IDR_HTML_JSONCHECKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnFileSettings();
	afx_msg void OnFileQuit();
	CComboBox m_ComVendorTemplate;
	int m_nVendorTemplate;
	CComboBox m_ComStandardTemplate;
	int m_nStandardTemplate;
	CEdit m_EditJSON;
	CString m_sJSON; // JSON�ļ�����
	CEdit m_EditTarget;
	CString m_sTarget; // ת�����
	afx_msg void OnBnClickedConvert();
	afx_msg void OnFileOpen();
	CComboBox m_ComInterfaceType;
	int m_nInterfaceType;
	afx_msg void OnEnChangeJSON();
};
