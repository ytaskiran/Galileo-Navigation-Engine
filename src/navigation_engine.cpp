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

/*void NavEng<SatelliteSystem::GPS>::yakala_subframe_data_LNAV (const std::uint32_t(&word)[10], std::uint8_t svId)
{
    std::cout << "GPS LNAV PRN : " << std::setw(3) <<  UINT32{svId} << " Subframe Id " << UINT32{HOW{.word = word[1]}.subframe_id} << std::endl;

    switch (as_subframe(HOW{.word = word[1]}.subframe_id))
    {
        case Subframe::one:
        {
            L1_Subframe1.decode(word);
            L1_Subframe1.interpret(&data, svId);
            yeni_subframe_map[svId].new_nav_msg.sbfrm1 = 1;
            week_geldi = true;
            break;
        }
        case Subframe::two:
        {
            L1_Subframe2.decode(word);
            L1_Subframe2.interpret(&data, svId);
            yeni_subframe_map[svId].new_nav_msg.sbfrm2 = 1;
            break;
        }
        case Subframe::three:
        {
            L1_Subframe3.decode(word);
            L1_Subframe3.interpret(&data, svId);
            yeni_subframe_map[svId].new_nav_msg.sbfrm3 = 1;
            break;
        }
        case Subframe::four:
        {
            L1_Subframe4.decode(word);
            L1_Subframe4.interpret(&data);
            if (L1_Subframe4.pageId == SUBFRAME_4_ALMANAC_PAGE)
                yeni_subframe_map[L1_Subframe4.svId].new_nav_msg.sbfrm4 = 1;
            break;
        }
        case Subframe::five:
        {
            L1_Subframe5.decode(word);
            L1_Subframe5.interpret(&data);
            if (L1_Subframe5.pageId == SUBFRAME_5_PAGE_1_THRU_24)
                yeni_subframe_map[L1_Subframe5.svId].new_nav_msg.sbfrm5 = 1;
            break;
        }
        default:
            break;
    }


    std::for_each(yeni_subframe_map.begin(), yeni_subframe_map.end(),
                  [&](auto& pair)
                  {
                      if (pair.second.new_nav_msg.sbfrm1 == 1 &&
                          pair.second.new_nav_msg.sbfrm2 == 1 &&
                          pair.second.new_nav_msg.sbfrm3 == 1   )
                      {
                          pair.second.new_nav_msg.ephemeris = 1;
                          pair.second.new_nav_msg.sbfrm1   = 0;
                          pair.second.new_nav_msg.sbfrm2   = 0;
                          pair.second.new_nav_msg.sbfrm3   = 0;

                          std::cout << "PRN : " << pair.first << " ephemeris geldi." << std::endl;
                      }
                      else
                      {
                          pair.second.new_nav_msg.ephemeris = 0;
                      }


                      if ((pair.second.new_nav_msg.sbfrm4 == 1  ||
                           pair.second.new_nav_msg.sbfrm5 == 1) && week_geldi )
                      {
                          pair.second.new_nav_msg.almanac  = 1;
                          pair.second.new_nav_msg.sbfrm4   = 0;
                          pair.second.new_nav_msg.sbfrm5   = 0;

                          std::cout << "PRN : " << pair.first << " almanac geldi." << std::endl;
                      }
                      else
                      {
                          pair.second.new_nav_msg.almanac  = 0;
                      }

                  });
}*/

void NavEng<SatelliteSystem::Galileo>::processPage(const std::uint32_t(&word)[10], std::uint8_t svId, std::uint8_t sigId)
{
    switch(getType(word[0]))
    {
        case WordType::EPHEMERIS_1:
        {
            gal_page_1.decode(word);
            gal_page_1.interpret(gal_nav_data, svId);
            setFlags(WordType::EPHEMERIS_1, svId);
        }

        case WordType::EPHEMERIS_2:
        {
            gal_page_2.decode(word);
            gal_page_2.interpret(gal_nav_data, svId);
            setFlags(WordType::EPHEMERIS_2, svId);
        }

        case WordType::EPHEMERIS_3:
        {
            gal_page_3.decode(word);
            gal_page_3.interpret(gal_nav_data, svId);
            setFlags(WordType::EPHEMERIS_3, svId);
        }

        case WordType::EPHEMERIS_4__CLOCK_CORRECTION:
        {
            gal_page_4.decode(word);
            gal_page_4.interpret(gal_nav_data, svId);
            setFlags(WordType::EPHEMERIS_4__CLOCK_CORRECTION, svId);
        }

        case WordType::IONOSPHERIC_CORRECTION__BGD__SIG_HEALTH__DVS__GST:
        {
            gal_page_5.decode(word);
            gal_page_5.interpret(gal_nav_data, svId);
        }

        case WordType::GST_UTC_CONVERSION:
        {
            gal_page_6.decode(word);
            gal_page_6.interpret(gal_nav_data, svId);
        }

        case WordType::ALMANAC_1:
        {
            if (sigId == 5)
            {
                gal_page_7_e5b.decode(word);
                alm_e5b_svid[svId-1] = gal_page_7_e5b.interpret(gal_nav_data);
            }

            else if (sigId == 1)
            {
                gal_page_7_e1.decode(word);
                alm_e1_svid[svId-1] = gal_page_7_e1.interpret(gal_nav_data);
            }
        }

        case WordType::ALMANAC_2:
        {
            if (sigId == 5)
            {
                gal_page_8_e5b.decode(word);
                UINT8 former_svid = alm_e5b_svid[svId-1];
                alm_e5b_svid[svId-1] = gal_page_8_e5b.interpret(gal_nav_data, former_svid);
                setFlags(WordType::ALMANAC_2, former_svid);
            }

            else if (sigId == 1)
            {
                gal_page_8_e1.decode(word);
                UINT8 former_svid = alm_e1_svid[svId-1];
                alm_e1_svid[svId-1] = gal_page_8_e1.interpret(gal_nav_data, former_svid);
                setFlags(WordType::ALMANAC_2, former_svid);
            }
        }

        case WordType::ALMANAC_3:
        {
            if (sigId == 5)
            {
                gal_page_9_e5b.decode(word);
                UINT8 former_svid = alm_e5b_svid[svId-1];
                alm_e5b_svid[svId-1] = gal_page_9_e5b.interpret(gal_nav_data, former_svid);
                setFlags(WordType::ALMANAC_3, former_svid);
            }

            else if (sigId == 1)
            {
                gal_page_9_e1.decode(word);
                UINT8 former_svid = alm_e1_svid[svId-1];
                alm_e1_svid[svId-1] = gal_page_9_e1.interpret(gal_nav_data, former_svid);
                setFlags(WordType::ALMANAC_3, former_svid);
            }
        }

        case WordType::ALMANAC_4:
        {
            if (sigId == 5)
            {
                gal_page_10_e5b.decode(word);
                UINT8 former_svid = alm_e5b_svid[svId-1];
                gal_page_10_e5b.interpret(gal_nav_data, former_svid);
                setFlags(WordType::ALMANAC_4, former_svid);
            }

            else if (sigId == 1)
            {
                gal_page_10_e1.decode(word);
                UINT8 former_svid = alm_e1_svid[svId-1];
                gal_page_9_e1.interpret(gal_nav_data, former_svid);
                setFlags(WordType::ALMANAC_4, former_svid);
            }
        }
        
    }
}

