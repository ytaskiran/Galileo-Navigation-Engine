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
//#include "gps_nav.hpp"
//#include "gps_subframe.hpp"
#include "galileo_page.hpp"

template<::SatelliteSystem s>
class NavEng {};

/*template <>
class NavEng<::SatelliteSystem::GPS> {
public:

    void yakala_subframe_data_LNAV (const std::uint32_t(&word)[10], std::uint8_t svId);

    typedef union {
        struct {
            UINT8 sbfrm1    : 1;
            UINT8 sbfrm2    : 1;
            UINT8 sbfrm3    : 1;
            UINT8 sbfrm4    : 1;
            UINT8 sbfrm5    : 1;
            UINT8 almanac   : 1;
            UINT8 ephemeris : 1;
            UINT8 reserved  : 1;
        } new_nav_msg;
        UINT8 u8;
    }yeni_subframe;

    using satId = int;
    std::map<satId, yeni_subframe> yeni_subframe_map{};
    bool       week_geldi {false};

    GpsNavData data{}; ///!< GPS Navigation Message Data

private:

    using GpsL1Subframe1 = GpsSubframe<TrackingCode::L1CA, Subframe::one>;
    using GpsL1Subframe2 = GpsSubframe<TrackingCode::L1CA, Subframe::two>;
    using GpsL1Subframe3 = GpsSubframe<TrackingCode::L1CA, Subframe::three>;
    using GpsL1Subframe4 = GpsSubframe<TrackingCode::L1CA, Subframe::four>;
    using GpsL1Subframe5 = GpsSubframe<TrackingCode::L1CA, Subframe::five>;

    GpsL1Subframe1 L1_Subframe1{};  ///!< L1 Subframe 1
    GpsL1Subframe2 L1_Subframe2{};  ///!< L1 Subframe 2
    GpsL1Subframe3 L1_Subframe3{};  ///!< L1 Subframe 3
    GpsL1Subframe4 L1_Subframe4{};  ///!< L1 Subframe 4
    GpsL1Subframe5 L1_Subframe5{};  ///!< L1 Subframe 5
};
*/

template<>
class NavEng<SatelliteSystem::Galileo> {
public:
    void processPage(const std::uint32_t(&word)[10], std::uint8_t svId, std::uint8_t sigId);
    WordType getType(UINT32 word);
    void setFlags(WordType type, std::uint8_t svId);
    void checkFull(WordType type, std::uint8_t svId);

    GalileoNavData gal_nav_data{};

    typedef struct {
        UINT8 page1     : 1;
        UINT8 page2     : 1;
        UINT8 page3     : 1;
        UINT8 page4     : 1;
        UINT8 ephemeris : 1;
        UINT8 almanac   : 1;
        UINT8 reserved  : 2;
    } nav_msg_flags;

    std::map<int, nav_msg_flags> nav_msg_flags_map{};

private:
    union {
        struct {
            UINT32 spare          : 24 ;
            UINT32 word_type      : 6  ;
            UINT32 page_type      : 1  ;
            UINT32 even_odd       : 1  ; 
        };
        UINT32 word;
    }first_word{};

    UINT8 alm_e5b_svid[Num_Galileo_Satellites];
    UINT8 alm_e1_svid[Num_Galileo_Satellites];

    GalileoPage<WordType::EPHEMERIS_1, TrackingCode::None>                                        gal_page_1{};
    GalileoPage<WordType::EPHEMERIS_2, TrackingCode::None>                                        gal_page_2{};
    GalileoPage<WordType::EPHEMERIS_3, TrackingCode::None>                                        gal_page_3{};
    GalileoPage<WordType::EPHEMERIS_4__CLOCK_CORRECTION, TrackingCode::None>                      gal_page_4{};
    GalileoPage<WordType::IONOSPHERIC_CORRECTION__BGD__SIG_HEALTH__DVS__GST, TrackingCode::None>  gal_page_5{};
    GalileoPage<WordType::GST_UTC_CONVERSION, TrackingCode::None>                                 gal_page_6{};
    GalileoPage<WordType::ALMANAC_1, TrackingCode::E5b>                                           gal_page_7_e5b{};
    GalileoPage<WordType::ALMANAC_1, TrackingCode::E1>                                            gal_page_7_e1{};
    GalileoPage<WordType::ALMANAC_2, TrackingCode::E5b>                                           gal_page_8_e5b{};
    GalileoPage<WordType::ALMANAC_2, TrackingCode::E1>                                            gal_page_8_e1{};
    GalileoPage<WordType::ALMANAC_3, TrackingCode::E5b>                                           gal_page_9_e5b{};
    GalileoPage<WordType::ALMANAC_3, TrackingCode::E1>                                            gal_page_9_e1{};
    GalileoPage<WordType::ALMANAC_4, TrackingCode::E5b>                                           gal_page_10_e5b{};
    GalileoPage<WordType::ALMANAC_4, TrackingCode::E1>                                            gal_page_10_e1{};
};


#endif /* NAVIGATION_ENGINE_HPP */