#include "Photon_bunches.h"
#include "TTree.h"
#include "TFile.h"
#include "initial.h"
#include "io_basic.h"
#include "mc_tel.h"
#include <signal.h>
#include <string>
#include <iostream>
#include "fileopen.h"
#include "rec_tools.h"
#include "Tel_groups.h"
#include "TMath.h"
#include "events.h"
/*
    First Version to convert the CORSIKA IACT OUTPUT(bunches) to ROOT
    Author zhangzhipeng
    email zhipzhang@mail.ustc.edu.cn
     
    VERSION 1.1
    not written bunch class now only event class
*/
// compute the Rc of each tel

int main(int argc, char** argv)
{
    IO_BUFFER* iobuf = NULL;
    IO_ITEM_HEADER item_header, item_header2, sub_item_header, block_header;
    const char* input_fname = NULL;
    float runh[273], rune[273], evth[273], evte[273];
    int iarray;
    int shower;
    int itel;
    int max_bunches = 50000000;
    int res;
    int i; 
    struct bunch *bunches = (struct bunch *) calloc(1, sizeof(bunch));
    std::string out_file = "out.root";
    auto photon = new Photon_bunches();
    auto tel_group = new Tel_groups();
    auto event = new events();
    if( ( iobuf = allocate_io_buffer(5000000L)) == NULL)
    {
        std::cout << "Cannot allocate I/O buffer" << std::endl;
        exit( EXIT_FAILURE );
    }
    iobuf->max_length = 10000000000L;

    while(argc > 1)
    {
        if((strcmp(argv[1], "--out_file") == 0) && argc >2)
        {
            out_file = argv[2];
            argc -= 2;
            argv += 2;
            continue;
        }
        else if((strcmp(argv[1], "--max_bunches") == 0) && argc >2)
        {
            max_bunches = atoi(argv[2]);
            argc -= 2;
            argv += 2;
            continue;
        }
        else
        {
            break;
        }
    }

    TFile* root_file = new TFile(out_file.c_str(), "RECREATE");
    if( root_file->IsZombie())
    {
        std::cout << "Error while creating the new root file" << std::endl;
        exit(1);
    }

    TTree* bunch = new TTree("bunch", "photon_bunches data");
    bunch->Branch("photon_bunches",&photon);

    TTree* event_data = new TTree("event_data", "photons in per tel");
    event_data->Branch("event", &event,500000);
    

    bunches = (struct bunch *) calloc(max_bunches, sizeof(struct bunch));

    //TTree* tel_data = new TTree("tel_data", "some data in each event");
    //tel_data->Branch("tel_group", &tel_group);

    while( argc >1 || input_fname != NULL)
    {
        if ( argc > 1)
        {
            if ( argv[1][0] =='-' && argv[1][1] != '\0')
            {
                std::cout << "Invalid Input_file name" << std::endl;
            }
            else
            {
                input_fname = argv[1];
                argc--;
                argv++;
            }
        }

        if( (iobuf->input_file = fileopen(input_fname, READ_BINARY)) == NULL)
        {
            perror( input_fname );
            std::cout << "Cannot open input file " << std::endl;
            exit( EXIT_FAILURE );
        }

        if (input_fname)
        {
            std::cout << "opening file " << input_fname << std::endl;
            fflush( stdout );

        }
        input_fname = NULL;

        for(;;)
        {
            if (find_io_block(iobuf, &block_header) != 0)
                break;
            if (read_io_block(iobuf, &block_header) != 0)
                break;

            switch ((int) block_header.type)
            {
                case IO_TYPE_MC_RUNH:
                    read_tel_block(iobuf, IO_TYPE_MC_RUNH, runh, 273);
                /* code */
                    break;

                case IO_TYPE_MC_INPUTCFG:
                     {
                        struct linked_string corsika_inputs;
                        corsika_inputs.text = NULL;
                        corsika_inputs.next = NULL;
                        read_input_lines(iobuf,&corsika_inputs);
                        if ( corsika_inputs.text != NULL )
                        {
                            struct linked_string *xl = NULL, *xln = NULL;
                            printf("\nCORSIKA was run with the following input lines:\n");
                            for (xl = &corsika_inputs; xl!=NULL; xl=xln)
                        {
                            printf("   %s\n",xl->text);
                            free(xl->text);
                            xl->text = NULL;
                            xln = xl->next;
                            xl->next = NULL;
                            if ( xl != &corsika_inputs )
                            free(xl);
                        }
                        }
                    }
                    break;

                case IO_TYPE_MC_TELPOS:
                    if(read_tel_pos(iobuf, 39, &tel_group->ntel, tel_group->xtel, tel_group->ytel,
                         tel_group->ztel, tel_group->rtel) < 0)
                         {
                            std::cout << "Problem when reading tel_pos" << std::endl;
                            fflush(stdout);
                         }
                         break;

                case IO_TYPE_MC_EVTH:
                    read_tel_block(iobuf, IO_TYPE_MC_EVTH, evth, 273);
                    shower = evth[1];
                    tel_group->alt = 90. - (180./M_PI)*evth[10];
                    tel_group->az  = 180. - (180./M_PI)*(evth[11]-evth[92]);
                    tel_group->az -= floor(tel_group->az/360.) * 360.;
                    
                    break;

                case IO_TYPE_MC_TELOFF:
                    res  = read_tel_offset(iobuf, 100, &tel_group->narray, tel_group->toff, 
                                                tel_group->xoff, tel_group->yoff);
                    tel_group->set();
                    tel_group->compute_dist();
                    if( res < 0)
                    {
                        exit(EXIT_FAILURE);
                    }
                    break;

                case IO_TYPE_MC_TELARRAY:
                    int type;
                    int nbunches;
                    int jarray;
                    double photons;
                    begin_read_tel_array(iobuf, &item_header, &iarray);
                    sub_item_header.type = IO_TYPE_MC_PHOTONS;
                    for( int itc = 0; itc < tel_group->ntel; itc ++)
                    {
                        if(search_sub_item(iobuf, &item_header, &sub_item_header) < 0)
                        {
                            break;
                        }
                        if(read_tel_photons(iobuf, max_bunches, &jarray, &itel, &photons, bunches, &nbunches) < 0)
                        {
                            fflush(stdout);
                            std::cout << "Error reading"<< std::endl;
                        }
                           // event->fill(shower*100+jarray, itel, bunches->photons,tel_group->dist[jarray*(tel_group->narray) + itel]);
                            //event_data->Fill();
                            //event->clear();
                            for(int ibunch = 0 ; ibunch < nbunches; ibunch++)
                            {
                                photon->fill_photon_bunch(bunches[ibunch], jarray, itel, tel_group->dist[jarray*(tel_group->narray) + itel]);
                                bunch->Fill();
                                photon->clear();
                            }
                            /* code */
                    }
                               
                    break;
                    
                case IO_TYPE_MC_TELARRAY_HEAD:
                    std::cout << "Start read photon bunch blocks " << item_header.ident << std::endl;
                    break;
                
                case IO_TYPE_MC_TELARRAY_END:
                    std::cout << "Finish read photon bunch blocks " << item_header.ident << std::endl;
                    break;

                case IO_TYPE_MC_EVTE:
                    read_tel_block(iobuf, IO_TYPE_MC_EVTE, evte, 273);
                   // tel_data->Fill();
                    tel_group->clear();
                    break;
                
                case IO_TYPE_MC_RUNE:
                    read_tel_block(iobuf, IO_TYPE_MC_RUNE, rune, 273);
                    break;



                default:
                    fflush(stdout);
                    std::cout << "Ingoring unknown data block type " << block_header.type << std::endl;
                    break;
            }
        }
        free(bunches);
        if(iobuf->input_file != NULL)
        {
            fileclose(iobuf->input_file);
        }
        iobuf->input_file  = NULL;
        reset_io_block(iobuf);

    }
    event_data->Write();
   // tel_data->Write();
    root_file->Write();
    root_file->Close();

}
