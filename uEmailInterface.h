﻿//---------------------------------------------------------------------------

#ifndef uEmailInterfaceH
#define uEmailInterfaceH
#include <System.Classes.hpp>
#include <IdMessageBuilder.hpp>

//---------------------------------------------------------------------------
#endif

 __interface INTERFACE_UUID("{81025DF3-DE24-4687-A339-EAB21D93B993}") IMailer : public System::IInterface
 {
	virtual String GetSmtpServer() = 0;
	virtual bool SetSmtpServer(String SmtpServer) = 0;

	virtual String GetSmtpLogin() = 0;
	virtual bool SetSmtpLogin(String SmtpLogin) = 0;

	virtual String GetSmtpPassword() = 0;
	virtual bool SetSmtpPassword(String SmtpPassword) = 0;

	virtual int GetSmtpPort() = 0;
	virtual bool SetSmtpPort(int SmtpPort) = 0;

	virtual String GetEmailFrom() = 0;
	virtual bool SetEmailFrom(String EmailFrom)= 0;

	virtual TStringList * GetEmailTo() = 0;        //Lista adresatów
	virtual bool SetEmailTo(TStringList * EmailToList)= 0;
	virtual bool AddEmailTo(String EmailTo)= 0;

	virtual String GetSubject() = 0;
	virtual bool SetSubject(String Subject)= 0;

	virtual String GetEmailBodyTXT() = 0;
	virtual bool SetEmailBodyTXT(String EmailBodyTXT) = 0;

	virtual String GetEmailBodyHTML() = 0;
	virtual bool SetEmailBodyHTML(String EmailBodyHTML) = 0;

	virtual String GetBodyEncoding() = 0;
	virtual bool SetBodyEncoding(String Encoding) = 0;

//	virtual TIdMessageBuilderHtml * GetAttachmentList() = 0;
//	virtual TIdMessageBuilderHtml * SetAttachmentList(TStringList * List) = 0;

	virtual TIdMessageBuilderHtml * GetMessageBuilder() = 0;
	virtual bool SetMessageBuilder(TIdMessageBuilderHtml * Builder) = 0;
	virtual bool IsMessageBuilderPresent() = 0;
//	virtual bool AddAttachment(String Path) = 0;
//	virtual bool AddAttachment(TFileStream * Stream) = 0;
//	virtual bool RemoveAttachment(int Index) = 0;

	virtual bool EmailSent(int Index, bool IsSendSuccesful) = 0;
	virtual bool BeforeEmailSend() = 0;

	//virtual ~IMailer() = 0;

	//virtual int GetEmailIndex() = 0;
	//dodac funkcje ktore wykonaja sie przed i po wysylce maile (przed mozna cos zmienic, po - zapisac do bazy etc.)
	//kazdy mail musi przed wyslaniem wypelnic dokumenty skryptowe, a po wysylce zapisac sie do bazy danych
	//DODANE EmailSent(int Index) oraz BeforeEmailSend()
 };

typedef System::DelphiInterface<IMailer> _di_IMailer;