///-----------------------------------------------------------------
///
/// @file      Sem Título1Frm.h
/// @author    Felipe
/// Created:   1/8/2011 14:43:42
/// @section   DESCRIPTION
///            Sem_T_tulo1Frm class declaration
///
///------------------------------------------------------------------

#ifndef __SEM_T_TULO1FRM_H__
#define __SEM_T_TULO1FRM_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/scrolwin.h>
#include <wx/menu.h>
#include <wx/panel.h>
////Header Include End

////Dialog Style Start
#undef Sem_T_tulo1Frm_STYLE
#define Sem_T_tulo1Frm_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class Sem_T_tulo1Frm : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		Sem_T_tulo1Frm(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Sem Título1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = Sem_T_tulo1Frm_STYLE);
		virtual ~Sem_T_tulo1Frm();
		
	private:
		//Do not add custom control declarations between
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxScrolledWindow *WxScrolledWindow1;
		wxMenuBar *WxMenuBar1;
		wxPanel *WxPanel4;
		wxPanel *WxPanel3;
		wxPanel *WxPanel2;
		wxPanel *WxPanel1;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXSCROLLEDWINDOW1 = 1020,
			ID_WXPANEL4 = 1017,
			ID_WXPANEL3 = 1016,
			ID_WXPANEL2 = 1015,
			ID_WXPANEL1 = 1014,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
		
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
