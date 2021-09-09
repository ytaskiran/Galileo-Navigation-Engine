/**
 * @file    : Ublox-Driver project file
 * @author  : Yusuf TAŞKIRAN
 * @note    : ytaskiran70@gmail.com
 * @date    : 6 / Eylül / 2021
 * @code    : galileo_page.cpp file
 * @details : 
 */

#include <cmath>
#include <iostream>
#include "galileo_page.hpp"
#include <bitset>



void GalileoPage<WordType::EPHEMERIS_1, TrackingCode::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::EPHEMERIS_1, TrackingCode::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT16  iodNav      =  Word1.issue_of_data;
    FLOAT64 toe_sn_f64  =  Word1.t0e * 60;
    FLOAT64 m0_rad_f64  =  Word2.mean_anomaly * pow(2, -31) * M_PI;
    FLOAT64 ecc_f64     =  Word3.eccentricity * pow(2, -33);
    FLOAT64 sqrtA_f64   =  concatenateBits(Word4.root_semi_major_axis_1, Word5.root_semi_major_axis_2, 18, 14) * pow(2, -19);

    gal_nav_data.eph[svId-1].iodNav      =  iodNav;
    gal_nav_data.eph[svId-1].toe_sn_f64  =  toe_sn_f64;
    gal_nav_data.eph[svId-1].m0_rad_f64  =  m0_rad_f64;
    gal_nav_data.eph[svId-1].ecc_f64     =  ecc_f64;
    gal_nav_data.eph[svId-1].sqrtA_f64   =  sqrtA_f64;
}   


void GalileoPage<WordType::EPHEMERIS_2, TrackingCode::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::EPHEMERIS_2, TrackingCode::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    FLOAT64 omega0_rad_f64    =  concatenateBits(Word1.longitude_1, Word2.longitude_2, 14, 18) * pow(2, -31) * M_PI;
    FLOAT64 i0_rad_f64        =  concatenateBits(Word2.inclination_angle_1, Word3.inclination_angle_2, 14, 18) * pow(2, -31) * M_PI;
    FLOAT64 omega_rad_f64     =  concatenateBits(Word3.perigee_1, Word4.perigee_2, 14, 18) * pow(2, -31) * M_PI;
    FLOAT64 IDOT_rad_sec_f64  =  Word5.ia_rate_of_change * pow(2, -43) * M_PI;

    gal_nav_data.eph[svId-1].omega0_rad_f64    =  omega0_rad_f64;
    gal_nav_data.eph[svId-1].i0_rad_f64        =  i0_rad_f64;
    gal_nav_data.eph[svId-1].omega_rad_f64     =  omega_rad_f64;
    gal_nav_data.eph[svId-1].IDOT_rad_sec_f64  =  IDOT_rad_sec_f64;
}   


void GalileoPage<WordType::EPHEMERIS_3, TrackingCode::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::EPHEMERIS_3, TrackingCode::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    FLOAT64 OMGdot_rad_f64      =  concatenateBits(Word1.ra_rate_of_change_1, Word2.ra_rate_of_change_2, 14, 10) * pow(2, -43) * M_PI;
    FLOAT64 deltaN_rad_sec_f64  =  Word2.mean_motion_difference * pow(2, -43) * M_PI;
    FLOAT64 Cuc_rad_f64         =  concatenateBits(Word2.C_uc_1, Word3.C_uc_2, 6, 10) * pow(2, -29);
    FLOAT64 Cus_rad_f64         =  Word3.C_us * pow(2, -29);
    FLOAT64 Crc_m_f64           =  concatenateBits(Word3.C_rc_1, Word4.C_rc_2, 6, 10) * pow(2, -5);
    FLOAT64 Crs_m_f64           =  concatenateBits(Word4.C_rs_1, Word5.C_rs_2, 8, 8) * pow(2, -5);
    UINT8 sisaIndexE1E5b;

    if (Word5.sisa > 0 && Word5.sisa <= 50) sisaIndexE1E5b = Word5.sisa * 0.01;
    else if (Word5.sisa > 50 && Word5.sisa <= 75) sisaIndexE1E5b = 0.5 + ((Word5.sisa - 50) * 0.02);
    else if (Word5.sisa > 75 && Word5.sisa <= 100) sisaIndexE1E5b = 1 + ((Word5.sisa - 75) * 0.04);
    else if (Word5.sisa > 100 && Word5.sisa <= 125) sisaIndexE1E5b = 2 + ((Word5.sisa - 100) * 0.16);

    gal_nav_data.eph[svId-1].OMGdot_rad_f64      =  OMGdot_rad_f64;
    gal_nav_data.eph[svId-1].deltaN_rad_sec_f64  =  deltaN_rad_sec_f64;
    gal_nav_data.eph[svId-1].Cuc_rad_f64         =  Cuc_rad_f64;
    gal_nav_data.eph[svId-1].Cus_rad_f64         =  Cus_rad_f64;
    gal_nav_data.eph[svId-1].Crc_m_f64           =  Crc_m_f64;
    gal_nav_data.eph[svId-1].Crs_m_f64           =  Crs_m_f64;
    gal_nav_data.eph[svId-1].sisaIndexE1E5b      =  sisaIndexE1E5b;
}   


