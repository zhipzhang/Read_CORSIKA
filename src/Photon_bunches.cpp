#include "Photon_bunches.h"
ClassImp(Photon_bunches)
Photon_bunches::Photon_bunches()
{
    bunch_x = bunch_y = 0.;
    cx = cy = 0.;
    time = p_height = lambda = 0.;
    nbunch = 0.;
    itel = -1;
    rc = -1;
}

void Photon_bunches::clear()
{
   bunch_x = bunch_y = 0.;
    cx = cy = 0.;
    time = p_height = lambda = 0.;
    nbunch = 0.; 
    itel = -1;
    rc = -1;
}

void Photon_bunches::fill_photon_bunch(struct bunch bunches, int array_id, int tel_id, double r)
{
    bunch_x = bunches.x * 0.01;
    bunch_y = bunches.y * 0.01;
    nbunch = bunches.photons;
    p_height = bunches.zem;
    time = bunches.ctime;
    cx = bunches.cx;
    cy = bunches.cy;
    lambda = bunches.lambda;
    rc = r;
    itel = tel_id;
    
}

Photon_bunches::~Photon_bunches()
{

}