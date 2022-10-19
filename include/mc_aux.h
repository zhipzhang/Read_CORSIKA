/* ============================================================================
 
   Copyright (C) 1997, 1998, ..., 2018, 2019, 2020, 2021, 2022  Konrad Bernloehr
   
   This file is part of sim_telarray (also known as sim_hessarray).

   The software in this file is free software: you can redistribute it 
   and/or modify it under the terms of the GNU General Public License 
   as published by the Free Software Foundation, either version 3 of 
   the License, or (at your option) any later version. 

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

============================================================================ */

/* ================================================================ */
/** @file mc_aux.h 
 *  @short Internal data structures and prototypes for telescope simulation.
 *
 *  This file contains definitions for internal data structures used
 *  in the telescope simulation, for dimensions of various things,
 *  and prototypes of relevant functions.
 *
 *  @author  Konrad Bernloehr 
 *  @date    1997 ... 2022
 *  @date    @verbatim CVS $Date: 2022/07/15 11:57:11 $ @endverbatim
 *  @version @verbatim CVS $Revision: 1.207 $ @endverbatim
 */
/* ================================================================ */

#ifndef _MC_AUX_LOADED

#define _MC_AUX_LOADED 1

#ifndef _MC_TEL_LOADED
# include "mc_tel.h"
#endif

#include "fileopen.h"
#include "rpolator.h"
#include "absorb.h"
#include "io_history.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------ Definitions for debugging and testing (change as required) ----- */

/* #define HESS_DEFAULTS 1 */   /* Default setup is for HESS */

#ifdef DEBUG_ALL
# define DEBUG_ADC       1
# define DEBUG_DELAY     1
//# define DEBUG_OPTIMIZE  1
# define DEBUG_PULSE_SHAPES 1
# define DEBUG_SAMPLING  1
# define DEBUG_SEQUENTIAL 1
# define DEBUG_TEST_ALL  1
# define DEBUG_TRACE_99  1
# define DEBUG_TRACE_ALL 1
# define DEBUG_TRANS     1
# define DEBUG_TRIGGER   1
# define RAYTRACING_CAMERA_BEFORE_PRIMARY 1
# define RAYTRACING_CAMERA_FRONT_AFTER_PRIMARY 1
#endif

/* #define DEBUG_TRACE_ALL 1 */
/* #define DEBUG_TRACE_5 */

#ifdef DEBUG_TRACE_ALL     /* Print lots of raytracing debugging information */
#  define DEBUG_TRACE   1
#  define DEBUG_TRACE_1 1
#  define DEBUG_TRACE_2 1
#  define DEBUG_TRACE_3 1
#  define DEBUG_TRACE_4 1
//#  define DEBUG_TRACE_5 1  /**< Checks if no mirrors are missing in 'grid' */
#  define DEBUG_TRACE_6 1
#  define DEBUG_CAMERA_HIT
#  define DEBUG_PIXEL_HIT
#endif /* DEBUG_TRACE_ALL */

/** ---------- Always enable the option to bypass the optics ------------- */

#ifndef WITH_BYPASS_OPTICS
#  define WITH_BYPASS_OPTICS 1 /* Only symbolic now, no longer tested in code */
#endif

/** ------------ Definitions for event analysis and output --------------- */

#ifndef PULSE_ANALYSIS
# define PULSE_ANALYSIS 1
#endif
#if ( PULSE_ANALYSIS != 0 )
# define WITH_PULSE_ANALYSIS 1
#endif
#define ANALYSE_SHOWER 1

/** ------------ Definitions of various array sizes and limits ----------- */

#define OVERSAMPLING 40       /**< Sampling of signal shape at different phase offsets. */
#define MAX_SHAPE_LENGTH 500  /**< Maximum length of shape in FADC bins for each offset. */

#define MAX_TRG_TYPES 4       /**< How many trigger types can be handled */
#define WITH_MAJORITY_TRG 1   /**< You better don't try to disable this one. */
#define WITH_ANALOGSUM_TRG 1
#define WITH_DIGITALSUM_TRG 1

/* Nothing special about building for CTA Prod-6. It is just like for Prod-4/5 */
/* Limit on the supported number of telescopes for now like Prod-5. May still change. */
#ifdef CTA_PROD6
# define CTA_PROD5 1
#endif
#ifdef CTA_PROD6_SC
# define CTA_PROD5_SC 1
#endif
#ifdef CTA_PROD6_DEMO
# define CTA_PROD5_DEMO 1
#endif
#ifdef CTA_MINI6
# ifndef CTA_PROD5_SC
#  define CTA_PROD5 2
# endif
# define CTA_MINI 6
#endif
#ifdef CTA_MINI6_SC
# define CTA_PROD5_SC 1
# define CTA_MINI 6
#endif

/* Versions optimized for mini-arrays (alternative: set MAXIMUM_TELESCOPES) */
#ifdef CTA_MINI2
# ifndef CTA_PROD2_SC
#  define CTA_PROD2 2
# endif
# define CTA_MINI 2
#endif
#ifdef CTA_MINI3
# ifndef CTA_PROD3_SC
#  define CTA_PROD3 2
# endif
# define CTA_MINI 3
#endif
#ifdef CTA_MINI4
# ifndef CTA_PROD4_SC
#  define CTA_PROD4 2
# endif
# define CTA_MINI 4
#endif
#ifdef CTA_MINI4_SC
# define CTA_PROD4_SC 1
# define CTA_MINI 4
#endif
#ifdef CTA_MINI5
# ifndef CTA_PROD5_SC
#  define CTA_PROD5 2
# endif
# define CTA_MINI 5
#endif
#ifdef CTA_MINI5_SC
# define CTA_PROD5_SC 1
# define CTA_MINI 5
#endif
#ifdef CTA_PROD3_DEMO
# define CTA_PROD3_SC 1
#endif
#ifdef CTA_ULTRA3
# define CTA_ULTRA 3
#endif
#ifdef CTA_ULTRA5
# define CTA_ULTRA 5
#endif
#ifdef CTA_PROD3_MERGE
# define CTA_MAX 1
#endif

/* We tried to avoid having the non-SC and the SC variants defined at
   the same time - but if we messed up, only keep the SC variant. */
#if defined CTA_PROD6 && defined CTA_PROD6_SC
# undef CTA_PROD6
#endif
#if defined CTA_PROD5 && defined CTA_PROD5_SC
# undef CTA_PROD5
#endif
#if defined CTA_PROD4 && defined CTA_PROD4_SC
# undef CTA_PROD4
#endif
#if defined CTA_PROD3 && defined CTA_PROD3_SC
# undef CTA_PROD3
#endif

#ifdef CTA_PROD5
/* Defining CTA_PROD5 is just a short-cut for CTA_PROD4 and 
   MAXIMUM_TELESCOPES=180 and not a really new configuration. */
# ifndef CTA_PROD4
#  define CTA_PROD4 CTA_PROD5
# endif
# ifndef CTA_MINI
#  ifndef MAXIMUM_TELESCOPES
#   define MAXIMUM_TELESCOPES 180
#  endif
# endif
#endif
#ifdef CTA_PROD5_SC
# ifndef CTA_PROD4_SC
#  define CTA_PROD4_SC CTA_PROD5_SC
# endif
#endif
#ifdef CTA_PROD5_DEMO
# ifndef CTA_PROD4_DEMO
#  define CTA_PROD4_DEMO CTA_PROD5_DEMO
# endif
#endif

#if defined(CTA_PROD1) || defined (CTA_ULTRA3)
# ifndef CTA_ULTRA
#  define CTA_ULTRA 3
# endif
#endif
#if defined(CTA_PROD2) || defined (CTA_ULTRA5)
# ifndef CTA_ULTRA
#  define CTA_ULTRA 5
# endif
#endif
#if defined(CTA_PROD3)
# ifndef CTA_ULTRA
#  define CTA_ULTRA 8
# endif
#endif
#if defined(CTA_PROD3_SC)
# ifndef CTA_ULTRA
#  define CTA_ULTRA 9
# endif
#endif
#if CTA_ULTRA == 5
# ifndef CTA_PROD2
#  define CTA_PROD2 1
# endif
#endif
#ifdef CTA_ULTRA              /** CTA_ULTRA implies CTA */
# ifdef CTA_PROD1
#  define SAVE_MEMORY 1
# endif
# ifdef CTA
#  undef CTA
# endif
# define CTA 2
#endif
#ifdef CTA_MAX                /** CTA_MAX also implies CTA */
# ifdef CTA
#  undef CTA
# endif
# ifdef CTA_ULTRA
#  undef CTA_ULTRA
# endif
# ifdef SAVE_MEMORY
#  undef SAVE_MEMORY
# endif
# define CTA 3
#endif
#if defined(CTA_SC) || defined(CTA_PROD2_SC) || defined(CTA_PROD3_SC)
# ifdef CTA
#  undef CTA
# endif
# ifdef CTA_PROD3_SC
#  define CTA 9
# else
#  ifdef CTA_PROD2_SC
#   define CTA 6
#  else
#   define CTA 4
#  endif
# endif
# ifdef CTA_ULTRA
#  undef CTA_ULTRA
# endif
# ifdef CTA_PROD2
#  undef CTA_PROD2
# endif
# ifdef CTA_PROD3
#  undef CTA_PROD3
# endif
#endif
#ifdef HESS_PHASE_2D
# define HESS_PHASE_2 2
#endif
#if defined(CTA) || defined(HESS_PHASE_1) || defined(HESS_PHASE_2) || defined(HESS_PHASE_3)
# ifndef HESS_DEFAULTS
#  define HESS_DEFAULTS 1     /**< Fall-back behaviour is similar to HESS (two gains, ...) */
# endif
#endif

/* Since all sorts of things are statically allocated we should take care about memory consumption */

#if defined(HESS_DEFAULTS)
# ifdef SAVE_MEMORY
#  ifndef CTA
#   ifndef HESS_PHASE_3
#    ifndef HESS_PHASE_2
#     define HESS_PHASE_1 1
#    endif
#   endif
#  endif
# endif
# ifndef HESS_PHASE_1
#  ifndef NO_LARGE_TELESCOPE
#   define LARGE_TELESCOPE 1
#  endif
# endif
#endif

/* A super-HESS telescope type with more pixels and more mirrors. */
#if defined(LARGE_TELESCOPE) && !defined(CTA)
# ifndef MAX_MIRRORS
#  define MAX_MIRRORS 1000
# endif
# ifndef MAX_PIXELS
#  define MAX_PIXELS 4095
# endif
# define PHI_ZONES 24
# define RAD_ZONES 30
#endif

#ifndef MAX_SEGMENTS
# define MAX_SEGMENTS 128
#endif

/* You should, apart from an optional CTA definition, not have defined more than one of the following: */
#if defined(CTA) || \
    defined(HESS_PHASE_3) || \
    defined(CTA_ULTRA) || \
    defined(CTA_MAX) || \
    defined(CTA_SC) || \
    defined(CTA_PROD1) || \
    defined(CTA_PROD2) || \
    defined(CTA_PROD2_SC) || \
    defined(CTA_PROD3) || \
    defined(CTA_PROD3_SC) || \
    defined(CTA_PROD3_DEMO) || \
    defined(CTA_PROD4) || \
    defined(CTA_PROD4_SC) || \
    defined(CTA_PROD4_DEMO) || \
    defined(CTA_MAX_SC)
/* Checking more systematically now */
# ifdef CTA_ULTRA
#  if (CTA_ULTRA -0 == 3) || (CTA_ULTRA -0 == 2) || (CTA_ULTRA -0 == 1) || (CTA_ULTRA -0 == 0)
#   define CTA_PROD1 1
#  elif CTA_ULTRA -0 == 5
#   ifndef CTA_PROD2
#    define CTA_PROD2 1
#   endif
#  endif
#  undef CTA_ULTRA
# endif
# ifdef CTA_KIND
#  undef CTA_KIND
# endif
# ifdef CTA_MAX_SC
#  define CTA_KIND 7
/* #  undef CTA_MAX_SC */
# endif
# ifdef CTA_PROD4
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 10
# endif
# if defined(CTA_PROD4_SC) || defined(CTA_PROD4_DEMO)
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 11
# endif
# ifdef CTA_PROD3_SC
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 9
# endif
# ifdef CTA_PROD3
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 8
# endif
# ifdef CTA_PROD2_SC
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 6
# endif
# ifdef CTA_PROD2
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 5
# endif
# ifdef CTA_SC
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 4
# endif
# ifdef CTA_MAX
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 3
# endif
# if defined(CTA_PROD1)
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 2
# endif
# ifdef HESS_PHASE_3
#  ifdef CTA_KIND
#   error "Configuration conflict - use only one of them"
#  endif
#  define CTA_KIND 1
# endif
# ifndef CTA_KIND
#  define CTA_KIND 0
# endif
# if defined(CTA_ULTRA)
#  error "Configuration conflict - CTA-ULTRA not matching prod-1 or prod-2"
#  undef CTA_ULTRA
# endif
# ifdef CTA
#  undef CTA
#  define CTA CTA_KIND
# endif

# ifdef MAX_TEL
#  undef MAX_TEL
# endif
# ifdef MAX_PIXELS
#  undef MAX_PIXELS
# endif
# ifdef MAX_MIRRORS
#  undef MAX_MIRRORS
# endif