void GalileoPage<WordType::EPHEMERIS_4__CLOCK_CORRECTION, TrackingCode::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::EPHEMERIS_4__CLOCK_CORRECTION, TrackingCode::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8   svId_u8      =  Word1.svid;
    FLOAT64 Cic_rad_f64  =  concatenateBits(Word1.C_ic_1, Word2.C_ic_2, 8, 8) * pow(2, -29);
    FLOAT64 Cis_rad_f64  =  Word2.C_is * pow(2, -29);
    UINT32  t0c          =  concatenateBits(Word2.t0c_1, Word3.t0c_2, 8, 6) * 60;
    FLOAT64 af0_s_f64    =  concatenateBits(Word3.clock_bias_corr_1, Word4.clock_bias_corr_2, 26, 5) * pow(2, -34);
    FLOAT64 af1_ss_f64   =  concatenateBits(Word4.clock_drift_corr_1, Word5.clock_drift_corr_2, 13, 8) * pow(2, -46);
    FLOAT64 af2_ss2_f64  =  Word5.clock_drift_rate_corr * pow(2, -59);

    gal_nav_data.eph[svId-1].svId_u8      =  svId_u8;
    gal_nav_data.eph[svId-1].Cic_rad_f64  =  Cic_rad_f64;
    gal_nav_data.eph[svId-1].Cis_rad_f64  =  Cis_rad_f64;
    gal_nav_data.eph[svId-1].t0c          =  t0c;
    gal_nav_data.eph[svId-1].af0_s_f64    =  af0_s_f64;
    gal_nav_data.eph[svId-1].af1_ss_f64   =  af1_ss_f64;
    gal_nav_data.eph[svId-1].af2_ss2_f64  =  af2_ss2_f64;
}   


void GalileoPage<WordType::IONOSPHERIC_CORRECTION__BGD__SIG_HEALTH__DVS__GST, TrackingCode::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::IONOSPHERIC_CORRECTION__BGD__SIG_HEALTH__DVS__GST, TrackingCode::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    FLOAT64 ai0_f64          =  Word1.ai0 * pow(2, -2);
    FLOAT64 ai1_f64          =  Word1.ai1 * pow(2, -8);
    FLOAT64 ai2_f64          =  concatenateBits(Word1.ai2_1, Word2.ai2_2, 2, 12) * pow(2, -15);
    INT16   bgdE1E5b         =  concatenateBits(Word2.bgd_2_1, Word3.bgd_2_2, 5, 5) * pow(2, -32);
    UINT8   healthE5b        =  Word3.sig_health_e5b;
    UINT8   healthE1         =  Word3.sig_health_e1;
    UINT8   dataValidityE5b  =  Word3.data_validity_e5b;
    UINT8   dataValidityE1   =  Word3.data_validity_e1;

    // week num and tow???

    gal_nav_data.iono.ai0_f64                 =  ai0_f64;
    gal_nav_data.iono.ai1_f64                 =  ai1_f64;
    gal_nav_data.iono.ai2_f64                 =  ai2_f64;
    gal_nav_data.eph[svId-1].bgdE1E5b         =  bgdE1E5b;
    gal_nav_data.eph[svId-1].healthE5b        =  healthE5b;
    gal_nav_data.eph[svId-1].healthE1         =  healthE1;
    gal_nav_data.eph[svId-1].dataValidityE5b  =  dataValidityE5b;
    gal_nav_data.eph[svId-1].dataValidityE1   =  dataValidityE1;
}   


