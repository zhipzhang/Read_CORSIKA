#ifndef P_b
#define P_b

#include "TObject.h"
#include "mc_tel.h"
class Photon_bunches : public TObject
{
    public:
        double bunch_x;
        double bunch_y;
        double cx;
        double cy;
        double time;
        double p_height;
        double lambda;
        double nbunch;
        int itel;
        double  rc; //use telescope instead

    public:
        Photon_bunches();
        virtual ~Photon_bunches();    
        
        void set_bunch_x(double x)
        {
            bunch_x = x;
        }
        void set_bunch_y(double y)
        {
            bunch_y = y;
        }
        void set_cx(double cosx)
        {
            cx = cosx;
        }
        void set_cy(double cosy)
        {
            cy = cosy;
        }
        void set_time(double p_time)
        {
            time = p_time;
        }
        void set_height(double height)
        {
            p_height = height;
        }
        void set_lambda(double lam)
        {
            lambda = lam;
        }
        void set_nbunch(double n)
        {
            nbunch = n;
        }
        void clear();
        void fill_photon_bunch(struct bunch, int i, int j, double r);
        ClassDef(Photon_bunches, 1);
};
















#endif