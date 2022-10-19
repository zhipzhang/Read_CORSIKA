#include "Photon_bunches.h"
#include "TTree.h"
#include "TFile.h"
#include <string>
#include "TH1D.h"
#include "TMath.h"
#include "events.h"

int main(int argc, char** argv)
{
    std::string out_file = "out.root";
    const char*  in_file;

    while(argc > 1)
    {
        if(strcmp(argv[1], "--out_file") == 0 && argc > 2)
        {
            out_file = argv[2];
            argc -= 2;
            argv += 2;
            continue;
        }
        else
        {
            break;
        }
    }

    TFile* h_file = new TFile(out_file.c_str(), "RECREATE");
    TH1D* h1 = new TH1D("h1", "photon_density",30, 0, 600);
    TH1D* h2 = new TH1D("h2", "density_with_weight",30, 0, 600);
    while(argc > 1 || in_file != NULL)
    {
        in_file = argv[1];
        double area = TMath::Pi() * pow(5*cos(10*TMath::DegToRad()), 2);
        TFile* root_file = new TFile(in_file, "READ");
        in_file = NULL;
        TTree* event_tree = (TTree*) root_file->Get("event_data");
        auto event = new events();
        event_tree->SetBranchAddress("event", &event);
        for(int i = 0; i < event_tree->GetEntries(); i++)
        {
            event_tree->GetEntry(i);
            h1->Fill(event->rc);
            double n = event->photons ;
            h2->Fill(event->rc,n/area);
        }

        root_file->Close();
        h_file->cd();
        h1->Write();
        h2->Write();
        h_file->Write();
        h_file->Close();
        argc--;


    }

}