# if CTA_KIND == 0       /* CTA and nothing else: benchmark array */
#  define MAX_TEL 100
#  define MAX_IGNORE 99
# elif CTA_KIND == 1     /* HESS_PHASE_3: 41 and 97 telescope configurations */
#  define MAX_TEL 100
#  define MAX_IGNORE 99
# elif CTA_KIND == 2     /* CTA_PROD1 and CTA_ULTRA = 1 to 4: Prod-1 and similar */
#  define MAX_TEL 275
#  define MAX_IGNORE 274
#  define SAVE_MEMORY 1
#   define MAX_PIXELS 2841
# elif CTA_KIND == 3     /* CTA_MAX: more telescopes */
#  define MAX_TEL 500
#  define MAX_IGNORE 499
#  define MAX_MIRRORS 4000
# elif CTA_KIND == 4     /* CTA_SC: fewer telescopes but more pixels */
#  define MAX_TEL 61
#  define MAX_IGNORE 60
#  if defined(CTA_SC)
#   if (CTA_SC == 3)
#    undef MAX_TEL
#    define MAX_TEL 111
#    undef MAX_IGNORE
#    define MAX_IGNORE 110
#    define MAX_PIXELS 11328
#    define NO_LOW_GAIN
#    define MAX_FADC_SIGNAL 65535
#   elif (CTA_SC == 2)
#    define MAX_PIXELS 11328
#    define NO_LOW_GAIN
#    define MAX_FADC_SIGNAL 65535
#   elif (CTA_SC == 1)
#    define MAX_PIXELS 11328
#    define MAX_FADC_SIGNAL 65535
#   else
#    define MAX_PIXELS 14336
#   endif
#  endif
# elif CTA_KIND == 5     /* CTA_PROD2: prod-2 (without SCTs) */
#  define MAX_TEL 200
#  define MAX_IGNORE 199
#  define MAX_PIXELS 2368
#  define NO_LOW_GAIN
#  define MAX_FADC_SIGNAL 65535
# elif CTA_KIND == 6     /* CTA_PROD2_SC: prod-2 with SCTs */
#  define MAX_TEL 229
#  define MAX_IGNORE 228
#  define MAX_PIXELS 11328
#  define NO_LOW_GAIN
#  define MAX_FADC_SIGNAL 65535
# elif CTA_KIND == 7    /* CTA_MAX_SC: more telescopes and pixels */
   /* Code size for 640 telescopes may need the -mcmodel=large compiler flag. */
#  define MAX_TEL 640 /* 197 + 111 + 102 = 410 for prod-2, ~640 for prod-3 */
#  define MAX_IGNORE 639
#  define MAX_MIRRORS 1000
#  define MAX_PIXELS 11328
#  define MAX_FADC_SIGNAL 65535
/* #  define NO_SIMTEL_ANALYSIS ** if ana_data structure gets too big */
# elif CTA_KIND == 8     /* CTA_PROD3: prod-3 preliminary (without SCTs) */
#  define MAX_TEL 200
#  define MAX_IGNORE 199
#  define MAX_PIXELS 2368
#  define MAX_FADC_SIGNAL 65535
# elif CTA_KIND == 9     /* CTA_PROD3_SC: prod-3 preliminary with SCTs */
#  ifdef CTA_PROD3_DEMO
#   define MAX_TEL 126
#   define MAX_IGNORE 125
#  else
#   define MAX_TEL 70
#   define MAX_IGNORE 69
#  endif
#  define MAX_PIXELS 11328
#  define MAX_FADC_SIGNAL 65535
# elif CTA_KIND == 10   /* CTA_PROD4: baseline layout */
#  define MAX_TEL 99
#  define MAX_IGNORE 98
#  define MAX_PIXELS 2368
#  define MAX_FADC_SIGNAL 65535
#  ifndef MAXIMUM_SLICES
#   define MAXIMUM_SLICES 160
#  endif
# elif CTA_KIND == 11   /* CTA_PROD4_SC or CTA_PROD4_DEMO: extended layout, with SCTs, line CTA_PROD3_DEMO */
#  define MAX_TEL 126
#  define MAX_IGNORE 125
#  define MAX_PIXELS 11328
#  define MAX_FADC_SIGNAL 65535
#  ifndef MAXIMUM_SLICES
#   define MAXIMUM_SLICES 160
#  endif
# endif

#ifdef CTA_MINI
# undef MAX_TEL 
# undef MAX_IGNORE
# define MAX_TEL 25
# define MAX_IGNORE 24
#endif

# ifndef MAX_MIRRORS
#  define MAX_MIRRORS 1000
# endif
# ifndef MAX_PIXELS
#  define MAX_PIXELS 4095
# endif

# define MAX_ARRAY_GROUPS 100  /**< Maximum number of array subsets which can form a system trigger. */

#else
# define MAX_ARRAY_GROUPS 16   /**< Maximum number of array subsets which can form a system trigger. */
#endif

#ifdef ULTRA_FINE_PIXELS
# ifdef MAX_PIXELS
#  undef MAX_PIXELS
# endif
# define MAX_PIXELS 40000
#endif

/* Defaults for HESS-type telescopes. */
#if defined(HESS_DEFAULTS)
# ifndef NO_LOW_GAIN
#  define WITH_LOW_GAIN_CHANNEL 1 /**< Has a high-gain and a low-gain channel */
# endif
# define SIXTEEN_BIT_ADC 1     /**< (F)ADC has more than 8 bits */
# ifndef MAX_TEL
#  ifdef HESS_PHASE_1
#   define MAX_TEL 4           /**< The largest no. of telescopes/array. */
#   define MAX_IGNORE 3        /**< The size of the list of telescopes to be ignored */
#  else
#   ifdef HESS_PHASE_2
#    define MAX_TEL 5          /**< The largest no. of telescopes/array. */
#    define MAX_IGNORE 4       /**< The size of the list of telescopes to be ignored */
#    if HESS_PHASE_2 == 2
#     undef MAX_FADC_SIGNAL
#     define MAX_FADC_SIGNAL 65535
#    endif
#   else
#    define MAX_TEL 16         /**< The largest no. of telescopes/array. */
#    define MAX_IGNORE 15      /**< The size of the list of telescopes to be ignored */
#   endif
#  endif
# endif
# ifndef MAX_MIRRORS
#  define MAX_MIRRORS 500      /**< Changing that requires changes in cfgitems[] */
# endif
# define DECONVOLUTION_OFF 1   /**< No FADC pulse shape deconvolution */
# ifndef MAX_FADC_SIGNAL
#  define MAX_FADC_SIGNAL 4095 /**< Largest possible FADC value (here for 12-bit) */
# endif
# if !defined(DISC_BITS_1) && !defined(DISC_BITS_2) && !defined(DISC_BITS_4) \
     && !defined(DISC_BITS_8) && !defined(DISC_BITS_16) && !defined(DISC_BITS_32)
#  define DISC_BITS_4          /**< Discriminator bin is quarter of FADC */
# endif
# define MAX_PER_CHANNEL 1     /**< Maximum no. of coupled FADC per channel. */
#elif defined(CTA_DEFAULTS)
# ifndef NO_LOW_GAIN
#  define WITH_LOW_GAIN_CHANNEL 1 /**< Has a high-gain and a low-gain channel */
# endif
# ifndef MAX_MIRRORS
#  define MAX_MIRRORS 500      /**< Changing that requires changes in cfgitems[] */
# endif
# define DECONVOLUTION_OFF 1   /**< No FADC pulse shape deconvolution */
# ifndef MAX_FADC_SIGNAL
#  define MAX_FADC_SIGNAL 4095 /**< Largest possible FADC value (here for 12-bit) */
# endif
# if !defined(DISC_BITS_1) && !defined(DISC_BITS_2) && !defined(DISC_BITS_4) \
     && !defined(DISC_BITS_8) && !defined(DISC_BITS_16) && !defined(DISC_BITS_32)
#  define DISC_BITS_4          /**< Discriminator bin is quarter of FADC */
# endif
# define MAX_PER_CHANNEL 1     /**< Maximum no. of coupled FADC per channel. */
#elif defined(HEGRA_DEFAULTS)  /* HEGRA system */
# define MAX_TEL 6             /**< The largest no. of telescopes/array. */
# define MAX_IGNORE 5          /**< The size of the list of telescopes to be ignored */
# define MAX_MIRRORS 30        /**< Changing that requires changes in cfgitems[] */
# define MAX_FADC_SIGNAL 255   /**< Largest possible FADC value */
# define MAX_PER_CHANNEL 2     /**< Maximum no. of coupled FADC per channel. */
#else
# error "Unknown configuration"
#endif /* HESS_DEFAULTS */

#ifdef MEGAPIX
# undef MAX_TEL
# define MAX_TEL 5
# undef MAX_IGNORE
# define MAX_IGNORE 4
# undef MAX_PIXELS
# define MAX_PIXELS 230000
#endif

#ifndef MAX_PIXELS
# ifdef HESS_PHASE_1
#  define MAX_PIXELS 960
# else
#  define MAX_PIXELS 1024
# endif
#endif

#ifdef SAVE_MEMORY
# if defined(CTA_PROD2) || defined (CTA_SC) || defined(CTA_PROD2_SC)
#  define MAX_FADC_BINS 50    /**< Maximum number of FADC time bins */
#  define MAX_TRIG_BINS 50    /**< Maximum number of trigger time bins (width as for FADC) */
# else
#  if defined(CTA_PROD3) || defined(CTA_PROD3_SC)
#   define MAX_FADC_BINS 80    /**< Maximum number of FADC time bins */
#   define MAX_TRIG_BINS 80    /**< Maximum number of trigger time bins (width as for FADC) */
#  else
#   define MAX_FADC_BINS 40    /**< Maximum number of FADC time bins */
#   define MAX_TRIG_BINS 40    /**< Maximum number of trigger time bins (width as for FADC) */
#  endif
# endif
#else
# define MAX_FADC_BINS 128    /**< Maximum number of FADC time bins */
# define MAX_TRIG_BINS 128    /**< Maximum number of trigger time bins (width as for FADC) */
#endif

#ifndef MAXIMUM_ARRAYS
# define MAX_ARRAY 100        /**< The largest no. of arrays to be handled (i.e. how often a CORSIKA shower is re-used) */
#else
# define MAX_ARRAY MAXIMUM_ARRAYS /**< The largest no. of arrays to be handled (i.e. how often a CORSIKA shower is re-used) */
#endif

#define MAX_PIX_TYPES 10      /**< Maximum number of different pixel types */
#ifndef MAX_INPUTS_IN_TRG
# define MAX_INPUTS_IN_TRG MAX_PIXELS /**< Maximum number of channels for each trigger group */
#endif
#define MAX_NEIGHBOURS 8      /**< Maximum number of neighbouring pixels */

#define MAX_LASER_LEVELS 10   /**< Maximum number of different laser/LED amplitudes */

#define MAX_NPAR_MIRR 20      /**< Maximum number of parameters describing a general mirror shape */

#define MAX_TRG_TYPES 4       /**< Support up to three or four different trigger types */

/** Compile-time override of the most relevant limits: */
#ifdef MAXIMUM_TELESCOPES
# undef MAX_TEL
# define MAX_TEL MAXIMUM_TELESCOPES
# undef MAX_IGNORE
# define MAX_IGNORE MAXIMUM_TELESCOPES
#endif
#ifdef MAXIMUM_PIXELS
# undef MAX_PIXELS
# define MAX_PIXELS MAXIMUM_PIXELS
#endif
#ifdef MAXIMUM_SLICES
# undef MAX_FADC_BINS
# undef MAX_TRIG_BINS
# define MAX_FADC_BINS MAXIMUM_SLICES
# define MAX_TRIG_BINS MAXIMUM_SLICES
#endif

/** Polar grid for mirror tile lookup */
#ifndef PHI_ZONES
# define PHI_ZONES 10
#endif
#ifndef RAD_ZONES
# define RAD_ZONES 12
#endif

#define PROFILE_PIXELS 5      /**< No. of pixels to be used for time profile */

#ifndef CTA_PROD2
/** The maximum number of allowed bunches per telescope. */
# ifndef MAX_BUNCHES
#  define MAX_BUNCHES 5000000  /**< Override with CORSIKA_MAX_BUNCHES env. var. */
# endif
/** The maximum number of allowed photo-electrons per telescope. */
# ifndef MAX_PHOTOELECTRONS
#  define MAX_PHOTOELECTRONS 2500000 /**< Override with MAX_PHOTOELECTRONS env. var. */
# endif
/** The maximum per pixel above which we can ignore more p.e.s in the same pixel */
# ifndef MAX_PIXEL_PHOTOELECTRONS
#  define MAX_PIXEL_PHOTOELECTRONS 25000 /**< Override with MAX_PIXEL_PHOTOELECTRONS env. var. */
# endif
#else
# define MAX_BUNCHES 8000000  /**< Prod-2 hat different limits (could not reconfigure at that time) */
# define MAX_PHOTOELECTRONS 4000000
# define MAX_PIXEL_PHOTOELECTRONS 40000
#endif

#define MAX_TELTRANS 6        /**< Function coefficients for telescope transmission */

#define MAX_LAMBDA 1000       /**< In nanometer steps. Lots of code still relying on hard-coded 1000 */

#define MAX_LONGI 1071        /**< Largest size of longitudinal distribution we can handle */

