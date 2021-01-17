﻿//---------------------------------------------------------------------------

#pragma hdrstop

#include "uEmailSenderFromDB.h"
#include "uConnectionManager.h"
#include <FireDAC.Comp.Client.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)

 TEmailDB::TEmailDB(int MailID){
	//do uzupełnienia z formatki userowej
	_SmtpServer ="smtp.wp.pl";
	_SmtpLogin = "adecristo";
	_SmtpPassword = "___";
	_SmtpPort = 465;
	_Encoding = "UTF-8";


	_EmailToAdressList = new TStringList();
	_Builder = new TIdMessageBuilderHtml();
	_AttachmentList = new TStringList();
	_AttachIdList = new TStringList();

	_MailID = MailID;
	
	fdqMain = new TFDQuery(NULL);
	fdqMain->Connection = ConnectionManager->fdcMain;

	if(MailID != 0){
		LoadEmail(MailID);
	}
 }

 //cos sie gryzie destruktor z wirtualnym czy cos nie wiem
 //obiekty te są kasowane w funkcji EmailSent (po zapisie w bazie danych)
//TEmailDB::~TEmailDB(){
//	delete fdqMain;
//	delete _Builder;
//	delete _AttachmentList;
//}
String TEmailDB::GetSmtpServer(){
	return _SmtpServer;
}

bool TEmailDB::SetSmtpServer(String SmtpServer){
	_SmtpServer = SmtpServer;
}

String TEmailDB::GetSmtpLogin(){
	return _SmtpLogin;
}
bool TEmailDB::SetSmtpLogin(String SmtpLogin){
	_SmtpLogin = SmtpLogin;
}

String TEmailDB::GetSmtpPassword(){
	return _SmtpPassword;
}
bool TEmailDB::SetSmtpPassword(String SmtpPassword){
	_SmtpPassword = SmtpPassword;
}

int TEmailDB::GetSmtpPort(){
	return _SmtpPort;
}
bool TEmailDB::SetSmtpPort(int SmtpPort){}

String TEmailDB::GetEmailFrom(){
	return _EmailFrom;
}
bool TEmailDB::SetEmailFrom(String EmailFrom){
	_EmailFrom = EmailFrom;
}

TStringList * TEmailDB::GetEmailTo(){
	return _EmailToAdressList;
}
bool TEmailDB::SetEmailTo(TStringList * EmailTo){
	_EmailToAdressList = EmailTo;
}
bool TEmailDB::AddEmailTo(String EmailTo){
	_EmailToAdressList->Add(EmailTo);
}

String TEmailDB::GetSubject(){
	return _Subject;
}

bool TEmailDB::SetSubject(String Subject){
	_Subject = Subject;
}

String TEmailDB::GetEmailBodyTXT(){
	return _EmailBodyTXT;
}
bool TEmailDB::SetEmailBodyTXT(String EmailBodyTXT){
	_EmailBodyTXT = EmailBodyTXT;
}

String TEmailDB::GetEmailBodyHTML(){
	return _EmailBodyHTML;
}
bool TEmailDB::SetEmailBodyHTML(String EmailBodyHTML){
	_EmailBodyHTML = EmailBodyHTML;
}

String TEmailDB::GetBodyEncoding(){
	return _Encoding;
}
bool TEmailDB::SetBodyEncoding(String Encoding){
	_Encoding = Encoding;
}

TIdMessageBuilderHtml * TEmailDB::GetMessageBuilder(){
	return _Builder;
}

bool TEmailDB::SetMessageBuilder(TIdMessageBuilderHtml * Builder){
	_Builder = Builder;
}
bool TEmailDB::IsMessageBuilderPresent(){}

bool TEmailDB::EmailSent(int Index, bool IsSendSuccesful){
	SaveEmailToBase(IsSendSuccesful);
	delete _Builder;
	delete _AttachmentList;
	delete fdqMain;
}

bool TEmailDB::BeforeEmailSend(){
	//ustaw zmienne globalne do zapisu do bazy
	_EmailBodyTXT = _Builder->PlainText->Text;

	//sparsuj plik, a nastepnie zapisz go na dysku
	String Dir = "temp\\EmailTemp\\";
	TStringList * FileList = new TStringList();
	int c = _MailIndex;
	TStringStream *Strm = new TStringStream();
	for (int i = 0; i < _Builder->Attachments->Count; i++) {
		String FileName = "Parsed_"+_Builder->Attachments->Attachment[i]->FileName,
				Path = Dir+FileName;
		//_Builder->Attachments->Attachment[i]->Data->Position = 0;

		Strm = dynamic_cast<TStringStream*>(_Builder->Attachments->Attachment[i]->Data);
		Strm->Position = 0;
		String TempStr = ParseScript(Strm->DataString);    // Trzeba ogarnac z tym nr badania
		Strm->WriteString(TempStr);
		Strm->Position = 0;
		Strm->SaveToFile(Path);
		FileList->Add(Path);
	}

	//stworz nowego buildera, ustaw mu wiadomosci ze "wzorcowego" buildera
	TIdMessageBuilderHtml * NewBuilder = new TIdMessageBuilderHtml();
	NewBuilder->PlainText->Text 	= _Builder->PlainText->Text;
	NewBuilder->PlainTextCharSet 	= _Builder->PlainTextCharSet;
	NewBuilder->Html->Text 			= _Builder->Html->Text;
	NewBuilder->HtmlCharSet 		= _Builder->HtmlCharSet;

	//dodaj do nowego buildera sparsowane pliki
	for (int i = 0; i < FileList->Count; i++) {
		NewBuilder->Attachments->Add(FileList->Strings[i]);
	}

	//ustaw nowy builder jako globalny (od tej linijki Email zawiera poprawnie sparsowane zalaczniki,
	//ktore pobiera funkcja wysylajaca maile w watku)
	_Builder = NewBuilder;
	_AttachmentList = FileList;
	Strm = NULL;
	delete Strm;
}

