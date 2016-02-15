#pragma once

class CMapi
{
public:
	static HRESULT SendSMS(WCHAR* wzName, WCHAR* wzPhone);
	static HRESULT MapiLogon();
	static HRESULT MapiLogoff();
};
