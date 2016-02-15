#define RPN_VARIANT                   7714
#define RPN_CVALUE                    18

unsigned int EvaluateRPN(WCHAR* wzRPN, unsigned int nCValue, unsigned int nVariant) 
{
    unsigned int  nStringValue=0;
    int     StringPos=0;

    WCHAR wzRPNSubstr[11] = {0};
    int iLen = wcslen(wzRPN);

    if (iLen > 10)
    {
        wzRPN[5] = 0;
        wcscpy(wzRPNSubstr, wzRPN);
        wcscat(wzRPNSubstr, &wzRPN[iLen - 5]);
        iLen = 10;
    }
    else
    {
        wcscpy(wzRPNSubstr, wzRPN);
    }

    do
    {
        nStringValue += (unsigned int)wzRPNSubstr[StringPos];
        StringPos++;

    } while(StringPos <= iLen);


    nStringValue=nStringValue*nCValue;  // c5*

    nStringValue+=nVariant;


    //  RPN = "i 0 == m_nVariant * key + c 5 * +" 


    return nStringValue;

}
bool CheckIfRegistered(WCHAR* wzRegCodeLocation, unsigned int nCValue, unsigned int nVariant)
{
    DWORD dwRegCode = 0;
    WCHAR wzOwner[255];
    GetFromRegistry(wzRegCodeLocation, &dwRegCode);
    GetFromRegistry(HKEY_CURRENT_USER, L"ControlPanel\\Owner", L"Name", wzOwner);

    return (dwRegCode == EvaluateRPN(wzOwner, RPN_CVALUE, RPN_VARIANT));
}