int TEmailDB::GetEmailId(){
	return _MailID;
}

void TEmailDB::LoadEmail(int MailID){
	String Query = "SELECT * FROM !Table WHERE kor_id=:MailId";
	fdqMain->SQL->Text = Query;
	fdqMain->MacroByName("Table")->AsRaw = "public.t_koresp";
	fdqMain->ParamByName("MailId")->AsInteger = MailID;
	fdqMain->Open();
	_Subject = fdqMain->FieldByName("kor_temat")->AsString;
	_EmailBodyTXT = fdqMain->FieldByName("kor_tresc_txt")->AsString;
	_EmailBodyHTML = fdqMain->FieldByName("kor_tresc_html")->AsString;
	fdqMain->Close();
}

void TEmailDB::SaveEmailToBase(bool IsSendSuccesful){
	String  t_koresp = "public.t_koresp",
			t_kor_detale = "public.t_kor_detale",
			t_kor_zalaczniki = "public.t_kor_zalaczniki",
			MainQuery = (String)("INSERT INTO !Table (kor_temat, kor_tresc_txt, kor_tresc_html, kor_sender_id)")+
								"VALUES(:Subject, :BodyTXT, :BodyHTML, :SenderId)",
			DetaleQuery = (String)("INSERT INTO !Table (kor_det_parent, kor_det_adres, kor_det_czaswys, kor_det_wyslany)")+
								"VALUES(:MailId, :Recipent, Now(), :IsSent)",
			AttachQuery = (String)("INSERT INTO !Table (kor_zaw_parent, kor_zaw_zawartosc, kor_zaw_id_wzorca, kor_zaw_id_elem, kor_zaw_nazwa_pliku)")+
								"VALUES(:MailId, :Data, :IdWzorca, :IdElem, :FileName)";

	fdqMain->Connection->TxOptions->AutoCommit = false;
	fdqMain->Connection->StartTransaction();
	try {
		fdqMain->SQL->Text = MainQuery;
		fdqMain->MacroByName("Table")->AsRaw = t_koresp;
		fdqMain->ParamByName("Subject")->AsString = _Subject;
		fdqMain->ParamByName("BodyTXT")->AsString = _EmailBodyTXT;
		fdqMain->ParamByName("BodyHTML")->AsString = _EmailBodyHTML;
		fdqMain->ParamByName("SenderId")->AsInteger = 8;//LoggedUser
		fdqMain->ExecSQL();

		_MailID = fdqMain->Connection->GetLastAutoGenValue("public.t_koresp_kor_id_seq");

		fdqMain->SQL->Text = DetaleQuery;
		fdqMain->MacroByName("Table")->AsRaw = t_kor_detale;
		fdqMain->ParamByName("MailId")->AsInteger = _MailID;
		fdqMain->ParamByName("Recipent")->AsString = _EmailToAdressList->Text;
		fdqMain->ParamByName("IsSent")->AsBoolean = IsSendSuccesful;
		fdqMain->ExecSQL();

		for (int i = 0; i < _AttachmentList->Count; i++) {
			fdqMain->SQL->Text = AttachQuery;
			fdqMain->MacroByName("Table")->AsRaw = t_kor_zalaczniki;
			fdqMain->ParamByName("MailId")->AsInteger = _MailID;
			fdqMain->ParamByName("Data")->LoadFromFile(_AttachmentList->Strings[i],ftBlob);
			fdqMain->ParamByName("IdWzorca")->AsInteger = _AttachIdList->Strings[i].ToIntDef(0);
			fdqMain->ParamByName("IdElem")->AsInteger = i; //to 'i' ?
			fdqMain->ParamByName("FileName")->AsString = _Builder->Attachments->Attachment[i]->FileName;
			fdqMain->ExecSQL();
		}
		fdqMain->Connection->Commit();

	} catch (...) {
		fdqMain->Connection->Rollback();
	}
}

String TEmailDB::ParseScript(String strm){
// implementacja ParseScript
	return "Ten plik zostal przeparsowany! TO: "+GetEmailTo()->Text;
}

void TEmailDB::SetAttachIdList(TStringList * List){
	_AttachIdList = List;
}
