/**
 * @file    : Ublox-Driver 
 * @authors : MEHMET AKSU
 * @note    : mehmet.aksu@cezeri.com
 * @date    : 07 / Mayıs / 2021
 * @code    : navigation_base.hpp file
 * @details : 
 */



#ifndef NAVIGATION_BASE_HPP
#define NAVIGATION_BASE_HPP

#include <type_traits>

#define UINT8  uint8_t  
#define UINT16 uint16_t 
#define UINT32 unsigned int
#define UINT64 uint64_t 
#define INT8  int8_t  
#define INT16 int16_t 
#define INT32 int
#define INT64 int64_t 
#define FLOAT64 double


enum class SatelliteSystem : std::uint8_t
{
    GPS           = 0,
    SBAS          = 1,
    Galileo       = 2,
    BeiDou        = 3,
    QZSS          = 5,
    Glonass       = 6,
    None          = 7,
};

enum class TrackingCode : std::int8_t
{
    L1CA      = 0, ///!< GPS L1C/A, SBAS, QZSS
    B1I_D1    = 0, ///!< BeiDou B1I D1
    L1OF      = 0, ///!< GLONASS
    E1        = 1, ///!< Galileo  E1 B
    B1I_D2    = 1, ///!< BeiDou B1I D2
    B2I_D1    = 2, ///!< BeiDou B2I D1
    L2OF      = 2, ///!< GLONASS
    GPS_L2CL  = 3, ///!< GPS L2CL
    B2I_D2    = 3, ///!< BeiDou B2I D2
    L2CM      = 4, ///!< GPS L2CM, QZSS
    E5b       = 5, ///!< Galileo
    QZSS_L2CL = 5, ///!< QZSS
    None      = -1 ///!< None
};

template<SatelliteSystem s>
struct Almanac {};

template<SatelliteSystem s>
struct Ephemeris {};

template<SatelliteSystem s>
struct UTC {};

template<SatelliteSystem s>
struct Ionosphere {};

template<SatelliteSystem s>
struct TimeOffset {};

extern inline constexpr auto Num_Gps_Satellites     = std::integral_constant<std::size_t, 32>::value;
extern inline constexpr auto Num_Glonass_Satellites = std::integral_constant<std::size_t, 24>::value;
extern inline constexpr auto Num_Galileo_Satellites = std::integral_constant<std::size_t, 36>::value;
extern inline constexpr auto Num_BeiDou_Satellites  = std::integral_constant<std::size_t, 35>::value;
extern inline constexpr auto Num_QZSS_Satellites    = std::integral_constant<std::size_t, 7>::value;
extern inline constexpr auto Num_SBAS_Satellites    = std::integral_constant<std::size_t, 22>::value;


#endif /* NAVIGATION_BASE_HPP */