void GalileoPage<WordType::GST_UTC_CONVERSION, TrackingCode::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::GST_UTC_CONVERSION, TrackingCode::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    FLOAT64 A0_s_f64     =  concatenateBits(Word1.A0_1, Word2.A0_2, 24, 8) * pow(2, -30); 
    FLOAT64 A1_ss_f64    =  Word2.A1 * pow(2, -50);
    FLOAT64 DtLS_s_f64   =  Word3.ls_count_before; 
    FLOAT64 Tot_s_f64    =  Word3.utc_reference_tow * 3600;
    FLOAT64 WNt_f64      =  Word3.utc_reference_week;
    FLOAT64 WNlsf_f64    =  Word3.WN_lsf;
    FLOAT64 Dn_f64       =  Word4.day_num;
    FLOAT64 DtLSF_s_f64  =  Word4.ls_count_after;

    // tow???

    gal_nav_data.utc.A0_s_f64    =  A0_s_f64;
    gal_nav_data.utc.A1_ss_f64   =  A1_ss_f64;
    gal_nav_data.utc.DtLS_s_f64  =  DtLS_s_f64;
    gal_nav_data.utc.Tot_s_f64   =  Tot_s_f64;
    gal_nav_data.utc.WNt_f64     =  WNt_f64;
    gal_nav_data.utc.WNlsf_f64   =  WNlsf_f64;
    gal_nav_data.utc.Dn_f64      =  Dn_f64;
    gal_nav_data.utc.DtLSF_s_f64 =  DtLSF_s_f64;
}   


void GalileoPage<WordType::ALMANAC_1, TrackingCode::E5b>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_1, TrackingCode::E5b>::interpret(GalileoNavData& gal_nav_data) const
{
    UINT8   svId_u8           =  Word1.svid_1;
    UINT8   ioda              =  Word1.issue_of_data;
    UINT8   almWNa            =  Word1.week_num;
    FLOAT64 toa_sn_f64        =  Word1.t0a * 600;
    FLOAT64 deltaSqrtA_m_f64  =  concatenateBits(Word1.delta_root_a_1, Word2.delta_root_a_2, 2, 11) * pow(2, -9);
    FLOAT64 ecc_f64           =  Word2.eccentricity * pow(2, -16);
    FLOAT64 omg_rad_f64       =  concatenateBits(Word2.perigee_1, Word3.perigee_2, 10, 6) * pow(2, -15) * M_PI;
    FLOAT64 deltai_rad_f64    =  Word3.diff_ia_na * pow(2, -14) * M_PI;
    FLOAT64 omg0_rad_f64      =  concatenateBits(Word3.longitude_1, Word4.longitude_2, 15, 1) * pow(2, -15) * M_PI;
    FLOAT64 OMGdot_rad_f64    =  Word4.roc_ra * pow(2, -33) * M_PI;
    FLOAT64 m0_rad_f64        =  concatenateBits(Word4.mean_anomaly_1, Word5.mean_anomaly_2, 6, 10) * pow(2, -15) * M_PI;

    if (svId_u8 != 0)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].ioda              =  ioda;
        gal_nav_data.alm[svId_u8-1].almWNa            =  almWNa;
        gal_nav_data.alm[svId_u8-1].toa_sn_f64        =  toa_sn_f64;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  deltaSqrtA_m_f64;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  ecc_f64;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  omg_rad_f64;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  deltai_rad_f64;
        gal_nav_data.alm[svId_u8-1].omg0_rad_f64      =  omg0_rad_f64;
        gal_nav_data.alm[svId_u8-1].OMGdot_rad_f64    =  OMGdot_rad_f64;
        gal_nav_data.alm[svId_u8-1].m0_rad_f64        =  m0_rad_f64;
    }

    return svId_u8;
}   


