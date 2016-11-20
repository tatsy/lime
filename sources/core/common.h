#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_COMMON_HPP_
#define _CORE_COMMON_HPP_

#include <cmath>
#include <iostream>
#include <sstream>

// -----------------------------------------------------------------------------
// API export macro
// -----------------------------------------------------------------------------

#if (defined(WIN32) || defined(_WIN32) || defined(WINCE) || defined(__CYGWIN__))
#   if defined(LIME_API_EXPORT)
#       define LIME_EXPORTS __declspec(dllexport)
#       define LIME_IMPORTS
#   else
#       define LIME_EXPORTS
#       define LIME_IMPORTS __declspec(dllimport)
#   endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#   define LIME_EXPORTS __attribute__((visibility ("default")))
#   define LIME_IMPORTS
#else
#   define LIME_EXPORTS
#   define LIME_IMPORTS
#endif

// ----------------------------------------------------------------------------
// Parameter constants
// ----------------------------------------------------------------------------

static const double PI     = 4.0 * atan(1.0);
static const double INV_PI = 1.0 / PI;
static const double INFTY  = 1.0e32;
static const double EPS    = 1.0e-6;

// ----------------------------------------------------------------------------
// Parallel for
// ----------------------------------------------------------------------------

#ifdef _OPENMP
    #include <omp.h>
    #if defined(WIN32) || defined(_WIN32)
        #define ompfor __pragma(omp parallel for) for
        #define omplock __pragma(omp critical)
    #else
        #define ompfor _Pragma("omp parallel for") for
        #define omplock _Pragma("omp critical")
    #endif
    const int kNumThread = omp_get_max_threads();
    inline int omp_thread_id() { return omp_get_thread_num(); }
#else  // _OPENMP
    #define ompfor for
    #define omplock
    const int kNumThreads = 1;
    inline int omp_thread_id() { return 0; }
#endif  // _OPENMP

// -----------------------------------------------------------------------------
// Assertion with message
// -----------------------------------------------------------------------------

#ifndef __FUNCTION_NAME__
#if defined(_WIN32) || defined(__WIN32__)
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

#undef NDEBUG
#ifdef PY_VERSION_HEX
#define Assertion(PREDICATE, ...) \
do { \
if (!(PREDICATE)) { \
std::stringstream ss; \
ss << "Asssertion \"" \
<< #PREDICATE << "\" failed in " << __FILE__ \
<< " line " << __LINE__ \
<< " in function \"" << (__FUNCTION_NAME__) << "\"" \
<< " : "; \
throw PyLimeException(ss.str()); \
} \
} while (false)
#elif !defined(NDEBUG)
#define Assertion(PREDICATE, ...) \
do { \
if (!(PREDICATE)) { \
std::cerr << "Asssertion \"" \
<< #PREDICATE << "\" failed in " << __FILE__ \
<< " line " << __LINE__ \
<< " in function \"" << (__FUNCTION_NAME__) << "\"" \
<< " : "; \
fprintf(stderr, __VA_ARGS__); \
std::cerr << std::endl; \
std::abort(); \
} \
} while (false)
#else  // NDEBUG
#define Assertion(PREDICATE, ...) do {} while (false)
#endif // NDEBUG


// -----------------------------------------------------------------------------
// Message handlers
// -----------------------------------------------------------------------------

#ifndef NDEBUG
#define InfoMsg(...) \
do { \
    std::cout << "[ INFO  ] "; \
    fprintf(stdout, __VA_ARGS__); \
    std::cerr << std::endl; \
} while (false);
#define WarnMsg(...) \
do { \
    std::cerr << "[WARNING] "; \
    fprintf(stdout, __VA_ARGS__); \
    std::cerr << std::endl; \
} while (false);
#else
#define InfoMsg(...)
#define WarnMsg(...)
#endif

#ifdef PY_VERSION_HEX
#define ErrorMsg(...) \
do { \
    char msg[512]; \
    sprintf(msg, __VA_ARGS__); \
    throw PyLimeException(msg); \
} while (false);
#else
#define ErrorMsg(...) \
do { \
    std::cerr << "[ ERROR ] "; \
    fprintf(stderr, __VA_ARGS__); \
    std::cerr << std::endl; \
    std::abort(); \
} while (false);
#endif

#endif  // _CORE_COMMON_HPP_
