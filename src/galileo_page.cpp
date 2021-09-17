/**
 * @file    : Ublox-Driver project file
 * @author  : Yusuf TAŞKIRAN
 * @note    : ytaskiran70@gmail.com
 * @date    : 6 / Eylül / 2021
 * @code    : galileo_page.cpp file
 * @details : 
 */

#include <cmath>
#include "galileo_page.hpp"


void GalileoPage<WordType::EPHEMERIS_1, UbxSigId::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::EPHEMERIS_1, UbxSigId::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    gal_nav_data.eph[svId-1].iodNav      =  Word1.issue_of_data;
    gal_nav_data.eph[svId-1].Toe         =  Word1.toe * 60.0;
    gal_nav_data.eph[svId-1].m0_rad_f64  =  Word2.mean_anomaly * P2_31 * M_PI;
    gal_nav_data.eph[svId-1].ecc_f64     =  Word3.eccentricity * P2_33;
    gal_nav_data.eph[svId-1].sqrtA_f64   =  concatenateBits(Word4.root_semi_major_axis_1, Word5.root_semi_major_axis_2, 18, 14) * P2_19;
}   


void GalileoPage<WordType::EPHEMERIS_2, UbxSigId::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::EPHEMERIS_2, UbxSigId::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    gal_nav_data.eph[svId-1].omega0_rad_f64    =  concatenateBits(Word1.longitude_1, Word2.longitude_2, 14, 18) * P2_31 * M_PI;
    gal_nav_data.eph[svId-1].i0_rad_f64        =  concatenateBits(Word2.inclination_angle_1, Word3.inclination_angle_2, 14, 18) * P2_31 * M_PI;
    gal_nav_data.eph[svId-1].omega_rad_f64     =  concatenateBits(Word3.perigee_1, Word4.perigee_2, 14, 18) * P2_31 * M_PI;
    gal_nav_data.eph[svId-1].IDOT_rad_sec_f64  =  Word5.ia_rate_of_change * P2_43 * M_PI;
}   


void GalileoPage<WordType::EPHEMERIS_3, UbxSigId::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::EPHEMERIS_3, UbxSigId::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    FLOAT32 sisaIndexE1E5b{};

    if (Word5.sisa > 0 && Word5.sisa <= 50)
        sisaIndexE1E5b = Word5.sisa * 0.01;
    else if (Word5.sisa > 50 && Word5.sisa <= 75)
        sisaIndexE1E5b = 0.5 + ((Word5.sisa - 50) * 0.02);
    else if (Word5.sisa > 75 && Word5.sisa <= 100)
        sisaIndexE1E5b = 1 + ((Word5.sisa - 75) * 0.04);
    else if (Word5.sisa > 100 && Word5.sisa <= 125)
        sisaIndexE1E5b = 2 + ((Word5.sisa - 100) * 0.16);

    gal_nav_data.eph[svId-1].OMGdot_rad_f64      =  concatenateBits(Word1.ra_rate_of_change_1, Word2.ra_rate_of_change_2, 14, 10) * P2_43* M_PI;
    gal_nav_data.eph[svId-1].deltaN_rad_sec_f64  =  Word2.mean_motion_difference * P2_43 * M_PI;
    gal_nav_data.eph[svId-1].Cuc_rad_f64         =  concatenateBits(Word2.C_uc_1, Word3.C_uc_2, 6, 10) * P2_29;
    gal_nav_data.eph[svId-1].Cus_rad_f64         =  Word3.C_us * P2_29;
    gal_nav_data.eph[svId-1].Crc_m_f64           =  concatenateBits(Word3.C_rc_1, Word4.C_rc_2, 6, 10) * P2_5;
    gal_nav_data.eph[svId-1].Crs_m_f64           =  concatenateBits(Word4.C_rs_1, Word5.C_rs_2, 8, 8) * P2_5;
    gal_nav_data.eph[svId-1].sisaIndexE1E5b      =  sisaIndexE1E5b;
}   


