#include "ExtraDataList.h"

bool ExtraDataList::Contains(ExtraData aType) const
{
    if(bitfield)
    {
        const auto value = static_cast<uint32_t>(aType);
        const auto index = value >> 3;

        const auto element = bitfield->data[index];

        return (element >> (value % 8)) & 1;
    }

    return false;
}

BSExtraData* ExtraDataList::GetByType(ExtraData aType) const
{
    BSScopedLock<BSRecursiveLock> _(lock);

    if (!Contains(aType))
        return nullptr;

    auto pEntry = data;
#if TP_SKYRIM
    while (pEntry != nullptr && pEntry->GetType() != aType)
#else
    while (pEntry != nullptr && pEntry->type != aType)
#endif
    {
        pEntry = pEntry->next;
    }

    return pEntry;
}

bool ExtraDataList::Add(ExtraData aType, BSExtraData* apNewData)
{
    if (Contains(aType))
        return false;

    BSScopedLock<BSRecursiveLock> _(lock);

    BSExtraData* pNext = data;
    data = apNewData;
    apNewData->next = pNext;
    SetType(aType, false);

    return true;
}

uint32_t ExtraDataList::GetCount() const
{
    uint32_t count = 0;

    BSExtraData* pNext = data;
    while (pNext)
    {
        count++;
        pNext = pNext->next;
    }

    return count;
}

void ExtraDataList::SetType(ExtraData aType, bool aClear)
{
    uint32_t index = static_cast<uint8_t>(aType) >> 3;
    uint8_t bitmask = 1 << (static_cast<uint8_t>(aType) % 8);
    uint8_t& flag = bitfield->data[index];
    if (aClear)
        flag &= ~bitmask;
    else
        flag |= bitmask;
}


void ExtraDataList::SetSoulData(SOUL_LEVEL aSoulLevel) noexcept
{
    TP_THIS_FUNCTION(TSetSoulData, void, ExtraDataList, SOUL_LEVEL aSoulLevel);
    POINTER_SKYRIMSE(TSetSoulData, setSoulData, 0x14011AF60 - 0x140000000);
    ThisCall(setSoulData, this, aSoulLevel);
}

void ExtraDataList::SetChargeData(float aCharge) noexcept
{
    TP_THIS_FUNCTION(TSetChargeData, void, ExtraDataList, float aCharge);
    POINTER_SKYRIMSE(TSetChargeData, setChargeData, 0x14011AF60 - 0x140000000);
    ThisCall(setChargeData, this, aCharge);
}

void ExtraDataList::SetWorn(bool aWornLeft) noexcept
{
    // TODO: what's this bool? seems to be true always except for one instance
    TP_THIS_FUNCTION(TSetWornData, void, ExtraDataList, bool aUnk1, bool aWornLeft);
    POINTER_SKYRIMSE(TSetWornData, setWornData, 0x14011A5D0 - 0x140000000);
    ThisCall(setWornData, this, true, aWornLeft);
}

void ExtraDataList::SetPoison(AlchemyItem* apItem, uint32_t aCount) noexcept
{
    TP_THIS_FUNCTION(TSetPoison, void, ExtraDataList, AlchemyItem* apItem, uint32_t aCount);
    POINTER_SKYRIMSE(TSetPoison, setPoison, 0x140124160 - 0x140000000);
    ThisCall(setPoison, this, apItem, aCount);
}

void ExtraDataList::SetHealth(float aHealth) noexcept
{
    TP_THIS_FUNCTION(TSetHealth, void, ExtraDataList, float aHealth);
    POINTER_SKYRIMSE(TSetHealth, setHealth, 0x14011AB30 - 0x140000000);
    ThisCall(setHealth, this, aHealth);
}

void ExtraDataList::SetEnchantmentData(EnchantmentItem* apItem, uint16_t aCharge, bool aRemoveOnUnequip) noexcept
{
    TP_THIS_FUNCTION(TSetEnchantmentData, void, ExtraDataList, EnchantmentItem* apItem, uint16_t aCharge, bool aRemoveOnUnequip);
    POINTER_SKYRIMSE(TSetEnchantmentData, setEnchantmentData, 0x14012D670 - 0x140000000);
    ThisCall(setEnchantmentData, this, apItem, aCharge, aRemoveOnUnequip);
}
