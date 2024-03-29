/**
 * @file    : Ublox-Driver project file
 * @author  : Yusuf TAŞKIRAN
 * @note    : ytaskiran70@gmail.com
 * @date    : 3 / Eylül / 2021
 * @code    : galileo_page.hpp file
 * @details : 
 */

#ifndef GALILEO_PAGE_HPP
#define GALILEO_PAGE_HPP

#include "galileo_nav.hpp"
#include "math_constants.hpp"

enum class UbxSigId : std::uint8_t
{
    GPS_L1CA     = 0,
    GPS_L2CL     = 3,
    GPS_L2CM     = 4,

    SBAS_L1CA    = 0,

    GALILEO_E1C  = 0,
    GALILEO_E1B  = 1,
    GALILEO_E5bl = 5,
    GALILEO_E5bQ = 6,

    BeiDou_B1D1  = 0,
    BeiDou_B1D2  = 1,
    BeiDou_B2D1  = 2,
    BeiDou_B2D2  = 3,

    QZSS_L1CA    = 0,
    QZSS_L1S     = 1,
    QZSS_L2CM    = 4,
    QZSS_L2CL    = 5,

    GLONASS_L1OF = 0,
    GLONASS_L2OF = 2,

    None         = 63
};


enum class WordType 
{
    SPARE,
    EPHEMERIS_1,
    EPHEMERIS_2,
    EPHEMERIS_3,
    EPHEMERIS_4_CLOCK_CORRECTION,
    IONOSPHERIC_CORRECTION_BGD_SIG_HEALTH_DVS_GST,
    GST_UTC_CONVERSION,
    ALMANAC_1,
    ALMANAC_2,
    ALMANAC_3,
    ALMANAC_4,
    REDUCED_CED = 16,
    FEC2,
    DUMMY = 63,
    ALERT,
    None
};

typedef union {
    struct {
        UINT32 spare          : 24 ;
        UINT32 word_type      : 6  ;
        UINT32 page_type      : 1  ;
        UINT32 even_odd       : 1  ;
    };
    UINT32 word;
} GalileoWord1;

inline WordType as_wordtype (UINT32 type)
{
    switch (type)
    {
        case 0:  return WordType::SPARE;
        case 1:  return WordType::EPHEMERIS_1;
        case 2:  return WordType::EPHEMERIS_2;
        case 3:  return WordType::EPHEMERIS_3;
        case 4:  return WordType::EPHEMERIS_4_CLOCK_CORRECTION;
        case 5:  return WordType::IONOSPHERIC_CORRECTION_BGD_SIG_HEALTH_DVS_GST;
        case 6:  return WordType::GST_UTC_CONVERSION;
        case 7:  return WordType::ALMANAC_1;
        case 8:  return WordType::ALMANAC_2;
        case 9:  return WordType::ALMANAC_3;
        case 10: return WordType::ALMANAC_4;
        case 16: return WordType::REDUCED_CED;
        case 17: return WordType::FEC2;
        case 63: return WordType::DUMMY;
        case 64: return WordType::ALERT;
        default: return WordType::None;
    }
}


template <typename T>
T concatenateBits(T data1, T data2, int size1, int size2) 
{
  T data = (data1 << size2) | (data2);
  return data;
}


template<WordType word_type, UbxSigId sigId>
struct GalileoPage {};


template<>
struct GalileoPage<WordType::EPHEMERIS_1, UbxSigId::None> {

    union {
        struct {
            UINT32 toe            : 14 ;
            UINT32 issue_of_data  : 10 ;
            UINT32 word_type      : 6  ;
            UINT32 page_type      : 1  ;
            UINT32 even_odd       : 1  ; 
        };
        UINT32 word;
    }Word1{};


    union {
        struct {
            INT32 mean_anomaly : 32 ;
        };
        UINT32 word;
    }Word2{};


    union {
        struct {
            UINT32 eccentricity : 32 ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad                    : 8  ;
            UINT32 tail                   : 6  ;
            UINT32 root_semi_major_axis_1 : 18 ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved               : 16 ;
            UINT32 root_semi_major_axis_2 : 14 ;
            UINT32 page_type              : 1  ;
            UINT32 even_odd               : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    void interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
};


template<>
struct GalileoPage<WordType::EPHEMERIS_2, UbxSigId::None> {

