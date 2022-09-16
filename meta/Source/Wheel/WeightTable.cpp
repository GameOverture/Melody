#include "pch.h"
#include "WeightTable.h"

WeightTable::WeightTable()
{
}

WeightTable::~WeightTable()
{
	for(size_t i = 0; i < m_EntryList.size(); ++i)
		delete m_EntryList[i];
}

uint32 WeightTable::GetTotalWeight() const
{
	uint32 uiTotalWeight = 0;
	for(auto *pEntry : m_EntryList)
		uiTotalWeight += pEntry->m_uiWeight;

	return uiTotalWeight;
}

uint32 WeightTable::GetNumEntries() const
{
	return static_cast<uint32>(m_EntryList.size());
}

const WeightEntry *WeightTable::GetEntryByIndex(uint32 uiIndex) const
{
	if(uiIndex >= m_EntryList.size())
		return nullptr;

	return m_EntryList[uiIndex];
}

const WeightEntry *WeightTable::PullEntry() const
{
	return GetEntryByIndex(FindIndex(HyRand::Range(0u, GetTotalWeight())));
}

void WeightTable::AddEntry(int iType, int iValue, uint32 uiWeight, std::string sDescription)
{
	m_EntryList.push_back(HY_NEW WeightEntry(m_EntryList.size(), iType, iValue, uiWeight, sDescription));
}

int WeightTable::FindIndex(uint32 uiTarget) const
{
	size_t uiIndex = 0;
	uint32 iCount = m_EntryList[uiIndex]->m_uiWeight;
	size_t uiMaxIndex = m_EntryList.size() - 1;

	while(iCount <= uiTarget && uiIndex < uiMaxIndex)
	{
		uiIndex++;
		iCount += m_EntryList[uiIndex]->m_uiWeight;
	}

	// Check if this 'uiTarget' exceeds the weight table max
	uint32 uiTotalTableWeight = GetTotalWeight();
	if(uiTarget >= uiTotalTableWeight && uiIndex == uiMaxIndex)
	{
		// If the 'uiTarget' exceeds the table max, it uses the last valid entry
		while(m_EntryList[uiIndex]->m_uiWeight == 0)
			uiIndex--;
	}

	return uiIndex;
}
