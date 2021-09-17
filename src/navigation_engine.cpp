/**
 * @file    : Ublox-Driver 
 * @authors : MEHMET AKSU
 * @note    : mehmet.aksu@cezeri.com
 * @date    : 12 / Nisan / 2021
 * @code    : navigation_engine.cpp file
 * @details : 
 */

#include <bitset>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "navigation_engine.hpp"

void NavEng<SatelliteSystem::Galileo>::yakala_page(const std::uint32_t(&word)[10], std::uint8_t svId, UbxSigId sigId)
{
    static const std::map<WordType, std::string> msg_info_map{{WordType::SPARE,                                         std::string{"Spare"}},
                                                              {WordType::EPHEMERIS_1,                                   std::string{"Ephemeris 1"}},
                                                              {WordType::EPHEMERIS_2,                                   std::string{"Ephemeris 2"}},
                                                              {WordType::EPHEMERIS_3,                                   std::string{"Ephemeris 3"}},
                                                              {WordType::EPHEMERIS_4_CLOCK_CORRECTION,                  std::string{"Ephemeris 4 ve Clock Correction"}},
                                                              {WordType::IONOSPHERIC_CORRECTION_BGD_SIG_HEALTH_DVS_GST, std::string{"Ionospheric Correction"}},
                                                              {WordType::GST_UTC_CONVERSION,                            std::string{"UTC"}},
                                                              {WordType::ALMANAC_1,                                     std::string{"Almanac 1"}},
                                                              {WordType::ALMANAC_2,                                     std::string{"Almanac 2"}},
                                                              {WordType::ALMANAC_3,                                     std::string{"Almanac 3"}},
                                                              {WordType::ALMANAC_4,                                     std::string{"Almanac 4"}},
                                                              {WordType::REDUCED_CED,                                   std::string{"REDUCED CED"}},
                                                              {WordType::FEC2,                                          std::string{"FEC2"}},
                                                              {WordType::DUMMY,                                         std::string{"DUMMY"}},
                                                              {WordType::ALERT,                                         std::string{"ALERT"}},
                                                              {WordType::None,                                          std::string{"None"}}
    };

    if (auto idx = msg_info_map.find(as_wordtype(GalileoWord1 {.word = word[0]}.word_type)); !idx->second.empty())
        std::cout << "Galileo " << (sigId == UbxSigId::GALILEO_E1B ? "E1" : "E5") <<
        " PRN : " << std::setw(3) <<  int{svId} << " WordType : " << idx->second.data() << std::endl;

    switch(as_wordtype(GalileoWord1{.word = word[0]}.word_type))
    {
        case WordType::EPHEMERIS_1:
        {
            gal_page_1.decode(word);
            gal_page_1.interpret(data, svId);
            nav_msg_flags_map[svId].new_nav_msg.eph1 = 1;
            break;
        }

        case WordType::EPHEMERIS_2:
        {
            gal_page_2.decode(word);
            gal_page_2.interpret(data, svId);
            nav_msg_flags_map[svId].new_nav_msg.eph2 = 1;
            break;
        }

        case WordType::EPHEMERIS_3:
        {
            gal_page_3.decode(word);
            gal_page_3.interpret(data, svId);
            nav_msg_flags_map[svId].new_nav_msg.eph3 = 1;
            break;
        }

        case WordType::EPHEMERIS_4_CLOCK_CORRECTION:
        {
            gal_page_4.decode(word);
            gal_page_4.interpret(data, svId);
            nav_msg_flags_map[svId].new_nav_msg.eph4 = 1;
            break;
        }

        case WordType::IONOSPHERIC_CORRECTION_BGD_SIG_HEALTH_DVS_GST:
        {
            gal_page_5.decode(word);
            gal_page_5.interpret(data, svId);
            nav_msg_flags_map[svId].new_nav_msg.iono = 1;
            break;
        }

        case WordType::GST_UTC_CONVERSION:
        {
            gal_page_6.decode(word);
            gal_page_6.interpret(data, svId);
            nav_msg_flags_map[svId].new_nav_msg.utc = 1;
            break;
        }

        case WordType::ALMANAC_1:
        {
            if (sigId == UbxSigId::GALILEO_E5bl)
            {
                gal_page_7_e5b.decode(word);
                alm_e5b_svid[svId-1] = gal_page_7_e5b.interpret(data);
            }

            else if (sigId == UbxSigId::GALILEO_E1B)
            {
                gal_page_7_e1.decode(word);
                alm_e1_svid[svId-1] = gal_page_7_e1.interpret(data);
            }
            break;
        }

        case WordType::ALMANAC_2:
        {
            if (sigId == UbxSigId::GALILEO_E5bl)
            {
                gal_page_8_e5b.decode(word);
                UINT8 alm_svid = alm_e5b_svid[svId-1];
                alm_e5b_svid[svId-1] = gal_page_8_e5b.interpret(data, alm_svid);
                nav_msg_flags_map[alm_svid].new_nav_msg.almanac = 1;
            }

            else if (sigId == UbxSigId::GALILEO_E1B)
            {
                gal_page_8_e1.decode(word);
                UINT8 alm_svid = alm_e1_svid[svId-1];
                alm_e1_svid[svId-1] = gal_page_8_e1.interpret(data, alm_svid);
                nav_msg_flags_map[alm_svid].new_nav_msg.almanac = 1;
            }
            break;
        }

        case WordType::ALMANAC_3:
        {
            if (sigId == UbxSigId::GALILEO_E5bl)
            {
                gal_page_9_e5b.decode(word);
                UINT8 alm_svid = alm_e5b_svid[svId-1];
                alm_e5b_svid[svId-1] = gal_page_9_e5b.interpret(data, alm_svid);
                nav_msg_flags_map[alm_svid].new_nav_msg.almanac = 1;
            }

            else if (sigId == UbxSigId::GALILEO_E1B)
            {
                gal_page_9_e1.decode(word);
                UINT8 alm_svid = alm_e1_svid[svId-1];
                alm_e1_svid[svId-1] = gal_page_9_e1.interpret(data, alm_svid);
                nav_msg_flags_map[alm_svid].new_nav_msg.almanac = 1;
            }
            break;
        }

        case WordType::ALMANAC_4:
        {
            if (sigId == UbxSigId::GALILEO_E5bl)
            {
                gal_page_10_e5b.decode(word);
                UINT8 alm_svid = alm_e5b_svid[svId-1];
                gal_page_10_e5b.interpret(data, alm_svid);
                nav_msg_flags_map[alm_svid].new_nav_msg.almanac = 1;
            }

            else if (sigId == UbxSigId::GALILEO_E1B)
            {
                gal_page_10_e1.decode(word);
                UINT8 alm_svid = alm_e1_svid[svId-1];
                gal_page_9_e1.interpret(data, alm_svid);
                nav_msg_flags_map[alm_svid].new_nav_msg.almanac = 1;
            }

            break;
        }

        default:
            break;

    }

    if (nav_msg_flags_map[svId].new_nav_msg.eph1 == 1 &&
        nav_msg_flags_map[svId].new_nav_msg.eph2 == 1 &&
        nav_msg_flags_map[svId].new_nav_msg.eph3 == 1 &&
        nav_msg_flags_map[svId].new_nav_msg.eph4 == 1 &&
        nav_msg_flags_map[svId].new_nav_msg.iono == 1   )
    {
        nav_msg_flags_map[svId].new_nav_msg.ephemeris = 1;
        nav_msg_flags_map[svId].new_nav_msg.eph1      = 0;
        nav_msg_flags_map[svId].new_nav_msg.eph2      = 0;
        nav_msg_flags_map[svId].new_nav_msg.eph3      = 0;
        nav_msg_flags_map[svId].new_nav_msg.eph4      = 0;
    }
    else
    {
        nav_msg_flags_map[svId].new_nav_msg.ephemeris = 0;
    }
}