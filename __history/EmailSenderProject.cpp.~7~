//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("uConnectionManager.cpp", ConnectionManager); /* TDataModule: File Type */
USEFORM("uEmailSender.cpp", frmEmail);
USEFORM("Unit1.cpp", BaseFormT1);
USEFORM("..\..\..\FairWareINc\importProgress\uImportProgress.cpp", frmImportProgress);
USEFORM("..\..\..\FairWareINc\BaseForm\uBaseFormT.cpp", BaseFormT);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TConnectionManager), &ConnectionManager);
		Application->CreateForm(__classid(TfrmEmail), &frmEmail);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
