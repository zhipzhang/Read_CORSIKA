/* ============================================================================

   Copyright (C) 2002, 2006, 2010, 2019 Konrad Bernloehr
   
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
/** @file absorb.h
 *  @short Functions definitions for loading transmission, reflectivitiy, and QE tables.
 *
 *  @author  Konrad Bernloehr 
 *  @date    @verbatim CVS $Date: 2022/03/30 20:16:45 $ @endverbatim
 *  @version @verbatim CVS $Revision: 1.17 $ @endverbatim
 */
/* ================================================================ */

#ifndef ABSORB_H__LOADED
#define ABSORB_H__LOADED 1

#include "rpolator.h"

#ifdef __cplusplus
extern "C" {
#endif

/* absorb.c */

double height_for_transmission(void);
int read_trans(const char *setup_trans_fname, double altitude);
int read_qe_ref(const char *qe_fname, const char *mirror_ref_fname, 
    const char *mirror2_ref_fname, 
    double *quantum_efficiency, double *optics_efficiency,
    int mirror_class, int bypass_optics, int max_lambda);
double atmospheric_transmission(int iwl, double zem, double airmass);
double atmospheric_transmission_x (int iwl, double zem, double airmass);
double atmospheric_transmission2 (int iwl, double zem, double airmass,
   double od2tel, double od2focus);

int rpt_qe_ref(RpolTable *rpt_qe, RpolTable *rpt_ref, RpolTable *rpt_ref2,
   double *quantum_efficiency,  double *optics_efficiency,
   int mirror_class, int bypass_optics, int max_lambda,
   double mirror_degraded, double mirror2_degraded);

#ifdef __cplusplus
}
#endif

#endif