#ifndef OLD_NSB_HANDLING
# define REVISED_NSB_HANDLING
#endif

/* Choose between internal calibration events recorded with all
   telescopes in the same event (..._COMMON) or each separate (..._BY_TEL) */
#ifndef CALIB_EVENT_LOOP_COMMON
# define CALIB_EVENT_LOOP_BY_TEL 1
#endif

/** Refraction index of air as a function of height in km (0km<=h<=8km), approximation. */
#define Nair(hkm) (1.+0.0002814*exp(-0.0947982*(hkm)-0.00134614*(hkm)*(hkm)))

/* -------- Abstract data types for FADC and discriminator output --------- */

#ifdef THIRTYTWO_BIT_ADC
typedef uint32_t fadc_data_t;  //!< For any ADC type with more than 16 bits.
#else
#ifdef SIXTEEN_BIT_ADC
typedef unsigned short fadc_data_t;    //!< Sufficient for a 9- to 16-bit FADC.
#else
typedef unsigned char fadc_data_t;     //!< Sufficient for an 8-bit FADC.
#endif
#endif

// Note: use unsigned char to have discriminator outputs at 1/8th of FADC bins,
// unsigned short for 1/16th of FADC bins and unsigned int for 1/32nd of it.
/* #define DISC_BITS_PER_BIN (8*sizeof(disc_data_t)) */
#if defined(DISC_BITS_16)
typedef unsigned short disc_data_t;    //!< Bit field for discriminator output.
# define DISC_BITS_PER_BIN 16
#elif defined(DISC_BITS_32)
typedef unsigned int disc_data_t;      //!< Bit field for discriminator output.
# define DISC_BITS_PER_BIN 32
#else /* Normally 8 bits are enough: */
typedef unsigned char disc_data_t;     //!< Bit field for discriminator output.
# if defined(DISC_BITS_4)
#  define DISC_BITS_PER_BIN 4
# elif defined(DISC_BITS_2)
#  define DISC_BITS_PER_BIN 2
# elif defined(DISC_BITS_1)
#  define DISC_BITS_PER_BIN 1
# else
#  define DISC_BITS_PER_BIN 8
# endif
#endif

/* ------------------ Declaration of data structures ------------------- */

/** Telescope-specific options passed through to low-level functions. */
struct mc_tel_options
{
   double tailcut_scale; ///< In the 'tail-cut' scheme used here, we can apply
                         ///< a common scale factor to the various threshold levels
                         ///< applied.
   double min_photons;   ///< Minimum no. of photons required for full simulation.
   int min_photoelectrons; ///< Minimum no. of photoelectrons required for electronics simulation.
   int store_photoelectrons; ///< Store p.e. data if at least that number are registered (also needs (save_photons&2)).
   int output_format;    ///< See 'sim_conv2hess.c' for interpretation.
   int zero_suppression; ///< Zero-suppression mode.
   int data_reduction;   ///< Data reduction mode.
   int peak_sensing;     ///< 0: ADC sums, 1: ADC sum is peak in one slice, >=2: ADC sum is peak in sliding sum in n bins.
   int bypass_optics;    ///< Bypass or partially bypass optics ray-tracing if non-zero.
   int use_fake_trigger; ///< 0: no fake trigger, 1: fake trigger instead of trigger derived from signals.
   int fake_trigger_pe;  ///< Fake a trigger decision and time, for nth p.e. if n>0, for median if n<0 (and we have >=n p.e.).
   double fake_trg_delay;///< Time added to derived fake trigger time since start of simulation window.
};

/** Options of the simulation passed through to low-level functions. */
struct mc_options
{
   struct mc_tel_options tel_options[MAX_TEL]; ///< Telescope-specific options.

   int verbose;          ///< More verbose (>0) or more quiet (<0) output than normal.
   char *input_fname;    ///< Input file name.
   char *pe_list_fname;  ///< Output file with list of photoelectrons in
                         ///< eventio format.
   char *output_fname;   ///< Output file of simulation in whatever format.
   int new_output;       ///< Replace any existing output file rather than append, if non-zero.
   char *plot_fname;     ///< ASCII output file with tables relevant for plotting.
   char *histogram_fname;///< ASCII output file for histograms.
   char *image_fname;    ///< Name for Postscript camera images.
   double power_law;     ///< Wanted power law (e.g. -2.67).
   int only_triggered_arrays; ///< Is non-zero if only triggered arrays should
                         ///< be written to the output file.
   int only_triggered_telescopes; ///< Is non-zero if only data from triggered
                         ///< telescopes should be written to the output file.
   int save_photons;     ///< Save incoming photon bunches from CORSIKA (bit 0) and/or photo-electrons are stored (bit 1).
   int save_pe_amp;      ///< Photo-electrons are intended to be stored including their amplitude.
   int movie_flag;       ///< If time sequence should be included in images.
   int sky_is_variable;  ///< If the nightsky should be recalculated after
                         ///< the pointing is changed (useful only with stars
			 ///< but takes a lot of time).
   int ignore_nontrig;   ///< Ignore non-triggered showers/events in MC output.
   int ignore_mcdata ;   ///< Ignore all MC data in output.
   long iobuf_max;       ///< Size limit for I/O buffers (input and pass-through).
   long iobuf_output_max;///< Similar size limit for output I/O buffers.
   char *random_state;   ///< State file for random generator or 'none' or 'auto'.
   int always_with_aweight; ///< Make MCEvent output always with area weights.
   int all_wl_random;    ///< All photon bunches have random wavelength. Needed
                         ///< for special runs where primary particles are marked
                         ///< with non-zero w.l.
   int select_light;     ///< 0: all photon bunches, 1: only pos. bs, -1: only neg. bs, 2: only zero wl, 3: only non-zero wl
   int max_events;       ///< Program should stop after processing so many events.
   int max_trig_events;  ///< Program should stop after processing so many triggered events.
#ifdef EXTRA_CLOUD
   double t_cloud;       ///< Transmission of a thin cloud layer not included in transmission table.
   double h_cloud;       ///< Height a.s.l. [m] of a thin cloud layer not included in transmission table.
#endif
   double array_clock_window; ///< Hide the CORSIKA zero time by an array-wide random offset.
};

/** Per-run information on what was actually simulated. */
struct mc_run
{
   int run;              ///< Run number.
   int corsika_version;  ///< CORSIKA version number * 1000.
   time_t run_start;     ///< Date when run was started.
   int atmosphere;       ///< Model atmosphere number.
   double height;        ///< Height of observation level [m]
   double e_min;         ///< Lower limit of simulated energies [TeV]
   double e_max;         ///< Upper limit of simulated energies [TeV]
   double slope;         ///< Spectral index of power-law spectrum
   double radius;        ///< Radius within which cores are thrown at random. [m]
   double radius1;       ///< Distance parameter 1 in CSCAT [m]
   double radius2;       ///< Distance parameter 2 in CSCAT [m]
   double area;          ///< Area over which offsets may be scattered [m^2].
   int num_arrays;       ///< Number of arrays simulated.
   double theta_min;     ///< Lower limit of zenith angle [degrees]
   double theta_max;     ///< Upper limit of zenith angle [degrees]
   double phi_min;       ///< Lower limit of azimuth angle [degrees]
   double phi_max;       ///< Upper limit of azimuth angle [degrees]
   double viewcone_min;  ///< Minimum of VIEWCONE range [degrees].
   double viewcone_max;  ///< Maximum of VIEWCONE range [degrees].
   double wlen_min;      ///< Lower limit of Cherenkov wavelength range [nm]
   double wlen_max;      ///< Upper limit of Cherenkov wavelength range [nm]
   double bunchsize;     ///< Cherenkov bunch size.
   
   int num_showers;      ///< Number of showers simulated in run.
   int iact_options;     ///< Option flags in CORSIKA (VOLUMEDET etc.)
   int low_E_model;      ///< Low energy interaction model flags.
   int low_E_detail;     ///< More details on low E interaction model (version etc.)
   int high_E_model;     ///< High energy interaction model flags.
   int high_E_detail;    ///< More details on high E interaction model (version etc.)
   double bfield_bx;     ///< x' component of B field [mT].
   double bfield_bz;     ///< z component of B field [mT].
   double bfield_rot;    ///< rotation angle mag. N -> geogr. N [degrees].
   double start_depth;   ///< Atmospheric depth where primary particle started.
};

/**
   Basic parameters of the simulated showers:
   (beware: lengths in [m] and energies in [TeV] - as in HEGRA analysis)
*/
struct simulated_shower_parameters
{
   int shower;                   ///< Shower number
   int array;                    ///< Array number = shower usage number
   double energy;                ///< Shower energy [TeV]
   double azimuth;               ///< Shower direction azimuth [deg]
   double altitude;              ///< Shower direction altitude above horizon
   double xcore, ycore, zcore;   ///< Shower core position [m]
   double aweight;               ///< Area weight to be used with non-uniform
                                 ///< core-position sampling [m^2].
   double x0;       ///< Atmospheric depth where particle was started [g/cm^2].
   double h1int;    ///< Height a.s.l. of first interaction [m].
   double core_dist_3d;          ///< Distance of reference point (array center) from shower axis
   double tel_core_dist_3d[MAX_TEL];  ///< Offset of telescopes from shower axis
   int particle;                 ///< Primary particle type [CORSIKA code]
   int have_longi;  ///< Indicates if vertical profiles were found in data.
                    ///< If not, the followig numbers should be all zeroes.
   double step_longi; ///< Step size of vertical profiles [g/cm**2].
   double xmax;     ///< Depth of shower maximum from all particles [g/cm**2]
   double emax;     ///< Depth of shower maximum from positrons and electrons
   double cmax;     ///< Depth of maximum of Cherenkov light emission [g/cm**2]
   double hmax;     ///< Height of shower maximum (from xmax above) [m] a.s.l.
   double xlongi[MAX_LONGI]; ///< Vertical profile of all particles.
   double elongi[MAX_LONGI]; ///< Vertical profile of all electrons+positrons.
   double clongi[MAX_LONGI]; ///< Vertical profile of Cherenkov light emission.
};

/**
    Reconstructed parameters of the showers:
    (beware: lengths in [m] and energies in [TeV] - as in HEGRA analysis)
*/
struct reconstructed
{
   double energy;                ///< Shower energy [TeV]
   double azimuth;               ///< Shower direction azimuth [deg]
   double altitude;              ///< Shower direction altitude above horizon [deg]
   double xcore, ycore, zcore;   ///< Shower core position [m]
   double m_scwid;               ///< Mean scaled width of images [deg]
   double m_sclen;               ///< Mean scaled length of images [deg]
   double amplitude[MAX_TEL][3]; ///< Image amplitude and halo [p.e.]
   double ximg[MAX_TEL];         ///< X position of image centre in camera. [deg]
   double yimg[MAX_TEL];         ///< Y position of image centre in camera. [deg]
   double img_angle[MAX_TEL];    ///< Angle of image major axis with camera X [deg].
   double width[MAX_TEL];        ///< Image parameter 'width' [deg]
   double length[MAX_TEL];       ///< Image parameter 'length' [deg]
   double dis[MAX_TEL];          ///< Image parameter 'dis' (distance) [deg]
   double miss[MAX_TEL];         ///< Image parameter 'miss' [deg]
   double conc[MAX_TEL];         ///< Image parameter 'conc' (concentration)
   double azwidth[MAX_TEL];      ///< Image parameter 'azwidth' [deg]
   double azlength[MAX_TEL];     ///< Image parameter 'azlength' [deg]
   double alpha[MAX_TEL];        ///< Image parameter 'alpha' [deg]
   double tel_core_distance[MAX_TEL]; ///< Offset of telescopes from shower core.
   double tel_core_dist_3d[MAX_TEL];  ///< Offset of telescopes from shower axis.
   double core_distance;         ///< Offset of CT3 from shower axis.
   double core_dist_3d;          ///< Offset of CT3 from shower axis in 3D.
   double theta;                 ///< Offset of reconstructed from simulated shower
                                 ///< direction [deg].
   double rise_time[MAX_TEL];    ///< Rise time of time profile [ns].
   double time_fwhm[MAX_TEL];    ///< Full width at half maximum of profile [ns].
   int    img_pixels[MAX_TEL];   ///< Number of pixels used for image.
   int    rec_level;     ///< 0: nothing, 1: image param., 2: shower dir, ... */
};

