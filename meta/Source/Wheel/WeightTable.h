#ifndef WeightTable_h__
#define WeightTable_h__

#include "pch.h"

struct WeightEntry
{
	int					m_iIndex;
	int					m_iType;
	int					m_iValue;
	uint32				m_uiWeight;
	std::string			m_sDescription;

	WeightEntry(int iIndex, int iType, int iValue, uint32 uiWeight, std::string sDescription) :
		m_iIndex(iIndex),
		m_iType(iType),
		m_iValue(iValue),
		m_uiWeight(uiWeight),
		m_sDescription(sDescription)
	{ }
};

class WeightTable
{
	std::vector<WeightEntry *> m_EntryList;

public:
	WeightTable();
	~WeightTable();

	uint32 GetTotalWeight() const;
	uint32 GetNumEntries() const;
	const WeightEntry *GetEntryByIndex(uint32 uiIndex) const;
	const WeightEntry *PullEntry() const;

	void AddEntry(int iType, int iValue, uint32 iWeight, std::string sDescription);

protected:
	int FindIndex(uint32 uiTarget) const;
};

#endif // WeightTable_h__
