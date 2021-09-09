#include "gtest/gtest.h"
#include "galileo_page.hpp"

#define PRECISION 6



TEST(GalileoFunctionTest, ConcatenateBits)
{
  std::vector<std::vector<uint16_t>> data_bytes = {{0x0a, 0xa9}, {0xb1, 0xa8}, {0x2b, 0x75}};
  std::vector<uint16_t> concatenated_data = {0xaa9, 0xb1a8, 0x2b75};

  uint8_t index = 0;
  for (auto data : data_bytes)
  {
    EXPECT_EQ(concatenateBits(data[0], data[1], 8, 8), concatenated_data[index]);
    ++index;
  }
}

/**
 * @note  EXPECT_NEAR precision is set to 6
 *        while compared data obtained with
 *        precision 8
 * 
 */

TEST(GalileoPageTest, WordType1)
{
    const UINT32 word[10] = {0x01059edc, 0x31b2886b, 0x002fb8d5, 0xaa050000, 0x853bc9e2, 0, 0, 0, 0, 0};
    const UINT8 svId = 1;

    GalileoPage<WordType::EPHEMERIS_1, TrackingCode::None> gal_page_1_test{};
    GalileoNavData gal_nav_data_test{};

    gal_page_1_test.decode(word);
    gal_page_1_test.interpret(gal_nav_data_test, svId);

    EXPECT_EQ(gal_nav_data_test.eph[svId-1].iodNav, 22);
    EXPECT_EQ(gal_nav_data_test.eph[svId-1].toe_sn_f64, 474000);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].m0_rad_f64, 1.21975756e+00, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].ecc_f64, 3.64089967e-04, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].sqrtA_f64, 5.44062755e+03, PRECISION);
}


TEST(GalileoPageTest, WordType2)
{
    const UINT32 word[10] = {0x0205a4df, 0x183d89cb, 0x167ffb96, 0xd0a30000, 0xbded4000, 0, 0, 0, 0, 0};
    const UINT8 svId = 1;

    GalileoPage<WordType::EPHEMERIS_2, TrackingCode::None> gal_page_2_test{};
    GalileoNavData gal_nav_data_test{};

    gal_page_2_test.decode(word);
    gal_page_2_test.interpret(gal_nav_data_test, svId);

    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].omega0_rad_f64, -2.66333783e+00, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].i0_rad_f64, 9.61456163e-01, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].omega_rad_f64, -4.33037029e-01, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].IDOT_rad_sec_f64, -1.89650757e-10, PRECISION);
}

TEST(GalileoPageTest, WordType3)
{
    const UINT32 word[10] = {0x0305bff0, 0xcd48633e, 0x658431c5, 0x4abdc000, 0x97dada69, 0, 0, 0, 0, 0};
    const UINT8 svId = 1;

    GalileoPage<WordType::EPHEMERIS_3, TrackingCode::None> gal_page_3_test{};
    GalileoNavData gal_nav_data_test{};

    gal_page_3_test.decode(word);
    gal_page_3_test.interpret(gal_nav_data_test, svId);

    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].OMGdot_rad_f64, -5.55844582e-09, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].deltaN_rad_sec_f64, 3.06727062e-09, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Cuc_rad_f64, -3.05846334e-06, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Cus_rad_f64, 8.00006092e-06, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Crc_m_f64, 1.69312500e+02, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Crs_m_f64, -6.90312500e+01, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].sisaIndexE1E5b, 3.12, PRECISION);
}

TEST(GalileoPageTest, WordType4)
{
    const UINT32 word[10] = {0x040589ff, 0xd100087b, 0x702dbcac, 0x7fff0000, 0x9e004000, 0, 0, 0, 0, 0};
    const UINT8 svId = 1;

    GalileoPage<WordType::EPHEMERIS_4__CLOCK_CORRECTION, TrackingCode::None> gal_page_4_test{};
    GalileoNavData gal_nav_data_test{};

    gal_page_4_test.decode(word);
    gal_page_4_test.interpret(gal_nav_data_test, svId);

    EXPECT_EQ(gal_nav_data_test.eph[svId-1].svId_u8, 9);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Cic_rad_f64, -8.75443220e-08, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Cis_rad_f64, 1.49011612e-08, PRECISION);
    EXPECT_EQ(gal_nav_data_test.eph[svId-1].t0c, 474000);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].af0_s_f64, 5.58313070e-03, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].af1_ss_f64, -1.28466127e-11, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].af2_ss2_f64, 0.00000000e+00, PRECISION);
}