/** Calibration parameters for a PM and its electronics channel(s): */
struct channel_calibration
{
   double pedestal_sum;                ///< Pedestal sum of all FADCs per channel.
   double sigma_pedestal_sum;          ///< Variation of that sum.
   double pedestal[MAX_PER_CHANNEL];   ///< Pedestals in individual FADCs.
   double pedestal_nsb[MAX_PER_CHANNEL];   ///< Pedestals in individual FADCs including possible DC shift by NSB.
   double sigma_pedestal[MAX_PER_CHANNEL]; ///< Variation of pedestal bin contents.
   double laser;                       ///< Laser amplitude.
   double sigma_laser;                 ///< Variation of laser amplitude.
   double laser_time;                  ///< Average time offset of laser pulses. [ns]
   double calib_rel;                   ///< Flatfielding calibration (relative to other pixels).
   double amp_to_npe;                  ///< Amplitude to #(p.e.) conversion.
#ifdef WITH_LOW_GAIN_CHANNEL
   double pedestal_sum_lg;             ///< Pedestal sum of all LG FADCs per channel.
   double sigma_pedestal_sum_lg;       ///< Variation of that sum.
   double pedestal_lg[MAX_PER_CHANNEL];///< Pedestals in individual LG FADCs.
   double pedestal_nsb_lg[MAX_PER_CHANNEL];///< Pedestals in individual LG FADCs including possible DC shift by NSB.
   double sigma_pedestal_lg[MAX_PER_CHANNEL]; ///< Variation of pedestal bin contents.
   double laser_lg;                    ///< Laser amplitude.
   double sigma_laser_lg;              ///< Variation of laser amplitude.
   double laser_time_lg;               ///< Average time offset of laser pulses. [ns]
   double calib_rel_lg;                ///< Flatfielding calibration.
   double amp_to_npe_lg;               ///< Amplitude to #(p.e.) conversion.
#endif
   int pedestal_comp_add;              ///< Value added to high-gain FADC values to achieve compensated pedestal
#ifdef WITH_LOW_GAIN_CHANNEL
   int pedestal_comp_add_lg;           ///< Value added to low-gain FADC values to achieve compensated pedestal
#endif
   double pedestal_comp_err;           ///< Error introduced in calculating the compensation for high-gain.
#ifdef WITH_LOW_GAIN_CHANNEL
   double pedestal_comp_err_lg;        ///< Error introduced in calculating the compensation for low-gain.
#endif
};

/** Electronics for one PM (including accumulated signals): */
struct pm_and_fadc_channel
{
   double qe_rel;                      ///< Quantum efficiency relative to avrg.
   double gain_rel;                    ///< Relative gain factor (common to HG+LG).
   double fadc_amplitude;              ///< In mV at peak of single photoelectron.
#ifdef WITH_LOW_GAIN_CHANNEL
   double fadc_amplitude_lg;           ///< Similar for low-gain channel (if any).
#endif
   double disc_amplitude;              ///< Same for signal at discriminator.
   double fadc_off_scale;              ///< Scaling factor for FADC offset which is
                                       ///< multiplied by background photoelectron rate to
                                       ///< achieve effective AC coupling.
   double fadc_ped_shift;              ///< Pedestal shift resulting from NSB for DC coupling.
#ifdef WITH_LOW_GAIN_CHANNEL
   double fadc_off_scale_lg;           ///< Similar for low-gain channel (if any) with AC coupling.
   double fadc_ped_shift_lg;           ///< Low-gain pedestal shift resulting from NSB for DC coupling
#endif
   double disc_off_scale;              ///< Similar for discriminator/comparator.
   double pedestal[MAX_PER_CHANNEL];   ///< Actual pedestal in FADC units.
#ifdef WITH_LOW_GAIN_CHANNEL
   double pedestal_lg[MAX_PER_CHANNEL];///< Similar for low-gain channel (if any).
#endif
   double sensitivity[MAX_PER_CHANNEL];///< ADC counts per mV of signal.
                                       ///< This is identical for high-gain and low-gain 
                                       ///< channels. H-g and l-g only differ in amplitude.
   double sensitivity_lg[MAX_PER_CHANNEL]; ///< Same for low-gain channel
   double voltage_rel;                 ///< Voltage relative to expected average.
   double transit_delay;               ///< Transit time delay of PM.
   double background;                  ///< Photoelectrons per nanosecond.
   double nightsky;                    ///< Backgr. contrib. from smooth nightsky
   double starlight;                   ///< Backgr. contrib. from list of stars.
   double nsb_pixfact;                 ///< Off-axis dependent scaling factor for NSB + star light.
   double current;                     ///< DC current [uA] due to background.
   fadc_data_t signal[MAX_FADC_BINS];  ///< Digitized FADC signal.
#ifdef WITH_LOW_GAIN_CHANNEL
   fadc_data_t signal_lg[MAX_FADC_BINS];///< Similar for low-gain channel (if any).
#endif
   int disc_output_intamp;             ///< Disc./comp. output amplitude in units of
                                       ///< percents of nominal amplitude (i.e. 100+/-10).
   // char disc_output[MAX_TRIG_BINS*DISC_BITS_PER_BIN]; ///< Only meaningful if pixel triggered.
   int disc_input_to;                  ///< If >= 0, this channel is added to another one.
   double ideal_signal;                ///< Without digitisation and background.
   double median_time;                 ///< Median time (not weighted) after PM.
   double disc_threshold;              ///< Discriminator threshold [mV].
   double min_sigsum_over_thr;         ///< Minimum integral signal over threshold
                                       ///< after the signal exceeded the threshold.
                                       ///< [disc. amp. * internal disc. time step]
   int cherenkov_pe;                   ///< Photo-electrons from Cherenkov light.
   int min_bins_over_thr;              ///< Time slices over threshold needed.
   disc_data_t trigger[MAX_TRIG_BINS]; ///< Above/below trigger at bin fraction
   int trigger_disabled;               ///< Non-zero if pixel cannot trigger.
   int triggered;                      ///< Non-zero if the pixel has triggered.
   int triggered_in_time;              ///< If trigger is coincident.
   int nn_triggered;                   ///< If pixel has NN triggered at same time.
   int gate_length;                    ///< Gate width as no. of 
                                       ///< 1/{DISC_BITS_PER_BIN}th of bins.
   int gate_strict;                    ///< Comparator (0) or discriminator style (1).
   struct channel_calibration calib;   ///< Calibration parameters.

   //! For extended configuration:
   int has_crosstalk;                  ///< Is 1 if crosstalk is relevant.
   int channel_id;                     ///< Channel no. on readout chip.
   int chip_id;                        ///< Chip no. on readout card.
   int card_id;                        ///< Card no. in readout module.
   int module_id;                      ///< Module no. in camera.
   int module_id_x;                    ///< Module ID in camera.
   int pixel_id;                       ///< Unique pixel no. in camera.
   int is_off;                         ///< Is 1 if pixel configured as off.
   int disc_sum_id;                    ///< Corresponding discriminator sum.
   int num_groups;                     ///< Number of pixel groups for trigger.
   int *to_groups;                     ///< List of groups for trigger.
   
   //! The following data is only set after the conversion to CT data format:
   int significant;                    ///< Is 1 if pixel signal is significant
                                       ///< in conversion to CT format.
   int is_in_image;                    ///< Is 1 if pixel included in image analysis.
   int overflow;                       ///< Is 1 if FADC range exceeded.
   long sum_adc;                       ///< Sum of raw ADC values in readout interval.
#ifdef WITH_LOW_GAIN_CHANNEL
   int overflow_lg;                    ///< Is 1 if low-gain FADC range exceeded.
   long sum_adc_lg;                    ///< Sum of raw low-gain ADC values in readout interval.
#endif
   int sum_bins;                       ///< Number of time bins over which we summed up.
   int pixeltrg_time_int;              ///< Sample sub-interval when pixel discriminator first fired.
                                       ///< (in DISC_BITS_PER_BIN finer sampling than FADC bins).
   double pixel_sat_coeff;             ///< Saturation coefficient is zero or 1 / number of cells in pixel.
   double peak_simple;                 ///< Simple pulse peak (max. bin).
   double peak_pp;                     ///< Pulse peak from pulse analysis.
   double peak_sc;                     ///< Similar but at time of pixel.
#ifdef WITH_PULSE_ANALYSIS
   //! Per pixel pulse analysis:
   double peak_pos;                    ///< Position of pulse peak [time slices].
   double pulse_rise[3];               ///< Position of first rise to 20%, 50%, and 80% of peak.
   double pulse_width[3];              ///< Width at 20%, 50%, and 80% of peak.
   double pulse_t_over_thr;            ///< Time over threshold.
   double pulse_sum_loc;               ///< Pulse summed up around local pixel pulse maximum.
   double pulse_sum_glob;              ///< Pulse summed up around global pulse maximum.
# ifdef WITH_LOW_GAIN_CHANNEL
   double pulse_sum_loc_lg;            ///< Pulse in low gain summed up around local pixel pulse maximum.
   double pulse_sum_glob_lg;           ///< Pulse in low gain summed up around global pulse maximum.
# endif
#endif
};

/** A collection of pixels which can form a telescope trigger condition */
struct trigger_group
{
   int trigger_mode;                   ///< 0: Discriminator/comparator majority logic,
                                       ///<    typically with second threshold applied
                                       ///<    to sum of 'logic' output signals where
                                       ///<    logic signals may have a finite rise and
                                       ///<    fall time.
                                       ///< 1: Analog sum mode, with additional shaping
                                       ///<    of 'discriminator' input pulses.
                                       ///< 2: Digital sum mode, with additional shaping
                                       ///<    applied to digitized signal.
   int num_pixels;                     ///< Number of pixels in one group.
   int *pixel;                         ///< List of pixel numbers in group (not including pre-sum pixels).

   int with_disc_sums;                 ///< Analog pixel sums before majority trigger if non-zero.
   int *num_disc_sums;                 ///< Number of discriminator sums in groups.
   int **disc_sum_from;                ///< List of pixels from where sums are formed 
                                       ///< (into matching pixel[...] entry).

   int with_fadc_presums;              ///< Digital pixel pre-sums before digital sum trigger if non-zero.
   int *num_fadc_presums;              ///< Number of pre-sums (after shaping and pre-scaling) in groups.
   int **fadc_presum_from;             ///< List of pixels from where digital pre-sums are formed .

   int num_required;                   ///< Number of pixels that must have fired.
   int *req_pixel;                     ///< Required pixels list (e.g. SmartPixel).

   int min_pixels;                     ///< Minimum no. of pixels required or 0.
   int triggered;                      ///< Number of channels with single trigger.
   int triggered_coinc;                ///< Largest number of coincident channels

   int group_triggered;                ///< Is 1 if group has coincidence trigger.
   double time;                        ///< Time of trigger (if any). [ns]
   double true_pe_amp;                 ///< Sum of true amplitudes of signal p.e. in contributing pixels [mean p.e.].
   int true_pe_count;                  ///< Number of registered signal p.e. in contributing pixels.
};

// #define DSUM_DOUBLE 1
#ifdef DSUM_DOUBLE
typedef double dsum_t;
#else
typedef int dsum_t;
#endif

/** Electronics of a whole camera and its parameters: */
struct camera_electronics
{
   int telescope;           ///< The 'official' number (ID) for the telescope.
   int itel;                ///< Internal telescope counter (starting at zero).
   int pixels;              ///< Number of pixels.
   int num_gains;           ///< One or two gains possible (if WITH_LOW_GAIN_CHANNEL).
   struct pm_and_fadc_channel *channels; // Per channel data.
   int disc_bins;           ///< Number of time bins used for discr./comparator simulation.
   int disc_start;          ///< No. of bins by which discr./comp. sim. is ahead of FADC.
   int disc_rise_steps;     ///< Time sub-steps for discr./comp. output signal rise.
   int disc_fall_steps;     ///< Time sub-steps for discr./comp. output signal fall-off.
   int use_logic_trigger;   ///< Use logic signals from disc./comp. instead of analogue outputs.
   int fadc_bins;           ///< Number of bins digitized in simulation (usually more than recorded).
   int fadc_per_channel;    ///< Number of FADCs per readout channel (each with a slower sampling,
                            ///<   phased to mimick reported sampling rate, if >1).
   int channels_per_chip;   ///< Number of channels readout per chip.
   int sum_bins;            ///< Number of bins in recorded ADC data or sum (read-out or integration window).
   int sum_offset;          ///< Number of bins before telescope trigger where summing starts.
   int sum_start;           ///< Bin where summation started for present image.
   int fadc_max_signal;     ///< Maximum value of digitized signal amplitude
   int fadc_max_sum;        ///< Maximum value of recorded signal sum
#ifdef WITH_LOW_GAIN_CHANNEL
   int fadc_max_signal_lg;  ///< Maximum value of digitized signal amplitude in low-gain channel.
   int fadc_max_sum_lg;     ///< Maximum value of recorded signal sum in low-gain channel.
#endif
   double *xspe_prompt;     ///< Random number lookup tables for single 
   double *yspe_prompt;     ///< photoelectron amplitude spectrum
                            ///< for 'prompt' pulses.
   double *xspe_bkgrnd;     ///< Same for nightsky background including ion
   double *yspe_bkgrnd;     ///< feedback.
   double spe_norm;         ///< Ratio of actually achieved mean single-p.e. signal to expected.
   int nspe_prompt;         ///< Size of preceding tables for signal
   int nspe_bkgrnd;         ///< and background.

   int use_comp_ped;        ///< Compensation of pedestal values is active (1) or not (0).
   int peak_sensing;        ///< If > 0 use peak sensing of given window size instead of ADC sum.
   int shape_length;        ///< Actual length of pulse shape in bins.
   double shape[OVERSAMPLING*MAX_SHAPE_LENGTH]; ///< Single photoelectron
                            ///< pulse shape with peak set to 1.
#ifdef WITH_LOW_GAIN_CHANNEL
   double shape_lg[OVERSAMPLING*MAX_SHAPE_LENGTH]; ///< For low-gain channel
#endif

