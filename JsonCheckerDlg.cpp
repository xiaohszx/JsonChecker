
// JsonCheckerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "JsonChecker.h"
#include "JsonCheckerDlg.h"
#include "afxdialogex.h"
#include <map>
#include <vector>
// ���¡��Ŀ:https://github.com/yuanyuanxiang/public
// �ŵ�ͬ��Ŀ¼
#include "../public/CodeTransform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// ģ��<ģ����, ģ��·��>
std::map<CString, CString> MAP;
// ģ��<��ţ�ģ����>
std::vector<CString> Template;
#define LENGTH 11
#define PREFIX L"single-"
// ��������
const CString Interfaces[LENGTH] = { _T("faces"),_T("persons"),
	_T("motorvehicles"),_T("nonmotorvehicles"),	_T("things"), _T("scenes"), 
	_T("videolabels"),_T("videoslices"), _T("images"), _T("files"), _T("cases") };
// ��������ID
const CString IDList[LENGTH] = { _T("\"faceid\""),_T("\"personid\""),
	_T("\"motorvehicleid\""),_T("\"nonmotorvehicleid\""),	_T("\"thingid\""), _T("\"sceneid\""),
	_T("\"videolabelid\""),_T("\"videosliceid\""), _T("\"imageid\""), _T("\"fileid\""), _T("\"caseid\"") };
const char LogFile[] = "./error.log"; // ��־�ļ�
const char Output[] = "./output.json"; // ת��������
char TemplateDir[_MAX_PATH] = "./template"; // ģ��Ŀ¼

bool JsonChanged = false;

// GA1400->GAT1400
CString mapFunc(const CString &ga1400) {
	return ga1400 == _T("GA1400") ? _T("GAT1400") : ga1400;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CJsonCheckerDlg �Ի���

BEGIN_DHTML_EVENT_MAP(CJsonCheckerDlg)
END_DHTML_EVENT_MAP()


CJsonCheckerDlg::CJsonCheckerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_JSONCHECKER_DIALOG, pParent)
	, m_nVendorTemplate(0)
	, m_nStandardTemplate(0)
	, m_sJSON(_T(""))
	, m_sTarget(_T(""))
	, m_nInterfaceType(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJsonCheckerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComVendorTemplate);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nVendorTemplate);
	DDX_Control(pDX, IDC_COMBO2, m_ComStandardTemplate);
	DDX_CBIndex(pDX, IDC_COMBO2, m_nStandardTemplate);
	DDX_Control(pDX, IDC_EDIT1, m_EditJSON);
	DDX_Text(pDX, IDC_EDIT1, m_sJSON);
	DDX_Control(pDX, IDC_EDIT2, m_EditTarget);
	DDX_Text(pDX, IDC_EDIT2, m_sTarget);
	DDX_Control(pDX, IDC_COMBO3, m_ComInterfaceType);
	DDX_CBIndex(pDX, IDC_COMBO3, m_nInterfaceType);
}

BEGIN_MESSAGE_MAP(CJsonCheckerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_FILE_SETTINGS, &CJsonCheckerDlg::OnFileSettings)
	ON_COMMAND(ID_FILE_QUIT, &CJsonCheckerDlg::OnFileQuit)
	ON_BN_CLICKED(IDC_CONVERT, &CJsonCheckerDlg::OnBnClickedConvert)
	ON_COMMAND(ID_FILE_OPEN32773, &CJsonCheckerDlg::OnFileOpen)
	ON_EN_CHANGE(IDC_EDIT1, &CJsonCheckerDlg::OnEnChangeJSON)
	ON_COMMAND(ID_FILE_GENERATE, &CJsonCheckerDlg::OnFileGenerate)
END_MESSAGE_MAP()

// �Ӹ���Ŀ¼��ȡģ��
std::map<CString, CString> GetTemplate(const char *dir) {
	std::map<CString, CString> strPath;
	CFileFind ff;
	char filter[256];
	sprintf_s(filter, "%s\\*.json", dir);
	BOOL ret = ff.FindFile(CString(filter));
	while (ret)
	{
		ret = ff.FindNextFile();
		strPath.insert(std::make_pair(ff.GetFileTitle(), ff.GetFilePath()));
	}
	ff.Close();
	return strPath;
}

// CJsonCheckerDlg ��Ϣ�������

