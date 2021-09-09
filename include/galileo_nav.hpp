/**
 * @file    : Ublox-Driver 
 * @authors : MEHMET AKSU
 * @note    : mehmet.aksu@cezeri.com
 * @date    : 07 / Mayıs / 2021
 * @code    : galileo_nav.hpp file
 * @details : 
 */



#ifndef GALILEO_NAV_HPP
#define GALILEO_NAV_HPP

#include "navigation_base.hpp"


template<>
struct Almanac<SatelliteSystem::Galileo> {
    UINT8    svId_u8{};             ///!< Satellite identifier PRN
    UINT8    ioda;                  ///!< Almanac Issue of Data
    UINT8    almWNa;                ///!< Almanac reference week number
    FLOAT64  toa_sn_f64{};          ///!< Toa (s) in week
    FLOAT64  deltaSqrtA_m_f64{};    ///!< SQRT(A)  (m 1/2), Square root of semi-major axis, semiperiod
    FLOAT64  ecc_f64{};             ///!< Eccentricity
    FLOAT64  deltai_rad_f64{};      ///!< Delta orbital Inclination(rad)
    FLOAT64  omg0_rad_f64{};        ///!< Longitude of ascending node of orbital plane (rad)
    FLOAT64  OMGdot_rad_f64{};      ///!< Rate of Right Ascension(rad/s) omegaDot
    FLOAT64  omg_rad_f64{};         ///!< Argument of Perigee(rad)
    FLOAT64  m0_rad_f64{};          ///!< Mean Anomaly at reference time(rad)
    FLOAT64  af0_s_f64{};           ///!< Af0(s): Satellite clock bias
    FLOAT64  af1_ss_f64{};          ///!< Af1(s/s): Satellite clock rate
    UINT8    healthE1{};           ///!< Satellite health information
    UINT8    healthE5b{};           ///!< Satellite health information
};


template<>
struct Ephemeris<SatelliteSystem::Galileo> {
    UINT8   svId_u8{};              ///!< Satellite identifier PRN
    UINT16  iodNav{};               ///!< Ephemeris and clock correction Issue of Data
    UINT8   sisaIndexE1E5b{};       ///!< Signal-In-Space Accuracy index for dual frequency E1-E5b
    UINT32  t0c{};                  ///!< Clock correction data reference Time of Week
    INT16   bgdE1E5b{};             ///!< E1-E5b Broadcast Group Delay
    UINT8   healthE1{};            ///!< E1-B Signal Health Status
    UINT8   dataValidityE1{};      ///!< E1-B Data Validity Status
    UINT8   healthE5b{};            ///!< E5b Signal Health Status
    UINT8   dataValidityE5b{};      ///!< E5b Data Validity Status

    /// Clock informations
    FLOAT64 af0_s_f64{};            ///!< Time polynomial coefficient 0 - SV clock bias (sec)
    FLOAT64 af1_ss_f64{};           ///!< Time polynomial coefficient 1 - SV clock drift (sec/sec)
    FLOAT64 af2_ss2_f64{};          ///!< Time polynomial coefficient 2 - SV clock drift rate (sec/sec**2)

    /// Major orbit parameters
    FLOAT64 sqrtA_f64{};            ///!< Square root of semi-major axis (m 1/2)
    FLOAT64 OMGdot_rad_f64{};       ///!< Rate of Right Ascension(rad/s)
    FLOAT64 deltaN_rad_sec_f64{};   ///!< Mean motion difference from computed value at reference time
    FLOAT64 m0_rad_f64{};           ///!< Mean anomaly at reference time
    FLOAT64 ecc_f64{};              ///!< Eccentricity
    FLOAT64 omega_rad_f64{};        ///!< Argument of Perigee(rad)
    FLOAT64 omega0_rad_f64{};       ///!< Longitude of ascending node of orbital plane at weekly epoch
    FLOAT64 i0_rad_f64{};           ///!< Inclination angle(rad), at reference time
    FLOAT64 IDOT_rad_sec_f64{};     ///!< Inclination Rate rad/sec
    FLOAT64 toe_sn_f64{};           ///!< Toe (s) in week

    /// Harmonic perturbations
    FLOAT64 Cis_rad_f64{};          ///!< Amplitude of sine harmonic correction term to angle of inclination
    FLOAT64 Cic_rad_f64{};          ///!< Amplitude of cosine harmonic correction term to angle of inclination
    FLOAT64 Crs_m_f64{};            ///!< Amplitude of sine correction term to orbit radius
    FLOAT64 Crc_m_f64{};            ///!< Amplitude of cosine correction term to orbit radius
    FLOAT64 Cus_rad_f64{};          ///!< Amplitude of sine harmonic correction term to argument of latitude
    FLOAT64 Cuc_rad_f64{};          ///!< Amplitude of cosine harmonic correction term to argument of latitude
};


template<>
struct TimeOffset<SatelliteSystem::Galileo> {
    FLOAT64 a0G_s{};
    FLOAT64 a1G_ss{};
    FLOAT64 t0G_s{};
    FLOAT64 wn0G_week{};
};

template<>
struct UTC<SatelliteSystem::Galileo> {
    FLOAT64 A0_s_f64{};             ///!< First parameter of UTC polynomial
    FLOAT64 A1_ss_f64{};            ///!< Second parameter of UTC polynomial
    FLOAT64 DtLS_s_f64{};           ///!< Delta time due to current leap seconds
    FLOAT64 Tot_s_f64{};            ///!< UTC parameters reference time of week (GPS time)
    FLOAT64 WNt_f64{};              ///!< UTC parameters reference week number
    FLOAT64 WNlsf_f64{};            ///!< Week number at the end of which the future leap second becomes effective
    FLOAT64 Dn_f64{};               ///!< Day number at the end of which the future leap second becomes effective
    FLOAT64 DtLSF_s_f64{};          ///!< Delta time due to future leap seconds
};

template<>
struct Ionosphere<SatelliteSystem::Galileo> {
    FLOAT64 ai0_f64;                ///!<  Effective ionisation level 1st order parameter
    FLOAT64 ai1_f64;                ///!<  Effective ionisation level 2nd order parameter
    FLOAT64 ai2_f64;                ///!<  Effective ionisation level 3rd order parameter
};

using GalileoEphemeris  = Ephemeris<SatelliteSystem::Galileo>;
using GalileoAlmanac    = Almanac<SatelliteSystem::Galileo>;
using GalileoUTC        = UTC<SatelliteSystem::Galileo>;
using GalileoTimeOffset = TimeOffset<SatelliteSystem::Galileo>;
using GalileoIonosphere = Ionosphere<SatelliteSystem::Galileo>;

struct GalileoNavData {

    GalileoAlmanac    alm[Num_Galileo_Satellites]{};
    GalileoEphemeris  eph[Num_Galileo_Satellites]{};
    GalileoUTC        utc{};
    GalileoTimeOffset time_offset{};
    GalileoIonosphere iono{};
};


#endif /* GALILEO_NAV_HPP */