   int bkg_shape_length;    ///< Simpler treatment for background from nightsky.
   double bkg_shape[MAX_SHAPE_LENGTH]; ///< Pulse shape from nightsky
#ifdef WITH_LOW_GAIN_CHANNEL
   double bkg_shape_lg[MAX_SHAPE_LENGTH]; ///< For low-gain channel
#endif
   double max_int_frac;     ///< Maximum fraction of pulse shape within sum_bins (not applicable to peak sensing)
   double max_pixtm_frac;   ///< Maximum fraction of pulse shape within local/global peak window (not applicable to peak sensing)
#ifdef WITH_LOW_GAIN_CHANNEL
   double max_int_frac_lg;  ///< Maximum fraction of low-gain pulse shape within sum_bins (not applicable to peak sensing)
   double max_pixtm_frac_lg;///< Maximum fraction of low-gain pulse shape within local/global peak window (not applicable to peak sensing)
#endif

   int disc_shape_length;   ///< Length of signal at discriminator 
                            ///< [# of bins * DISC_BITS_PER_BIN]
   double disc_shape[MAX_SHAPE_LENGTH]; ///< Pulse shape at discriminators

   int default_trigger_type; ///< 0: majority, 1: analog sum, 2: dig. sum
   int trigger_mode_bits;   ///< Bit mask of trigger types present in camera configuration.
   int with_analog_trigger; ///< Non-zero if any trigger groups with majority or analog sum.
   int with_majo_trigger;   ///< Non-zero if there are any trigger groups with majority trigger.
   int with_asum_trigger;   ///< Non-zero if an analog sum trigger was defined.
   int asum_shape_length;   ///< Length of signal shaped for analog sum (optional).
                            ///< [# of bins * DISC_BITS_PER_BIN] (independent of disc_shape)
   int asum_shape_offset;   ///< Offset where shaping is done, typically offset of peak.
                            ///< [# of bins * DISC_BITS_PER_BIN]
   double asum_shape[MAX_SHAPE_LENGTH];
   double asum_clipping;    ///< Clipping level for analog sums, if any.
   double asum_threshold;   ///< Threshold for analog sum trigger.

   int with_dsum_trigger;   ///< Non-zero if a digital sum trigger was defined.
   int dsum_pedsub;         ///< If non-zero, the pedestal (integer) is subtracted before shaping.
   int dsum_zero_clip;      ///< If non-zero, clip negative values after shaping at zero, before further summation.
   int dsum_prescale[2];    ///< After shaping and before clipping and possible pre-summation,
                            ///< an integer multiplication[0]/division[1] is applied as pre-scaling.
   int dsum_presum_max;     ///< Before a final bit shift the patch-wise pre-sum cannot be larger than that.
   int dsum_presum_shift;   ///< After pre-summation and clipping a (right) bit-shift may be applied.
   int dsum_shape_length;   ///< Length of signal shaped for digital sum (optional, high-gain only).
                            ///< [# of bins] (after digitization, i.e. in addition to 'shape')
   int dsum_shape_offset;   ///< Offset where shaping is done, typically offset of peak.
                            ///< [# of bins]
   dsum_t dsum_shape[MAX_SHAPE_LENGTH];
   int dsum_clipping;       ///< Clipping level for digital sums after shaping, if any. [ADC counts]
   int dsum_pre_clipping;   ///< Clipping level for digital sums before shaping, if any. [ADC counts]
   int dsum_threshold;      ///< Threshold for digital sum trigger. [ADC counts]
   int dsum_ignore_below;   ///< If > 0, use shaped clipped signal only where exceeding this value. [ADC counts]

   double frequency;        ///< FADC frequency in MHz;
   double interval;         ///< Corresponding time interval in nanoseconds.
   double trigger_delay_compensation[MAX_TRG_TYPES]; ///< Trigger type specific delay compensation [ns].
   double photon_delay;     ///< Incoming photons are delayed by so many nanoseconds.
   double fadc_noise;       ///< Dark noise / digitising noise (FADC counts).
   double fadc_noise_lg;    ///< Same for low-gain channel.
   double quantum_efficiency[MAX_LAMBDA]; ///< Q.E. times mirror reflectivity times
                            ///< camera transmission as a vector where the
                            ///< index is wavelength in steps of nanometers.
   double max_qe_rel;       ///< Maximum of relative quantum efficiencies
                            ///< of all channels.
   double collection_efficiency; ///< Photoelectron collection efficiency; is included in the quantum_efficiency
                            ///< array above for non-NSB light above but is used separately for NSB pixel rates.
   double optics_efficiency[MAX_LAMBDA]; ///< Everything before camera window, is part of 
                            ///< quantum_efficiency[] above.
   int min_wl_eff;          ///< Minimum wavelength where the telescope has any efficiency.
   int max_wl_eff;          ///< Maximum wavelength where the telescope has any efficiency.
   double lightguide_reflectivity; ///< Reflectivity of the 'funnel' before PMs.
   double transit_time_jitter; ///< Jitter of transit times for each pm [ns].
   double simple_threshold; ///< Simple 'discriminator' threshold in units of
                            ///< photoelectrons (only used when discriminator
                            ///< signals not fully simulated).
   double signal_area;      ///< Area under signal pulse shape.
#ifdef WITH_LOW_GAIN_CHANNEL
   double signal_area_lg;   ///< Area under signal pulse shape (low-gain channel).
#endif
   double bkgrnd_area;      ///< Area under background pulse shape.
#ifdef WITH_LOW_GAIN_CHANNEL
   double bkgrnd_area_lg;   ///< Area under background pulse shape (low-gain channel).
#endif
   double disc_area;        ///< Area under discriminator pulse shape.
   int fadc_ac_coupled;     ///< Is 1 if FADCs are AC coupled.
   int disc_ac_coupled;     ///< Is 1 if discriminators are AC coupled.
   double time_offset;      ///< Time offset of start of FADC memory with respect
                            ///< to shower core crossing observation level [ns].
   double nominal_delay;    ///< Nominal value of the delay of telescope trigger
                            ///< signals at the central station depending on
                            ///< 'global' alt/az and assuming a plane light front
                            ///< propagating with the speed of light in air.
   double telescope_delay;  ///< Telescope-specific delay to be added at the
                            ///< array trigger level, correcting for things like
                            ///< optical transit time (depending on focal length
                            ///< and displacement of reflector w.r.t. the centre
                            ///< of the fiducial sphere), ...
                            ///< For comparing trigger times of different telescope
                            ///< types, some calibration may be needed.
   double pedestal_sysvar;  ///< Systematic variation of pedestals.
#ifdef WITH_LOW_GAIN_CHANNEL
   double pedestal_sysvar_lg;  ///< Systematic variation of low-gain pedestals.
#endif
   double trigger_time;     ///< Time of first telescope trigger relative to
                            ///< start of FADC memory. [ns]
   double trigger_time_by_type[MAX_TRG_TYPES]; ///< Similar but separate for trigger types.
   double nom_disc_threshold; ///< Nominal discriminator threshold. [mV]
   double nom_disc_amplitude; ///< Nominal single p.e. amplitude [mV] at discriminator.
   double nom_disc_gate_length; ///< Nominal gate length. [ns]
   double nom_disc_delay;     ///< Nominal time over threshold needed. [ns]
   double nom_disc_sigsum_over_thr; ///< Nominal signal sum required over threshold. [mV*ns]
   double disc_hysteresis;  ///< Hysteresis of discr./comparator [mV].
   double pe_conversion;    ///< P.e. per (F)ADC peak ampl.
#ifdef WITH_LOW_GAIN_CHANNEL
   double pe_conversion_lg; ///< P.e. per (F)ADC peak ampl. for low gain ch.
#endif
   double trigger_current_limit; ///< Pixels are excluded from trigger if the
                            ///< current is above this threshold [uA].

   double disc_output_amplitude; ///< Output amplitude of discr./comparator [mV].
   double disc_output_var_percent; ///< By how many percent the output varies.
   double teltrig_min_time; ///< Minimum time of sector trigger over minimum [ns].
   double teltrig_min_sigsum; ///< Minimum signal sum at sector trigger
                            ///< over threshold [pV*s].
   double pixeltrg_time_step;  ///< Time step [ns] at which a fired discriminator etc. gets reported (if > 0).

   double multiplicity_offset; ///< [-0.5...0.5] offset of multiplicity
                            ///< threshold w.r.t. nominal trigger condition

   int num_trigger_groups;  ///< Number of trigger groups used (0 for NN trigger).
   struct trigger_group *trg_group; ///< List of trigger groups.
   int groups_required;     ///< Number of fired groups required for tel. trigger.
   int groups_triggered;    ///< Numer of groups triggered.

   int triggered;           ///< No. of triggered pixels.
   int nn_triggered;        ///< No. of next-neighbour triggered pixels.
   int trigger_pixels_req;  ///< No. of pixels required for telescope trigger.
   int trigger_nn_pixels_req; ///< No. of pixels with a next-neighbour required.
   int telescope_triggered; ///< Is 1 if the telescope has triggered.
   int triggered_by_type[MAX_TRG_TYPES];///< Are 1 if triggered by majority/analog sum/digital sum.
   double central_time;     ///< Time of delay-corrected central trigger time.
   double central_time_by_type[MAX_TRG_TYPES]; ///< Central time separate for trigger types.
   int simulated;           ///< Is 1 if the signal simulation was done.
   int full_simulation;     ///< Configured simulation level:
                            ///< Level 0: Only integral signals. 
                            ///< Level 1: FADC simulation but not discriminator.
                            ///< Level 2: FADC and discriminator fully simulated.
#ifdef WITH_PULSE_ANALYSIS
   int pulse_analysis;      ///< 0: no pixel pulse timing analysis, else with.
   int sum_before_peak;     ///< How many slices before peak should be included in sum.
   int sum_after_peak;      ///< How many slices after peak should be included in sum.
   double peak_frac[3];     ///< Fractions of peak at which rise and fall get evaluated.
   double global_peak_pos;  ///< Position of peak in pulse sum of significant pixels or
                            ///<   weighted mean of peak positions of sign. pixels.
#endif
   int laser_calib_done;    ///< If flatfielding calibration done.
   int laser_external_trigger; ///< Is 1 if the laser events should assume an external trigger.
                               ///< Use this at least for low intensity like single-p.e. calibration.
   double laser_photons[MAX_LASER_LEVELS];
                            ///< Mean number of laser photons per pixel.
   double laser_photons_var[MAX_LASER_LEVELS];
                            ///< Common relative fluctuation of photons per laser shot.
   double laser_pulse_offset;  ///< Time offset [ns] of laser pulse in readout window (external trigger).
   double laser_pulse_exptime; ///< Laser pulse exponential decay time [ns].
   double laser_pulse_sigtime; ///< Laser pulse duration gaussian shape (r.m.s.) [ns].
   double laser_pulse_twidth;  ///< Laser pulse duration top-hat shape (full width) [ns].
   double laser_wavelength; ///< Wavelength [nm] at which 'laser' emits light.
   double led_photons;      ///< Mean number of LED photons per pixel.
   double led_photons_var;  ///< Common relative fluctuation of photons per led event.
   double led_pulse_offset; ///< Time offset [ns] of LED pulses in readout window.
   double led_pulse_sigtime;///< LED pulse duration gaussian shape (r.m.s.) [ns].
   double median_time;      ///< Median arrival time of all photons in camera
                            ///< with respect to shower core at obs. level. [ns]
   double phase_delay;      ///< Random, ([-0.5:0.5[)*fadc_per_channel, delay as part of fadc_delay.
   double fadc_delay;       ///< Delay added, taking into account of random
                            ///< FADC phase at time of arrival. [ns]
   double photons_all;      ///< All photons hitting telescope fiducial sphere.
   double photons_atm;      ///< Photons surviving atmospheric transmission.
   double photons_atm_300_550;  ///< Photons surv. atm. tr. in the 300 to 550 nm range.
   double photons_atm_300_600;  ///< Photons surv. atm. tr. in the 300 to 600 nm range.
   double photons_atm_300_650;  ///< Photons surv. atm. tr. in the 300 to 650 nm range.
   double photons_atm_400;  ///< Photons surv. atm. tr. in the 350 to 450 nm range.
   double photons_atm_qe;   ///< Photons surviving atmospheric transmission,
                            ///< mirror reflectivity (except funnel), and max. Q.E. 
   double photons_tel_300_550;  ///< Representing (but not actually counting) the number
                            ///< of photons (before optics) in the 300 to 550 nm range hitting pixels.
   double photons_cam_300_550;  ///< Representing (but not actually counting) the number
                            ///< of photons (after optics) in the 300 to 550 nm range hitting pixels.
   double photons_pix_300_550;  ///< Representing photons hitting pixels just before applying QE.
   double photons_signal_ideal; ///< Sum of ideal signal from photons detected (photo-electrons).
   int photons_detected; ///< Number of photons detected (photo-electrons).
   int significant_pixels;  ///< No. of pixels above zero-suppression cut.
   long amplitude_histogram[356];
#ifdef WITH_LOW_GAIN_CHANNEL
   long amplitude_histogram_lg[356];
#endif
   double time_profile[MAX_FADC_BINS];  ///< Filled for each event with the 
                            ///< n hottest non-overflow significant pixels
                            ///< (n <= PROFILE_PIXELS).
   double rise_time;        ///< Rise time of time profile [ns].
   double time_fwhm;        ///< Full width at half maximum of profile [ns].
   double ref_fadc_amp;     ///< Reference (F)ADC amplitude.
#ifdef WITH_LOW_GAIN_CHANNEL
   double ref_fadc_amp_lg;  ///< Reference (F)ADC amplitude for low-gain channel.
#endif
   double ref_disc_amp;     ///< Reference ampl. at discriminator/comparator.
   double ref_pm_gain;      ///< Reference PM gain factor.
   double ref_fadc_pedestal;    ///< Reference high-gain channel pedestal
#ifdef WITH_LOW_GAIN_CHANNEL
   double ref_fadc_pedestal_lg; ///< Reference low-gain channel pedestal
#endif
   int ref_fadc_comp_pedestal;  ///< Reference compensated high-gain channel pedestal
#ifdef WITH_LOW_GAIN_CHANNEL
   int ref_fadc_comp_pedestal_lg;;  ///< Reference compensated low-gain channel pedestal
#endif
   double ref_fadc_comp_pedestal_err;  ///< Error for compensated high-gain channel pedestal
#ifdef WITH_LOW_GAIN_CHANNEL
   double ref_fadc_comp_pedestal_err_lg;;  ///< Error for compensated low-gain channel pedestal
#endif

#ifdef ADDITIONAL_AFTERPULSING
   double extra_ap_fraction;///< Afterpulse fraction above amplitude 0.
   double extra_ap_scale;   ///< Amplitude scale of exponential distribution [p.e.].
   double extra_ap_max;     ///< Max. possible afterpulse amplitude (cut-off) [p.e.].
#endif

#ifdef WITH_AUX_TRACES
   /* Recording of auxilliary traces for debugging and demonstration purposes; not routinely needed. */
   fadc_data_t *faux;         ///< Auxilliary FADC-type data, e.g. shaped sum trigger trace with dsum.
                              ///< Note: Currently no auxilliary data foreseen as derived from low-gain channels.
   size_t nfaux, lfaux;       ///< Number and length of faux traces.
   disc_data_t *aaux[4];      ///< 0: Auxilliary analog data type 1, e.g. pixel input to majority/asum triggers.
                              ///< 1: Auxilliary analog data type 2, e.g. analog sum signal.
                              ///< 2: Auxilliary analog data type 3, e.g. discriminator/comparator output.
                              ///< 3: Auxilliary analog data type 4, e.g. sum of disc/comp. outputs in trg group.
   size_t naaux[4], laaux[4]; ///< Number and length of corresponding aaux traces.
   uint32_t aux_data_switch;  ///< Bit 0: enable faux, bits 4/5/6/7: enable aaux1/2/3/4.
#endif
};

