/*
 * Created on Tue Apr 07 2021
 *
 * Copyright (c) 2021 - Math�o Galuba
 */

#include "CContent.h"

CContent::CContent(wxWindow* parent, wxWindowID id, const wxString username, const wxString textMessage, const wxString filePath) : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize)
{
	// Install all supported image handler
	wxInitAllImageHandlers();

	// Update the time atribut to the current system time
	m_contentTime = std::chrono::system_clock::now();
	std::string strTime = "";

	// Time point to string conversion
	// >>> CREDIT : Adrian Maire > https://stackoverflow.com/questions/34857119/how-to-convert-stdchronotime-point-to-string
	{
		std::time_t tt = std::chrono::system_clock::to_time_t(m_contentTime);
		//std::tm tm = *std::gmtime(&tt); // GMT (UTC)
		std::tm tm = *std::localtime(&tt); // Local time
		std::stringstream ss;
		std::string format = "%Y-%m-%d %H:%M:%S";
		ss << std::put_time(&tm, format.c_str());
		strTime = ss.str();
	}

	// Initialize the username string atribute
	m_username = username;

	// Initialize the message string atribut
	m_textMessage = textMessage;

	// Initialize the file path string atribut
	m_filePath = filePath;

	// ========== GUI GENERATION ========== //
	
	// Set panel background color
	this->SetBackgroundColour(wxColour(220, 230, 240));

	// Sizer
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

	// Static text
	// Time label
	wxStaticText* timeLabel = new wxStaticText(this, wxID_ANY, strTime, wxDefaultPosition, wxSize(100, -1), wxALIGN_RIGHT);
	timeLabel->SetForegroundColour(wxColor(90, 100, 100));
	timeLabel->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

	// Username label
	wxStaticText* usernameLabel = new wxStaticText(this, wxID_ANY, m_username, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	usernameLabel->SetForegroundColour(wxColor(90, 100, 100));
	usernameLabel->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
	
	// Structure the two previous labels in a sizer
	headerSizer->Add(usernameLabel, 2, wxEXPAND);
	headerSizer->Add(timeLabel, 1, wxEXPAND);

	// Main message
	wxStaticText* mainMessage = new wxStaticText(this, wxID_ANY, m_textMessage, wxDefaultPosition, wxDefaultSize, wxTE_BESTWRAP | wxTE_MULTILINE);
	mainMessage->SetForegroundColour(wxColor(20, 20, 20));
	mainMessage->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
	mainMessage->Wrap(this->GetSize().GetWidth());

	// Add everything to the sizer
	mainSizer->Add(headerSizer, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(mainMessage, 1, wxEXPAND | wxALL, 5);

	// File management
	// >>> CREDIT : Benedict > https://forums.wxwidgets.org/viewtopic.php?t=6664
	if (m_filePath != "")
	{
		// Install all supported image handler
		wxInitAllImageHandlers();

		// Spacxer in the sizer
		mainSizer->AddSpacer(25);
		
		// Get the file name
		size_t npos = m_filePath.rfind("/");
		npos = m_filePath.rfind("\\");
		wxString filename = m_filePath.substr(npos + 1);

		// File name label
		wxStaticText* fileNameLabel = new wxStaticText(this, wxID_ANY, filename, wxDefaultPosition, wxSize(100, -1), wxALIGN_RIGHT);
		fileNameLabel->SetForegroundColour(wxColor(90, 100, 100));
		fileNameLabel->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

		mainSizer->Add(fileNameLabel);

		//Image render

		wxBitmap* bitMapImage = new wxBitmap(m_filePath, wxBITMAP_TYPE_JPEG);
		
		wxPaintDC dc(this);

		float fWScale = 1.0f;   // horizontal scaling factor
		float fHScale = 1.0f;   // vertical scaling factor
		int iImageH = -1;       // the bitmap's height
		int iImageW = -1;       // the bitmap's width
		int iThisH = 50;        // the panel's height
		int iThisW = -1;        // the panel's width

		iImageH = (*bitMapImage).GetHeight();
		iImageW = (*bitMapImage).GetWidth();

		GetSize(&iThisW, &iThisH);

		if ((iImageH > 0) && (iImageW > 0))
		{
			// calculate the scaling factor for the 2 dimensions
			fHScale = (float)iThisH / (float)iImageH;
			fWScale = (float)iThisW / (float)iImageW;

			// always take the smaller scaling factor,
			// so that the bitmap will always fit into the panel's paintable area
			if (fHScale < fWScale)
			{
				fWScale = fHScale;
			}
			else
			{
				fHScale = fWScale;
			}
		}

		dc.SetUserScale(fHScale, fWScale);
		dc.DrawBitmap(*bitMapImage, 0, 0, false);
		
		//mainSizer->Add(bitMapImage);
	}

	// Sizer structuration
	this->SetSizer(mainSizer);

	// ======== END GUI GENERATION ======== //
}

CContent::~CContent()
{
}
