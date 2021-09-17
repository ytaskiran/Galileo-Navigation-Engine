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
#include <stdint.h>
#include <cstdio>
#include <iostream>

#define UINT8  uint8_t  
#define UINT16 uint16_t 
#define UINT32 uint32_t
#define UINT64 uint64_t 
#define INT8  int8_t  
#define INT16 int16_t 
#define INT32 int32_t
#define INT64 int64_t 
#define FLOAT64 double
#define FLOAT32 float


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