void GalileoPage<WordType::ALMANAC_1, TrackingCode::E1>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_1, TrackingCode::E1>::interpret(GalileoNavData& gal_nav_data) const
{
    UINT8   svId_u8           =  Word1.svid_1;
    UINT8   ioda              =  Word1.issue_of_data;
    UINT8   almWNa            =  Word1.week_num;
    FLOAT64 toa_sn_f64        =  Word1.t0a * 600;
    FLOAT64 deltaSqrtA_m_f64  =  concatenateBits(Word1.delta_root_a_1, Word2.delta_root_a_2, 2, 11) * pow(2, -9);
    FLOAT64 ecc_f64           =  Word2.eccentricity * pow(2, -16);
    FLOAT64 omg_rad_f64       =  concatenateBits(Word2.perigee_1, Word3.perigee_2, 10, 6) * pow(2, -15) * M_PI;
    FLOAT64 deltai_rad_f64    =  Word3.diff_ia_na * pow(2, -14) * M_PI;
    FLOAT64 omg0_rad_f64      =  concatenateBits(Word3.longitude_1, Word4.longitude_2, 15, 1) * pow(2, -15) * M_PI;
    FLOAT64 OMGdot_rad_f64    =  Word4.roc_ra * pow(2, -33) * M_PI;
    FLOAT64 m0_rad_f64        =  concatenateBits(Word4.mean_anomaly_1, Word5.mean_anomaly_2, 6, 10) * pow(2, -15) * M_PI;

    if (svId_u8 != 0)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].ioda              =  ioda;
        gal_nav_data.alm[svId_u8-1].almWNa            =  almWNa;
        gal_nav_data.alm[svId_u8-1].toa_sn_f64        =  toa_sn_f64;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  deltaSqrtA_m_f64;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  ecc_f64;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  omg_rad_f64;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  deltai_rad_f64;
        gal_nav_data.alm[svId_u8-1].omg0_rad_f64      =  omg0_rad_f64;
        gal_nav_data.alm[svId_u8-1].OMGdot_rad_f64    =  OMGdot_rad_f64;
        gal_nav_data.alm[svId_u8-1].m0_rad_f64        =  m0_rad_f64;
    }

    return svId_u8;
}


void GalileoPage<WordType::ALMANAC_2, TrackingCode::E5b>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_2, TrackingCode::E5b>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8   ioda              =  Word1.issue_of_data;

    FLOAT64 af0_s_f64         =  Word1.clock_corr_bias * pow(2, -19);
    FLOAT64 af1_ss_f64        =  concatenateBits(Word1.clock_corr_linear_1, Word2.clock_corr_linear_2, 4, 9) * pow(2, -38);
    UINT8   healthE5b         =  Word2.sig_health_e5b;
    UINT8   healthE1          =  Word2.sig_health_e1;

    UINT8   svId_u8           =  Word2.svid_2;
    FLOAT64 deltaSqrtA_m_f64  =  Word2.delta_root_a * pow(2, -9);
    FLOAT64 ecc_f64           =  Word3.eccentricity * pow(2, -16);
    FLOAT64 omg_rad_f64       =  Word3.perigee * pow(2, -15) * M_PI;
    FLOAT64 deltai_rad_f64    =  concatenateBits(Word3.diff_ia_na_1, Word4.diff_ia_na_2, 5, 6) * pow(2, -14) * M_PI;
    FLOAT64 omg0_rad_f64      =  concatenateBits(Word4.longitude_1, Word5.longitude_2, 12, 4) * pow(2, -15) * M_PI;
    FLOAT64 OMGdot_rad_f64    =  Word5.roc_ra * pow(2, -33) * M_PI;

    if (svId != 0 && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].af0_s_f64   =  af0_s_f64;
        gal_nav_data.alm[svId-1].af1_ss_f64  =  af1_ss_f64;
        gal_nav_data.alm[svId-1].healthE5b   =  healthE5b;
        gal_nav_data.alm[svId-1].healthE1    =  healthE1;
    }

    if (svId_u8 != 0)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  deltaSqrtA_m_f64;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  ecc_f64;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  omg_rad_f64;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  deltai_rad_f64;
        gal_nav_data.alm[svId_u8-1].omg0_rad_f64      =  omg0_rad_f64;
        gal_nav_data.alm[svId_u8-1].OMGdot_rad_f64    =  OMGdot_rad_f64;
    }

    return svId_u8;
}   