void GalileoPage<WordType::EPHEMERIS_4_CLOCK_CORRECTION, UbxSigId::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::EPHEMERIS_4_CLOCK_CORRECTION, UbxSigId::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    gal_nav_data.eph[svId-1].svId_u8      =  Word1.svid;
    gal_nav_data.eph[svId-1].Cic_rad_f64  =  concatenateBits(Word1.C_ic_1, Word2.C_ic_2, 8, 8) * P2_29;
    gal_nav_data.eph[svId-1].Cis_rad_f64  =  Word2.C_is * P2_29;
    gal_nav_data.eph[svId-1].Toc          = concatenateBits(Word2.toc_1, Word3.toc_2, 8, 6) * 60;
    gal_nav_data.eph[svId-1].af0_s_f64    =  concatenateBits(Word3.clock_bias_corr_1, Word4.clock_bias_corr_2, 26, 5) * P2_34;
    gal_nav_data.eph[svId-1].af1_ss_f64   =  concatenateBits(Word4.clock_drift_corr_1, Word5.clock_drift_corr_2, 13, 8) * P2_46;
    gal_nav_data.eph[svId-1].af2_ss2_f64  =  Word5.clock_drift_rate_corr * P2_59;
}   


void GalileoPage<WordType::IONOSPHERIC_CORRECTION_BGD_SIG_HEALTH_DVS_GST, UbxSigId::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::IONOSPHERIC_CORRECTION_BGD_SIG_HEALTH_DVS_GST, UbxSigId::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    gal_nav_data.iono.ai0_f64                 =  Word1.ai0 * P2_2;
    gal_nav_data.iono.ai1_f64                 =  Word1.ai1 * P2_8;
    gal_nav_data.iono.ai2_f64                 =  concatenateBits(Word1.ai2_1, Word2.ai2_2, 2, 12) * P2_15;
    gal_nav_data.eph[svId-1].bgdE1E5b         =  concatenateBits(Word2.bgd_2_1, Word3.bgd_2_2, 5, 5) * P2_32;
    gal_nav_data.eph[svId-1].healthE5b        =  Word3.sig_health_e5b;
    gal_nav_data.eph[svId-1].healthE1         =  Word3.sig_health_e1;
    gal_nav_data.eph[svId-1].dataValidityE5b  =  Word3.data_validity_e5b;
    gal_nav_data.eph[svId-1].dataValidityE1   =  Word3.data_validity_e1;
    gal_nav_data.week_u32                     =  Word3.week_num;
    gal_nav_data.eph[svId-1].txmit_time       =  concatenateBits(Word3.time_of_week_1, Word4.time_of_week_2, 9, 11);
}


void GalileoPage<WordType::GST_UTC_CONVERSION, UbxSigId::None>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::GST_UTC_CONVERSION, UbxSigId::None>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    gal_nav_data.utc.A0_s_f64    =  concatenateBits(Word1.A0_1, Word2.A0_2, 24, 8) * P2_30;
    gal_nav_data.utc.A1_ss_f64   =  Word2.A1 * P2_50;
    gal_nav_data.utc.DtLS_s_f64  =  Word3.ls_count_before;
    gal_nav_data.utc.Tot_s_f64   =  Word3.utc_reference_tow * 3600;
    gal_nav_data.utc.WNt_f64     =  Word3.utc_reference_week;
    gal_nav_data.utc.WNlsf_f64   =  Word3.WN_lsf;
    gal_nav_data.utc.Dn_f64      =  Word4.day_num;
    gal_nav_data.utc.DtLSF_s_f64 =  Word4.ls_count_after;
}   


void GalileoPage<WordType::ALMANAC_1, UbxSigId::GALILEO_E5bl>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_1, UbxSigId::GALILEO_E5bl>::interpret(GalileoNavData& gal_nav_data) const
{
    UINT8 svId (Word1.svid_1);
    if (svId)
    {
        gal_nav_data.alm[svId-1].svId_u8           =  svId;
        gal_nav_data.alm[svId-1].ioda              =  Word1.issue_of_data;
        gal_nav_data.alm[svId-1].almWNa            =  Word1.week_num;
        gal_nav_data.alm[svId-1].toa_sn_f64        =  Word1.t0a * 600.0;
        gal_nav_data.alm[svId-1].deltaSqrtA_m_f64  =  concatenateBits(Word1.delta_root_a_1, Word2.delta_root_a_2, 2, 11) * P2_9;
        gal_nav_data.alm[svId-1].ecc_f64           =  Word2.eccentricity * P2_16;
        gal_nav_data.alm[svId-1].omg_rad_f64       =  concatenateBits(Word2.perigee_1, Word3.perigee_2, 10, 6) * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].deltai_rad_f64    =  Word3.diff_ia_na * P2_14 * M_PI;
        gal_nav_data.alm[svId-1].omg0_rad_f64      =  concatenateBits(Word3.longitude_1, Word4.longitude_2, 15, 1) * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].OMGdot_rad_f64    =  Word4.roc_ra * P2_33 * M_PI;
        gal_nav_data.alm[svId-1].m0_rad_f64        =  concatenateBits(Word4.mean_anomaly_1, Word5.mean_anomaly_2, 6, 10) * P2_15 * M_PI;
    }

    return svId;
}   


