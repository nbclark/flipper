//
// sproxy.exe generated file
// do not modify this file
//
// Created: 01/11/2008@16:27:25
//

#pragma once


#if !defined(_WIN32_WINDOWS) && !defined(_WIN32_WINNT) && !defined(_WIN32_WCE)
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace DemographixQuery
{

struct RacePercentagesCls
{
	double Asian;
	double Black;
	double Indian;
	double Mixed;
	double NativeHawaiian;
	double Other;
	double White;
};

struct PlaceInformationCls
{
	BSTR StateAbbrev;
	BSTR PlaceID;
	bool Rural;
};

struct PlaceInfo
{
	PlaceInformationCls PlaceInformation;
	bool Error;
	BSTR ErrorString;
};

struct CensusInfoWithDataSet
{
	bool Error;
	BSTR ErrorString;
	BSTR CensusDataSet;
};

struct MaritalStatusPercentagesCls
{
	double NeverMarried;
	double Married;
	double Separated;
	double MarriedOther;
	double Widowed;
	double Divorced;
};

struct GenderPercentagesCls
{
	double Female;
	double Male;
};

struct SummaryInformation
{
	PlaceInformationCls PlaceInformation;
	bool Error;
	BSTR ErrorString;
	unsigned char MedianAge;
	int MedianIncome;
	unsigned char MedianRoomsInHouse;
	int MedianHouseValue;
	unsigned char MedianVehicles;
	MaritalStatusPercentagesCls MaritalStatusPercentages;
	RacePercentagesCls RacePercentages;
	GenderPercentagesCls GenderPercentages;
};

struct IncomeAndHouseValue
{
	PlaceInformationCls PlaceInformation;
	bool Error;
	BSTR ErrorString;
	int MedianIncome;
	int MedianHouseValue;
};

template <typename TClient = CSoapSocketClientT<> >
class CDemographixQueryT : 
	public TClient, 
	public CSoapRootHandler
{
protected:

	const _soapmap ** GetFunctionMap();
	const _soapmap ** GetHeaderMap();
	void * GetHeaderValue();
	const wchar_t * GetNamespaceUri();
	const char * GetServiceName();
	const char * GetNamespaceUriA();
	HRESULT CallFunction(
		void *pvParam, 
		const wchar_t *wszLocalName, int cchLocalName,
		size_t nItem);
	HRESULT GetClientReader(ISAXXMLReader **ppReader);

public:

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (ppv == NULL)
		{
			return E_POINTER;
		}

		*ppv = NULL;

		if (InlineIsEqualGUID(riid, IID_IUnknown) ||
			InlineIsEqualGUID(riid, IID_ISAXContentHandler))
		{
			*ppv = static_cast<ISAXContentHandler *>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		return 1;
	}

	ULONG __stdcall Release()
	{
		return 1;
	}

	CDemographixQueryT(ISAXXMLReader *pReader = NULL)
		:TClient(_T("http://ws.cdyne.com/DemographixWS/DemographixQuery.asmx"))
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CDemographixQueryT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT GetNeighborhoodLinguisticIsolation(
		BSTR StateAbbr, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodLinguisticIsolationResult
	);

	HRESULT GetNeighborhoodAgeInDataSet(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodAgeInDataSetResult
	);

	HRESULT GetVersion(
		BSTR* GetVersionResult
	);

	HRESULT GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset(
		BSTR StateAbbr, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult
	);

	HRESULT GetSummaryInformationByPlaceID(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		SummaryInformation* GetSummaryInformationByPlaceIDResult
	);

	HRESULT GetNeighborhoodRealtyValueInDataSet(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodRealtyValueInDataSetResult
	);

	HRESULT GetLocationInformationByAddress(
		BSTR AddressLine1, 
		BSTR City, 
		BSTR StateAbbrev, 
		BSTR ZipCode, 
		BSTR LicenseKey, 
		SummaryInformation* GetLocationInformationByAddressResult
	);

	HRESULT GetNeighborhoodAgeGenderFemaleInDataSet(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodAgeGenderFemaleInDataSetResult
	);

	HRESULT GetPlaceIDbyAddress(
		BSTR AddressLine1, 
		BSTR City, 
		BSTR StateAbbrev, 
		BSTR ZipCode, 
		BSTR LicenseKey, 
		PlaceInfo* GetPlaceIDbyAddressResult
	);

	HRESULT GetLocationInformationByLatitudeLongitude(
		double Latitude, 
		double Longitude, 
		BSTR LicenseKey, 
		SummaryInformation* GetLocationInformationByLatitudeLongitudeResult
	);

	HRESULT GetNeighborhoodVehiclesPerHouseholdInDataset(
		BSTR StateAbbr, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodVehiclesPerHouseholdInDatasetResult
	);

	HRESULT GetNeighborhoodYearStructuresBuilt(
		BSTR StateAbbr, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodYearStructuresBuiltResult
	);

	HRESULT GetIncomeHouseValueByAddress(
		BSTR AddressLine1, 
		BSTR City, 
		BSTR StateAbbrev, 
		BSTR ZipCode, 
		BSTR LicenseKey, 
		IncomeAndHouseValue* GetIncomeHouseValueByAddressResult
	);

	HRESULT GetNeighborhoodAgeGenderMaleInDataSet(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodAgeGenderMaleInDataSetResult
	);

	HRESULT GetPlaceIDbyCensusTractAndBlock(
		BSTR StateAbbrev, 
		BSTR CensusTract, 
		BSTR CensusBlock, 
		BSTR LicenseKey, 
		PlaceInfo* GetPlaceIDbyCensusTractAndBlockResult
	);
};

typedef CDemographixQueryT<> CDemographixQuery;

__if_not_exists(__RacePercentagesCls_entries)
{
extern __declspec(selectany) const _soapmapentry __RacePercentagesCls_entries[] =
{
	{ 
		0x04D913EC, 
		"Asian", 
		L"Asian", 
		sizeof("Asian")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(RacePercentagesCls, Asian),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x04E733FD, 
		"Black", 
		L"Black", 
		sizeof("Black")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(RacePercentagesCls, Black),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xB247BED3, 
		"Indian", 
		L"Indian", 
		sizeof("Indian")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(RacePercentagesCls, Indian),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x05ACFE77, 
		"Mixed", 
		L"Mixed", 
		sizeof("Mixed")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(RacePercentagesCls, Mixed),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x8DD9BAA9, 
		"NativeHawaiian", 
		L"NativeHawaiian", 
		sizeof("NativeHawaiian")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(RacePercentagesCls, NativeHawaiian),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x05D6F3A2, 
		"Other", 
		L"Other", 
		sizeof("Other")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(RacePercentagesCls, Other),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x06612941, 
		"White", 
		L"White", 
		sizeof("White")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(RacePercentagesCls, White),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __RacePercentagesCls_map =
{
	0x151C7DCE,
	"RacePercentagesCls",
	L"RacePercentagesCls",
	sizeof("RacePercentagesCls")-1,
	sizeof("RacePercentagesCls")-1,
	SOAPMAP_STRUCT,
	__RacePercentagesCls_entries,
	sizeof(RacePercentagesCls),
	7,
	-1,
	SOAPFLAG_NONE,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};
}

__if_not_exists(__PlaceInformationCls_entries)
{
extern __declspec(selectany) const _soapmapentry __PlaceInformationCls_entries[] =
{
	{ 
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(PlaceInformationCls, StateAbbrev),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(PlaceInformationCls, PlaceID),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x060DF386, 
		"Rural", 
		L"Rural", 
		sizeof("Rural")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_FIELD, 
		offsetof(PlaceInformationCls, Rural),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __PlaceInformationCls_map =
{
	0x9B3AF5CD,
	"PlaceInformationCls",
	L"PlaceInformationCls",
	sizeof("PlaceInformationCls")-1,
	sizeof("PlaceInformationCls")-1,
	SOAPMAP_STRUCT,
	__PlaceInformationCls_entries,
	sizeof(PlaceInformationCls),
	3,
	-1,
	SOAPFLAG_NONE,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};
}

__if_not_exists(__PlaceInfo_entries)
{
extern __declspec(selectany) const _soapmapentry __PlaceInfo_entries[] =
{
	{ 
		0x12C0EA4B, 
		"PlaceInformation", 
		L"PlaceInformation", 
		sizeof("PlaceInformation")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_FIELD, 
		offsetof(PlaceInfo, PlaceInformation),
		NULL, 
		&__PlaceInformationCls_map, 
		-1 
	},
	{ 
		0x052111AA, 
		"Error", 
		L"Error", 
		sizeof("Error")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_FIELD, 
		offsetof(PlaceInfo, Error),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x2025C041, 
		"ErrorString", 
		L"ErrorString", 
		sizeof("ErrorString")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(PlaceInfo, ErrorString),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __PlaceInfo_map =
{
	0x2BB66EB1,
	"PlaceInfo",
	L"PlaceInfo",
	sizeof("PlaceInfo")-1,
	sizeof("PlaceInfo")-1,
	SOAPMAP_STRUCT,
	__PlaceInfo_entries,
	sizeof(PlaceInfo),
	3,
	-1,
	SOAPFLAG_NONE,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};
}

__if_not_exists(__CensusInfoWithDataSet_entries)
{
extern __declspec(selectany) const _soapmapentry __CensusInfoWithDataSet_entries[] =
{
	{ 
		0x052111AA, 
		"Error", 
		L"Error", 
		sizeof("Error")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_FIELD, 
		offsetof(CensusInfoWithDataSet, Error),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x2025C041, 
		"ErrorString", 
		L"ErrorString", 
		sizeof("ErrorString")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(CensusInfoWithDataSet, ErrorString),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xF010F057, 
		"CensusDataSet", 
		L"CensusDataSet", 
		sizeof("CensusDataSet")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(CensusInfoWithDataSet, CensusDataSet),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CensusInfoWithDataSet_map =
{
	0x4691D67F,
	"CensusInfoWithDataSet",
	L"CensusInfoWithDataSet",
	sizeof("CensusInfoWithDataSet")-1,
	sizeof("CensusInfoWithDataSet")-1,
	SOAPMAP_STRUCT,
	__CensusInfoWithDataSet_entries,
	sizeof(CensusInfoWithDataSet),
	3,
	-1,
	SOAPFLAG_NONE,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};
}

__if_not_exists(__MaritalStatusPercentagesCls_entries)
{
extern __declspec(selectany) const _soapmapentry __MaritalStatusPercentagesCls_entries[] =
{
	{ 
		0xA0DA20C4, 
		"NeverMarried", 
		L"NeverMarried", 
		sizeof("NeverMarried")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(MaritalStatusPercentagesCls, NeverMarried),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x11D7E3C4, 
		"Married", 
		L"Married", 
		sizeof("Married")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(MaritalStatusPercentagesCls, Married),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x9256AE99, 
		"Separated", 
		L"Separated", 
		sizeof("Separated")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(MaritalStatusPercentagesCls, Separated),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x2247D1E6, 
		"MarriedOther", 
		L"MarriedOther", 
		sizeof("MarriedOther")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(MaritalStatusPercentagesCls, MarriedOther),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x2548C7B3, 
		"Widowed", 
		L"Widowed", 
		sizeof("Widowed")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(MaritalStatusPercentagesCls, Widowed),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x6F7906D0, 
		"Divorced", 
		L"Divorced", 
		sizeof("Divorced")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(MaritalStatusPercentagesCls, Divorced),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __MaritalStatusPercentagesCls_map =
{
	0xA119C8C1,
	"MaritalStatusPercentagesCls",
	L"MaritalStatusPercentagesCls",
	sizeof("MaritalStatusPercentagesCls")-1,
	sizeof("MaritalStatusPercentagesCls")-1,
	SOAPMAP_STRUCT,
	__MaritalStatusPercentagesCls_entries,
	sizeof(MaritalStatusPercentagesCls),
	6,
	-1,
	SOAPFLAG_NONE,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};
}

__if_not_exists(__GenderPercentagesCls_entries)
{
extern __declspec(selectany) const _soapmapentry __GenderPercentagesCls_entries[] =
{
	{ 
		0xAAAA372A, 
		"Female", 
		L"Female", 
		sizeof("Female")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(GenderPercentagesCls, Female),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x002BE41F, 
		"Male", 
		L"Male", 
		sizeof("Male")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_FIELD, 
		offsetof(GenderPercentagesCls, Male),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __GenderPercentagesCls_map =
{
	0xA028F288,
	"GenderPercentagesCls",
	L"GenderPercentagesCls",
	sizeof("GenderPercentagesCls")-1,
	sizeof("GenderPercentagesCls")-1,
	SOAPMAP_STRUCT,
	__GenderPercentagesCls_entries,
	sizeof(GenderPercentagesCls),
	2,
	-1,
	SOAPFLAG_NONE,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};
}

__if_not_exists(__SummaryInformation_entries)
{
extern __declspec(selectany) const _soapmapentry __SummaryInformation_entries[] =
{
	{ 
		0x12C0EA4B, 
		"PlaceInformation", 
		L"PlaceInformation", 
		sizeof("PlaceInformation")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, PlaceInformation),
		NULL, 
		&__PlaceInformationCls_map, 
		-1 
	},
	{ 
		0x052111AA, 
		"Error", 
		L"Error", 
		sizeof("Error")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, Error),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x2025C041, 
		"ErrorString", 
		L"ErrorString", 
		sizeof("ErrorString")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(SummaryInformation, ErrorString),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x4D2D1C3B, 
		"MedianAge", 
		L"MedianAge", 
		sizeof("MedianAge")-1, 
		SOAPTYPE_UNSIGNEDBYTE, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, MedianAge),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xFCB0B4C9, 
		"MedianIncome", 
		L"MedianIncome", 
		sizeof("MedianIncome")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, MedianIncome),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xCD6BE159, 
		"MedianRoomsInHouse", 
		L"MedianRoomsInHouse", 
		sizeof("MedianRoomsInHouse")-1, 
		SOAPTYPE_UNSIGNEDBYTE, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, MedianRoomsInHouse),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xC99A130F, 
		"MedianHouseValue", 
		L"MedianHouseValue", 
		sizeof("MedianHouseValue")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, MedianHouseValue),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x41749F41, 
		"MedianVehicles", 
		L"MedianVehicles", 
		sizeof("MedianVehicles")-1, 
		SOAPTYPE_UNSIGNEDBYTE, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, MedianVehicles),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x66D381BF, 
		"MaritalStatusPercentages", 
		L"MaritalStatusPercentages", 
		sizeof("MaritalStatusPercentages")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, MaritalStatusPercentages),
		NULL, 
		&__MaritalStatusPercentagesCls_map, 
		-1 
	},
	{ 
		0xABBA89EC, 
		"RacePercentages", 
		L"RacePercentages", 
		sizeof("RacePercentages")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, RacePercentages),
		NULL, 
		&__RacePercentagesCls_map, 
		-1 
	},
	{ 
		0xDFCAA8E6, 
		"GenderPercentages", 
		L"GenderPercentages", 
		sizeof("GenderPercentages")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_FIELD, 
		offsetof(SummaryInformation, GenderPercentages),
		NULL, 
		&__GenderPercentagesCls_map, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __SummaryInformation_map =
{
	0xE063D7D4,
	"SummaryInformation",
	L"SummaryInformation",
	sizeof("SummaryInformation")-1,
	sizeof("SummaryInformation")-1,
	SOAPMAP_STRUCT,
	__SummaryInformation_entries,
	sizeof(SummaryInformation),
	11,
	-1,
	SOAPFLAG_NONE,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};
}

__if_not_exists(__IncomeAndHouseValue_entries)
{
extern __declspec(selectany) const _soapmapentry __IncomeAndHouseValue_entries[] =
{
	{ 
		0x12C0EA4B, 
		"PlaceInformation", 
		L"PlaceInformation", 
		sizeof("PlaceInformation")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_FIELD, 
		offsetof(IncomeAndHouseValue, PlaceInformation),
		NULL, 
		&__PlaceInformationCls_map, 
		-1 
	},
	{ 
		0x052111AA, 
		"Error", 
		L"Error", 
		sizeof("Error")-1, 
		SOAPTYPE_BOOLEAN, 
		SOAPFLAG_FIELD, 
		offsetof(IncomeAndHouseValue, Error),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x2025C041, 
		"ErrorString", 
		L"ErrorString", 
		sizeof("ErrorString")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(IncomeAndHouseValue, ErrorString),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xFCB0B4C9, 
		"MedianIncome", 
		L"MedianIncome", 
		sizeof("MedianIncome")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(IncomeAndHouseValue, MedianIncome),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xC99A130F, 
		"MedianHouseValue", 
		L"MedianHouseValue", 
		sizeof("MedianHouseValue")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(IncomeAndHouseValue, MedianHouseValue),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __IncomeAndHouseValue_map =
{
	0x241428EF,
	"IncomeAndHouseValue",
	L"IncomeAndHouseValue",
	sizeof("IncomeAndHouseValue")-1,
	sizeof("IncomeAndHouseValue")-1,
	SOAPMAP_STRUCT,
	__IncomeAndHouseValue_entries,
	sizeof(IncomeAndHouseValue),
	5,
	-1,
	SOAPFLAG_NONE,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};
}

struct __CDemographixQuery_GetNeighborhoodLinguisticIsolation_struct
{
	BSTR StateAbbr;
	BSTR PlaceID;
	CensusInfoWithDataSet GetNeighborhoodLinguisticIsolationResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetNeighborhoodLinguisticIsolation_entries[] =
{

	{
		0x12DD84D8, 
		"StateAbbr", 
		L"StateAbbr", 
		sizeof("StateAbbr")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodLinguisticIsolation_struct, StateAbbr),
		NULL,
		NULL,
		-1,
	},
	{
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodLinguisticIsolation_struct, PlaceID),
		NULL,
		NULL,
		-1,
	},
	{
		0xACC662E4, 
		"GetNeighborhoodLinguisticIsolationResult", 
		L"GetNeighborhoodLinguisticIsolationResult", 
		sizeof("GetNeighborhoodLinguisticIsolationResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetNeighborhoodLinguisticIsolation_struct, GetNeighborhoodLinguisticIsolationResult),
		NULL,
		&__CensusInfoWithDataSet_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetNeighborhoodLinguisticIsolation_map =
{
	0xF791CC34,
	"GetNeighborhoodLinguisticIsolation",
	L"GetNeighborhoodLinguisticIsolationResponse",
	sizeof("GetNeighborhoodLinguisticIsolation")-1,
	sizeof("GetNeighborhoodLinguisticIsolationResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetNeighborhoodLinguisticIsolation_entries,
	sizeof(__CDemographixQuery_GetNeighborhoodLinguisticIsolation_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetNeighborhoodAgeInDataSet_struct
{
	BSTR StateAbbrev;
	BSTR PlaceID;
	CensusInfoWithDataSet GetNeighborhoodAgeInDataSetResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetNeighborhoodAgeInDataSet_entries[] =
{

	{
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodAgeInDataSet_struct, StateAbbrev),
		NULL,
		NULL,
		-1,
	},
	{
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodAgeInDataSet_struct, PlaceID),
		NULL,
		NULL,
		-1,
	},
	{
		0x0FFC96E1, 
		"GetNeighborhoodAgeInDataSetResult", 
		L"GetNeighborhoodAgeInDataSetResult", 
		sizeof("GetNeighborhoodAgeInDataSetResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetNeighborhoodAgeInDataSet_struct, GetNeighborhoodAgeInDataSetResult),
		NULL,
		&__CensusInfoWithDataSet_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetNeighborhoodAgeInDataSet_map =
{
	0x0124F371,
	"GetNeighborhoodAgeInDataSet",
	L"GetNeighborhoodAgeInDataSetResponse",
	sizeof("GetNeighborhoodAgeInDataSet")-1,
	sizeof("GetNeighborhoodAgeInDataSetResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetNeighborhoodAgeInDataSet_entries,
	sizeof(__CDemographixQuery_GetNeighborhoodAgeInDataSet_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetVersion_struct
{
	BSTR GetVersionResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetVersion_entries[] =
{

	{
		0xE6C65CE5, 
		"GetVersionResult", 
		L"GetVersionResult", 
		sizeof("GetVersionResult")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetVersion_struct, GetVersionResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetVersion_map =
{
	0xB1784A75,
	"GetVersion",
	L"GetVersionResponse",
	sizeof("GetVersion")-1,
	sizeof("GetVersionResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetVersion_entries,
	sizeof(__CDemographixQuery_GetVersion_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_struct
{
	BSTR StateAbbr;
	BSTR PlaceID;
	CensusInfoWithDataSet GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_entries[] =
{

	{
		0x12DD84D8, 
		"StateAbbr", 
		L"StateAbbr", 
		sizeof("StateAbbr")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_struct, StateAbbr),
		NULL,
		NULL,
		-1,
	},
	{
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_struct, PlaceID),
		NULL,
		NULL,
		-1,
	},
	{
		0x9E848F30, 
		"GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult", 
		L"GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult", 
		sizeof("GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_struct, GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult),
		NULL,
		&__CensusInfoWithDataSet_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_map =
{
	0x518C3B80,
	"GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset",
	L"GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResponse",
	sizeof("GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset")-1,
	sizeof("GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_entries,
	sizeof(__CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetSummaryInformationByPlaceID_struct
{
	BSTR StateAbbrev;
	BSTR PlaceID;
	SummaryInformation GetSummaryInformationByPlaceIDResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetSummaryInformationByPlaceID_entries[] =
{

	{
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetSummaryInformationByPlaceID_struct, StateAbbrev),
		NULL,
		NULL,
		-1,
	},
	{
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetSummaryInformationByPlaceID_struct, PlaceID),
		NULL,
		NULL,
		-1,
	},
	{
		0xF429DF40, 
		"GetSummaryInformationByPlaceIDResult", 
		L"GetSummaryInformationByPlaceIDResult", 
		sizeof("GetSummaryInformationByPlaceIDResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetSummaryInformationByPlaceID_struct, GetSummaryInformationByPlaceIDResult),
		NULL,
		&__SummaryInformation_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetSummaryInformationByPlaceID_map =
{
	0xA5C5CF90,
	"GetSummaryInformationByPlaceID",
	L"GetSummaryInformationByPlaceIDResponse",
	sizeof("GetSummaryInformationByPlaceID")-1,
	sizeof("GetSummaryInformationByPlaceIDResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetSummaryInformationByPlaceID_entries,
	sizeof(__CDemographixQuery_GetSummaryInformationByPlaceID_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_struct
{
	BSTR StateAbbrev;
	BSTR PlaceID;
	CensusInfoWithDataSet GetNeighborhoodRealtyValueInDataSetResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_entries[] =
{

	{
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_struct, StateAbbrev),
		NULL,
		NULL,
		-1,
	},
	{
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_struct, PlaceID),
		NULL,
		NULL,
		-1,
	},
	{
		0x5BED5642, 
		"GetNeighborhoodRealtyValueInDataSetResult", 
		L"GetNeighborhoodRealtyValueInDataSetResult", 
		sizeof("GetNeighborhoodRealtyValueInDataSetResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_struct, GetNeighborhoodRealtyValueInDataSetResult),
		NULL,
		&__CensusInfoWithDataSet_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_map =
{
	0x0C430F12,
	"GetNeighborhoodRealtyValueInDataSet",
	L"GetNeighborhoodRealtyValueInDataSetResponse",
	sizeof("GetNeighborhoodRealtyValueInDataSet")-1,
	sizeof("GetNeighborhoodRealtyValueInDataSetResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_entries,
	sizeof(__CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetLocationInformationByAddress_struct
{
	BSTR AddressLine1;
	BSTR City;
	BSTR StateAbbrev;
	BSTR ZipCode;
	BSTR LicenseKey;
	SummaryInformation GetLocationInformationByAddressResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetLocationInformationByAddress_entries[] =
{

	{
		0xD5CAC95F, 
		"AddressLine1", 
		L"AddressLine1", 
		sizeof("AddressLine1")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetLocationInformationByAddress_struct, AddressLine1),
		NULL,
		NULL,
		-1,
	},
	{
		0x00268B79, 
		"City", 
		L"City", 
		sizeof("City")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetLocationInformationByAddress_struct, City),
		NULL,
		NULL,
		-1,
	},
	{
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetLocationInformationByAddress_struct, StateAbbrev),
		NULL,
		NULL,
		-1,
	},
	{
		0x0CF8612E, 
		"ZipCode", 
		L"ZipCode", 
		sizeof("ZipCode")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetLocationInformationByAddress_struct, ZipCode),
		NULL,
		NULL,
		-1,
	},
	{
		0xBF4DE04C, 
		"LicenseKey", 
		L"LicenseKey", 
		sizeof("LicenseKey")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetLocationInformationByAddress_struct, LicenseKey),
		NULL,
		NULL,
		-1,
	},
	{
		0x52D8493F, 
		"GetLocationInformationByAddressResult", 
		L"GetLocationInformationByAddressResult", 
		sizeof("GetLocationInformationByAddressResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetLocationInformationByAddress_struct, GetLocationInformationByAddressResult),
		NULL,
		&__SummaryInformation_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetLocationInformationByAddress_map =
{
	0x69B6B54F,
	"GetLocationInformationByAddress",
	L"GetLocationInformationByAddressResponse",
	sizeof("GetLocationInformationByAddress")-1,
	sizeof("GetLocationInformationByAddressResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetLocationInformationByAddress_entries,
	sizeof(__CDemographixQuery_GetLocationInformationByAddress_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_struct
{
	BSTR StateAbbrev;
	BSTR PlaceID;
	CensusInfoWithDataSet GetNeighborhoodAgeGenderFemaleInDataSetResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_entries[] =
{

	{
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_struct, StateAbbrev),
		NULL,
		NULL,
		-1,
	},
	{
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_struct, PlaceID),
		NULL,
		NULL,
		-1,
	},
	{
		0x19AAEB80, 
		"GetNeighborhoodAgeGenderFemaleInDataSetResult", 
		L"GetNeighborhoodAgeGenderFemaleInDataSetResult", 
		sizeof("GetNeighborhoodAgeGenderFemaleInDataSetResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_struct, GetNeighborhoodAgeGenderFemaleInDataSetResult),
		NULL,
		&__CensusInfoWithDataSet_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_map =
{
	0x2FBAEBD0,
	"GetNeighborhoodAgeGenderFemaleInDataSet",
	L"GetNeighborhoodAgeGenderFemaleInDataSetResponse",
	sizeof("GetNeighborhoodAgeGenderFemaleInDataSet")-1,
	sizeof("GetNeighborhoodAgeGenderFemaleInDataSetResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_entries,
	sizeof(__CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetPlaceIDbyAddress_struct
{
	BSTR AddressLine1;
	BSTR City;
	BSTR StateAbbrev;
	BSTR ZipCode;
	BSTR LicenseKey;
	PlaceInfo GetPlaceIDbyAddressResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetPlaceIDbyAddress_entries[] =
{

	{
		0xD5CAC95F, 
		"AddressLine1", 
		L"AddressLine1", 
		sizeof("AddressLine1")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetPlaceIDbyAddress_struct, AddressLine1),
		NULL,
		NULL,
		-1,
	},
	{
		0x00268B79, 
		"City", 
		L"City", 
		sizeof("City")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetPlaceIDbyAddress_struct, City),
		NULL,
		NULL,
		-1,
	},
	{
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetPlaceIDbyAddress_struct, StateAbbrev),
		NULL,
		NULL,
		-1,
	},
	{
		0x0CF8612E, 
		"ZipCode", 
		L"ZipCode", 
		sizeof("ZipCode")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetPlaceIDbyAddress_struct, ZipCode),
		NULL,
		NULL,
		-1,
	},
	{
		0xBF4DE04C, 
		"LicenseKey", 
		L"LicenseKey", 
		sizeof("LicenseKey")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetPlaceIDbyAddress_struct, LicenseKey),
		NULL,
		NULL,
		-1,
	},
	{
		0x0851C532, 
		"GetPlaceIDbyAddressResult", 
		L"GetPlaceIDbyAddressResult", 
		sizeof("GetPlaceIDbyAddressResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetPlaceIDbyAddress_struct, GetPlaceIDbyAddressResult),
		NULL,
		&__PlaceInfo_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetPlaceIDbyAddress_map =
{
	0x637EFA02,
	"GetPlaceIDbyAddress",
	L"GetPlaceIDbyAddressResponse",
	sizeof("GetPlaceIDbyAddress")-1,
	sizeof("GetPlaceIDbyAddressResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetPlaceIDbyAddress_entries,
	sizeof(__CDemographixQuery_GetPlaceIDbyAddress_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetLocationInformationByLatitudeLongitude_struct
{
	double Latitude;
	double Longitude;
	BSTR LicenseKey;
	SummaryInformation GetLocationInformationByLatitudeLongitudeResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetLocationInformationByLatitudeLongitude_entries[] =
{

	{
		0x64A048BC, 
		"Latitude", 
		L"Latitude", 
		sizeof("Latitude")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetLocationInformationByLatitudeLongitude_struct, Latitude),
		NULL,
		NULL,
		-1,
	},
	{
		0x10F3F56B, 
		"Longitude", 
		L"Longitude", 
		sizeof("Longitude")-1, 
		SOAPTYPE_DECIMAL, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetLocationInformationByLatitudeLongitude_struct, Longitude),
		NULL,
		NULL,
		-1,
	},
	{
		0xBF4DE04C, 
		"LicenseKey", 
		L"LicenseKey", 
		sizeof("LicenseKey")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetLocationInformationByLatitudeLongitude_struct, LicenseKey),
		NULL,
		NULL,
		-1,
	},
	{
		0x74ABB9C0, 
		"GetLocationInformationByLatitudeLongitudeResult", 
		L"GetLocationInformationByLatitudeLongitudeResult", 
		sizeof("GetLocationInformationByLatitudeLongitudeResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetLocationInformationByLatitudeLongitude_struct, GetLocationInformationByLatitudeLongitudeResult),
		NULL,
		&__SummaryInformation_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetLocationInformationByLatitudeLongitude_map =
{
	0x4E284A10,
	"GetLocationInformationByLatitudeLongitude",
	L"GetLocationInformationByLatitudeLongitudeResponse",
	sizeof("GetLocationInformationByLatitudeLongitude")-1,
	sizeof("GetLocationInformationByLatitudeLongitudeResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetLocationInformationByLatitudeLongitude_entries,
	sizeof(__CDemographixQuery_GetLocationInformationByLatitudeLongitude_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_struct
{
	BSTR StateAbbr;
	BSTR PlaceID;
	CensusInfoWithDataSet GetNeighborhoodVehiclesPerHouseholdInDatasetResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_entries[] =
{

	{
		0x12DD84D8, 
		"StateAbbr", 
		L"StateAbbr", 
		sizeof("StateAbbr")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_struct, StateAbbr),
		NULL,
		NULL,
		-1,
	},
	{
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_struct, PlaceID),
		NULL,
		NULL,
		-1,
	},
	{
		0xBA272699, 
		"GetNeighborhoodVehiclesPerHouseholdInDatasetResult", 
		L"GetNeighborhoodVehiclesPerHouseholdInDatasetResult", 
		sizeof("GetNeighborhoodVehiclesPerHouseholdInDatasetResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_struct, GetNeighborhoodVehiclesPerHouseholdInDatasetResult),
		NULL,
		&__CensusInfoWithDataSet_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_map =
{
	0xE0325129,
	"GetNeighborhoodVehiclesPerHouseholdInDataset",
	L"GetNeighborhoodVehiclesPerHouseholdInDatasetResponse",
	sizeof("GetNeighborhoodVehiclesPerHouseholdInDataset")-1,
	sizeof("GetNeighborhoodVehiclesPerHouseholdInDatasetResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_entries,
	sizeof(__CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetNeighborhoodYearStructuresBuilt_struct
{
	BSTR StateAbbr;
	BSTR PlaceID;
	CensusInfoWithDataSet GetNeighborhoodYearStructuresBuiltResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetNeighborhoodYearStructuresBuilt_entries[] =
{

	{
		0x12DD84D8, 
		"StateAbbr", 
		L"StateAbbr", 
		sizeof("StateAbbr")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodYearStructuresBuilt_struct, StateAbbr),
		NULL,
		NULL,
		-1,
	},
	{
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodYearStructuresBuilt_struct, PlaceID),
		NULL,
		NULL,
		-1,
	},
	{
		0x8788EEAC, 
		"GetNeighborhoodYearStructuresBuiltResult", 
		L"GetNeighborhoodYearStructuresBuiltResult", 
		sizeof("GetNeighborhoodYearStructuresBuiltResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetNeighborhoodYearStructuresBuilt_struct, GetNeighborhoodYearStructuresBuiltResult),
		NULL,
		&__CensusInfoWithDataSet_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetNeighborhoodYearStructuresBuilt_map =
{
	0x8D2669FC,
	"GetNeighborhoodYearStructuresBuilt",
	L"GetNeighborhoodYearStructuresBuiltResponse",
	sizeof("GetNeighborhoodYearStructuresBuilt")-1,
	sizeof("GetNeighborhoodYearStructuresBuiltResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetNeighborhoodYearStructuresBuilt_entries,
	sizeof(__CDemographixQuery_GetNeighborhoodYearStructuresBuilt_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetIncomeHouseValueByAddress_struct
{
	BSTR AddressLine1;
	BSTR City;
	BSTR StateAbbrev;
	BSTR ZipCode;
	BSTR LicenseKey;
	IncomeAndHouseValue GetIncomeHouseValueByAddressResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetIncomeHouseValueByAddress_entries[] =
{

	{
		0xD5CAC95F, 
		"AddressLine1", 
		L"AddressLine1", 
		sizeof("AddressLine1")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetIncomeHouseValueByAddress_struct, AddressLine1),
		NULL,
		NULL,
		-1,
	},
	{
		0x00268B79, 
		"City", 
		L"City", 
		sizeof("City")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetIncomeHouseValueByAddress_struct, City),
		NULL,
		NULL,
		-1,
	},
	{
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetIncomeHouseValueByAddress_struct, StateAbbrev),
		NULL,
		NULL,
		-1,
	},
	{
		0x0CF8612E, 
		"ZipCode", 
		L"ZipCode", 
		sizeof("ZipCode")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetIncomeHouseValueByAddress_struct, ZipCode),
		NULL,
		NULL,
		-1,
	},
	{
		0xBF4DE04C, 
		"LicenseKey", 
		L"LicenseKey", 
		sizeof("LicenseKey")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetIncomeHouseValueByAddress_struct, LicenseKey),
		NULL,
		NULL,
		-1,
	},
	{
		0xCF91A5DC, 
		"GetIncomeHouseValueByAddressResult", 
		L"GetIncomeHouseValueByAddressResult", 
		sizeof("GetIncomeHouseValueByAddressResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetIncomeHouseValueByAddress_struct, GetIncomeHouseValueByAddressResult),
		NULL,
		&__IncomeAndHouseValue_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetIncomeHouseValueByAddress_map =
{
	0xFA39AD2C,
	"GetIncomeHouseValueByAddress",
	L"GetIncomeHouseValueByAddressResponse",
	sizeof("GetIncomeHouseValueByAddress")-1,
	sizeof("GetIncomeHouseValueByAddressResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetIncomeHouseValueByAddress_entries,
	sizeof(__CDemographixQuery_GetIncomeHouseValueByAddress_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_struct
{
	BSTR StateAbbrev;
	BSTR PlaceID;
	CensusInfoWithDataSet GetNeighborhoodAgeGenderMaleInDataSetResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_entries[] =
{

	{
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_struct, StateAbbrev),
		NULL,
		NULL,
		-1,
	},
	{
		0x113369F2, 
		"PlaceID", 
		L"PlaceID", 
		sizeof("PlaceID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_struct, PlaceID),
		NULL,
		NULL,
		-1,
	},
	{
		0x7344A155, 
		"GetNeighborhoodAgeGenderMaleInDataSetResult", 
		L"GetNeighborhoodAgeGenderMaleInDataSetResult", 
		sizeof("GetNeighborhoodAgeGenderMaleInDataSetResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_struct, GetNeighborhoodAgeGenderMaleInDataSetResult),
		NULL,
		&__CensusInfoWithDataSet_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_map =
{
	0x56996AE5,
	"GetNeighborhoodAgeGenderMaleInDataSet",
	L"GetNeighborhoodAgeGenderMaleInDataSetResponse",
	sizeof("GetNeighborhoodAgeGenderMaleInDataSet")-1,
	sizeof("GetNeighborhoodAgeGenderMaleInDataSetResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_entries,
	sizeof(__CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};


struct __CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_struct
{
	BSTR StateAbbrev;
	BSTR CensusTract;
	BSTR CensusBlock;
	BSTR LicenseKey;
	PlaceInfo GetPlaceIDbyCensusTractAndBlockResult;
};

extern __declspec(selectany) const _soapmapentry __CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_entries[] =
{

	{
		0x40522853, 
		"StateAbbrev", 
		L"StateAbbrev", 
		sizeof("StateAbbrev")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_struct, StateAbbrev),
		NULL,
		NULL,
		-1,
	},
	{
		0xEF491D6F, 
		"CensusTract", 
		L"CensusTract", 
		sizeof("CensusTract")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_struct, CensusTract),
		NULL,
		NULL,
		-1,
	},
	{
		0xEE00559C, 
		"CensusBlock", 
		L"CensusBlock", 
		sizeof("CensusBlock")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_struct, CensusBlock),
		NULL,
		NULL,
		-1,
	},
	{
		0xBF4DE04C, 
		"LicenseKey", 
		L"LicenseKey", 
		sizeof("LicenseKey")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_struct, LicenseKey),
		NULL,
		NULL,
		-1,
	},
	{
		0x07C5EE79, 
		"GetPlaceIDbyCensusTractAndBlockResult", 
		L"GetPlaceIDbyCensusTractAndBlockResult", 
		sizeof("GetPlaceIDbyCensusTractAndBlockResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_struct, GetPlaceIDbyCensusTractAndBlockResult),
		NULL,
		&__PlaceInfo_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_map =
{
	0x10A29109,
	"GetPlaceIDbyCensusTractAndBlock",
	L"GetPlaceIDbyCensusTractAndBlockResponse",
	sizeof("GetPlaceIDbyCensusTractAndBlock")-1,
	sizeof("GetPlaceIDbyCensusTractAndBlockResponse")-1,
	SOAPMAP_FUNC,
	__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_entries,
	sizeof(__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xE3186EC1,
	"http://ws.cdyne.com/DemographixWS",
	L"http://ws.cdyne.com/DemographixWS",
	sizeof("http://ws.cdyne.com/DemographixWS")-1
};

extern __declspec(selectany) const _soapmap * __CDemographixQuery_funcs[] =
{
	&__CDemographixQuery_GetNeighborhoodLinguisticIsolation_map,
	&__CDemographixQuery_GetNeighborhoodAgeInDataSet_map,
	&__CDemographixQuery_GetVersion_map,
	&__CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_map,
	&__CDemographixQuery_GetSummaryInformationByPlaceID_map,
	&__CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_map,
	&__CDemographixQuery_GetLocationInformationByAddress_map,
	&__CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_map,
	&__CDemographixQuery_GetPlaceIDbyAddress_map,
	&__CDemographixQuery_GetLocationInformationByLatitudeLongitude_map,
	&__CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_map,
	&__CDemographixQuery_GetNeighborhoodYearStructuresBuilt_map,
	&__CDemographixQuery_GetIncomeHouseValueByAddress_map,
	&__CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_map,
	&__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_map,
	NULL
};

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetNeighborhoodLinguisticIsolation(
		BSTR StateAbbr, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodLinguisticIsolationResult
	)
{
    if ( GetNeighborhoodLinguisticIsolationResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetNeighborhoodLinguisticIsolation_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbr = StateAbbr;
	__params.PlaceID = PlaceID;

	__atlsoap_hr = SetClientStruct(&__params, 0);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetNeighborhoodLinguisticIsolation\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetNeighborhoodLinguisticIsolationResult = __params.GetNeighborhoodLinguisticIsolationResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetNeighborhoodAgeInDataSet(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodAgeInDataSetResult
	)
{
    if ( GetNeighborhoodAgeInDataSetResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetNeighborhoodAgeInDataSet_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbrev = StateAbbrev;
	__params.PlaceID = PlaceID;

	__atlsoap_hr = SetClientStruct(&__params, 1);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetNeighborhoodAgeInDataSet\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetNeighborhoodAgeInDataSetResult = __params.GetNeighborhoodAgeInDataSetResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetVersion(
		BSTR* GetVersionResult
	)
{
    if ( GetVersionResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetVersion_struct __params;
	memset(&__params, 0x00, sizeof(__params));

	__atlsoap_hr = SetClientStruct(&__params, 2);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetVersion\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetVersionResult = __params.GetVersionResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset(
		BSTR StateAbbr, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult
	)
{
    if ( GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbr = StateAbbr;
	__params.PlaceID = PlaceID;

	__atlsoap_hr = SetClientStruct(&__params, 3);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult = __params.GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetSummaryInformationByPlaceID(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		SummaryInformation* GetSummaryInformationByPlaceIDResult
	)
{
    if ( GetSummaryInformationByPlaceIDResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetSummaryInformationByPlaceID_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbrev = StateAbbrev;
	__params.PlaceID = PlaceID;

	__atlsoap_hr = SetClientStruct(&__params, 4);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetSummaryInformationByPlaceID\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetSummaryInformationByPlaceIDResult = __params.GetSummaryInformationByPlaceIDResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetNeighborhoodRealtyValueInDataSet(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodRealtyValueInDataSetResult
	)
{
    if ( GetNeighborhoodRealtyValueInDataSetResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbrev = StateAbbrev;
	__params.PlaceID = PlaceID;

	__atlsoap_hr = SetClientStruct(&__params, 5);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetNeighborhoodRealtyValueInDataSet\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetNeighborhoodRealtyValueInDataSetResult = __params.GetNeighborhoodRealtyValueInDataSetResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetLocationInformationByAddress(
		BSTR AddressLine1, 
		BSTR City, 
		BSTR StateAbbrev, 
		BSTR ZipCode, 
		BSTR LicenseKey, 
		SummaryInformation* GetLocationInformationByAddressResult
	)
{
    if ( GetLocationInformationByAddressResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetLocationInformationByAddress_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.AddressLine1 = AddressLine1;
	__params.City = City;
	__params.StateAbbrev = StateAbbrev;
	__params.ZipCode = ZipCode;
	__params.LicenseKey = LicenseKey;

	__atlsoap_hr = SetClientStruct(&__params, 6);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetLocationInformationByAddress\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetLocationInformationByAddressResult = __params.GetLocationInformationByAddressResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetNeighborhoodAgeGenderFemaleInDataSet(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodAgeGenderFemaleInDataSetResult
	)
{
    if ( GetNeighborhoodAgeGenderFemaleInDataSetResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbrev = StateAbbrev;
	__params.PlaceID = PlaceID;

	__atlsoap_hr = SetClientStruct(&__params, 7);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetNeighborhoodAgeGenderFemaleInDataSet\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetNeighborhoodAgeGenderFemaleInDataSetResult = __params.GetNeighborhoodAgeGenderFemaleInDataSetResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetPlaceIDbyAddress(
		BSTR AddressLine1, 
		BSTR City, 
		BSTR StateAbbrev, 
		BSTR ZipCode, 
		BSTR LicenseKey, 
		PlaceInfo* GetPlaceIDbyAddressResult
	)
{
    if ( GetPlaceIDbyAddressResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetPlaceIDbyAddress_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.AddressLine1 = AddressLine1;
	__params.City = City;
	__params.StateAbbrev = StateAbbrev;
	__params.ZipCode = ZipCode;
	__params.LicenseKey = LicenseKey;

	__atlsoap_hr = SetClientStruct(&__params, 8);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetPlaceIDbyAddress\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetPlaceIDbyAddressResult = __params.GetPlaceIDbyAddressResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetLocationInformationByLatitudeLongitude(
		double Latitude, 
		double Longitude, 
		BSTR LicenseKey, 
		SummaryInformation* GetLocationInformationByLatitudeLongitudeResult
	)
{
    if ( GetLocationInformationByLatitudeLongitudeResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetLocationInformationByLatitudeLongitude_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.Latitude = Latitude;
	__params.Longitude = Longitude;
	__params.LicenseKey = LicenseKey;

	__atlsoap_hr = SetClientStruct(&__params, 9);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetLocationInformationByLatitudeLongitude\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetLocationInformationByLatitudeLongitudeResult = __params.GetLocationInformationByLatitudeLongitudeResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetNeighborhoodVehiclesPerHouseholdInDataset(
		BSTR StateAbbr, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodVehiclesPerHouseholdInDatasetResult
	)
{
    if ( GetNeighborhoodVehiclesPerHouseholdInDatasetResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbr = StateAbbr;
	__params.PlaceID = PlaceID;

	__atlsoap_hr = SetClientStruct(&__params, 10);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetNeighborhoodVehiclesPerHouseholdInDataset\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetNeighborhoodVehiclesPerHouseholdInDatasetResult = __params.GetNeighborhoodVehiclesPerHouseholdInDatasetResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetNeighborhoodYearStructuresBuilt(
		BSTR StateAbbr, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodYearStructuresBuiltResult
	)
{
    if ( GetNeighborhoodYearStructuresBuiltResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetNeighborhoodYearStructuresBuilt_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbr = StateAbbr;
	__params.PlaceID = PlaceID;

	__atlsoap_hr = SetClientStruct(&__params, 11);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetNeighborhoodYearStructuresBuilt\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetNeighborhoodYearStructuresBuiltResult = __params.GetNeighborhoodYearStructuresBuiltResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetIncomeHouseValueByAddress(
		BSTR AddressLine1, 
		BSTR City, 
		BSTR StateAbbrev, 
		BSTR ZipCode, 
		BSTR LicenseKey, 
		IncomeAndHouseValue* GetIncomeHouseValueByAddressResult
	)
{
    if ( GetIncomeHouseValueByAddressResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetIncomeHouseValueByAddress_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.AddressLine1 = AddressLine1;
	__params.City = City;
	__params.StateAbbrev = StateAbbrev;
	__params.ZipCode = ZipCode;
	__params.LicenseKey = LicenseKey;

	__atlsoap_hr = SetClientStruct(&__params, 12);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetIncomeHouseValueByAddress\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetIncomeHouseValueByAddressResult = __params.GetIncomeHouseValueByAddressResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetNeighborhoodAgeGenderMaleInDataSet(
		BSTR StateAbbrev, 
		BSTR PlaceID, 
		CensusInfoWithDataSet* GetNeighborhoodAgeGenderMaleInDataSetResult
	)
{
    if ( GetNeighborhoodAgeGenderMaleInDataSetResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbrev = StateAbbrev;
	__params.PlaceID = PlaceID;

	__atlsoap_hr = SetClientStruct(&__params, 13);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetNeighborhoodAgeGenderMaleInDataSet\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetNeighborhoodAgeGenderMaleInDataSetResult = __params.GetNeighborhoodAgeGenderMaleInDataSetResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CDemographixQueryT<TClient>::GetPlaceIDbyCensusTractAndBlock(
		BSTR StateAbbrev, 
		BSTR CensusTract, 
		BSTR CensusBlock, 
		BSTR LicenseKey, 
		PlaceInfo* GetPlaceIDbyCensusTractAndBlockResult
	)
{
    if ( GetPlaceIDbyCensusTractAndBlockResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.StateAbbrev = StateAbbrev;
	__params.CensusTract = CensusTract;
	__params.CensusBlock = CensusBlock;
	__params.LicenseKey = LicenseKey;

	__atlsoap_hr = SetClientStruct(&__params, 14);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://ws.cdyne.com/DemographixWS/GetPlaceIDbyCensusTractAndBlock\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetPlaceIDbyCensusTractAndBlockResult = __params.GetPlaceIDbyCensusTractAndBlockResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CDemographixQueryT<TClient>::GetFunctionMap()
{
	return __CDemographixQuery_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CDemographixQueryT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CDemographixQuery_GetNeighborhoodLinguisticIsolation_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetNeighborhoodLinguisticIsolation_atlsoapheader_map = 
	{
		0xF791CC34,
		"GetNeighborhoodLinguisticIsolation",
		L"GetNeighborhoodLinguisticIsolationResponse",
		sizeof("GetNeighborhoodLinguisticIsolation")-1,
		sizeof("GetNeighborhoodLinguisticIsolationResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetNeighborhoodLinguisticIsolation_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetNeighborhoodAgeInDataSet_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetNeighborhoodAgeInDataSet_atlsoapheader_map = 
	{
		0x0124F371,
		"GetNeighborhoodAgeInDataSet",
		L"GetNeighborhoodAgeInDataSetResponse",
		sizeof("GetNeighborhoodAgeInDataSet")-1,
		sizeof("GetNeighborhoodAgeInDataSetResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetNeighborhoodAgeInDataSet_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetVersion_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetVersion_atlsoapheader_map = 
	{
		0xB1784A75,
		"GetVersion",
		L"GetVersionResponse",
		sizeof("GetVersion")-1,
		sizeof("GetVersionResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetVersion_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_atlsoapheader_map = 
	{
		0x518C3B80,
		"GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset",
		L"GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResponse",
		sizeof("GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset")-1,
		sizeof("GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDatasetResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetSummaryInformationByPlaceID_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetSummaryInformationByPlaceID_atlsoapheader_map = 
	{
		0xA5C5CF90,
		"GetSummaryInformationByPlaceID",
		L"GetSummaryInformationByPlaceIDResponse",
		sizeof("GetSummaryInformationByPlaceID")-1,
		sizeof("GetSummaryInformationByPlaceIDResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetSummaryInformationByPlaceID_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_atlsoapheader_map = 
	{
		0x0C430F12,
		"GetNeighborhoodRealtyValueInDataSet",
		L"GetNeighborhoodRealtyValueInDataSetResponse",
		sizeof("GetNeighborhoodRealtyValueInDataSet")-1,
		sizeof("GetNeighborhoodRealtyValueInDataSetResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetLocationInformationByAddress_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetLocationInformationByAddress_atlsoapheader_map = 
	{
		0x69B6B54F,
		"GetLocationInformationByAddress",
		L"GetLocationInformationByAddressResponse",
		sizeof("GetLocationInformationByAddress")-1,
		sizeof("GetLocationInformationByAddressResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetLocationInformationByAddress_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_atlsoapheader_map = 
	{
		0x2FBAEBD0,
		"GetNeighborhoodAgeGenderFemaleInDataSet",
		L"GetNeighborhoodAgeGenderFemaleInDataSetResponse",
		sizeof("GetNeighborhoodAgeGenderFemaleInDataSet")-1,
		sizeof("GetNeighborhoodAgeGenderFemaleInDataSetResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetPlaceIDbyAddress_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetPlaceIDbyAddress_atlsoapheader_map = 
	{
		0x637EFA02,
		"GetPlaceIDbyAddress",
		L"GetPlaceIDbyAddressResponse",
		sizeof("GetPlaceIDbyAddress")-1,
		sizeof("GetPlaceIDbyAddressResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetPlaceIDbyAddress_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetLocationInformationByLatitudeLongitude_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetLocationInformationByLatitudeLongitude_atlsoapheader_map = 
	{
		0x4E284A10,
		"GetLocationInformationByLatitudeLongitude",
		L"GetLocationInformationByLatitudeLongitudeResponse",
		sizeof("GetLocationInformationByLatitudeLongitude")-1,
		sizeof("GetLocationInformationByLatitudeLongitudeResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetLocationInformationByLatitudeLongitude_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_atlsoapheader_map = 
	{
		0xE0325129,
		"GetNeighborhoodVehiclesPerHouseholdInDataset",
		L"GetNeighborhoodVehiclesPerHouseholdInDatasetResponse",
		sizeof("GetNeighborhoodVehiclesPerHouseholdInDataset")-1,
		sizeof("GetNeighborhoodVehiclesPerHouseholdInDatasetResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetNeighborhoodYearStructuresBuilt_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetNeighborhoodYearStructuresBuilt_atlsoapheader_map = 
	{
		0x8D2669FC,
		"GetNeighborhoodYearStructuresBuilt",
		L"GetNeighborhoodYearStructuresBuiltResponse",
		sizeof("GetNeighborhoodYearStructuresBuilt")-1,
		sizeof("GetNeighborhoodYearStructuresBuiltResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetNeighborhoodYearStructuresBuilt_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetIncomeHouseValueByAddress_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetIncomeHouseValueByAddress_atlsoapheader_map = 
	{
		0xFA39AD2C,
		"GetIncomeHouseValueByAddress",
		L"GetIncomeHouseValueByAddressResponse",
		sizeof("GetIncomeHouseValueByAddress")-1,
		sizeof("GetIncomeHouseValueByAddressResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetIncomeHouseValueByAddress_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_atlsoapheader_map = 
	{
		0x56996AE5,
		"GetNeighborhoodAgeGenderMaleInDataSet",
		L"GetNeighborhoodAgeGenderMaleInDataSetResponse",
		sizeof("GetNeighborhoodAgeGenderMaleInDataSet")-1,
		sizeof("GetNeighborhoodAgeGenderMaleInDataSetResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};

	static const _soapmapentry __CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_atlsoapheader_map = 
	{
		0x10A29109,
		"GetPlaceIDbyCensusTractAndBlock",
		L"GetPlaceIDbyCensusTractAndBlockResponse",
		sizeof("GetPlaceIDbyCensusTractAndBlock")-1,
		sizeof("GetPlaceIDbyCensusTractAndBlockResponse")-1,
		SOAPMAP_HEADER,
		__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xE3186EC1,
		"http://ws.cdyne.com/DemographixWS",
		L"http://ws.cdyne.com/DemographixWS",
		sizeof("http://ws.cdyne.com/DemographixWS")-1
	};


	static const _soapmap * __CDemographixQuery_headers[] =
	{
		&__CDemographixQuery_GetNeighborhoodLinguisticIsolation_atlsoapheader_map,
		&__CDemographixQuery_GetNeighborhoodAgeInDataSet_atlsoapheader_map,
		&__CDemographixQuery_GetVersion_atlsoapheader_map,
		&__CDemographixQuery_GetNeighborhoodPlaceofBirthbyCitizenshipStatusInDataset_atlsoapheader_map,
		&__CDemographixQuery_GetSummaryInformationByPlaceID_atlsoapheader_map,
		&__CDemographixQuery_GetNeighborhoodRealtyValueInDataSet_atlsoapheader_map,
		&__CDemographixQuery_GetLocationInformationByAddress_atlsoapheader_map,
		&__CDemographixQuery_GetNeighborhoodAgeGenderFemaleInDataSet_atlsoapheader_map,
		&__CDemographixQuery_GetPlaceIDbyAddress_atlsoapheader_map,
		&__CDemographixQuery_GetLocationInformationByLatitudeLongitude_atlsoapheader_map,
		&__CDemographixQuery_GetNeighborhoodVehiclesPerHouseholdInDataset_atlsoapheader_map,
		&__CDemographixQuery_GetNeighborhoodYearStructuresBuilt_atlsoapheader_map,
		&__CDemographixQuery_GetIncomeHouseValueByAddress_atlsoapheader_map,
		&__CDemographixQuery_GetNeighborhoodAgeGenderMaleInDataSet_atlsoapheader_map,
		&__CDemographixQuery_GetPlaceIDbyCensusTractAndBlock_atlsoapheader_map,
		NULL
	};
	
	return __CDemographixQuery_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CDemographixQueryT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CDemographixQueryT<TClient>::GetNamespaceUri()
{
	return L"http://ws.cdyne.com/DemographixWS";
}

template <typename TClient>
ATL_NOINLINE inline const char * CDemographixQueryT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CDemographixQueryT<TClient>::GetNamespaceUriA()
{
	return "http://ws.cdyne.com/DemographixWS";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CDemographixQueryT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CDemographixQueryT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
{
	if (ppReader == NULL)
	{
		return E_INVALIDARG;
	}
	
	CComPtr<ISAXXMLReader> spReader = GetReader();
	if (spReader.p != NULL)
	{
		*ppReader = spReader.Detach();
		return S_OK;
	}
	return TClient::GetClientReader(ppReader);
}

} // namespace DemographixQuery

template<>
inline HRESULT AtlCleanupValue<DemographixQuery::RacePercentagesCls>(DemographixQuery::RacePercentagesCls *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->Asian);
	AtlCleanupValue(&pVal->Black);
	AtlCleanupValue(&pVal->Indian);
	AtlCleanupValue(&pVal->Mixed);
	AtlCleanupValue(&pVal->NativeHawaiian);
	AtlCleanupValue(&pVal->Other);
	AtlCleanupValue(&pVal->White);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<DemographixQuery::RacePercentagesCls>(DemographixQuery::RacePercentagesCls *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->Asian, pMemMgr);
	AtlCleanupValueEx(&pVal->Black, pMemMgr);
	AtlCleanupValueEx(&pVal->Indian, pMemMgr);
	AtlCleanupValueEx(&pVal->Mixed, pMemMgr);
	AtlCleanupValueEx(&pVal->NativeHawaiian, pMemMgr);
	AtlCleanupValueEx(&pVal->Other, pMemMgr);
	AtlCleanupValueEx(&pVal->White, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<DemographixQuery::PlaceInformationCls>(DemographixQuery::PlaceInformationCls *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->StateAbbrev);
	AtlCleanupValue(&pVal->PlaceID);
	AtlCleanupValue(&pVal->Rural);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<DemographixQuery::PlaceInformationCls>(DemographixQuery::PlaceInformationCls *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->StateAbbrev, pMemMgr);
	AtlCleanupValueEx(&pVal->PlaceID, pMemMgr);
	AtlCleanupValueEx(&pVal->Rural, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<DemographixQuery::PlaceInfo>(DemographixQuery::PlaceInfo *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->PlaceInformation);
	AtlCleanupValue(&pVal->Error);
	AtlCleanupValue(&pVal->ErrorString);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<DemographixQuery::PlaceInfo>(DemographixQuery::PlaceInfo *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->PlaceInformation, pMemMgr);
	AtlCleanupValueEx(&pVal->Error, pMemMgr);
	AtlCleanupValueEx(&pVal->ErrorString, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<DemographixQuery::CensusInfoWithDataSet>(DemographixQuery::CensusInfoWithDataSet *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->Error);
	AtlCleanupValue(&pVal->ErrorString);
	AtlCleanupValue(&pVal->CensusDataSet);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<DemographixQuery::CensusInfoWithDataSet>(DemographixQuery::CensusInfoWithDataSet *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->Error, pMemMgr);
	AtlCleanupValueEx(&pVal->ErrorString, pMemMgr);
	AtlCleanupValueEx(&pVal->CensusDataSet, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<DemographixQuery::MaritalStatusPercentagesCls>(DemographixQuery::MaritalStatusPercentagesCls *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->NeverMarried);
	AtlCleanupValue(&pVal->Married);
	AtlCleanupValue(&pVal->Separated);
	AtlCleanupValue(&pVal->MarriedOther);
	AtlCleanupValue(&pVal->Widowed);
	AtlCleanupValue(&pVal->Divorced);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<DemographixQuery::MaritalStatusPercentagesCls>(DemographixQuery::MaritalStatusPercentagesCls *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->NeverMarried, pMemMgr);
	AtlCleanupValueEx(&pVal->Married, pMemMgr);
	AtlCleanupValueEx(&pVal->Separated, pMemMgr);
	AtlCleanupValueEx(&pVal->MarriedOther, pMemMgr);
	AtlCleanupValueEx(&pVal->Widowed, pMemMgr);
	AtlCleanupValueEx(&pVal->Divorced, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<DemographixQuery::GenderPercentagesCls>(DemographixQuery::GenderPercentagesCls *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->Female);
	AtlCleanupValue(&pVal->Male);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<DemographixQuery::GenderPercentagesCls>(DemographixQuery::GenderPercentagesCls *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->Female, pMemMgr);
	AtlCleanupValueEx(&pVal->Male, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<DemographixQuery::SummaryInformation>(DemographixQuery::SummaryInformation *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->PlaceInformation);
	AtlCleanupValue(&pVal->Error);
	AtlCleanupValue(&pVal->ErrorString);
	AtlCleanupValue(&pVal->MedianAge);
	AtlCleanupValue(&pVal->MedianIncome);
	AtlCleanupValue(&pVal->MedianRoomsInHouse);
	AtlCleanupValue(&pVal->MedianHouseValue);
	AtlCleanupValue(&pVal->MedianVehicles);
	AtlCleanupValue(&pVal->MaritalStatusPercentages);
	AtlCleanupValue(&pVal->RacePercentages);
	AtlCleanupValue(&pVal->GenderPercentages);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<DemographixQuery::SummaryInformation>(DemographixQuery::SummaryInformation *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->PlaceInformation, pMemMgr);
	AtlCleanupValueEx(&pVal->Error, pMemMgr);
	AtlCleanupValueEx(&pVal->ErrorString, pMemMgr);
	AtlCleanupValueEx(&pVal->MedianAge, pMemMgr);
	AtlCleanupValueEx(&pVal->MedianIncome, pMemMgr);
	AtlCleanupValueEx(&pVal->MedianRoomsInHouse, pMemMgr);
	AtlCleanupValueEx(&pVal->MedianHouseValue, pMemMgr);
	AtlCleanupValueEx(&pVal->MedianVehicles, pMemMgr);
	AtlCleanupValueEx(&pVal->MaritalStatusPercentages, pMemMgr);
	AtlCleanupValueEx(&pVal->RacePercentages, pMemMgr);
	AtlCleanupValueEx(&pVal->GenderPercentages, pMemMgr);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValue<DemographixQuery::IncomeAndHouseValue>(DemographixQuery::IncomeAndHouseValue *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->PlaceInformation);
	AtlCleanupValue(&pVal->Error);
	AtlCleanupValue(&pVal->ErrorString);
	AtlCleanupValue(&pVal->MedianIncome);
	AtlCleanupValue(&pVal->MedianHouseValue);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<DemographixQuery::IncomeAndHouseValue>(DemographixQuery::IncomeAndHouseValue *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->PlaceInformation, pMemMgr);
	AtlCleanupValueEx(&pVal->Error, pMemMgr);
	AtlCleanupValueEx(&pVal->ErrorString, pMemMgr);
	AtlCleanupValueEx(&pVal->MedianIncome, pMemMgr);
	AtlCleanupValueEx(&pVal->MedianHouseValue, pMemMgr);
	return S_OK;
}
