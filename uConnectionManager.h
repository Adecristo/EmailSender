//---------------------------------------------------------------------------

#ifndef uConnectionManagerH
#define uConnectionManagerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.PG.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Stan.Param.hpp>
//---------------------------------------------------------------------------
class TConnectionManager : public TDataModule
{
__published:	// IDE-managed Components
	TFDConnection *fdcMain;
private:	// User declarations
public:		// User declarations

	__fastcall TConnectionManager(TComponent* Owner);
	__property TFDConnection *Connection = {read=fdcMain};
};
//---------------------------------------------------------------------------
extern PACKAGE TConnectionManager *ConnectionManager;
//---------------------------------------------------------------------------
#endif
