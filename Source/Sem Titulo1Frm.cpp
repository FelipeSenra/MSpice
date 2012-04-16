///-----------------------------------------------------------------
///
/// @file      Sem Título1Frm.cpp
/// @author    Felipe
/// Created:   1/8/2011 14:43:42
/// @section   DESCRIPTION
///            Sem_T_tulo1Frm class implementation
///
///------------------------------------------------------------------

#include "Sem Título1Frm.h"

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// Sem_T_tulo1Frm
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(Sem_T_tulo1Frm,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(Sem_T_tulo1Frm::OnClose)
END_EVENT_TABLE()
////Event Table End

Sem_T_tulo1Frm::Sem_T_tulo1Frm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

Sem_T_tulo1Frm::~Sem_T_tulo1Frm()
{
}

void Sem_T_tulo1Frm::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxPanel1 = new wxPanel(this, ID_WXPANEL1, wxPoint(0, 0), wxSize(318, 41));

	WxPanel2 = new wxPanel(this, ID_WXPANEL2, wxPoint(0, 41), wxSize(51, 255));

	WxPanel3 = new wxPanel(this, ID_WXPANEL3, wxPoint(0, 297), wxSize(320, 19));

	WxPanel4 = new wxPanel(this, ID_WXPANEL4, wxPoint(51, 41), wxSize(267, 255));

	WxMenuBar1 = new wxMenuBar();

	WxScrolledWindow1 = new wxScrolledWindow(WxPanel4, ID_WXSCROLLEDWINDOW1, wxPoint(8, 7), wxSize(250, 242), wxVSCROLL | wxHSCROLL);

	SetTitle(wxT("Sem Título1"));
	SetIcon(wxNullIcon);
	SetSize(8,8,327,351);
	Center();
	
	////GUI Items Creation End
}

void Sem_T_tulo1Frm::OnClose(wxCloseEvent& event)
{
	Destroy();
}
