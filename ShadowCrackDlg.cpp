// ShadowCrackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowCrack.h"
#include "ShadowCrackDlg.h"

#include <stdio.h>
#define NUMBER 3
#define FILESIZE 3646464

struct patch_table {
	long offset;
	unsigned char oldvalue;
	unsigned char newvalue;
} pt[NUMBER];

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CShadowCrackDlg dialog



CShadowCrackDlg::CShadowCrackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShadowCrackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShadowCrackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShadowCrackDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTNSOBRE, OnBnClickedBtnsobre)
	ON_BN_CLICKED(IDC_BTNCRACKEAR, OnBnClickedBtncrackear)
END_MESSAGE_MAP()


// CShadowCrackDlg message handlers

BOOL CShadowCrackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// Inicialização da tabela de locais a alterar
	pt[0].offset = 0x8F29A;
	pt[0].newvalue = 0x3;
	pt[0].oldvalue = 0x5;
	pt[1].offset = 0x8F2CC;
	pt[1].newvalue = 0x90;
	pt[1].oldvalue = 0x75;
	pt[2].offset = 0x8F2CD;
	pt[2].newvalue = 0x90;
	pt[2].oldvalue = 0x05;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShadowCrackDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShadowCrackDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShadowCrackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShadowCrackDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CShadowCrackDlg::OnBnClickedBtnsobre()
{
	CAboutDlg about;
	about.DoModal();
}

void CShadowCrackDlg::OnBnClickedBtncrackear()
{
	CFileDialog arq(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, "Arquivo \"Shadows.exe\"|Shadows.exe||", NULL);
	FILE *shadows;
	long size;
	int i;

	if (arq.DoModal() == IDOK)
	{
		shadows = fopen(arq.GetPathName(), "rb+");
		if (!shadows) {
			MessageBox("Erro ao abrir o arquivo!", "Erro", MB_OK + MB_ICONERROR);
			return;
		}

		fseek(shadows, 0, SEEK_END);
		size = ftell(shadows);
		rewind(shadows);

		if (size != FILESIZE) {
			MessageBox("Arquivos de tamanhos diferentes!", "Erro", MB_OK + MB_ICONERROR);
			fclose(shadows);
			return;
		}

		fseek(shadows, pt[0].offset, SEEK_SET);
		
		// Arquivo crackeado
		if (fgetc(shadows) == pt[0].newvalue) {
			int op = MessageBox("Arquivo crackeado. Deseja restaurar?", "Pergunta", MB_YESNO + MB_ICONQUESTION);
			
			if (op == IDYES) {
				for (i = 0; i < NUMBER; i++) {
					fseek(shadows, pt[i].offset, SEEK_SET);
					fputc(pt[i].oldvalue, shadows);
				}

				MessageBox("Arquivo restaurado!", "Informação", MB_OK + MB_ICONINFORMATION);
				fclose(shadows);
			}

			return;
		}

		for (i = 0; i < NUMBER; i++) {
			fseek(shadows, pt[i].offset, SEEK_SET);
			fputc(pt[i].newvalue, shadows);
		}

		MessageBox("Arquivo crackeado!", "Informação", MB_OK + MB_ICONINFORMATION);
		fclose(shadows);
	}
}