/** Structure for rectangular grid used for accelerated pixel search */
struct PM_GridList
{
   int num_pm;              ///< Number of pixels touching grid element.
   int i_pm;                ///< Temporary index.
   int *list;               ///< List of pixel numbers.
};
struct PM_Grid
{
   double x_low, x_high;    ///< Lower and upper boundary in x [cm]
   double y_low, y_high;    ///< Lower and upper boundary in y [cm]
   double dxm1, dym1;       ///< nx/(x_high-x_low) for index calculation
   int nx, ny;              ///< Number of grid elements in each dimension
   struct PM_GridList *field; ///< Which pixels touch that grid element
};

/** List of pixel types. */
struct Pix_Type
{
   int is_defined;         ///< Is 1 after the pixel type was properly defined.
   double r;                ///< Radius fully enclosing pixel [cm]
   double r2;               ///< Radius squared [cm**2]
   int pixel_shape;         ///< 0: circular, 1: hexagonal flat x, 2: square,
                            ///< 3: hexagonal flat y.
   int pm_type;             ///< We can have different PM types in one camera.
   int cathode_shape;       ///< Shape of free cathode area, as pixel_shape.
   double half_size;        ///< Half flat-to-flat size [cm] if hex. or square.
   double pixel_depth;      ///< Depth of PM below pixel entrance [cm].
   double pixel_cathode_r;  ///< Visible cathode radius [cm].
   double pixel_cathode_r2; ///< Square of visible cathode radius [cm**2].
   double pixel_cathode_hs; ///< Half flat-to-flat size [cm] if hex. or square.
   // Without a funnel table the following data is used:
   double reflectivity;     ///< Reflectivity of the lightguide which is
                            ///< assumed as wavelength independent.
   double transparency;     ///< Transparency of funnel system for photons
                            ///< directly hitting the PM.
   // When a funnel table is available, it is interpolated:
   int funnel_angle_filled;
   int funnel_wl_filled;
   double funnel_angle_table[1000]; ///< Effective funnel transparency as a
                                   ///< function of 0<tan(theta)<10.
   double funnel_wl_table[MAX_LAMBDA];  ///< Effective funnel transparency(wavelength)
                                 ///< multiplied with angle value.
   int angle_table_size;        ///< Used to check if index is in range.
//   RpolTable *funnel_eff_2d;    ///< If we have a 2-D (wl,angle) funnel efficiency table.
   int pixel_cells;         ///< For SiPM, if saturation by cells already hit is to be taken into account.
};

/** List of photomultipliers for custom camera */
struct PM_List
{
   double x, y, z;          ///< Position of pixel centre [cm]
   double cx, sx;           ///< Pixel rotation for radial projection.
//   double cz;               ///< Projection factor for inclined pixel (cosine of angle with optical axis)
//   double sr;               ///< Sine of angle in radial projection
//   double st;               ///< Sine of angle in tangential projection
   double nx, ny;           /// Normal vector is (nx,ny,1) as for focal surface.
   double axx, axy, axz, ayx, ayy, ayz, azx, azy, azz; ///< Coefficients for affine transformation with inclined pixels.
   int pix_type;            ///< Pixel type
   int module;              ///< Pixel module (drawer) number.
   double dz;               ///< Manually set displacement along z axis w.r.t. focal surface.
   double rot;              ///< Manually set rotation angle around normal vector.
};

/** Some parameters for image plots. */
struct camera_image_plot_param
{
   double range;            ///< Dynamic range scale [p.e.]
   double gamma;            ///< Gamma correction coefficient
};

/** Geometric and optical parameters of a camera */
struct pm_camera
{
   int telescope;           ///< The 'official' number (ID) for the telescope.
   int itel;                ///< Internal telescope counter (starting at zero).
   int camera_type;         ///< 1: hexagonal, 2: square, 3: custom
   int pixels;              ///< Number of pixels in the camera.
   int min_pix_type;        ///< Lowest pixel type used.
   int max_pix_type;        ///< Highest pixel type used.
   struct Pix_Type pixtype[MAX_PIX_TYPES]; ///< Definition of pixel types.
   struct PM_List *pm_list; ///< List of individual PMs (if custom) or NULL.
   struct PM_Grid pm_grid;  ///< for accelerated pixel search with custom camera.
   double camera_body_diameter; ///< Diameter of camera body for shadowing [cm]
   // If pm_list is NULL then the following applies:
   double pixel_size;       ///< Flat-to-flat 'size' (=separation) of pixels [cm].
   double pixel_depth;      ///< Depth of PM below pixel entrance [cm].
   double pixel_x_pos[MAX_PIXELS];  ///< X position of pixel [cm] as seen from
                                    ///< the camera front, X->north in zenith
                                    ///< with telescope at azimuth=0.
   double pixel_y_pos[MAX_PIXELS];  ///< Y position of pixel [cm].
   int camera_pixel_assignment_initialized; ///< Is 1 if initialisation is done.
   int square_camera_pixel_rows;    ///< No. of columns/rows of square camera.
   double pixel_cathode_r_squared;  ///< Square of visible cathode radius [cm**2].
   double pixel_lightguide_extend;  ///< Fraction of pixel 'size' to which the
                                    ///< lightguide is working; the rest is dead.
   double lightguide_reflectivity;  ///< Reflectivity of the lightguide which is
                                    ///< assumed as wavelength independent.
   struct camera_image_plot_param img; ///< Image plot parameters.
   double cam_rot;                  ///< Camera rotation angle (as in telescope optics).
   int curved_surface;              ///< Is 1 if the pixels are not in a focal plane.
   int pixels_parallel;             ///< Is 1 if the pixels look along the optical axis.
   int module_parallel;             ///< Is 1 if the pixels in a module (drawer) are looking in the same direction.
   double filter_trans[MAX_LAMBDA]; ///< 1-D (or max. of 2-D) filter transmission table for quick access.
   int with_filter_2d;              ///< Short-hand flag for having to use the 2-D table (0: no, 1: yes).
   RpolTable *filter_trans_2d;      ///< If we have a 2-D (wl,angle) transmission table.
   RpolTable *rpt_qe;               ///< Keep link to quantum efficiency table for reference only.
};

/** Coordinate shift and rotation: */
struct transform_struct
{
   double offset[3];                ///< Offset between coordinate frames (subtracted before forward rotation).
   double rot[3][3];                ///< Rotation matrix (forward: first index is column, second is row).
};

/** Transformation with non-intersecting rotation axes: */
struct transform_off_struct
{
   double offset0[3];               ///< Offset before start of rotation.
   double rot1[3][3];               ///< Rotation matrix for first rotation.
   double offset1[3];               ///< Offset between rotations.
   double rot2[3][3];               ///< Rotation matrix for second rotation.
   double offset2[3];               ///< Offset after rotations.
   int simple;                      ///< No offset in use if simple==1.
   struct transform_struct strmat;  ///< Simple transformation matrix for no-offset case (=rot2*rot1).
};

/** Parameters for a telescope mirror: */
struct mirror_struct
{
   double x;                        ///< X position of mirror centre [cm]
                                    ///< with respect to telescope optics frame
                                    ///< (where (x,y)=(0,0) defines optical axis).
   double y;                        ///< Y position of mirror centre [cm]
   double z;                        ///< Z position of mirror centre [cm]
   double r;                        ///< Radius of curvature [cm]
   double f;                        ///< Focal length [cm]
   double d;                        ///< Mirror diameter [cm]
   int type;                        ///< 0: circ., 1,2: hex (flat-> x,y)
   double distance;                 ///< Distance from camera centre [cm]
   double inclination;              ///< Inclination of mirror to tel. axis.
   double phi;                      ///< Azimuth angle of inclined mirror.
   struct transform_struct trans;   ///< Telescope optics <-> mirror transformation.
};
typedef struct mirror_struct Mirror;

/** Vector (2-D) with starting point and direction */
struct vector_xy_struct
{
   double x;                        ///< X positions of polyon corners. [cm]
   double y;                        ///< Y positions of polyon corners. [cm]
   double cx;                       ///< X direction to next corner. [cm]
   double cy;                       ///< Y direction to next corner. [cm]
};
typedef struct vector_xy_struct VectorXY;

/** A convex polygon with no positions showing up twice, except first=last, and
    all turns being right turns or all turns being left turns (no zero or 180 deg turns). */
struct convex_polygon
{
   int rtp;                         ///< Right turns in polygon (clock-wise): 1, left turns: -1.
   size_t np;                       ///< Number of points for polygon, not counting the last one being the same as the first.
   VectorXY *v;                     ///< All the corners from first point back to first=last point.
   double xc, yc;                   ///< Assumed center of polygon [cm].
   double r2max;                    ///< Maximum distance squared of corners from center [cm^2].
};
typedef struct convex_polygon ConvexPolygon;

/** Segmentation of primary and secondary in dual mirror optics. Each set 
    can cover one or multiple segments. */
struct mirror_segmentation
{
   int type;                        ///< 0: circles, 1: hexagons, 2: squares, 4: ring segments, 5: convex polygon
   int nseg;                        ///< Number of segments in this set
   int first;                       ///< Segment ID of first segment in the set.
   double phi0;                     ///< Start of first segment (ring) or rotation angle of segment (hex/sq). [radians]
   double dphi;                     ///< The step in orientation angle from one segment of the set to the next (ring only). [radians]
   double rc;                       ///< Distance of segment center from telescope axis. [cm]
   double xc, yc, zc;               ///< Position of the center of the first segment (yc=zc=0 for ring). [cm]
   double *xcr, *ycr;               ///< Individual rotated centers for ring segments only (all with the same zc). [cm]
   double diameter;                 ///< Diameter of the segment [cm]. For types 0,1,2 on surface, for type 4 in projection.
   double radius;                   ///< Half of diameter. [cm]
   double rmin, rmax;               ///< Minimum and maximum radius (type 4 only). [cm]
   double r2max;                    ///< Maximum radius squared in projection for detailed processing 
                                    ///< (1.0/1.5/2.0*radius^2, depending on type), including some margin.
   double gap;                      ///< Gap between segments (ring segments in one set only) [cm]
   double ci, si;                   ///< Cosine and sine of inclination angle, filled at first use to reduce evaluations of polynomial derivative.
   double cp, sp;                   ///< Cosine and sine of rotation angle (phi).
   double cx, sx;                   ///< Cosine and sine of position angle.
   double axx, axy, axz, ayx, ayy, ayz;  ///< Affine transformation coefficients, instead of four explicit rotations.
   
   ConvexPolygon *cvxp;             ///< For calculating hits with polygon-shaped mirror segments.

   struct transform_off_struct *segment_trafo; ///< Transformation between ideally aligned and misaligned segment (separate for each of nseg pieces).
};

struct mirror_segment_map
{
   int segment_set;                 ///< Index of the corresponding mirror_segmentation structure.
   int iseg;                        ///< Which of its nseg segments is being addressed?
};