WordType NavEng<SatelliteSystem::Galileo>::getType(UINT32 word)
{
    first_word.word = word;

    if (first_word.page_type == 1) return WordType::ALERT; // Alert page
    
    if (first_word.word_type == 1) return WordType::EPHEMERIS_1;
    else if (first_word.word_type == 2) return WordType::EPHEMERIS_2;
    else if (first_word.word_type == 3) return WordType::EPHEMERIS_3;
    else if (first_word.word_type == 4) return WordType::EPHEMERIS_4__CLOCK_CORRECTION;
    else if (first_word.word_type == 5) return WordType::IONOSPHERIC_CORRECTION__BGD__SIG_HEALTH__DVS__GST;
    else if (first_word.word_type == 6) return WordType::GST_UTC_CONVERSION;
    else if (first_word.word_type == 7) return WordType::ALMANAC_1;
    else if (first_word.word_type == 8) return WordType::ALMANAC_2;
    else if (first_word.word_type == 9) return WordType::ALMANAC_3;
    else if (first_word.word_type == 10) return WordType::ALMANAC_4;
}


void NavEng<SatelliteSystem::Galileo>::setFlags(WordType type, std::uint8_t svId)
{
    if (type == WordType::EPHEMERIS_1) { nav_msg_flags_map[svId].page1 = 1; checkFull(WordType::EPHEMERIS_1, svId); }
    else if (type == WordType::EPHEMERIS_2) { nav_msg_flags_map[svId].page2 = 1; checkFull(WordType::EPHEMERIS_2, svId); }
    else if (type == WordType::EPHEMERIS_3) { nav_msg_flags_map[svId].page3 = 1; checkFull(WordType::EPHEMERIS_3, svId); }
    else if (type == WordType::EPHEMERIS_4__CLOCK_CORRECTION) { nav_msg_flags_map[svId].page4 = 1; checkFull(WordType::EPHEMERIS_4__CLOCK_CORRECTION, svId); }

    else if (type == WordType::ALMANAC_2) { if (svId != 0) checkFull(WordType::ALMANAC_2, svId); }
    else if (type == WordType::ALMANAC_3) { if (svId != 0) checkFull(WordType::ALMANAC_3, svId); }
    else if (type == WordType::ALMANAC_4) { if (svId != 0) checkFull(WordType::ALMANAC_4, svId); }
}


void NavEng<SatelliteSystem::Galileo>::checkFull(WordType type, std::uint8_t svId)
{
    if (type == WordType::EPHEMERIS_1 || type == WordType::EPHEMERIS_2 ||
        type == WordType::EPHEMERIS_3 || type == WordType::EPHEMERIS_4__CLOCK_CORRECTION)
    {
        if (nav_msg_flags_map[svId].page1 == 1 && nav_msg_flags_map[svId].page2 == 1 &&
            nav_msg_flags_map[svId].page3 == 1 && nav_msg_flags_map[svId].page4 == 1)
        {
            nav_msg_flags_map[svId].ephemeris = 1;
            nav_msg_flags_map[svId].page1 = 0;
            nav_msg_flags_map[svId].page2 = 0;
            nav_msg_flags_map[svId].page3 = 0;
            nav_msg_flags_map[svId].page4 = 0;
        }

        else nav_msg_flags_map[svId].ephemeris = 0;
    }

    else if (type == WordType::ALMANAC_2 || type == WordType::ALMANAC_3 || type == WordType::ALMANAC_4)
    {
        nav_msg_flags_map[svId].almanac = 1;
    }
}