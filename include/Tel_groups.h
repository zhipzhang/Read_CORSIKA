#ifndef T_G1
#define T_G1
#include "TMath.h"
class Tel_groups 
{
    public:
    int ntel;
    double xtel[39];//!
    double ytel[39];//!
    double ztel[39]; //!
    double rtel[39];//!
    int narray;
    double xoff[100];
    double yoff[100];
    double toff[100];
    double alt;
    double az;
    double *dist;//!
    Tel_groups();
    ~Tel_groups();
    void set();
    void clear();
    void compute_dist();
};





















#endif