void GalileoPage<WordType::ALMANAC_1, UbxSigId::GALILEO_E1B>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_1, UbxSigId::GALILEO_E1B>::interpret(GalileoNavData& gal_nav_data) const
{
    UINT8 svId(Word1.svid_1);
    if (svId)
    {
        gal_nav_data.alm[svId-1].svId_u8           =  svId;
        gal_nav_data.alm[svId-1].ioda              =  Word1.issue_of_data;
        gal_nav_data.alm[svId-1].almWNa            =  Word1.week_num;
        gal_nav_data.alm[svId-1].toa_sn_f64        =  Word1.t0a * 600.0;
        gal_nav_data.alm[svId-1].deltaSqrtA_m_f64  =  concatenateBits(Word1.delta_root_a_1, Word2.delta_root_a_2, 2, 11) * P2_9;
        gal_nav_data.alm[svId-1].ecc_f64           =  Word2.eccentricity * P2_16;
        gal_nav_data.alm[svId-1].omg_rad_f64       =  concatenateBits(Word2.perigee_1, Word3.perigee_2, 10, 6) * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].deltai_rad_f64    =  Word3.diff_ia_na * P2_14 * M_PI;
        gal_nav_data.alm[svId-1].omg0_rad_f64      =  concatenateBits(Word3.longitude_1, Word4.longitude_2, 15, 1) * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].OMGdot_rad_f64    =  Word4.roc_ra * P2_33 * M_PI;
        gal_nav_data.alm[svId-1].m0_rad_f64        =  concatenateBits(Word4.mean_anomaly_1, Word5.mean_anomaly_2, 6, 10) * P2_15 * M_PI;
    }

    return svId;
}


void GalileoPage<WordType::ALMANAC_2, UbxSigId::GALILEO_E5bl>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_2, UbxSigId::GALILEO_E5bl>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8  ioda =  Word1.issue_of_data;
    if (svId && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].af0_s_f64   =  Word1.clock_corr_bias * P2_19;
        gal_nav_data.alm[svId-1].af1_ss_f64  =  concatenateBits(Word1.clock_corr_linear_1, Word2.clock_corr_linear_2, 4, 9) * P2_38;
        gal_nav_data.alm[svId-1].healthE5b   =  Word2.sig_health_e5b;
        gal_nav_data.alm[svId-1].healthE1    =  Word2.sig_health_e1;
    }

    UINT8 svId_u8(Word2.svid_2);
    if (svId_u8 != 0)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].ioda              =  ioda;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  Word2.delta_root_a * P2_9;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  Word3.eccentricity * P2_16;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  Word3.perigee * P2_15 * M_PI;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  concatenateBits(Word3.diff_ia_na_1, Word4.diff_ia_na_2, 5, 6) * P2_14 * M_PI;
        gal_nav_data.alm[svId_u8-1].omg0_rad_f64      =  concatenateBits(Word4.longitude_1, Word5.longitude_2, 12, 4)  * P2_15 * M_PI;
        gal_nav_data.alm[svId_u8-1].OMGdot_rad_f64    =  Word5.roc_ra * P2_33 * M_PI;
    }

    return svId_u8;
}   


