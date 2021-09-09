#include "gtest/gtest.h"
#include "galileo_page.hpp"

#define PRECISION 6


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

    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].omega0_rad_f64, -2.66334, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].i0_rad_f64, 0.961456, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].omega_rad_f64, -0.433037, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].IDOT_rad_sec_f64, -1.89651e-10, PRECISION);
}

TEST(GalileoPageTest, WordType3)
{
    const UINT32 word[10] = {0x0305bff0, 0xcd48633e, 0x658431c5, 0x4abdc000, 0x97dada69, 0, 0, 0, 0, 0};
    const UINT8 svId = 1;

    GalileoPage<WordType::EPHEMERIS_3, TrackingCode::None> gal_page_3_test{};
    GalileoNavData gal_nav_data_test{};

    gal_page_3_test.decode(word);
    gal_page_3_test.interpret(gal_nav_data_test, svId);

    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].OMGdot_rad_f64, -5.55845e-09, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].deltaN_rad_sec_f64, 3.06727e-09, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Cuc_rad_f64, -3.05846e-06, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Cus_rad_f64, 8.00006e-06, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Crc_m_f64, 169.312, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].Crs_m_f64, -69.0312, PRECISION);
    EXPECT_NEAR(gal_nav_data_test.eph[svId-1].sisaIndexE1E5b, 3.12, PRECISION);
}
