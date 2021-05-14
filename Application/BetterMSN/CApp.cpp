/*
 * Created on Tue Apr 07 2021
 *
 * Copyright (c) 2021 - Math�o G & Sahel H - All Right Reserved
 *
 * Licensed under the Apache License, Version 2.0
 * Available on GitHub at https://github.com/Paracetamol56/Cpp-Network-App
 */

#include "CApp.h"

// Entry point definition
wxIMPLEMENT_APP(CApp);
wxDECLARE_APP(CApp);

/// <summary>
/// Constructor
/// </summary>
CApp::CApp()
{
}

/// <summary>
/// Destructor
/// </summary>
CApp::~CApp()
{
    closesocket(sock);
    WSACleanup();
}

/// <summary>
/// Main launcher methode
/// </summary>
/// <returns>
/// Always true
/// </returns>
bool CApp::OnInit()
{
    m_mainFrame = new CMain;
    m_mainFrame->Show();

    m_mainFrame->getSettings()->atach(this);
    m_mainFrame->atach(this);

    return true;
}

/// <summary>
/// Update the state (server/client) when the Ok button is pressed in settings
/// </summary>
void CApp::update(Notification notif)
{
    switch (notif)
    {
        // CASE : update the state (Client/Server)
    case Notification_State:
    {
        // Destroying the old socket

        closesocket(sock);
        WSACleanup();

        // Network initialization

        // IF SERVER SIDE
        if (m_mainFrame->getSettings()->getStatusIsServer())
        {
            // Change the main window title
            m_mainFrame->SetTitle("Better MSN - Serveur");

            // Socket initializing
            WSAStartup(MAKEWORD(2, 0), &wsa);

            sin.sin_family = AF_INET;
            sin.sin_addr.s_addr = INADDR_ANY;
            sin.sin_port = htons(m_mainFrame->getSettings()->getPort());

            // Socket creation
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            // Socket configuration to listen the port
            bind(sock, (SOCKADDR*)&sin, sizeof(sin));

            // No queue
            listen(sock, 0);

            m_error = 0;
            m_listen = false;
        }
        // IF CLIENT SIDE
        else
        {
            // Change the main window title
            m_mainFrame->SetTitle("Better MSN - Client");

            // Socket initializing
            WSAStartup(MAKEWORD(2, 0), &wsa);

            sin.sin_family = AF_INET;
            inet_pton(AF_INET, m_mainFrame->getSettings()->getIPAdressChar(), &sin.sin_addr.s_addr);
            sin.sin_port = htons(m_mainFrame->getSettings()->getPort());

            // Socket creation
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            // Socket connection
            if (connect(sock, (SOCKADDR*)&sin, sizeof(sin)))
            {
                wxMessageDialog ErrorEmptyDialog(nullptr, "Socket connection failed", "ERROR", wxICON_STOP | wxOK_DEFAULT | wxCENTER, wxDefaultPosition);
                ErrorEmptyDialog.ShowModal();
                exit(0);
            }

            m_error = 0;
            m_listen = true;
        }
    }
        break;
        // CASE : send a message
    case Notification_Send:
        if(m_listen == false)
        {
            CDataStructure ServerData(m_mainFrame->getSettings()->getUsername(), m_mainFrame->getInputText());

            err = send(sock, (char*)&ServerData, sizeof(ServerData), 0);
            if (err >= -1)
            {
                wxMessageDialog ErrorEmptyDialog(nullptr, "sendind error", "ERROR", wxICON_STOP | wxOK_DEFAULT | wxCENTER, wxDefaultPosition);
                ErrorEmptyDialog.ShowModal();
            }
            m_listen = !m_listen;
            Listen();
        }
        else
        {
            wxMessageDialog ErrorEmptyDialog(nullptr, "This is not your turn", "ERROR", wxICON_WARNING | wxOK_DEFAULT | wxCENTER, wxDefaultPosition);
            ErrorEmptyDialog.ShowModal();
        }
        break;
    default:
        break;
    }
}

void CApp::Listen()
{
    while (m_listen)
    {
        m_sinsize = sizeof(sin);
        if ((sock = accept(sock, (SOCKADDR*)&sin, &m_sinsize)) != INVALID_SOCKET)
        {
            if (err > -1)
            {
                CDataStructure ClientData;
                recv(sock, (char*)&ClientData, sizeof(ClientData), 0);
                m_mainFrame->addContent(ClientData.m_name, ClientData.m_message);
                m_listen = !m_listen;
            }
            else
            {
                wxMessageDialog ErrorEmptyDialog(nullptr, "Invalid socket", "ERROR", wxICON_STOP | wxOK_DEFAULT | wxCENTER, wxDefaultPosition);
                ErrorEmptyDialog.ShowModal();
                closesocket(sock);
            }
        }
    }
}