CString getPathByTemplate(CString name) {
	for (std::map<CString, CString>::iterator i = MAP.begin(); i != MAP.end(); ++i)
	{
		if (i->first == name)
			return i->second;
	}
	return _T("GA1400");
}

BOOL CJsonCheckerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_NORMAL);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetPrivateProfileStringA("settings", "templateDir", "./template",
		TemplateDir, _MAX_PATH, "./settings.ini");
	if (!init(TemplateDir)) // ��ʼ��ģ���ļ�
	{
		MessageBox(_T("Template file \"GA1400\" is missing."));
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

bool CJsonCheckerDlg::init(const char *path) {
	MAP = GetTemplate(path);
	int n = 0, m = 0;
	bool standardExist = false;
	while(m_ComVendorTemplate.GetCount())m_ComVendorTemplate.DeleteString(0);
	while (m_ComStandardTemplate.GetCount())m_ComStandardTemplate.DeleteString(0);
	for (std::map<CString, CString>::iterator i = MAP.begin(); i != MAP.end(); ++i, ++n)
	{
		m_ComVendorTemplate.InsertString(n, i->first);
		m_ComStandardTemplate.InsertString(n, i->first);
		Template.push_back(i->first);
		if (i->first == _T("GA1400"))
		{
			standardExist = true;
			m = n;
		}
	}
	if (MAP.size())
	{
		m_ComVendorTemplate.SetCurSel(0);
		m_ComStandardTemplate.SetCurSel(m);
	}
	while (m_ComInterfaceType.GetCount())m_ComInterfaceType.DeleteString(0);
	m_ComInterfaceType.InsertString(0, _T("auto(non-collection)"));
	m_ComInterfaceType.SetCurSel(0);
	for (int i = 1; i <= LENGTH; ++i)
	{
		m_ComInterfaceType.InsertString(i, Interfaces[i - 1]);
	}
	for (int i = 1; i <= LENGTH; ++i)
	{
		m_ComInterfaceType.InsertString(LENGTH + i, PREFIX + Interfaces[i - 1]);
	}
	return standardExist;
}

void CJsonCheckerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CJsonCheckerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CJsonCheckerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CJsonCheckerDlg::OnFileSettings()
{
	char szPath[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = m_hWnd;
	bi.lpszTitle = _T("Select template dir");
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDListA(lp, szPath))
	{
		if (init(szPath))
		{
			strcpy_s(TemplateDir, szPath);
			WritePrivateProfileStringA("settings", "templateDir", TemplateDir, "./settings.ini");
		} else {
			MessageBox(_T("Template file \"GA1400\" is missing."));
		}
	}
}

void CJsonCheckerDlg::OnFileQuit()
{
	SendMessage(WM_CLOSE, 0, 0);
}

// ��ȡ����JSON�ļ�
CString ReadJSON(CString path) {
	CFile file;
	if (!file.Open(path, CFile::modeRead, NULL)) {
		AfxMessageBox(_T("Read JSON file failed."));
		return _T("");
	}
	UINT len = file.GetLength();
	char *buf = new char[len + 1];
	buf[len] = 0;
	file.Read(buf, len);
	file.Close();
	int Length = 0;
	CString text = UTF8Convert2Unicode(buf, Length);
	if (-1 == text.Find('\r')) {
		text.Replace(_T("\n"), _T("\r\n"));
	}
	delete[] buf;
	return text;
}

bool WriteJSON(CString text, CString path) {
	CFile file;
	if (!file.Open(path, CFile::modeWrite, NULL)) {
		AfxMessageBox(_T("Create JSON file failed."));
		return false;
	}
	int Length = 0;
	const char *buf = UnicodeConvert2UTF8(text, Length);
	if (buf)
	{
		file.Write(buf, Length-1);
		file.Close();
		delete[] buf;
	}

	return true;
}

// ͨ��JSON���Զ���ȡ�ӿ�����.���ԷǼ���������Ч.
const char* GetJsonType(CString jsonPath) {
	static char type[48];
	memset(type, 0, sizeof(type));
	CString json = ReadJSON(jsonPath);
	CString lower = json.MakeLower();
	for (int i = 0; i < LENGTH; ++i) {
		if (IDList[i].IsEmpty()) {
			continue;
		}
		if (-1 != lower.Find(IDList[i])){
			USES_CONVERSION;
			strcpy_s(type, W2A(Interfaces[i]));
			break;
		}
	}
	return type;
}

void CJsonCheckerDlg::OnBnClickedConvert()
{
	if (m_sJsonFile.IsEmpty()) {
		MessageBox(_T("Please select a JSON file."));
		return;
	}
	if (JsonChanged) {
		JsonChanged = false;
		UpdateData(TRUE);
		if (!WriteJSON(m_sJSON, m_sJsonFile))
			TRACE("===> Save JSON failed.\n");
	}
	
	char buf[512];
	USES_CONVERSION;
	int n = m_ComVendorTemplate.GetCurSel();
	int m = m_ComInterfaceType.GetCurSel();
	int k = m_ComStandardTemplate.GetCurSel();
	sprintf_s(buf, 
		".\\Checker.exe -debug=false -templateDir=\"%s\" "\
		"-json=\"%s\" -template=\"%s\" -std=\"%s\" -interface=\"%s\"", 
		TemplateDir, W2A(m_sJsonFile), W2A(mapFunc(Template[n])), W2A(mapFunc(Template[k])),
		m == 0 ? GetJsonType(m_sJsonFile) : 
		(m <= LENGTH ? W2A(Interfaces[m-1]):W2A(PREFIX+Interfaces[m-1-LENGTH]))
	);
	// ����Golang Checker
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW;
	CString arg = CString(buf);
	LPWSTR p = (LPWSTR)(LPCWSTR)arg;
	BeginWaitCursor();
	BOOL fRet = CreateProcess(_T(".\\Checker.exe"), p,
		NULL, FALSE, NULL, NULL, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	EndWaitCursor();
	if (fRet)
	{
		BOOL b = PathFileExists(CString(Output));
		m_sTarget = ReadJSON(CString(b ? Output : LogFile));
		m_EditTarget.SetWindowText(m_sTarget);
		if (!(b = PathFileExists(CString(Output)))) {
			MessageBox(_T("Convert JSON file failed."));
		}
	}else
		MessageBox(_T("Create process failed."));
}

void CJsonCheckerDlg::OnFileOpen()
{
	TCHAR szFilter[] = _T("JSON files(*.json)|*.json|All files(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("json"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		szFilter, this);
	if (IDOK == dlg.DoModal())
	{
		JsonChanged = false;
		m_sJsonFile = dlg.GetPathName();
		m_sJSON = ReadJSON(m_sJsonFile);
		m_EditJSON.SetWindowText(m_sJSON);
		m_sTarget.Empty();
		m_EditTarget.SetWindowText(m_sTarget);
	}
}

void CJsonCheckerDlg::OnEnChangeJSON()
{
	JsonChanged = true;
	TRACE("===> JSON is changed.\n");
}


void CJsonCheckerDlg::OnFileGenerate()
{
	int n = m_ComVendorTemplate.GetCurSel();
	int m = m_ComInterfaceType.GetCurSel();
	USES_CONVERSION;
	CString t = getPathByTemplate(Template[n]);
	const char *temp = W2A(t);
	const char *rules = m == 0 ? "faces" :
		(m <= LENGTH ? W2A(Interfaces[m - 1]) : W2A(PREFIX + Interfaces[m - 1 - LENGTH]));
	char buf[1024];
	sprintf_s(buf, ".\\generate.exe -debug=false -template=\"%s\" -rules=\"%s\"", temp, rules);
	// ����Golang Checker
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW;
	CString arg = CString(buf);
	LPWSTR p = (LPWSTR)(LPCWSTR)arg;
	BeginWaitCursor();
	BOOL fRet = CreateProcess(_T(".\\generate.exe"), p,
		NULL, FALSE, NULL, NULL, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	EndWaitCursor();
	if (fRet)
	{
		m_sJsonFile = CString("./generate.json");
		BOOL b = PathFileExists(m_sJsonFile);
		if (b) {
			m_sJSON = ReadJSON(m_sJsonFile);
			m_EditJSON.SetWindowText(m_sJSON);
		}
		if (!(b = PathFileExists(m_sJsonFile))) {
			MessageBox(_T("Generate JSON failed."));
		}
	}
	else
		MessageBox(_T("Create process failed."));
}
