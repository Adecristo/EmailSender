//---------------------------------------------------------------------------

#ifdef WIN32
//	Initialize Winsock
//	WSADATA wsaData;
//	WSAStartup(MAKEWORD(2,2), &wsaData);
#endif

#include <System.hpp>
#pragma hdrstop

#include "EmailSenderThread.h"
#include <IdMessageBuilder.hpp>

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TMailSenderThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TMailSenderThread::TMailSenderThread(bool CreateSuspended, TInterfaceList * List)
	: TThread(CreateSuspended)
{
	_Log = new TStringList();
	IdSSL = new TIdSSLIOHandlerSocketOpenSSL();
	IdMess = new TIdMessage();
	IdSmtp = new TIdSMTP();

	_List = List;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		ShowMessage("WSA Failed!");
	}
	String Dir = "temp\\EmailTemp";
	if( !DirectoryExists(Dir,true))
		CreateDir(Dir);
}
//---------------------------------------------------------------------------

void __fastcall TMailSenderThread::Execute()
{
	FreeOnTerminate=true;
	_LogMsg = "SMTP:	Trying send mail.";
	AddLogLine(_LogMsg);
	_di_IMailer Temp = _List->Items[0];
	PrepareLoginInfo(Temp);
	try
	{
		_LogMsg = "SMTP:	Connecting...";
		AddLogLine(_LogMsg);

		if(!IdSmtp->Connected()) IdSmtp->Connect();

		_LogMsg = "SMTP:	Connected.";
		AddLogLine(_LogMsg);
		try
		{
			_EmailSent = 1;
			for (int i = 0; i < _List->Count; i++) {
				_LogMsg = "Message:	"+(String)_EmailSent + " from "+ (String)_List->Count+".";
				AddLogLine(_LogMsg);

				Temp = _List->Items[i];

				Temp->BeforeEmailSend();
				PrepareEmail(Temp);

				_LogMsg = "SMTP:	Sending Email...";
				AddLogLine(_LogMsg);
				try {
					//IdSmtp->Send(IdMess);

					_LogMsg = "SMTP:	Sent.";
					AddLogLine(_LogMsg);

					Temp->EmailSent(_EmailSent,true);
				} catch (Exception &e) {
					_LogMsg = "[!] Problem with number "+(String)_EmailSent+"! Exception: " + e.Message;
					AddLogLine(_LogMsg);

					Temp->EmailSent(_EmailSent,false);
				}
				Synchronize(UpdateProgressBar);
				_EmailSent++;
			}
        }
        __finally
		{
			_LogMsg = "SMTP:	Disconecting...";
			AddLogLine(_LogMsg);

			IdSmtp->Disconnect();
			WSACleanup();

			_LogMsg = "SMTP:	Disconected.";
			AddLogLine(_LogMsg);
			_Log->SaveToFile("temp\\EmailTemp\\EmailLog.txt");

			IdSSL->Close();
			delete _List;
			delete IdMess;
			delete IdSSL;
			delete IdSmtp;
		}
    }
	catch(const Exception &e)
	{
		_Msg = e.Message;
		_LogMsg = "SMTP:	Something wrong... Message: "+e.Message;
		AddLogLine(_LogMsg);

		Synchronize(Alert);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMailSenderThread::AddLogLine(String Line){
	_Log->Add(Line);
}

bool TMailSenderThread::PrepareEmail(_di_IMailer EI){ //EI - Email Interface

	_LogMsg = "Thread:	Prepare Email...";
	AddLogLine(_LogMsg);

	String		EmailFrom 		= 	EI->GetEmailFrom(),
				Subject 		=   EI->GetSubject(),
				BodyMessageTXT 	= 	EI->GetEmailBodyTXT(),
				BodyMessageHTML = 	EI->GetEmailBodyHTML(),
				Encoding 		= 	EI->GetBodyEncoding();
	TStringList *EmailTo 		= 	EI->GetEmailTo();

	IdMess->ContentType = "multipart/mixed";
	IdMess->CharSet 	= Encoding;
	IdMess->Encoding 	= meMIME;
	IdMess->Priority 	= mpNormal;

	IdMess->From->Text 	= EmailFrom;
	IdMess->Recipients->EMailAddresses = EmailTo->CommaText;

	IdMess->Subject		= Subject;

	TIdMessageBuilderHtml * Builder = EI->GetMessageBuilder();
	Builder->FillMessage(IdMess);

	_LogMsg = "Thread:	Prepared.";
	AddLogLine(_LogMsg);
	return true;
}

bool TMailSenderThread::PrepareLoginInfo(_di_IMailer EI){
	String 		Host 	  	   	= 	EI->GetSmtpServer(),
				Login 			= 	EI->GetSmtpLogin(),
				Pass 			= 	EI->GetSmtpPassword();
	int 		Port 			= 	EI->GetSmtpPort();

	IdSSL->Host = Host;
	IdSSL->Port = Port;
	IdSSL->SSLOptions->Method = sslvTLSv1_2;
	IdSSL->SSLOptions->SSLVersions << sslvTLSv1_2 << sslvTLSv1_1 << sslvTLSv1;

	IdSmtp->Host 		= Host;
	//IdSmtp->Port 		= Port;  //Port jest ustawiany w IdSSL, nie potrzeba ustawiac go w IdSmtp
	IdSmtp->Username 	= Login;
	IdSmtp->Password 	= Pass;
	IdSmtp->IOHandler 	= IdSSL;
	IdSmtp->UseTLS 		<< utUseImplicitTLS;
	return true;
}

TStringList * TMailSenderThread::GetLog(){
	return _Log;
}

void __fastcall TMailSenderThread::UpdateProgressBar(){
	int Procent = (_EmailSent*100)/_List->Count;
	frmImportProgress->pgbMain->Position = Procent;
	frmImportProgress->lblMain->Caption = Rs_EmailSendingInProgress+"("+(String)_EmailSent+"/"+(String)_List->Count+")!";
}

void __fastcall TMailSenderThread::Alert(){
	ShowMessage(Rs_MsgProblem+_Msg);
}
