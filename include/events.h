#ifndef E_s
#define E_s
#include "TObject.h"
class events: public TObject
{
    public:
        double photons;
        int itel;
        double rc;
        int run_id;

        void clear()
        {
            run_id = itel = 0;
            photons = rc = 0.;
        }
        void fill(int , int ,  double, double);
        events();
        ~events();
    ClassDef(events, 1);
};

void events::fill(int i, int j,  double size, double dist )
{
    run_id = i;
    itel = j;
    photons = size;
    rc = dist;
}

events::events()
{
 run_id = 0;
 itel = 0;
 photons = rc =0.;
}
events::~events()
{

}

ClassImp(events)








#endif