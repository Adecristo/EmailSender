//---------------------------------------------------------------------------

#ifndef EmailSenderThreadH
#define EmailSenderThreadH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <System.Classes.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdSMTPBase.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <IdMessage.hpp>
#include "uEmailSenderFromDB.h"
#include "uImportProgress.h"
//---------------------------------------------------------------------------
class TMailSenderThread : public TThread
{
private:
	String _LogMsg, _Msg;

	TIdSMTP *IdSmtp;
	TIdMessage *IdMess;
	TIdSSLIOHandlerSocketOpenSSL *IdSSL;
	TInterfaceList * _List;

	TStringList * _Log;
	int _EmailSent;
	bool IsEmailSent;

	bool PrepareLoginInfo(_di_IMailer EI);
	bool PrepareEmail(_di_IMailer EI);
	bool SendEmail();
	void __fastcall UpdateProgressBar();
	void __fastcall Alert();

protected:
	void __fastcall Execute();
	void __fastcall AddLogLine(String Line);
public:
	TStringList * GetLog();
	__fastcall TMailSenderThread(bool CreateSuspended, TInterfaceList * List);


};
//---------------------------------------------------------------------------
#endif
