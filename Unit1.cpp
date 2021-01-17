//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "uBaseFormT"
#pragma resource "*.dfm"
TBaseFormT1 *BaseFormT1;
//---------------------------------------------------------------------------
__fastcall TBaseFormT1::TBaseFormT1(TComponent* Owner)
	: TBaseFormT(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBaseFormT1::Button1Click(TObject *Sender)
{
	frmEmail = new TfrmEmail(0);
	frmEmail->ShowModal();
	delete frmEmail;
}
//---------------------------------------------------------------------------
void __fastcall TBaseFormT1::Button2Click(TObject *Sender)
{
	ConnectionManager->Connection->Close();
}
//---------------------------------------------------------------------------
