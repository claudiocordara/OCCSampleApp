
// OCCSampleApp.h : main header file for the OCCSampleApp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COCCSampleAppApp:
// See OCCSampleApp.cpp for the implementation of this class
//

class COCCSampleAppApp : public CWinApp
{
public:
	COCCSampleAppApp();

	Handle(Graphic3d_GraphicDriver) GetGraphicDriver() { return m_GraphicDriver; }

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	Handle(Graphic3d_GraphicDriver) m_GraphicDriver;		// Graphic Driver
};

extern COCCSampleAppApp theApp;
