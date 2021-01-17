//---------------------------------------------------------------------------

#ifndef uEmailSenderH
#define uEmailSenderH
//---------------------------------------------------------------------------
#include "AdvRichEditor.hpp"
#include "AdvRichEditorBase.hpp"
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "uBaseFormT.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.ScreenTips.hpp>
#include "AdvGrid.hpp"
#include "AdvListV.hpp"
#include "AdvObj.hpp"
#include "AdvUtil.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include "DBAdvLst.hpp"
#include "DBAdvGrid.hpp"
#include <FireDAC.Comp.UI.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include "TaskDialog.hpp"

#include "EmailSenderThread.h"

#include "AdvScrollControl.hpp"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdSMTPBase.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>




//---------------------------------------------------------------------------
class TfrmEmail : public TBaseFormT
{
__published:	// IDE-managed Components
	TLabeledEdit *edtSubject;
	TGroupBox *gbEmailEdit;
	TGroupBox *gbAttachments;
	TGroupBox *gbRecipients;
	TAdvListView *alvAttachments;
	TButton *btnAddAttachment;
	TButton *btnDelAttachment;
	TFDQuery *fdqMain;
	TFDQuery *fdqRecipents;
	TDataSource *dsRecipents;
	TButton *btnAddRecipent;
	TDBAdvGrid *dbGridRecipents;
	TFDGUIxWaitCursor *FDGUIxWaitCursor1;
	TAdvInputTaskDialog *inptRecipent;
	TGroupBox *gbEmailFrom;
	TEdit *edtEmailFrom;
	TLabel *lblBody;
	TEdit *Edit1;
	TButton *LoadEmail;
	TDBAdvListView *dbListViewRecipents;
	TRichEdit *edtBody;
	TAdvRichEditor *advRichEditBody;
	TButton *btnCheckAll;
	TButton *Button2;
	TIdSSLIOHandlerSocketOpenSSL *IdSSLIOHandlerSocketOpenSSL1;
	TIdSMTP *IdSMTP1;
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnAddRecipentClick(TObject *Sender);
	void __fastcall btnSaveOkClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnAddAttachmentClick(TObject *Sender);
	void __fastcall btnDelAttachmentClick(TObject *Sender);
	void __fastcall LoadEmailClick(TObject *Sender);
	void __fastcall btnCheckAllClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);

private:	// User declarations
	void GetRecipents();
	TInterfaceList * GetInterfaceList();
	void AddRecipent(String Input);
	void AddAttachment(int Id, String Opis,TMemoryStream * Attach);
	void DeleteAttachment(int Index);
	void LoadMail(int MailID);

	TIdMessageBuilderHtml * _Builder;

	TObjectList * _AttachList;
	TStringList * _AttachIdList;

	bool CheckedAll;
public:		// User declarations
	__fastcall TfrmEmail(TComponent* Owner);
	void __fastcall OnTerminate(TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEmail *frmEmail;
//---------------------------------------------------------------------------
#endif