void GalileoPage<WordType::ALMANAC_2, TrackingCode::E1>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_2, TrackingCode::E1>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8   ioda              =  Word1.issue_of_data;

    FLOAT64 af0_s_f64         =  Word1.clock_corr_bias * pow(2, -19);
    FLOAT64 af1_ss_f64        =  concatenateBits(Word1.clock_corr_linear_1, Word2.clock_corr_linear_2, 4, 9) * pow(2, -38);
    UINT8   healthE5b         =  Word2.sig_health_e5b;
    UINT8   healthE1          =  Word2.sig_health_e1;

    UINT8   svId_u8           =  Word2.svid_2;
    FLOAT64 deltaSqrtA_m_f64  =  Word2.delta_root_a * pow(2, -9);
    FLOAT64 ecc_f64           =  Word3.eccentricity * pow(2, -16);
    FLOAT64 omg_rad_f64       =  Word3.perigee * pow(2, -15) * M_PI;
    FLOAT64 deltai_rad_f64    =  concatenateBits(Word3.diff_ia_na_1, Word4.diff_ia_na_2, 5, 6) * pow(2, -14) * M_PI;
    FLOAT64 omg0_rad_f64      =  concatenateBits(Word4.longitude_1, Word5.longitude_2, 12, 4) * pow(2, -15) * M_PI;
    FLOAT64 OMGdot_rad_f64    =  Word5.roc_ra * pow(2, -33) * M_PI;

    if (svId != 0 && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].af0_s_f64   =  af0_s_f64;
        gal_nav_data.alm[svId-1].af1_ss_f64  =  af1_ss_f64;
        gal_nav_data.alm[svId-1].healthE5b   =  healthE5b;
        gal_nav_data.alm[svId-1].healthE1    =  healthE1;
    }

    if (svId_u8 != 0)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].ioda              =  ioda;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  deltaSqrtA_m_f64;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  ecc_f64;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  omg_rad_f64;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  deltai_rad_f64;
        gal_nav_data.alm[svId_u8-1].omg0_rad_f64      =  omg0_rad_f64;
        gal_nav_data.alm[svId_u8-1].OMGdot_rad_f64    =  OMGdot_rad_f64;
    }

    return svId_u8;
}   


void GalileoPage<WordType::ALMANAC_3, TrackingCode::E5b>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_3, TrackingCode::E5b>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8   ioda              =  Word1.issue_of_data;

    UINT8   almWNa            =  Word1.week_num;
    FLOAT64 toa_sn_f64        =  Word1.t0a * 600;
    FLOAT64 m0_rad_f64        =  concatenateBits(Word1.mean_anomaly_1, Word2.mean_anomaly_2, 8, 8) * pow(2, -15) * M_PI;
    FLOAT64 af0_s_f64         =  Word2.clock_corr_bias * pow(2, -19);
    FLOAT64 af1_ss_f64        =  concatenateBits(Word2.clock_corr_linear_1, Word3.clock_corr_linear_2, 8, 5) * pow(2, -38);
    UINT8   healthE5b         =  Word3.sig_health_e5b;
    UINT8   healthE1          =  Word3.sig_health_e1;
 
    UINT8   svId_u8           =  Word3.svid_3;
    FLOAT64 deltaSqrtA_m_f64  =  Word3.delta_root_a * pow(2, -9);
    FLOAT64 ecc_f64           =  concatenateBits(Word3.eccentricity_1, Word4.eccentricity_2, 4, 7) * pow(2, -16);
    FLOAT64 omg_rad_f64       =  concatenateBits(Word4.perigee_1, Word5.perigee_2, 11, 5) * pow(2, -15) * M_PI;
    FLOAT64 deltai_rad_f64    =  Word5.diff_ia_na * pow(2, -14) * M_PI;

    if (svId != 0 && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].almWNa      =  almWNa;
        gal_nav_data.alm[svId-1].toa_sn_f64  =  toa_sn_f64;
        gal_nav_data.alm[svId-1].m0_rad_f64  =  m0_rad_f64;
        gal_nav_data.alm[svId-1].af0_s_f64   =  af0_s_f64;
        gal_nav_data.alm[svId-1].af1_ss_f64  =  af1_ss_f64;
        gal_nav_data.alm[svId-1].healthE5b   =  healthE5b;
        gal_nav_data.alm[svId-1].healthE1    =  healthE1;
    }

    if (svId_u8 != 0)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].ioda              =  ioda;
        gal_nav_data.alm[svId_u8-1].almWNa            =  almWNa;
        gal_nav_data.alm[svId_u8-1].toa_sn_f64        =  toa_sn_f64;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  deltaSqrtA_m_f64;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  ecc_f64;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  omg_rad_f64;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  deltai_rad_f64;
    }

    return svId_u8;
}   


