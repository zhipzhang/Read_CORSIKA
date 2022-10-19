#include "Tel_groups.h"
#include "rec_tools.h"
#include <iostream>

void Tel_groups::set()
{
    for(int i = 0 ; i < ntel; i++)
    {
        xtel[i] = xtel[i] * 0.01;
        ytel[i] = ytel[i] * 0.01;
        ztel[i] = ztel[i] * 0.01;

    }
    for( int k = 0; k < narray; k++)
    {
        xoff[k] = -0.01 * xoff[k];
        yoff[k] = -0.01 * yoff[k];
    }
    alt = alt * TMath::DegToRad();
    az = az * TMath::DegToRad();
    dist = (double *) calloc(ntel* narray, sizeof(double));
}

Tel_groups::Tel_groups()
{
    
}


void Tel_groups::compute_dist()
{
    for(int i = 0; i< narray; i++)
    {
        for(int j = 0; j < ntel; j++)
        {
            dist[i * narray + j] = line_point_distance(xoff[i], yoff[i], 0, cos(alt)*cos(az),
                                                        -cos(alt) * sin(az), sin(alt), xtel[j], ytel[j], ztel[j]);
        }
    }
}

// only clear the offset data differ in every event
void Tel_groups::clear()
{
    std::fill(xoff, xoff+100, 0.);
    std::fill(yoff, yoff+100, 0.);
    std::fill(toff, toff+100, 0.);
    free(dist);
    alt = 0.;
    az = 0.;

}
