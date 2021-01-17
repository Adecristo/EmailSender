﻿//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AdvRichEditor.hpp"
#include "AdvRichEditorBase.hpp"
#include "uEmailSender.h"
#include "uConnectionManager.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvRichEditor"
#pragma link "AdvRichEditorBase"
#pragma link "uBaseFormT"
#pragma link "AdvGrid"
#pragma link "AdvListV"
#pragma link "AdvObj"
#pragma link "AdvUtil"
#pragma link "BaseGrid"
#pragma link "DBAdvLst"
#pragma link "DBAdvGrid"
#pragma link "TaskDialog"
#pragma link "AdvRichEditor"
#pragma link "AdvRichEditorBase"
#pragma link "AdvScrollControl"
#pragma link "AdvRichEditor"
#pragma link "AdvRichEditorBase"
#pragma resource "*.dfm"
#include "RS.hpp"
TfrmEmail *frmEmail;
//---------------------------------------------------------------------------
__fastcall TfrmEmail::TfrmEmail(TComponent* Owner)
	: TBaseFormT(Owner)
{
	_Builder = new TIdMessageBuilderHtml();
	edtSubject->EditLabel->Caption = Rs_Title;
	_AttachList = new TObjectList();
	dbListViewRecipents->Checkboxes = true;
	dbListViewRecipents->RowSelect = true;
	CheckedAll = false;
	_AttachIdList = new TStringList();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEmail::btnCancelClick(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------

void TfrmEmail::GetRecipents(){
	fdqRecipents->SQL->Text = "SELECT jb_id, jb_symboljedn, jb_nazwa_firmy, jb_email FROM public.t_jbadana WHERE jb_aktywny = true AND jb_email != ''";
	fdqRecipents->Open();
}


void __fastcall TfrmEmail::FormShow(TObject *Sender)
{
	GetRecipents();
//	dbGridRecipents->AddCheckBoxColumn(1,false,false); //AddCheckBoxColumn(1,true,false);
//	dbGridRecipents->AddCheckBox(1,0,false,false);  // AddCheckBox(1,0,true,false);
//	dbGridRecipents->MouseActions->CheckAllCheck = true;
	edtEmailFrom->Text = "adecristo@wp.pl";//User Email;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEmail::btnAddRecipentClick(TObject *Sender)
{
	inptRecipent->InputText = "azajeczkowski@wp.pl"; //do testow
	if(inptRecipent->Execute() == 100){
		AddRecipent(inptRecipent->InputText);
	}
}
//---------------------------------------------------------------------------



void __fastcall TfrmEmail::btnSaveOkClick(TObject *Sender)
{
	try {
		_di_IMailer Temp;
		TInterfaceList * EmailList = GetInterfaceList();
		if(EmailList->Count > 0){
			ShowMessage(Rs_StartSending+ (String)EmailList->Count);
			frmImportProgress = new TfrmImportProgress(this);
			frmImportProgress->Position = poScreenCenter;
			frmImportProgress->lblMain->Caption = Rs_EmailSendingInProgress;
			frmImportProgress->Caption = Rs_EmailSendingInProgress;
			frmImportProgress->Show();
			TMailSenderThread *EmailSender = new TMailSenderThread(true, EmailList);
			EmailSender->FreeOnTerminate = true;
			EmailSender->OnTerminate = OnTerminate;
			EmailSender->Start();
		}else
			ShowMessage(Rs_SelectRecipents); // RS_PAS

	} catch (...) {
		ShowMessage(Rs_ThreadCreateError);
	}
}
//---------------------------------------------------------------------------

TInterfaceList * TfrmEmail::GetInterfaceList(){
	_di_IMailer Temp;
	TInterfaceList * EmailList = new TInterfaceList();
	bool IsChosen = false;
	int Col = dbGridRecipents->ColumnByName["Email"]->Index;
	String EmailBodyTXT = advRichEditBody->ContentAsPlainText,
			EmailBodyHTML = advRichEditBody->ContentAsHTML(),
			Encoding = "UTF-8";

	_Builder->PlainText->Text 	= EmailBodyTXT;
	_Builder->PlainTextCharSet 	= Encoding;
	_Builder->Html->Text 		= EmailBodyHTML;
	_Builder->HtmlCharSet 		= Encoding;

	for (int i = 0; i < _AttachList->Count; i++){
		String FileName = alvAttachments->Items->Item[i]->Caption;
		TMemoryStream *s = dynamic_cast<TMemoryStream*>(_AttachList->Items[i]);

		s->Position = 0;

		TStringStream *MS = new TStringStream();
		MS->CopyFrom(s,s->Size);
		MS->Position=0;

		_Builder->Attachments->Add(MS,"text/plain",(String)i)->FileName = FileName;
	}

	//zbierz wybranych odbiorców
	int MailIndex = 1;
	for (int i = 0; i < dbListViewRecipents->Items->Count; i++) {
		IsChosen = dbListViewRecipents->Items->Item[i]->Checked;
		if(IsChosen){
			TEmailDB * Email = new TEmailDB();
			Email->MailIndex = MailIndex++;
			//ustaw zmienne
			Email->SetSubject(edtSubject->Text);
			Email->SetEmailFrom(edtEmailFrom->Text);
			String EmailTo = dbListViewRecipents->Items->Item[i]->SubItems->Strings[1];
			Email->SetEmailBodyTXT(EmailBodyTXT);
			Email->SetEmailBodyHTML(EmailBodyHTML);
			Email->AddEmailTo(EmailTo);
			Email->SetMessageBuilder(_Builder);
			Email->SetAttachIdList(_AttachIdList);
			Temp = Email;
			EmailList->Add(Temp);
		}
	}
	return EmailList;
}
void __fastcall TfrmEmail::FormDestroy(TObject *Sender)
{
	delete _AttachList;
	delete _AttachIdList;
}
//---------------------------------------------------------------------------

void TfrmEmail::AddAttachment(int Id, String Opis, TMemoryStream * Attach){

	_AttachList->Add(Attach);

	TListItem * Lista;
	Lista = alvAttachments->Items->Add();
	Lista->Caption = Opis;

	_AttachIdList->Add(Id);
}

void __fastcall TfrmEmail::btnAddAttachmentClick(TObject *Sender)
{
	int Id = Edit1->Text.ToInt();
//	frmDocuments = new TfrmDocuments(this);
//	frmDocuments->btnSaveOk->Caption=Rs_Choose;
//	frmDocuments->btnCancel->Visible = true;
//	frmDocuments->ShowModal();
//	if(frmDocuments->ModalResult==mrOk  && frmDocuments->DocId!=0){
//		Id = frmDocuments->DocId;
//	}
//	delete frmDocuments;

	TMemoryStream *Attach = new TMemoryStream();

	fdqMain->SQL->Text = "SELECT * FROM public.t_zad_element WHERE zad_el_id = :Id";
	fdqMain->ParamByName("Id")->AsInteger = Id;
	fdqMain->Open();
	TBlobField *BF = dynamic_cast <TBlobField *> (fdqMain->FieldByName("zad_el_zawartosc"));
	BF->SaveToStream(Attach);
	String Opis = fdqMain->FieldByName("zad_el_nazwadokumentu")->AsString;
	Attach->Position=0;
		AddAttachment(Id,Opis,Attach);
	fdqMain->Close();
}
//---------------------------------------------------------------------------

void TfrmEmail::DeleteAttachment(int Index){
	alvAttachments->Items->Delete(Index);
	_AttachList->Delete(Index);
	_AttachIdList->Delete(Index);
}
void __fastcall TfrmEmail::btnDelAttachmentClick(TObject *Sender)
{
	int Index = alvAttachments->ItemIndex;
	if(Index != -1) DeleteAttachment(Index);
}
//---------------------------------------------------------------------------


void TfrmEmail::AddRecipent(String Input){
	TListItem * Lista;
	Lista = dbListViewRecipents->Items->Add();
	Lista->SubItems->Add("-1");
	Lista->SubItems->Add(Input);
	dbListViewRecipents->Items->Item[dbListViewRecipents->Items->Count-1]->Checked = true;
}

void TfrmEmail::LoadMail(int MailID){
	TEmailDB * Email = new TEmailDB(MailID);
	edtSubject->Text = Email->GetSubject();
	String Txt = Email->GetEmailBodyTXT();
	advRichEditBody->AddText(Txt);
	ShowMessage(Email->GetEmailBodyHTML());
	delete Email;
}
void __fastcall TfrmEmail::LoadEmailClick(TObject *Sender)
{
	LoadMail(220);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEmail::OnTerminate(TObject * Sender) {
	ShowMessage(Rs_EndEmailSending);
	for (int i = 0; i < _Builder->Attachments->Count; i++) {
		TStringStream *MS = dynamic_cast<TStringStream*>(_Builder->Attachments->Attachment[i]->Data);
		MS->Clear();
	}
}
void __fastcall TfrmEmail::btnCheckAllClick(TObject *Sender)
{
	if(!CheckedAll){
		for (int i = 0; i < dbListViewRecipents->Items->Count; i++)
			dbListViewRecipents->Items->Item[i]->Checked = true;
		CheckedAll = true;
	}else{
		for (int i = 0; i < dbListViewRecipents->Items->Count; i++)
			dbListViewRecipents->Items->Item[i]->Checked = false;
		CheckedAll = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmEmail::Button2Click(TObject *Sender)
{
	ShowMessage(dbListViewRecipents->Items->Item[1]->SubItems->Strings[1]);
}
//---------------------------------------------------------------------------