void GalileoPage<WordType::ALMANAC_3, TrackingCode::E1>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_3, TrackingCode::E1>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8   ioda              =  Word1.issue_of_data;

    UINT8   almWNa            =  Word1.week_num;
    FLOAT64 toa_sn_f64        =  Word1.t0a * 600;
    FLOAT64 m0_rad_f64        =  concatenateBits(Word1.mean_anomaly_1, Word2.mean_anomaly_2, 8, 8) * pow(2, -15) * M_PI;
    FLOAT64 af0_s_f64         =  Word2.clock_corr_bias * pow(2, -19);
    FLOAT64 af1_ss_f64        =  concatenateBits(Word2.clock_corr_linear_1, Word3.clock_corr_linear_2, 8, 5) * pow(2, -38);
    UINT8   healthE5b         =  Word3.sig_health_e5b;
    UINT8   healthE1          =  Word3.sig_health_e1;
 
    UINT8   svId_u8           =  Word3.svid_3;
    FLOAT64 deltaSqrtA_m_f64  =  Word3.delta_root_a * pow(2, -9);
    FLOAT64 ecc_f64           =  concatenateBits(Word3.eccentricity_1, Word4.eccentricity_2, 4, 7) * pow(2, -16);
    FLOAT64 omg_rad_f64       =  concatenateBits(Word4.perigee_1, Word5.perigee_2, 11, 5) * pow(2, -15) * M_PI;
    FLOAT64 deltai_rad_f64    =  Word5.diff_ia_na * pow(2, -14) * M_PI;

    if (svId != 0 && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].almWNa      =  almWNa;
        gal_nav_data.alm[svId-1].toa_sn_f64  =  toa_sn_f64;
        gal_nav_data.alm[svId-1].m0_rad_f64  =  m0_rad_f64;
        gal_nav_data.alm[svId-1].af0_s_f64   =  af0_s_f64;
        gal_nav_data.alm[svId-1].af1_ss_f64  =  af1_ss_f64;
        gal_nav_data.alm[svId-1].healthE5b   =  healthE5b;
        gal_nav_data.alm[svId-1].healthE1    =  healthE1;
    }

    if (svId_u8 != 0)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].ioda              =  ioda;
        gal_nav_data.alm[svId_u8-1].almWNa            =  almWNa;
        gal_nav_data.alm[svId_u8-1].toa_sn_f64        =  toa_sn_f64;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  deltaSqrtA_m_f64;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  ecc_f64;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  omg_rad_f64;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  deltai_rad_f64;
    }

    return svId_u8;
}   


