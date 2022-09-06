// config file for sstest

#ifndef _SSTEST_CONFIG_H_
#define _SSTEST_CONFIG_H_


#define SSTEST_VERSION_MAJOR 0
#define SSTEST_VERSION_MINOR 1
#define SSTEST_VERSION_PATCH 0

#if defined(SSTEST_DEVELOPMENTAL)
#define SSTEST_VERSION_STR "" " (DEVELOPMENTAL)"
#else
#define SSTEST_VERSION_STR "0.1.0"
#endif

/**
 * \file sstest_config.h
 * \brief Configuration file generated by CMake, containing SSTest metadata such as version information
 * 
 */

namespace sstest
{

    struct
    {
        unsigned int major = SSTEST_VERSION_MAJOR;
        unsigned int minor = SSTEST_VERSION_MINOR;
        unsigned int patch = SSTEST_VERSION_PATCH;
    } constexpr version;

    constexpr const char* version_string = SSTEST_VERSION_STR; 

}

#endif // _SSTEST_CONFIG_H_
