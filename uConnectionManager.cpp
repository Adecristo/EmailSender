//---------------------------------------------------------------------------


#pragma hdrstop

#include "uConnectionManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TConnectionManager *ConnectionManager;
//---------------------------------------------------------------------------
__fastcall TConnectionManager::TConnectionManager(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