TEST(GalileoPageTest, WordType5)
{
    const UINT32 word[10] = {0x052b2148, 0x047000c0, 0x3008f0e7, 0xcb6a8000, 0xaaaaa72d, 0, 0, 0, 0, 0};
    const UINT8 svId = 1;

    GalileoPage<WordType::IONOSPHERIC_CORRECTION__BGD__SIG_HEALTH__DVS__GST, TrackingCode::None> gal_page_5_test{};
    GalileoNavData gal_nav_data_test{};

    gal_page_5_test.decode(word);
    gal_page_5_test.interpret(gal_nav_data_test, svId);

    EXPECT_NEAR(gal_nav_data_test.iono.ai0_f64, 8.62500000e+01, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.iono.ai1_f64, 3.20312500e-01, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.iono.ai2_f64, 2.16674805e-03, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].bgdE1E5b, 1.39698386e-09, PRECISION);
    EXPECT_EQ(gal_nav_data_test.eph[svId-1].healthE5b, 0);
    EXPECT_EQ(gal_nav_data_test.eph[svId-1].healthE1, 0);
    EXPECT_EQ(gal_nav_data_test.eph[svId-1].dataValidityE5b, 0);
    EXPECT_EQ(gal_nav_data_test.eph[svId-1].dataValidityE1, 0);
}

TEST(GalileoPageTest, WordType6)
{
    const UINT32 word[10] = {0x06000000, 0x01000000, 0x12787889, 0xe24e4000, 0xbcb48000, 0, 0, 0, 0, 0};
    const UINT8 svId = 1;

    GalileoPage<WordType::GST_UTC_CONVERSION, TrackingCode::None> gal_page_6_test{};
    GalileoNavData gal_nav_data_test{};

    gal_page_6_test.decode(word);
    gal_page_6_test.interpret(gal_nav_data_test, svId);

    EXPECT_NEAR(gal_nav_data_test.utc.A0_s_f64, 9.31322575e-10, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.utc.A1_ss_f64, 0.00000000e+00, PRECISION);
    EXPECT_EQ(gal_nav_data_test.utc.DtLS_s_f64, 18);
    EXPECT_EQ(gal_nav_data_test.utc.Tot_s_f64, 432000);
    EXPECT_EQ(gal_nav_data_test.utc.WNt_f64, 120);
    EXPECT_EQ(gal_nav_data_test.utc.WNlsf_f64, 137);
    EXPECT_EQ(gal_nav_data_test.utc.Dn_f64, 7);
    EXPECT_EQ(gal_nav_data_test.utc.DtLSF_s_f64, 18);
}

/*TEST(GalileoPageTest, WordType7E5b)
{
    const UINT32 word[10] = {0x06000000, 0x01000000, 0x12787889, 0xe24e4000, 0xbcb48000, 0, 0, 0, 0, 0};

    GalileoPage<WordType::ALMANAC_1, TrackingCode::E5b> gal_page_7_test{};
    GalileoNavData gal_nav_data_test{};

    gal_page_7_test.decode(word);
    gal_page_7_test.interpret(gal_nav_data_test);

    EXPECT_NEAR(gal_nav_data_test.utc.A0_s_f64, 9.31322575e-10, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.utc.A1_ss_f64, 0.00000000e+00, PRECISION);
    EXPECT_EQ(gal_nav_data_test.utc.DtLS_s_f64, 18);
    EXPECT_EQ(gal_nav_data_test.utc.Tot_s_f64, 432000);
    EXPECT_EQ(gal_nav_data_test.utc.WNt_f64, 120);
    EXPECT_EQ(gal_nav_data_test.utc.WNlsf_f64, 137);
    EXPECT_EQ(gal_nav_data_test.utc.Dn_f64, 7);
    EXPECT_EQ(gal_nav_data_test.utc.DtLSF_s_f64, 18);
}*/