void GalileoPage<WordType::ALMANAC_2, UbxSigId::GALILEO_E1B>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_2, UbxSigId::GALILEO_E1B>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8 ioda = Word1.issue_of_data;
    if (svId != 0 && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].af0_s_f64   =  Word1.clock_corr_bias * P2_19;
        gal_nav_data.alm[svId-1].af1_ss_f64  =  concatenateBits(Word1.clock_corr_linear_1, Word2.clock_corr_linear_2, 4, 9) * P2_38;
        gal_nav_data.alm[svId-1].healthE5b   =  Word2.sig_health_e5b;
        gal_nav_data.alm[svId-1].healthE1    =  Word2.sig_health_e1;
    }

    UINT8 svId_u8 = Word2.svid_2;
    if (svId_u8)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].ioda              =  ioda;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  Word2.delta_root_a * P2_9;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  Word3.eccentricity * P2_16;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  Word3.perigee * P2_15 * M_PI;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  concatenateBits(Word3.diff_ia_na_1, Word4.diff_ia_na_2, 5, 6) * P2_14 * M_PI;
        gal_nav_data.alm[svId_u8-1].omg0_rad_f64      =  concatenateBits(Word4.longitude_1, Word5.longitude_2, 12, 4)  * P2_15 * M_PI;
        gal_nav_data.alm[svId_u8-1].OMGdot_rad_f64    =  Word5.roc_ra * P2_33 * M_PI;
    }

    return svId_u8;
}   


void GalileoPage<WordType::ALMANAC_3, UbxSigId::GALILEO_E5bl>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_3, UbxSigId::GALILEO_E5bl>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8 ioda    =  Word1.issue_of_data;
    UINT8 svId_u8 =  Word3.svid_3;

    if (svId && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].almWNa      =  Word1.week_num;
        gal_nav_data.alm[svId-1].toa_sn_f64  =  Word1.t0a * 600.0;
        gal_nav_data.alm[svId-1].m0_rad_f64  =  concatenateBits(Word1.mean_anomaly_1, Word2.mean_anomaly_2, 8, 8) * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].af0_s_f64   =  Word2.clock_corr_bias * P2_19;
        gal_nav_data.alm[svId-1].af1_ss_f64  =  concatenateBits(Word2.clock_corr_linear_1, Word3.clock_corr_linear_2, 8, 5) * P2_38;
        gal_nav_data.alm[svId-1].healthE5b   =  Word3.sig_health_e5b;
        gal_nav_data.alm[svId-1].healthE1    =  Word3.sig_health_e1;
    }

    if (svId_u8)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].ioda              =  ioda;
        gal_nav_data.alm[svId_u8-1].almWNa            =  Word1.week_num;
        gal_nav_data.alm[svId_u8-1].toa_sn_f64        =  Word1.t0a * 600.0;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  Word3.delta_root_a * P2_9;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  concatenateBits(Word3.eccentricity_1, Word4.eccentricity_2, 4, 7) * P2_16;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  concatenateBits(Word4.perigee_1, Word5.perigee_2, 11, 5) * P2_15 * M_PI;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  Word5.diff_ia_na * P2_14 * M_PI;
    }

    return svId_u8;
}   


void GalileoPage<WordType::ALMANAC_3, UbxSigId::GALILEO_E1B>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

UINT8 GalileoPage<WordType::ALMANAC_3, UbxSigId::GALILEO_E1B>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    UINT8   ioda    =  Word1.issue_of_data;
    UINT8   svId_u8 =  Word3.svid_3;

    if (svId && ioda == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].almWNa      =  Word1.week_num;
        gal_nav_data.alm[svId-1].toa_sn_f64  =  Word1.t0a * 600.0;
        gal_nav_data.alm[svId-1].m0_rad_f64  =  concatenateBits(Word1.mean_anomaly_1, Word2.mean_anomaly_2, 8, 8) * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].af0_s_f64   =  Word2.clock_corr_bias * P2_19;
        gal_nav_data.alm[svId-1].af1_ss_f64  =  concatenateBits(Word2.clock_corr_linear_1, Word3.clock_corr_linear_2, 8, 5) * P2_38;
        gal_nav_data.alm[svId-1].healthE5b   =  Word3.sig_health_e5b;
        gal_nav_data.alm[svId-1].healthE1    =  Word3.sig_health_e1;
    }

    if (svId_u8)
    {
        gal_nav_data.alm[svId_u8-1].svId_u8           =  svId_u8;
        gal_nav_data.alm[svId_u8-1].ioda              =  ioda;
        gal_nav_data.alm[svId_u8-1].almWNa            =  Word1.week_num;
        gal_nav_data.alm[svId_u8-1].toa_sn_f64        =  Word1.t0a * 600.0;
        gal_nav_data.alm[svId_u8-1].deltaSqrtA_m_f64  =  Word3.delta_root_a * P2_9;
        gal_nav_data.alm[svId_u8-1].ecc_f64           =  concatenateBits(Word3.eccentricity_1, Word4.eccentricity_2, 4, 7) * P2_16;
        gal_nav_data.alm[svId_u8-1].omg_rad_f64       =  concatenateBits(Word4.perigee_1, Word5.perigee_2, 11, 5) * P2_15 * M_PI;
        gal_nav_data.alm[svId_u8-1].deltai_rad_f64    =  Word5.diff_ia_na * P2_14 * M_PI;
    }

    return svId_u8;
}   


