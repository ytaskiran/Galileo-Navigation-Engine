/**
 * @file    : Ublox-Driver 
 * @authors : Mehmet Aksu, Uzay Uysal, Furkan Pala
 * @note    : mehmet.aksu@cezeri.com
 * @date    : 12 / Nisan / 2021
 * @code    : navigation_engine.hpp file
 * @details : 
 */

#ifndef NAVIGATION_ENGINE_HPP
#define NAVIGATION_ENGINE_HPP

#include <map>
#include <string>
#include "galileo_page.hpp"

template<::SatelliteSystem s>
class NavEng {};

template<>
class NavEng<SatelliteSystem::Galileo> {
public:
    void yakala_page (const std::uint32_t(&word)[10], std::uint8_t svId, UbxSigId sigId);
    GalileoNavData data{};
private:
    typedef union
    {
        struct
        {
            UINT8 eph1      : 1;
            UINT8 eph2      : 1;
            UINT8 eph3      : 1;
            UINT8 eph4      : 1;
            UINT8 iono      : 1;
            UINT8 utc       : 1;
            UINT8 ephemeris : 1;
            UINT8 almanac   : 1;
        } new_nav_msg;
        UINT8 u8;
    } nav_msg_flags;

public:
    std::map<int, nav_msg_flags> nav_msg_flags_map{};

private:
    UINT8 alm_e5b_svid[Num_Galileo_Satellites]{};
    UINT8 alm_e1_svid[Num_Galileo_Satellites] {};

    GalileoPage<WordType::EPHEMERIS_1, UbxSigId::None>                                        gal_page_1{};
    GalileoPage<WordType::EPHEMERIS_2, UbxSigId::None>                                        gal_page_2{};
    GalileoPage<WordType::EPHEMERIS_3, UbxSigId::None>                                        gal_page_3{};
    GalileoPage<WordType::EPHEMERIS_4_CLOCK_CORRECTION, UbxSigId::None>                       gal_page_4{};
    GalileoPage<WordType::IONOSPHERIC_CORRECTION_BGD_SIG_HEALTH_DVS_GST, UbxSigId::None>      gal_page_5{};
    GalileoPage<WordType::GST_UTC_CONVERSION, UbxSigId::None>                                 gal_page_6{};
    GalileoPage<WordType::ALMANAC_1, UbxSigId::GALILEO_E5bl>                                  gal_page_7_e5b{};
    GalileoPage<WordType::ALMANAC_1, UbxSigId::GALILEO_E1B>                                   gal_page_7_e1{};
    GalileoPage<WordType::ALMANAC_2, UbxSigId::GALILEO_E5bl>                                  gal_page_8_e5b{};
    GalileoPage<WordType::ALMANAC_2, UbxSigId::GALILEO_E1B>                                   gal_page_8_e1{};
    GalileoPage<WordType::ALMANAC_3, UbxSigId::GALILEO_E5bl>                                  gal_page_9_e5b{};
    GalileoPage<WordType::ALMANAC_3, UbxSigId::GALILEO_E1B>                                   gal_page_9_e1{};
    GalileoPage<WordType::ALMANAC_4, UbxSigId::GALILEO_E5bl>                                  gal_page_10_e5b{};
    GalileoPage<WordType::ALMANAC_4, UbxSigId::GALILEO_E1B>                                   gal_page_10_e1{};
};


#endif /* NAVIGATION_ENGINE_HPP */