    union {
        struct {
            INT32  longitude_1    : 14 ;
            UINT32 issue_of_data  : 10 ;
            UINT32 word_type      : 6  ;
            UINT32 page_type      : 1  ;
            UINT32 even_odd       : 1  ; 
        };
        UINT32 word;
    }Word1{};


    union {
        struct {
            INT32 inclination_angle_1 : 14 ;
            INT32 longitude_2         : 18 ;
        };
        UINT32 word;
    }Word2{};


    union {
        struct {
            INT32 perigee_1           : 14 ;
            INT32 inclination_angle_2 : 18 ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad       : 8  ;
            UINT32 tail      : 6  ;
            INT32  perigee_2 : 18 ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved          : 16 ;
            INT32  ia_rate_of_change : 14 ;
            UINT32 page_type         : 1  ;
            UINT32 even_odd          : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    void interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::EPHEMERIS_3, UbxSigId::None> {

    union {
        struct {
            INT32  ra_rate_of_change_1 : 14 ;
            UINT32 issue_of_data       : 10 ;
            UINT32 word_type           : 6  ;
            UINT32 page_type           : 1  ;
            UINT32 even_odd            : 1  ; 
        };
        UINT32 word;
    }Word1{};


    union {
        struct {
            INT32 C_uc_1                 : 6  ;
            INT32 mean_motion_difference : 16 ;
            INT32 ra_rate_of_change_2    : 10 ;
        };
        UINT32 word;
    }Word2{};


    union {
        struct {
            INT32 C_rc_1 : 6 ;
            INT32 C_us   : 16 ;
            INT32 C_uc_2 : 10 ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad    : 8  ;
            UINT32 tail   : 6  ;
            INT32  C_rs_1 : 8  ;
            INT32  C_rc_2 : 10 ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved  : 14 ;
            UINT32 sisa      : 8  ;
            INT32  C_rs_2    : 8  ;
            UINT32 page_type : 1  ;
            UINT32 even_odd  : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    void interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::EPHEMERIS_4_CLOCK_CORRECTION, UbxSigId::None> {

    union {
        struct {
            INT32  C_ic_1         : 8 ;
            UINT32 svid           : 6 ;
            UINT32 issue_of_data  : 10 ;
            UINT32 word_type      : 6  ;
            UINT32 page_type      : 1  ;
            UINT32 even_odd       : 1  ; 
        };
        UINT32 word;
    }Word1{};


    union {
        struct {
            UINT32 toc_1  : 8 ;
            INT32  C_is   : 16 ;
            INT32  C_ic_2 : 8  ;
        };
        UINT32 word;
    }Word2{};


    union {
        struct {
            INT32  clock_bias_corr_1 : 26 ;
            UINT32 toc_2             : 6  ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad                : 8  ;
            UINT32 tail               : 6  ;
            INT32  clock_drift_corr_1 : 13 ;
            INT32  clock_bias_corr_2  : 5  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved               : 16 ;
            INT32  clock_drift_rate_corr  : 6 ;
            INT32  clock_drift_corr_2     : 8 ;
            UINT32 page_type              : 1  ;
            UINT32 even_odd               : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    void interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::IONOSPHERIC_CORRECTION_BGD_SIG_HEALTH_DVS_GST, UbxSigId::None> {

    union {
        struct {
            INT32  ai2_1     : 2 ;
            INT32  ai1       : 11 ;
            UINT32 ai0       : 11 ;
            UINT32 word_type : 6  ;
            UINT32 page_type : 1  ;
            UINT32 even_odd  : 1  ; 
        };
        UINT32 word;
    }Word1{};


    union {
        struct {
            INT32  bgd_2_1   : 5  ;
            INT32  bgd_1     : 10 ;
            UINT32 region5   : 1  ;
            UINT32 region4   : 1  ;
            UINT32 region3   : 1  ;
            UINT32 region2   : 1  ;
            UINT32 region1   : 1  ;
            INT32  ai2_2     : 12 ;
        };
        UINT32 word;
    }Word2{};


    union {
        struct {
            UINT32 time_of_week_1    : 9  ;
            UINT32 week_num          : 12 ;
            UINT32 data_validity_e1  : 1  ;
            UINT32 data_validity_e5b : 1  ;
            UINT32 sig_health_e1     : 2  ;
            UINT32 sig_health_e5b    : 2  ;
            INT32  bgd_2_2           : 5  ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad            : 8  ;
            UINT32 tail           : 6  ;
            UINT32 spare          : 7  ;
            UINT32 time_of_week_2 : 11 ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved  : 14 ;
            UINT32 spare     : 16 ;
            UINT32 page_type : 1  ;
            UINT32 even_odd  : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    void interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::GST_UTC_CONVERSION, UbxSigId::None> {

    union {
        struct {
            INT32  A0_1      : 24 ;
            UINT32 word_type : 6  ;
            UINT32 page_type : 1  ;
            UINT32 even_odd  : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32 A1   : 24 ;
            INT32 A0_2 : 8  ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            UINT32 WN_lsf             : 8 ;
            UINT32 utc_reference_week : 8 ;
            UINT32 utc_reference_tow  : 8 ;
            INT32  ls_count_before    : 8 ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad            : 8 ;
            UINT32 tail           : 6 ;
            UINT32 time_of_week_1 : 7 ;
            INT32  ls_count_after : 8 ;
            UINT32 day_num        : 3 ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved       : 14 ;
            UINT32 spare          : 3  ;
            UINT32 time_of_week_2 : 13 ;
            UINT32 page_type      : 1  ;
            UINT32 even_odd       : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    void interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::ALMANAC_1, UbxSigId::GALILEO_E5bl> {

    union {
        struct {
            INT32 delta_root_a_1 : 2  ;
            UINT32 svid_1        : 6  ;
            UINT32 t0a           : 10 ;
            UINT32 week_num      : 2  ;
            UINT32 issue_of_data : 4  ;
            UINT32 word_type     : 6  ;
            UINT32 page_type     : 1  ;
            UINT32 even_odd      : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32  perigee_1      : 10 ;
            UINT32 eccentricity   : 11 ;
            INT32  delta_root_a_2 : 11 ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            INT32  longitude_1 : 15 ;
            INT32  diff_ia_na  : 11 ;
            INT32  perigee_2   : 6 ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad            : 8  ;
            UINT32 tail           : 6  ;
            INT32  mean_anomaly_1 : 6  ;
            INT32  roc_ra         : 11 ;
            INT32  longitude_2    : 1  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved       : 20 ;
            INT32  mean_anomaly_2 : 10  ;
            UINT32 page_type      : 1  ;
            UINT32 even_odd       : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    UINT8 interpret (GalileoNavData& gal_nav_data) const;
}; 

template<>
struct GalileoPage<WordType::ALMANAC_1, UbxSigId::GALILEO_E1B> {

    union {
        struct {
            INT32 delta_root_a_1 : 2  ;
            UINT32 svid_1        : 6  ;
            UINT32 t0a           : 10 ;
            UINT32 week_num      : 2  ;
            UINT32 issue_of_data : 4  ;
            UINT32 word_type     : 6  ;
            UINT32 page_type     : 1  ;
            UINT32 even_odd      : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32  perigee_1      : 10 ;
            UINT32 eccentricity   : 11 ;
            INT32  delta_root_a_2 : 11 ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            INT32  longitude_1 : 15 ;
            INT32  diff_ia_na  : 11 ;
            INT32  perigee_2   : 6 ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad            : 8  ;
            UINT32 tail           : 6  ;
            INT32  mean_anomaly_1 : 6  ;
            INT32  roc_ra         : 11 ;
            INT32  longitude_2    : 1  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved       : 20 ;
            INT32  mean_anomaly_2 : 10  ;
            UINT32 page_type      : 1  ;
            UINT32 even_odd       : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    UINT8 interpret (GalileoNavData& gal_nav_data) const;
}; 

template<>
struct GalileoPage<WordType::ALMANAC_2, UbxSigId::GALILEO_E5bl> {

    union {
        struct {
            INT32 clock_corr_linear_1 : 4  ;
            INT32 clock_corr_bias     : 16 ;
            UINT32 issue_of_data      : 4  ;
            UINT32 word_type          : 6  ;
            UINT32 page_type          : 1  ;
            UINT32 even_odd           : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32  delta_root_a        : 13 ;
            UINT32 svid_2              : 6  ;
            UINT32 sig_health_e1       : 2  ;
            UINT32 sig_health_e5b      : 2  ;
            INT32  clock_corr_linear_2 : 9  ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            INT32  diff_ia_na_1  : 5  ;
            INT32  perigee       : 16 ;
            UINT32 eccentricity  : 11 ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad          : 8  ;
            UINT32 tail         : 6  ;
            INT32  longitude_1  : 12 ;
            INT32  diff_ia_na_2 : 6  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved    : 15 ;
            INT32  roc_ra      : 11 ;
            INT32  longitude_2 : 4  ;
            UINT32 page_type   : 1  ;
            UINT32 even_odd    : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    UINT8 interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::ALMANAC_2, UbxSigId::GALILEO_E1B> {

    union {
        struct {
            INT32 clock_corr_linear_1 : 4  ;
            INT32 clock_corr_bias     : 16 ;
            UINT32 issue_of_data      : 4  ;
            UINT32 word_type          : 6  ;
            UINT32 page_type          : 1  ;
            UINT32 even_odd           : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32  delta_root_a        : 13 ;
            UINT32 svid_2              : 6  ;
            UINT32 sig_health_e1       : 2  ;
            UINT32 sig_health_e5b      : 2  ;
            INT32  clock_corr_linear_2 : 9  ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            INT32  diff_ia_na_1  : 5  ;
            INT32  perigee       : 16 ;
            UINT32 eccentricity  : 11 ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad          : 8  ;
            UINT32 tail         : 6  ;
            INT32  longitude_1  : 12 ;
            INT32  diff_ia_na_2 : 6  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved    : 15 ;
            INT32  roc_ra      : 11 ;
            INT32  longitude_2 : 4  ;
            UINT32 page_type   : 1  ;
            UINT32 even_odd    : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    UINT8 interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::ALMANAC_3, UbxSigId::GALILEO_E5bl> {

    union {
        struct {
            INT32 mean_anomaly_1 : 8  ;
            UINT32 t0a           : 10 ;
            UINT32 week_num      : 2  ;
            UINT32 issue_of_data : 4  ;
            UINT32 word_type     : 6  ;
            UINT32 page_type     : 1  ;
            UINT32 even_odd      : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32  clock_corr_linear_1 : 8  ;
            INT32  clock_corr_bias     : 16 ;
            INT32  mean_anomaly_2      : 8  ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            UINT32  eccentricity_1     : 4  ;
            INT32  delta_root_a        : 13 ;
            UINT32 svid_3              : 6  ;
            UINT32 sig_health_e1       : 2  ;
            UINT32 sig_health_e5b      : 2  ;
            INT32  clock_corr_linear_2 : 5  ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad            : 8  ;
            UINT32 tail           : 6  ;
            INT32  perigee_1      : 11 ;
            UINT32 eccentricity_2 : 7  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved   : 14 ;
            INT32  diff_ia_na : 11 ;
            INT32  perigee_2  : 5  ;
            UINT32 page_type  : 1  ;
            UINT32 even_odd   : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    UINT8 interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::ALMANAC_3, UbxSigId::GALILEO_E1B> {

    union {
        struct {
            INT32 mean_anomaly_1 : 8  ;
            UINT32 t0a           : 10 ;
            UINT32 week_num      : 2  ;
            UINT32 issue_of_data : 4  ;
            UINT32 word_type     : 6  ;
            UINT32 page_type     : 1  ;
            UINT32 even_odd      : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32  clock_corr_linear_1 : 8  ;
            INT32  clock_corr_bias     : 16 ;
            INT32  mean_anomaly_2      : 8  ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            UINT32  eccentricity_1     : 4  ;
            INT32  delta_root_a        : 13 ;
            UINT32 svid_3              : 6  ;
            UINT32 sig_health_e1       : 2  ;
            UINT32 sig_health_e5b      : 2  ;
            INT32  clock_corr_linear_2 : 5  ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad            : 8  ;
            UINT32 tail           : 6  ;
            INT32  perigee_1      : 11 ;
            UINT32 eccentricity_2 : 7  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved   : 14 ;
            INT32  diff_ia_na : 11 ;
            INT32  perigee_2  : 5  ;
            UINT32 page_type  : 1  ;
            UINT32 even_odd   : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    UINT8 interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::ALMANAC_4, UbxSigId::GALILEO_E5bl> {

    union {
        struct {
            INT32 roc_ra_1       : 4  ;
            INT32 longitude      : 16 ;
            UINT32 issue_of_data : 4  ;
            UINT32 word_type     : 6  ;
            UINT32 page_type     : 1  ;
            UINT32 even_odd      : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32  clock_corr_bias_1 : 9  ;
            INT32  mean_anomaly      : 16 ;
            INT32  roc_ra_2          : 7  ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            INT32  const_term_offset_1 : 8  ;
            UINT32 sig_health_e1       : 2  ;
            UINT32 sig_health_e5b      : 2  ;
            INT32  clock_corr_linear   : 13 ;
            INT32  clock_corr_bias_2   : 7  ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad                 : 8  ;
            UINT32 tail                : 6  ;
            INT32  roc_offset_1        : 10 ;
            INT32  const_term_offset_2 : 8  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved     : 14 ;
            UINT32 wn0G         : 6  ;
            UINT32 t0G          : 8  ;
            INT32  roc_offset_2 : 2  ;
            UINT32 page_type    : 1  ;
            UINT32 even_odd     : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    void interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::ALMANAC_4, UbxSigId::GALILEO_E1B> {

    union {
        struct {
            INT32 roc_ra_1       : 4  ;
            INT32 longitude      : 16 ;
            UINT32 issue_of_data : 4  ;
            UINT32 word_type     : 6  ;
            UINT32 page_type     : 1  ;
            UINT32 even_odd      : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32  clock_corr_bias_1 : 9  ;
            INT32  mean_anomaly      : 16 ;
            INT32  roc_ra_2          : 7  ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            INT32  const_term_offset_1 : 8  ;
            UINT32 sig_health_e1       : 2  ;
            UINT32 sig_health_e5b      : 2  ;
            INT32  clock_corr_linear   : 13 ;
            INT32  clock_corr_bias_2   : 7  ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad                 : 8  ;
            UINT32 tail                : 6  ;
            INT32  roc_offset_1        : 10 ;
            INT32  const_term_offset_2 : 8  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved     : 14 ;
            UINT32 wn0G         : 6  ;
            UINT32 t0G          : 8  ;
            INT32  roc_offset_2 : 2  ;
            UINT32 page_type    : 1  ;
            UINT32 even_odd     : 1  ; 
        };
        UINT32 word;
    }Word5{};

    void decode (const std::uint32_t(&word)[10]);

    void interpret (GalileoNavData& gal_nav_data, std::uint8_t svId) const;
}; 

template<>
struct GalileoPage<WordType::REDUCED_CED, UbxSigId::None> {

    union {
        struct {
            INT32 eccentricity_rced_y_1 : 6  ;
            INT32 eccentricity_rced_x   : 13 ;
            INT32 delta_rced_smajor     : 5  ;
            UINT32 word_type            : 6  ;
            UINT32 page_type            : 1  ;
            UINT32 even_odd             : 1  ; 
        };
        UINT32 word;
    }Word1{};

    union {
        struct {
            INT32  rced_longitude_1       : 8  ;
            INT32  delta_rced_inclination : 17 ;
            INT32  eccentricity_rced_y_2  : 7  ;
        };
        UINT32 word;
    }Word2{};

    union {
        struct {
            INT32  lambda_rced_1    : 17 ;
            INT32  rced_longitude_2 : 15 ;
        };
        UINT32 word;
    }Word3{};

    union {
        struct {
            UINT32 pad                    : 8  ;
            UINT32 tail                   : 6  ;
            INT32  rced_clock_corr_bias_1 : 12 ;
            INT32  lambda_rced_2          : 6  ;
        };
        UINT32 word;
    }Word4{};

    union {
        struct {
            UINT32 reserved               : 14 ;
            INT32  rced_clock_corr_drift  : 6  ;
            INT32  rced_clock_corr_bias_2 : 10  ;
            UINT32 page_type              : 1  ;
            UINT32 even_odd               : 1  ; 
        };
        UINT32 word;
    }Word5{};
}; 

template<>
struct GalileoPage<WordType::FEC2, UbxSigId::None> {};

template<>
struct GalileoPage<WordType::DUMMY, UbxSigId::None> {};

template<>
struct GalileoPage<WordType::SPARE, UbxSigId::None> {};



#endif // GALILEO_PAGE_HPP