/** Parameters for the optics of a telescope (except the camera): */
struct telescope_optics
{
   int telescope;                      ///< The 'official' number (ID) for the telescope.
   int itel;                  	       ///< Internal telescope counter (starting at zero).
   struct transform_off_struct tel_trans;  ///< Ground <-> telescope optics transformation. May or may not include offsets between axes.
   struct transform_struct cam_trans;  ///< Telescope optics <-> camera transformation.
   Mirror mirror_setup[MAX_MIRRORS];   ///< Setup/alignment of individual mirrors.
   int mirrors;                        ///< No. of mirrors present.
   int num_teltrans;                   ///< No. of coefficients used in expanding telescope transmission.
   int method_teltrans;                ///< Method to be used for expanding telescope transmission (0: constant)
   int camera_body_shape;              ///< Shape: 0: circ. 1,3: hex, 2: square; for shadowing only.
   double camera_body_diameter;        ///< Outer diameter of camera body, used for shadowing. [cm]
   double camera_body_radius;          ///< Half of outer diameter [cm]
   double camera_body_radius_squared;  ///< Square of half of outer diameter [cm]
   double camera_depth;                ///< Depth of camera body, used for shadowing. [cm]
   double camera_transmission;         ///< For bookkeeping only. Multiplied into quantum efficiency.
   double telescope_transmission[MAX_TELTRANS]; ///< Account for shadowing by masts etc. (constant or function in tan(off-axis angle)).
   double max_teltrans;                ///< Maximum transmission value over the field-of-view.
   double mirror_area;                 ///< Total area of individual mirrors corrected
                                       ///< for inclination [cm^2]. Not accounting for any shadowing.
   double mirror_diameter;             ///< Diameter of optics / primary mirror. May have some margin for segment corners.
   double focal_length;                ///< Nominal focal length of mirrors and the whole reflector. [cm]
   double focus_offset;                ///< Since we may be focusing on showers
                                       ///< at finite distances the entrance of
				       ///< the lightguides can be behind the
				       ///< nominal focus point. [cm]
   double overall_offset;              ///< Offset of the centre of the point
                                       ///< where the optical axis intersects the
                                       ///< sphere of mirror centres with respect
                                       ///< to the fixed pos. of the mount. [cm]
				       ///< Has no impact on mirror-camera distance.
   double az_alt_offset;               ///< Offset between azimuth axis and altitude axis. [cm]
   double alt_optics_offset;           ///< Offset between altitude axis and optical axis. [cm]
   double effective_focal_length;      ///< Effective focal length of mirrors for focal plane (projection) image scale. [cm]
   double effective_focal_length_x;    ///< Scale for x projection may differ if mirror not rotationally symmetric. [cm]
   double effective_focal_length_y;    ///< Scale for y projection may differ if mirror not rotationally symmetric. [cm]
   double effective_focal_length_dx;   ///< Asymmetric mirror may result in effective image displacement in x. [cm]
   double effective_focal_length_dy;   ///< Asymmetric mirror may result in effective image displacement in y. [cm]
   double assumed_focal_length;        ///< If the real effective one is not known, we may have a guess, for internal use only. [cm]
   double dslen;                       ///< Dish shape length [cm]. Resulting central curvature radius depends on parabolic or not (2x for par.)
   double mflen;                       ///< Mirror focal length (0: individually adapted) [cm].
   double mirror_flen_grading;         ///< Center-to-edge grading of mirror focal lengths [cm]
   double mirror_flen_random[2];       ///< Gaussian r.m.s. and top-hat component of random mirror focal lengths. [cm][cm]
   double mirror_rnd_ref_angle;        ///< Microscopic random reflection angle (first or only component). Now: [radians] (was [deg])
   double mirror_rnd_ref_angle2;       ///< Microscopic random reflection angle (optional second component). Now: [degrees] (was [deg])
   double mirror_rnd_ref_frac2;        ///< Fraction of reflection cases following the second component.
   double mirror_rnd_align_h;          ///< Effective horizontal mirror alignment accuracy at given zenith angle. [radians]
   double mirror_rnd_align_v;          ///< Effective vertical mirror alignment accuracy at given zenith angle. [radians]
   double mirror_rnd_distance;         ///< Random component of distance of mirror from focus. [cm]
   double mirror_dc_opt[3];            ///< Parameters which may (or may not) improve overall PSF with DC optics. (Deprecated)
   double azimuth;                     ///< Azimuth angle of tel. orientation. [degrees]
   double azimuth_nom;                 ///< Nominal value of azimuth. [degrees]
   double azimuth_basic;               ///< Azimuth angle without random setting of direction. [degrees]
   double altitude;                    ///< Altitude angle of tel. orientation. [degrees]
   double altitude_nom;                ///< Nominal value of altitude. [degrees]
   double altitude_basic;              ///< Altitude angle without random setting of direction. [degrees]
   double random_angle;                ///< Known random angle in each axis. [degrees]
   double random_error;                ///< Unknown random angle in each axis. [degrees]
   int reverse_mode;                   ///< Rev. or normal mode of alt-az mount.
   int *mirror_zone_link[PHI_ZONES][RAD_ZONES];///< Assign each mirror to one or
   int mirrors_in_zone[PHI_ZONES][RAD_ZONES];  ///< several zones in azimuth and
   double rad_zone_width;                      ///< radius to speed up raytracing.
   int parabolic_dish;                 ///< 0: Davies-Cotton, 1: parabolic base shape (for mirror class=0 only)
   int mirror_class;                   ///< 0: standard segmented primary of DC or parabolic shape,
                                       ///< 1: single solid parabolic primary,
                                       ///< 2: general primary + secondary optics (optionally segmented) + curved focal surface.
   int npar_primary;                   ///< No. of parameters describing a general primary. Only for mirror_class==2.
   int npar_secondary;                 ///< No. of parameters describing a general secondary. Only for mirror_class==2.
   int npar_focal;                     ///< No. of parameters describing a general focal surface. Only for mirror_class==2.
   double primary_parameters[MAX_NPAR_MIRR]; ///< Parameters describing a general primary. Only for mirror_class==2.
   double secondary_parameters[MAX_NPAR_MIRR]; ///< Parameters describing a general secondary. Only for mirror_class==2.
   double focal_surface_parameters[MAX_NPAR_MIRR]; ///< Parameters describing the shape of the focal surface. Only for mirror_class==2.
   double primary_hole;                ///< Diameter of a hole in the centre of the primary
                                       ///< mirror (mirror_class==2 only). Unit: cm.
   double secondary_hole;              ///< Diameter of a hole in the centre of the secondary
                                       ///< mirror (mirror_class==2 only). Unit: cm.
   double primary_offset;              ///< Any additional offset of the primary mirror from the common optics
                                       ///< reference point (on top of mirror offset, i.e. not really needed). Only for mirror_class==2.
   double secondary_offset;            ///< Offset of the secondary mirror from the common optics reference point. Only for mirror_class==2.
   double camera_offset;               ///< Offset of the camera front from the common optics reference point. Only for mirror_class==2.
   double secondary_diameter;          ///< Diameter of secondary mirror. May include mount in addition to pure mirror, for shadowing.
   double secondary_shadow_diameter;   ///< Diameter of secondary mirror. May include mount in addition to pure mirror, for shadowing.
   double secondary_shadow_radius_squared; ///< Square of shadowing radius of secondary. Only for mirror_class==2.
   double secondary_shadow_offset;     ///< z position of secondary shadowing element (0: to be initialized from polynomial). [cm]
   double secondary_baffle[6];         ///< z1, z2, r1, dr, r2, c parameters of a baffle around the secondary mirror. Only for mirror_class==2.
   double camera_rotation;             ///< Camera rotation angle.
   double cos_cam_rot;                 ///< Cosine of rotation angle.
   double sin_cam_rot;                 ///< Sine of rotation angle.
   int num_primary_segment_sets;       ///< Number of segmentation sets of the primary in dual mirror optics. Only for mirror_class==2.
   int num_primary_segments;           ///< Total number of primary segments, as ring segment sets can account for more than one segment.
   int num_secondary_segment_sets;     ///< Number of segmentation sets of the secondary in dual mirror optics. Only for mirror_class==2.
   int num_secondary_segments;         ///< Total number of secondary segments, as ring segment sets can account for more than one segment.
   int have_secondary_baffle;          ///< Flag for presence of a baffle around the secondary mirror (0: no baffle, 1: with cylindrical baffle).
   struct mirror_segmentation primary_segment_set[MAX_SEGMENTS]; ///< Only for mirror_class==2.
   struct mirror_segmentation secondary_segment_set[MAX_SEGMENTS]; ///< Only for mirror_class==2.
   struct mirror_segment_map *primary_segment_map;
   struct mirror_segment_map *secondary_segment_map;
   int with_prim_segm_misaligned;      ///< Is 1 if misalignment for segments of the primary has been set up.
   int with_sec_segm_misaligned;       ///< Is 1 if misalignment for segments of the secondary has been set up.
   int with_mirror_ref_2d;             ///< Short-hand flag for having to use 2-D with (primary) mirror reflection (0: no, 1: yes).
   int with_mirror2_ref_2d;            ///< Short-hand flag for having to use 2-D with secondary mirror reflection (0: no, 1: yes).
   RpolTable *mirror_ref_2d;           ///< If we have a 2-D (wl,angle) primary mirror reflectivity table.
   RpolTable *mirror2_ref_2d;          ///< If we have a 2-D (wl,angle) secondary mirror reflectivity table.
   double mirror_degraded_reflection;  ///< Need this separately for NSB p.e. rate re-calculation. Intended for primary.
   double mirror2_degraded_reflection; ///< Need this separately for NSB p.e. rate re-calculation. Intended for seconday.
   double camera_degraded_efficiency;  ///< Need this separately for NSB p.e. rate re-calculation. Intended for camera.
   RpolTable *mirror1_degraded_map;    ///< Map of additional degradation of reflection on surface of primary mirror.
   RpolTable *mirror2_degraded_map;    ///< Map of additional degradation of reflection on surface of secondary mirror.
   RpolTable *camera_degraded_map;     ///< Map of additional degradation of camera (assumed in focal surface).
   double lens_refidx_nominal;         ///< Nominal index of refraction of the material of a Fresnel lens, only for mirror_class==3.
   RpolTable *rpt_lens_refidx;         ///< Wavelength dependency of lens index of refraction, only for mirror_class==3.
   RpolTable *nsb_sky_map;             ///< NSB background scaling factor (az,alt) or NULL. May be copy of array-wide map.
   double optical_depth_tel[MAX_LAMBDA]; ///< Atmospheric transmission optical depth for vertical path
                                       ///< from center of telescope sphere to nominal level of atmtrans table.
   double optical_depth_focus[MAX_LAMBDA]; ///< Atmospheric transmission optical depth for a typical path
                                       ///< of photons within the telescope (order of focal length for 1M type telescope).
   double pathlen_tel;                 ///< Optical pathlength assumed for in-telescope atmospheric transmission.

   double convergent_distance;         ///< Distance from reference position [m] for convergence.
   double convergent_height;           ///< Height above observation plane [m] for convergence.
   double convergent_depth;            ///< It may as well be specified in terms of atmospheric depth;
};

/** A collection of telescopes which can form an array trigger condition */
struct telescope_array_group
{
   int num_telescopes;                 ///< Number of telescopes in one group.
   int *telescope;                     ///< List of telescope IDs in group.

   int num_required;                   ///< Number of specific telescope that must have fired.
   int *req_telescope;                 ///< Required telescopes list.
   int trg_type_required;              ///< -1: any, 0: majority, 1: an. sum, 2: dig. sum , 3: dig. majority (not yet supported)
   int hard_stereo;                    ///< 0: readout also if other group triggered, 1: only readout if this group triggered.

   double trigger_window;              ///< Group-specific trigger window (optional) [ns]
   double min_sep;                     ///< Minimum separation between two telescopes both contributing to a trigger [cm]

   int min_telescopes;                 ///< Minimum no. of telescopes required or 0.
   int triggered;                      ///< Number of telescopes with telescope trigger.
   int has_triggered;                  ///< Is 1 if group has a coincidence trigger.
};