void GalileoPage<WordType::ALMANAC_4, UbxSigId::GALILEO_E5bl>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::ALMANAC_4, UbxSigId::GALILEO_E5bl>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    if (svId && UINT8(Word1.issue_of_data) == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].omg0_rad_f64    =  Word1.longitude * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].OMGdot_rad_f64  =  concatenateBits(Word1.roc_ra_1, Word2.roc_ra_2, 4, 7) * P2_33 * M_PI;
        gal_nav_data.alm[svId-1].m0_rad_f64      =  Word2.mean_anomaly * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].af0_s_f64       =  concatenateBits(Word2.clock_corr_bias_1, Word3.clock_corr_bias_2, 9, 7) * P2_19;
        gal_nav_data.alm[svId-1].af1_ss_f64      =  Word3.clock_corr_linear * P2_38;
        gal_nav_data.alm[svId-1].healthE5b       =  Word3.sig_health_e5b;
        gal_nav_data.alm[svId-1].healthE1        =  Word3.sig_health_e1;
    }

    gal_nav_data.time_offset.a0G_s      =  concatenateBits(Word3.const_term_offset_1, Word4.const_term_offset_2, 8, 8) * P2_35;
    gal_nav_data.time_offset.a1G_ss     =  concatenateBits(Word4.roc_offset_1, Word5.roc_offset_2, 10, 2) * P2_51;
    gal_nav_data.time_offset.t0G_s      =  Word5.t0G * 3600.0;
    gal_nav_data.time_offset.wn0G_week  =  Word5.wn0G;
}


void GalileoPage<WordType::ALMANAC_4, UbxSigId::GALILEO_E1B>::decode(const std::uint32_t(&word)[10])
{
    Word1.word = word[0];
    Word2.word = word[1];
    Word3.word = word[2];
    Word4.word = word[3];
    Word5.word = word[4];
}

void GalileoPage<WordType::ALMANAC_4, UbxSigId::GALILEO_E1B>::interpret(GalileoNavData& gal_nav_data, std::uint8_t svId) const
{
    if (svId && UINT8(Word1.issue_of_data) == gal_nav_data.alm[svId-1].ioda)
    {
        gal_nav_data.alm[svId-1].omg0_rad_f64    =  Word1.longitude * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].OMGdot_rad_f64  =  concatenateBits(Word1.roc_ra_1, Word2.roc_ra_2, 4, 7) * P2_33 * M_PI;
        gal_nav_data.alm[svId-1].m0_rad_f64      =  Word2.mean_anomaly * P2_15 * M_PI;
        gal_nav_data.alm[svId-1].af0_s_f64       =  concatenateBits(Word2.clock_corr_bias_1, Word3.clock_corr_bias_2, 9, 7) * P2_19;
        gal_nav_data.alm[svId-1].af1_ss_f64      =  Word3.clock_corr_linear * P2_38;
        gal_nav_data.alm[svId-1].healthE5b       =  Word3.sig_health_e5b;
        gal_nav_data.alm[svId-1].healthE1        =  Word3.sig_health_e1;
    }

    gal_nav_data.time_offset.a0G_s      =  concatenateBits(Word3.const_term_offset_1, Word4.const_term_offset_2, 8, 8) * P2_35;
    gal_nav_data.time_offset.a1G_ss     =  concatenateBits(Word4.roc_offset_1, Word5.roc_offset_2, 10, 2) * P2_51;
    gal_nav_data.time_offset.t0G_s      =  Word5.t0G * 3600.0;
    gal_nav_data.time_offset.wn0G_week  =  Word5.wn0G;
}