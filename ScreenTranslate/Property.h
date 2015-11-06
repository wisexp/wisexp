#pragma once

#include <vector>
#include <map>
#include <regex>
#include <sstream>
#include <fstream>





#define DECLAR_PROPERTY(a, b) m_properties[std::wstring(L#a)] = std::make_pair(std::wstring(b), 0);

struct Property
{


    std::map<std::wstring, std::pair<std::wstring, int>> m_properties;

    static const std::vector<std::wstring>& KnownUnmatchedPattern()
    {
        static std::vector<std::wstring> pattern;
        static bool initialized = false;

        if (!initialized)
        {
            initialized = true;
            std::wifstream ifs("knownUnmatchedPattern.txt");
            do
            {
                wchar_t buf[1024];
                ifs.getline(buf, 1024);
                pattern.push_back(std::wstring(buf));
            } while (ifs.good());
            ifs.close();
        }
        return pattern;
    }
    
    Property()
    {
        DECLAR_PROPERTY(Magic, L"\\+(\\d+)TOMAG1C")
            DECLAR_PROPERTY(Strength, L"\\+(\\d+)TOSTRENGTH")
            DECLAR_PROPERTY(Dexterity, L"\\+(\\d+)TODEXTER1TY")
            DECLAR_PROPERTY(Vitality, L"\\+(\\d+)TOV1TAL1TY")
            DECLAR_PROPERTY(AllAttributes, L"\\+(\\d+)TOALLATTR1BUTES")
            DECLAR_PROPERTY(ResistCold, L"RES1STCOLD:\\+(\\d+)%")
            DECLAR_PROPERTY(ResistFire, L"RES1STF1RE:\\+(\\d+)%")
            DECLAR_PROPERTY(ResistMagic, L"RES1STMAG1C:\\+(\\d+)%")
            DECLAR_PROPERTY(ResistLightning, L"RES1STL1GHTN1NG:\\+(\\d+)%")
            DECLAR_PROPERTY(ResistAll, L"RES1STALL:\\+(\\d+)%")
            DECLAR_PROPERTY(MaxResistAll, L"MAX1MUMRES1STALL:\\+(\\d+)%")
            DECLAR_PROPERTY(IAS, L"\\+(\\d+)%1NCREASEDATTACKSP.*")
            DECLAR_PROPERTY(IAS2, L"ATTACKSPEED:\\+(\\d+)%")
            DECLAR_PROPERTY(CriticalHitDamage, L"CR1T1CALH1TDAMAGE:\\+(\\d+)%")
            DECLAR_PROPERTY(StealLife, L"H1TSTEALS(\\d+)%L1FE")
            DECLAR_PROPERTY(StealMana, L"H1TSTEALS(\\d+)%MANA")
            DECLAR_PROPERTY(EnhancedDamage, L"\\+(\\d+)%ENHANCEDDAMAGE")
            DECLAR_PROPERTY(EnhancedArmor, L"\\+(\\d+)%ENHANCEDARMOR")
            DECLAR_PROPERTY(HitPoints, L"H1TPO1NTS:\\+(\\d+)")
            DECLAR_PROPERTY(Mana, L"MANA:\\+(\\d+)")
            DECLAR_PROPERTY(MeteorCluster, L"\\+(\\d+)TOMETEORCLUSTERSPELL")
            DECLAR_PROPERTY(Golem, L"\\+(\\d+)TOGOLEMSPELL")
            DECLAR_PROPERTY(Guardian, L"\\+(\\d+)TOGUARD1ANSPELL")
            DECLAR_PROPERTY(Spell, L"SPELLSARE1NCREASED(\\d+)LEVELS?")
            DECLAR_PROPERTY(AddDamage, L"ADDS(\\d+)PO1NTSTODAMAG.*")
            DECLAR_PROPERTY(AddElementDamage, L"ADDS\\d+TO(\\d+).*DAMAG.*")
            DECLAR_PROPERTY(MF, L"\\+(\\d+)%CHANCEOFGETT1NGMAG1C1TEMS")
    }

    void Parse(const std::vector<std::wstring>& lines)
    {
        TRACE(L"======================\r\n");
        for (auto& l : lines)
        {
            std::wsmatch sm;
            const std::wstring& src = l;
            if (l.size() < 6) continue;
            bool matched = false;
            for (auto& p : m_properties)
            {
                int value = 0;
                auto pattern = std::wregex(p.second.first.c_str());
                std::regex_match(src, sm, pattern);
                if (sm.size() == 2)
                {
                    std::wstring ws(sm[1].first, sm[1].second);
                    std::wstringstream ss(ws);
                    ss >> value;
                    p.second.second += value;
                    TRACE(L"%s: %d\r\n", p.first.c_str(), p.second.second);
                    matched = true;
                    break;
                }
            }

            if (!matched)
            {
                bool known = false;
                for (const auto& p : KnownUnmatchedPattern())
                {
                    auto pattern = std::wregex(p.c_str());
                    if (std::regex_match(l, pattern))
                    {
                        known = true;
                        break;
                    }
                }
                if (!known)
                {
                    std::wofstream ofs("unmatched.txt", std::ofstream::app);
                    ofs << l << std::endl;
                    ofs.close();
                }
            }
        }
        int toAll = m_properties[L"AllAttributes"].second;
        m_properties[L"Strength"].second += toAll;
        m_properties[L"Dexterity"].second += toAll;
        m_properties[L"Vitality"].second += toAll;
        m_properties[L"Magic"].second += toAll;

        toAll = m_properties[L"ResistAll"].second;
        m_properties[L"ResistCold"].second += toAll;
        m_properties[L"ResistFire"].second += toAll;
        m_properties[L"ResistMagic"].second += toAll;
        m_properties[L"ResistLightning"].second += toAll;

        m_properties[L"IAS"].second += m_properties[L"IAS2"].second;
        //m_properties[L"AddDamage"].second += m_properties[L"AddElementDamage"].second;
        TRACE(L"======================\r\n");

        
    }
       


};
