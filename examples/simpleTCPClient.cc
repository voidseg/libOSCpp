// -*- Mode:C++ -*-

/* Open SoundControl kit in C++                                              */
/* Copyright (C) 2002 Daniel Holth, Clinton McChesney                        */
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
/* Daniel Holth <dholth@stetson.edu> or visit                                */
/* http://www.stetson.edu/~ProctoLogic/                                      */

/*
 * This example was written by Martin Rumori (martin@rumori.de)
 *
 * it shows the use of the new network classes to build
 * a simple OSC client, which just prints out wat it gets
 */


#include <stdio.h>
#include <stdlib.h>

#include "OSCAssociativeNamespace.h"
#include "OSCProcessor.h"
#include "OSCPrintCallable.h"
#include "InetTCPMaster.h"
#include "InetTransportManager.h"
#include "OSCPacker.h"

int
main (int argc, char **argv)
{
  InetTransportManager transMan;
  InetTCPMaster tcpMaster;
  OSCPacker packer;

  InetTCPSlave *slave;

  tcpMaster.setTransportManager(&transMan);

  // construct osc message
  packer.reset();
  packer.packString("/osc/test");
  packer.packString(",s"); // typetag
  packer.packString("hello");

  //   packer.packInt((int32_t)32);
  //   packer.packFloat(4.6f);

  InetTCPSlave *slaves[200];

  while (1)
    {
      for (int i = 0; i < 200; ++i)
	{
	  slaves[i] = tcpMaster.newconnection("localhost", 10001);

	  if (! slaves[i])
	    {
	      std::cerr << "simpleTCPClient: slave is NULL" << std::endl;

	      return EXIT_FAILURE;
	    }
	}

      sleep(2);

      for (int i = 0; i < 50; ++i)
	{
	  tcpMaster.transmit(packer.getData());

// 	  for (int j = 0; j < 200; ++j)
// 	    {
// 	      slaves[j]->transmit(packer.getData());
// 	    }
	}

      sleep(6);

      tcpMaster.disconnectAll();

      sleep(8);
    }

  return EXIT_SUCCESS;
}

// EOF
