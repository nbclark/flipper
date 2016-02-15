#include "StdAfx.h"
#include "Mapi.h"
#include <atlbase.h>
#include <cemapi.h>
#include <mapiutil.h>
#include <mapidefs.h>

HRESULT CMapi::MapiLogon()
{
	return S_OK;
}
HRESULT CMapi::MapiLogoff()
{
	return S_OK;
}

HRESULT CMapi::SendSMS(WCHAR *wzName, WCHAR *wzPhone)
{
	WCHAR wzCommand[MAX_PATH];

	wsprintf(wzCommand, L"-service \"SMS\" -to \"%s <%s>\"", wzName, wzPhone);

	PROCESS_INFORMATION pi;
	CreateProcess(L"tmail.exe", wzCommand, 0,0,0,0,0,0,0,&pi);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return S_OK;
	/*
	static const SizedSPropTagArray (2, spta) = { 2, PR_DISPLAY_NAME, PR_ENTRYID };
	HRESULT hr = E_FAIL;
	CComPtr<IMAPITable> ptbl;
    CComPtr<IMAPISession> pSession;
    CComPtr<IMsgStore> pStore;

    SRowSet *prowset = NULL;
    SPropValue  *pval = NULL;

    // Log onto MAPI
    if (SUCCEEDED(MAPILogonEx(0, NULL, NULL, 0, static_cast<LPMAPISESSION *>(&pSession))))
	{
		pSession->GetMsgStoresTable(0, &ptbl);
		ptbl->SetColumns((SPropTagArray *) &spta, 0);
		while (TRUE)
		{
			// Free the previous row
			FreeProws (prowset);
			prowset = NULL;
	 
			hr = ptbl->QueryRows (1, 0, &prowset);
			if ((hr != S_OK) || (prowset == NULL) || (prowset->cRows == 0))
			{
				break;
			}
	 
			ASSERT (prowset->aRow[0].cValues == spta.cValues);
			SPropValue *pval = prowset->aRow[0].lpProps;
	 
			ASSERT (pval[0].ulPropTag == PR_DISPLAY_NAME);
			ASSERT (pval[1].ulPropTag == PR_ENTRYID);
	 
			if (!_tcscmp(pval[0].Value.lpszW, TEXT("SMS")))
			{
				// Get the Message Store pointer
				if (SUCCEEDED(pSession->OpenMsgStore(0, pval[1].Value.bin.cb, (LPENTRYID)pval[1].Value.bin.lpb, 0, 0, &pStore)))
				{
					pStore->get
					pStore->StoreLogoff(0);
					//SaveMessages(pStore, pszFilename);
				}
			}
		}
		FreeProws(prowset);
		pSession->Logoff(0,0,0);
	}

	return S_OK;
	*/
}