/**
   Parameters relevant for the whole array of telescopes:
   Actually some refer to ONE array (usually the one presently simulated),
   some to ANY array (the layout), and some to ALL arrays (random shifts). 
*/
struct telescope_array
{
   double longitude;           ///< Geographic longitude (positive towards east) [deg]
   double latitude;            ///< Geographic latitude (positive towards north) [deg]
   double obs_height;          ///< Height of observation level [cm] from shower simulation
   double atm_height;          ///< Height assumed for atmospheric transmission [cm]
   double refpos[3];           ///< Reference position with respect to obs. level [cm]
                               ///< or divergent pointing w.r.t. nominal array viewing direction.
   int ntel;                   ///< Number of telescopes simulated per array
   int max_tel;                ///< Maximum number of telescopes acceptable
   int narray;                 ///< Number of arrays with random shifts per shower
   double xtel[MAX_TEL];       ///< X positions of telescopes ([cm] -> north)
   double ytel[MAX_TEL];       ///< Y positions of telescopes ([cm] -> west)
   double ztel[MAX_TEL];       ///< Z positions of telescopes ([cm] -> up)
   double rtel[MAX_TEL];       ///< Radius of spheres enclosing telescopes [cm]
   double stel[MAX_TEL];       ///< Separation from nearest other telescope [cm]
   double toff;                ///< Time offset from first interaction to the moment
                               ///< when the extrapolated primary flying with the vacuum
                               ///< speed of light would be at the observation level.
   double xoff[MAX_ARRAY];     ///< X offsets of the randomly shifted arrays [cm]
   double yoff[MAX_ARRAY];     ///< Y offsets of the randomly shifted arrays [cm]
   double aweight[MAX_ARRAY];  ///< Area weight for non-uniformly distributed core offsets
                               ///< (may be 0 when older data with uniform offsets are read). [cm^2]
   double azimuth;             ///< Nominal azimuth angle of telescope system [deg].
   double altitude;            ///< Nominal altitude angle of telescope system [deg].
   double source_azimuth;      ///< Azimuth of assumed source. [deg]
   double source_altitude;     ///< Altitude of assumed source. [deg]
   double convergent_pos[3];   ///< Reference position [m] for convergent/divergent observations.
   int conv_div_opt;           ///< Non-zero if any telescope is configured for convergent or divergent pointing.
   double mean_convergent_depth; ///< Mean atmospheric depth of telescope-specific convergence. Zero for all parallel.
   double trigger_window;      ///< The time window within which central trigger signals
                               ///< must be seen, after correction for nominal delay [ns].
   int min_tel_trigger;        ///< No. of triggered telescoped needed for array trigger
   int tel_triggered;          ///< No. of telescopes triggered in current array
   int array_triggered;        ///< Is 1 if current array was triggered
   int with_aweight;           ///< Is 1 if input data came with weights.
   int telescope_ignore[MAX_IGNORE]; ///< List of telescopes to be ignored in simulation
   int closed_pedestal_events; ///< Number of pedestal events with camera closed
   int opened_pedestal_events; ///< Number of pedestal events with camera opened
   int led_events;             ///< Number of events with lid LEDs firing.
   int laser_events[MAX_LASER_LEVELS];           ///< Number of laser events
   struct simulated_shower_parameters shower_sim;///< Shower parameters simulated
   struct reconstructed shower_reco;             ///< and reconstructed.
   // The following are pointers to the arrays of data structures for all
   // telescopes. They should be filled in at program start.
   struct telescope_optics *optics;              ///< Optics and mounts.
   struct pm_camera *camera;                     ///< Camera parameters.
   struct camera_electronics *electronics;       ///< Electronics.
   struct mc_options *options;                   ///< File names etc.
   struct mc_run mc_run;
   double wfront_comp_az;      ///< Azimuth angle for which plane wavefront compensation was evaluated. [deg]
   double wfront_comp_alt;     ///< Altitude angle for which plane wavefront compensation was evaluated. [deg]
   double wfront_comp_ls;      ///< Assumed light speed (in air) for plane wavefront compensation. [cm/ns]
   int num_array_groups;
   struct telescope_array_group array_group[MAX_ARRAY_GROUPS];
   RpolTable *nsb_sky_map;     ///< NSB background scaling factor (az,alt) or NULL.

   int run;
   int event;

#ifdef BENCHMARKING
   /* Used in benchmarking: */
   double data_size;
   double cpu_time;
   double shower_ntrg;
   double shower_data_size;
   double shower_cpu_time;
#endif
};

/* ---------------------- Function prototypes --------------------------- */

#ifndef NO_PROTOTYPES

/* For sim_absorb.c see prototypes in absorb.h. */
/* For rpolator.c see prototypes in rpolator.h. */
/* For rndm2.c see prototypes in rndm2.h. */

/* rndm_table.c */
double *make_random_table(double *x, double *y, int n, int ni, int nr);
double random_from_table(double *xr, int nr, double randnr);
int make_random_ipol_table(double *x, double *y, int n, double **xr, double **yr);
double random_from_ipol_table(double *xr, double *yr, int n, double randnr);

/* sim_signal.c */
int read_spe(char *fname, double **xspe_prompt, double **yspe_prompt, 
   double **xspe_bkgrnd, double **yspe_bkgrnd, int *nspe_prompt, 
   int *nspe_bkgrnd, int afterpulse_alt, double *norm_fact);
int read_pulse_shape (int num_gains,
       char *fname_fadc, char *fname_disc, double binwidth,
       double *fshape, double *fshape_lg, int *flength, 
       double *farea, double *farea_lg,
       double *bshape, double *bshape_lg, int *blength, 
       double *barea, double *barea_lg,
       double *dshape, int *dlength, double *darea);
double delay_signals (struct camera_electronics *el);
void laser_calib_eval (struct camera_electronics *el);
void init_pm_electronics (struct pm_and_fadc_channel *ch,
   struct camera_electronics *el,
   double fadc_amp, double fadc_amp_lg, double disc_amp, 
   double qe_var, double g_var, double hglg_var, int flatfielding,
   double transit, double transit1, double v1frac,
   double voltage_var, double gain_index,
   double transit_error, double transit_calib_error,
   double transit_comp_step, double transit_comp_err,
   double pedestal, double pedestal_dev, 
   double pedestal_var, double pedestal_err,
   double pedestal_lg, double pedestal_dev_lg, 
   double pedestal_var_lg, double pedestal_err_lg,
   double sensitivity, double sens_var,
   double sensitivity_lg, double sens_var_lg,
   double background, double gain, 
   double disc_threshold, double disc_var,
   double gate_length_bins, double gate_length_var_bins,
   double gate_delay_bins, double gate_delay_var_bins,
   double disc_sigsum_bins, double disc_sigsum_var_bins,
   double trigger_current_limit,
   int fadc_per_channel);
void create_pm_signals(int *pe_counts, int *istart, 
   double *atimes, double *aamp, 
   double t0, struct camera_electronics *el);
void pulse_shape_analysis (struct camera_electronics *el);
void telescope_trigger (struct pm_camera *cam, 
   struct camera_electronics *el);
void array_trigger (struct telescope_array *array,
   struct camera_electronics *elec, int iarray);
void sum_adc_bins (struct telescope_array *array);
int read_shape(const char *fname, double *shape, int maxlen,
   double timestep, double scale, int *shape_len, int *toffsteps);

#if 0
/* ipol.c */
void interp(double x, double *v, int n, int *ipl, double *rpl);
double rpol(double *x, double *y, int n, double xp);
#endif

/* dhsort.c */
void dhsort(double *dnum, int nel);

/* sim_imaging.c */
int solve_quadratic_equation(double a, double b, double c, 
   double *x1, double *x2);
int reflect_on_spherical_mirror(double r, double mrad, double arnd,
   double *p, double *d, int type, double phi, double *angle);
int reflect_on_parabolic_mirror(double r, double mrad, double arnd,
   double *p, double *d, int type, double phi, double *angle);
int refract_in_fresnel_lens(double r, double mrad, double arnd,
   double *p, double *d, int type, double n, double *angle);
int reflect_on_polynomial_mirror(double r, double mrad, double arnd,
   double *p, double *d, int type, double phi, int npar, double *params,
   double hole_diam, double *angle);
double poly_eval_even(double x, int npar, const double *params);
double poly_deriv_even(double x, int npar, const double *params);
void make_trafo(double alpha_ang, double beta_ang, double gamma_ang, 
   double x, double y, double z, struct transform_struct *trans);
void make_trafo_off (const struct transform_struct *t1, 
   const struct transform_struct *t2, const double *o3,
   struct transform_off_struct *ts);
void transform(double *p, double *d, const struct transform_struct *ts, int mode);
void transform_off(double *p, double *d, const struct transform_off_struct *ts, int mode);
int tel_newdir (double phi, double theta, double tel_rand, struct telescope_optics *optics);
int tel_setup_primary(struct telescope_optics *optics, double phi, double theta);
int tel_setup_secondary(struct telescope_optics *optics, double phi, double theta);
int tel_mirror_grid_setup(struct telescope_optics *optics);
int pm_grid_setup(struct pm_camera *camera);
int pm_grid_search(struct pm_camera *camera, 
   double *photon_pos, double *photon_dir,
   double *x_on_camera, double *y_on_camera, 
   double *sx_camera, double *sy_camera, 
   double *time_to_camera);
int camera_setup_inclined_pixels(struct pm_camera *camera, struct telescope_optics *optics);
int trace_photon_in_segmented(struct telescope_optics *optics, 
   double *photon_pos, double *photon_dir,
   double *x_on_camera, double *y_on_camera, 
   double *sx_camera, double *sy_camera,
   double *time_to_camera, int *mirror, double *releff, double rlambda,
   double distance);
int trace_photon_in_paraboloid(struct telescope_optics *optics, 
   double *photon_pos, double *photon_dir,
   double *x_on_camera, double *y_on_camera, 
   double *sx_camera, double *sy_camera,
   double *time_to_camera, int *mirror, double *releff, double rlambda,
   double distance);
int trace_photon_in_fresnel(struct telescope_optics *optics, 
   double *photon_pos, double *photon_dir,
   double *x_on_camera, double *y_on_camera, 
   double *sx_camera, double *sy_camera,
   double *time_to_camera, int *mirror, double *releff, double rlambda,
   double distance);
int trace_photon_with_secondary(struct telescope_optics *optics, 
   double *photon_pos, double *photon_dir,
   double *x_on_camera, double *y_on_camera, 
   double *sx_camera, double *sy_camera, 
   double *time_to_camera, int *mirror, double *releff, double rlambda, 
   double distance, int bypass);
int read_table (const char *fname, int maxrow, double *col1, double *col2);
int assign_camera_pixels(struct pm_camera *cam);
const char *geo_text (int geo);
int assign_flexible_camera(struct pm_camera *cam, 
   struct camera_electronics *el, char *fname, 
   double scale_factor, int default_min_pixels);
int camera_hit(struct pm_camera *cam, struct telescope_optics *optics,
   double *photon_pos, double *photon_dir,
   double *x_on_camera, double *y_on_camera, 
   double *sx_camera, double *sy_camera, 
   double *time_to_camera);
int cathode_hit(struct pm_camera *cam, int ipix,
   double x, double y, double xs, double ys, int iwl, double *releff);
void offset_in_camera (double ref_azimuth, double ref_altitude,
   struct telescope_optics *optics, double *xoff, double *yoff);
int read_mirror_segments(const char *fname, struct mirror_segmentation *set_sets,
   size_t max_sets);
ConvexPolygon *make_convex_polygon(size_t n, double *xp, double *yp);
int inside_convex_polygon(double x, double y, ConvexPolygon *cp);
int baffle_intersection(double *photon_pos, double *photon_dir, double *baffle, int fwd_only);

/* sim_config.c */
int nearest_int(double d);
void config_setup_imaging(void);
void config_setup_atm_transmission(void);
int init_setup (int argc, char **argv, struct telescope_array *array,
   struct telescope_optics *tel_optics, struct pm_camera *camera,
   struct camera_electronics *electronics, struct mc_options *options);
int convergence_correction (struct telescope_array *array);
int randomize_viewing_direction(struct telescope_array *array);
MetaParamList *metapar_deliver (int itel);

/* sim_conv2{hegra|hess|cta}.c */
void init_fill_mc_norm_pulse(int ntel, struct camera_electronics *elec);
void convert_check_setup(void);
void convert_config(struct telescope_array *array, int format, char *output_fname, int replace);
void convert_runheader(struct telescope_array *array, int run);
void convert_input_lines (struct linked_string *list);
void convert_mc_event (struct telescope_array *array);
void convert_mc_photons (int shower, int iarray, int itel,
      double photons, struct bunch *bunches, int nbunches);
void convert_mc_photons3d (int shower, int iarray, int itel,
      double photons, struct bunch3d *bunches3d, int nbunches);
void convert_mc_pe_list (int shower, int array, int tel, 
      int npe, int flags, int pixels, 
      int *pe_counts, int *tstart, double *t, double *a,
      int *phot_counts);
void convert_basic_data (struct telescope_array *array,
   int run, int event, int write_all_pixels, int only_if_triggered);
void convert_calibdata (struct telescope_array *array, int run, int event, int type);
void convert_analyse_shower (struct telescope_array *array);
void convert_eventdata(struct telescope_array *array, int run, int event, 
   int write_all_pixels, int only_if_triggered);
void convert_runend(struct telescope_array *array);
void convert_finish(struct telescope_array *array);

/* sim_histograms.c */
int init_histograms(struct telescope_array *array, struct pm_camera *camera,
   struct camera_electronics *electronics);
int fill_histograms(struct telescope_array *array, int iarray, 
   struct camera_electronics *electronics, double energy, double wt, 
   double alt, double az);
#ifdef IO_BASIC_H__LOADED
int save_histograms(char *histogram_fname, IO_BUFFER *iobuf);
#endif

/* sim_telarray.c */
double line_point_distance(double x1, double y1a, double z1, 
   double cx, double cy, double cz, double x, double y, double z);
double find_max_pos(double *y, int n);
double Square(double x);
void sim_calib_events(int events, int laser, int open,
   struct telescope_array *array, double *atimes, long len_atimes);
double rhof_ (double *height);
double thick_ (double *height);
double refid_ (double *height);
double refim1_ (double *height);
double heigh_ (double *thick);
const char * get_simtel_prog(void);
const char * get_simtel_prog_path(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