void GalileoPage<WordType::ALMANAC_4, TrackingCode::E5b>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::ALMANAC_4, TrackingCode::E5b>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8   ioda            =  Word1.issue_of_data;
 
    FLOAT64 omg0_rad_f64    =  Word1.longitude * pow(2, -15) * M_PI;
    FLOAT64 OMGdot_rad_f64  =  concatenateBits(Word1.roc_ra_1, Word2.roc_ra_2, 4, 7) * pow(2, -33) * M_PI;
    FLOAT64 m0_rad_f64      =  Word2.mean_anomaly * pow(2, -15) * M_PI;
    FLOAT64 af0_s_f64       =  concatenateBits(Word2.clock_corr_bias_1, Word3.clock_corr_bias_2, 9, 7) * pow(2, -19);
    FLOAT64 af1_ss_f64      =  Word3.clock_corr_linear * pow(2, -38);
    UINT8   healthE5b       =  Word3.sig_health_e5b;
    UINT8   healthE1        =  Word3.sig_health_e1;
 
    FLOAT64 a0G_s           =  concatenateBits(Word3.const_term_offset_1, Word4.const_term_offset_2, 8, 8) * pow(2, -35);
    FLOAT64 a1G_ss          =  concatenateBits(Word4.roc_offset_1, Word5.roc_offset_2, 10, 2) * pow(2, -51);
    FLOAT64 t0G_s           =  Word5.t0G * 3600;
    FLOAT64 wn0G_week       =  Word5.wn0G;

    if (svId != 0 && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].omg0_rad_f64    =  omg0_rad_f64;
        gal_nav_data.alm[svId-1].OMGdot_rad_f64  =  OMGdot_rad_f64;
        gal_nav_data.alm[svId-1].m0_rad_f64      =  m0_rad_f64;
        gal_nav_data.alm[svId-1].af0_s_f64       =  af0_s_f64;
        gal_nav_data.alm[svId-1].af1_ss_f64      =  af1_ss_f64;
        gal_nav_data.alm[svId-1].healthE5b       =  healthE5b;
        gal_nav_data.alm[svId-1].healthE1        =  healthE1;
    }

    gal_nav_data.time_offset.a0G_s      =  a0G_s;
    gal_nav_data.time_offset.a1G_ss     =  a1G_ss;
    gal_nav_data.time_offset.t0G_s      =  t0G_s;
    gal_nav_data.time_offset.wn0G_week  =  wn0G_week;

}


void GalileoPage<WordType::ALMANAC_4, TrackingCode::E1>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::ALMANAC_4, TrackingCode::E1>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8   ioda            =  Word1.issue_of_data;
 
    FLOAT64 omg0_rad_f64    =  Word1.longitude * pow(2, -15) * M_PI;
    FLOAT64 OMGdot_rad_f64  =  concatenateBits(Word1.roc_ra_1, Word2.roc_ra_2, 4, 7) * pow(2, -33) * M_PI;
    FLOAT64 m0_rad_f64      =  Word2.mean_anomaly * pow(2, -15) * M_PI;
    FLOAT64 af0_s_f64       =  concatenateBits(Word2.clock_corr_bias_1, Word3.clock_corr_bias_2, 9, 7) * pow(2, -19);
    FLOAT64 af1_ss_f64      =  Word3.clock_corr_linear * pow(2, -38);
    UINT8   healthE5b       =  Word3.sig_health_e5b;
    UINT8   healthE1        =  Word3.sig_health_e1;
 
    FLOAT64 a0G_s           =  concatenateBits(Word3.const_term_offset_1, Word4.const_term_offset_2, 8, 8) * pow(2, -35);
    FLOAT64 a1G_ss          =  concatenateBits(Word4.roc_offset_1, Word5.roc_offset_2, 10, 2) * pow(2, -51);
    FLOAT64 t0G_s           =  Word5.t0G * 3600;
    FLOAT64 wn0G_week       =  Word5.wn0G;

    if (svId != 0 && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].omg0_rad_f64    =  omg0_rad_f64;
        gal_nav_data.alm[svId-1].OMGdot_rad_f64  =  OMGdot_rad_f64;
        gal_nav_data.alm[svId-1].m0_rad_f64      =  m0_rad_f64;
        gal_nav_data.alm[svId-1].af0_s_f64       =  af0_s_f64;
        gal_nav_data.alm[svId-1].af1_ss_f64      =  af1_ss_f64;
        gal_nav_data.alm[svId-1].healthE5b       =  healthE5b;
        gal_nav_data.alm[svId-1].healthE1        =  healthE1;
    }

    gal_nav_data.time_offset.a0G_s      =  a0G_s;
    gal_nav_data.time_offset.a1G_ss     =  a1G_ss;
    gal_nav_data.time_offset.t0G_s      =  t0G_s;
    gal_nav_data.time_offset.wn0G_week  =  wn0G_week;

}