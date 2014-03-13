/* Open SoundControl kit in C++                                              */
/* Copyright (C) 2002-2004 libOSC++ contributors. See AUTHORS                */
/*                                                                           */
/* This library is free software; you can redistribute it and/or             */
/* modify it under the terms of the GNU Lesser General Public                */
/* License as published by the Free Software Foundation; either              */
/* version 2.1 of the License, or (at your option) any later version.        */
/*                                                                           */
/* This library is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         */
/* Lesser General Public License for more details.                           */
/*                                                                           */
/* You should have received a copy of the GNU Lesser General Public          */
/* License along with this library; if not, write to the Free Software       */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                           */
/* For questions regarding this program contact                              */
/* Daniel Holth <dholth@fastmail.fm> or visit                                */
/* http://wiretap.stetson.edu/                                               */

/**
 * @file MikmodServer.cc
 *
 * Play samples via mikmod
 *
 * @author Daniel Holth
 */

#include <iostream>
#include <pthread.h>
#include <vector>
#include <getopt.h>
#include <stdlib.h>

#include "InetTransportManager.h"
#include "InetUDPMaster.h"
#include "OSCHierarchicalNamespace.h"
#include "OSCDocumenter.h"
#include "OSCFunctionCallable.h"
#include "OSCLister.h"
#include "OSCProcessor.h"
#include "OSCTypeReturner.h"

#include "sampleplayer.h"

using namespace std;

#define PORT 4950


void PlayPanVolumeSpeedCallback(void *context, 
                                const std::string &data, 
                                Transmit *reply)
{
  int sampleNumber;
  int playbackSpeed;
  int panning;
  int volume;
  string temp;

  OSCUnpacker u;
  u.setData(data);
  
  u.unpackString(&temp);
  cout << "address: " << temp << endl;

  u.unpackString(&temp);
  cout << "type tags: " << temp << endl;

  u.unpackInt(&sampleNumber);
  u.unpackInt(&playbackSpeed);
  u.unpackInt(&panning);
  u.unpackInt(&volume);

  cout << "  Sample number: " << sampleNumber << endl;

  sampleplayer_play_speed_pan_volume(sampleNumber, 
                                     playbackSpeed, 
                                     panning, volume);
}


int main(int argc, char **argv)
{
    int  portNumber = PORT;
    int  c;

    // Parse command line arguments:
    while(1) {
        int optionIndex;
        static struct option longOptions[] = {
            {"port", 1, 0, 'p'},
            {"help", 0, 0, 'h'},
            {0, 0, 0, 0}
        };

        c = getopt_long(argc, argv, "hp:", longOptions, &optionIndex);
        if (c == -1)
        {
            break;
        }

        switch (c)
        {
            case 0:
                cout << "option " << longOptions[optionIndex].name << endl;
                break;

            case 'p':
                cout << "port " << optarg << endl;
                if (optarg != NULL)
                {
                    portNumber = strtol(optarg, NULL, 10);
                    if(portNumber == 0)
                    {
                        cout << "Check your port number!" << endl;
                    }
                }
                break;

            case 'h':
                cout << "Usage: " << *argv << " [OPTION] [sample.wav]+" << endl;
                cout << "\t-h, --help\t display this helpful message" << endl;
                cout << "\t-p, --port\t port number to listen on" << endl;        
                exit(0);
                break;
        }
    }

    // Move the argv pointer past the options (to the list of samples):
    // (the zeroth element should be 1 before the list of samples.)
    argv = argv + optind - 1;
    argc = argc - optind + 1;

    cout << "The Amazing OSC Server" << endl;

    InetTransportManager transMan;
    InetUDPMaster udpMaster;
    OSCHierarchicalNamespace nspace;
    OSCProcessor processor;

    OSCLister *lister;
    OSCDocumentor *documentman;
    OSCTypeReturner *typeReturner;


    // Set up utility namespace items.
    lister = new OSCLister();
    lister -> setNamespace(&nspace);
    nspace.add("/list", lister);

    documentman = new OSCDocumentor();
    documentman -> setNamespace(&nspace);
    nspace.add("/document", documentman);

    documentman -> setDocumentation("Takes a string (an OSC address) and returns documentation for it.");

    typeReturner = new OSCTypeReturner();
    typeReturner -> setNamespace(&nspace);
    nspace.add("/typeSignature", typeReturner);

    typeReturner -> setDocumentation("Returns the address and type signature of an OSCCallable.");


    // Our custom callable. 
    OSCFunctionCallable *play;
    play = new OSCFunctionCallable(&PlayPanVolumeSpeedCallback, NULL);
    play -> setTypeSignature("iiii");
    play -> setFunction(&PlayPanVolumeSpeedCallback, NULL);
    play -> setDocumentation("Sample player.  Takes four arguments:\n\
\tsample number (integer, 0-(samples loaded - 1))\n\
\tplayback rate (integer samples per second),\n\
\tpanning (0 (left) to 255 (right)),\n\
\tvolume (0 (silent) to 255 (loud))\n");

    nspace.add("/play", play);  


    pthread_t thread;
    cout << "Starting synth thread..." << endl;
    sampleplayer_init(argc, argv);
    pthread_create(&thread, NULL, sampleplayer_start, NULL);


    cout << "Now listening on port " << portNumber << endl;

    processor.setNamespace(&nspace);
    udpMaster.setProcessor(&processor);
    udpMaster.setTransportManager(&transMan);
    udpMaster.startlisten(portNumber);
    transMan.serveForever();

    return 0;
}
