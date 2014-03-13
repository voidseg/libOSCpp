/* Open SoundControl kit in C++                                              */
/* Copyright (C) 2002-2004 libOSC++ contributors. See AUTHORS		     */
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


/*
 *  This class was written by Martin Rumori (martin@rumori.de)
 */

/**
 * @class InetTransportManager
 *
 * @author  Daniel Holth
 * @author  Martin Rumori
 */


#include "InetTransportManager.h"

using namespace std;

InetTransportManager::InetTransportManager ()
{
  _pollfd = NULL;
  _pollnum = 0;
} // InetTransportManager


InetTransportManager::~InetTransportManager ()
{
  free(_pollfd);
} // ~InetTransportManager


void InetTransportManager::serveForever ()
{
  while (1)
    {
      runCycle(30000);
    }
} // serveForever


void InetTransportManager::runCycle ()
{
  runCycle(0);
} // runCycle


void InetTransportManager::runCycle (int timeout)
{
  int changenum;

  // poll on handles
  while(1)
    {
      switch (changenum = poll(_pollfd, _pollnum, timeout))
	{
	case -1 : // error occured

#ifdef LIBOSC_DEBUG
	  cerr << "InetTransportManager::runCycle():poll(): " << strerror(errno) << endl;
#endif
	  if (errno == EINTR)
	    {
	      continue;
	    }
#ifdef LIBOSC_DEBUG
	  cerr << "InetTransportManager::runCycle():poll(): " << strerror(errno) << endl;
#endif
	  break;

	case 0 : // no change on handles
#ifdef LIBOSC_REAL_DEBUG
	  cerr << "InetTransportManager::runCylce():poll(): leaving after timeout" << endl;
#endif
	  break;

	default : // change on handle(s)

	  InetTransport *itrans = 0;
	  struct pollfd *piter;
	  int pindex = 0;

	  while (changenum)
	    {
	      piter = _pollfd + pindex++;

	      switch (piter->revents)
		{
		case 0 :
		  // no change on descriptor
		  continue; // while (changenum)

		case POLLIN :
		case POLLPRI :
#ifdef LIBOSC_DEBUG
		  cerr << "InetTransportManager::runCycle(): data ready on handle " << piter->fd << endl;
#endif
		  // reset revents field
		  piter->revents = 0;

		  itrans = _itrans.find(piter->fd)->second;

		  if(! itrans->handle())
		    {
		      switch (itrans->getError())
			{
			case InetOSC::OSC_IN_CLOSED_CONN :

			  // case InetOSC::OSC_IN_RECV_ERRNO :

			  _itrans.erase(piter->fd);
			  delHandleFromPollfd(piter->fd);
			  delete itrans;

			  // since we deleted objects, we have to restart
			  pindex = 0; 

			  break;

			default :
#ifdef LIBOSC_DEBUG
			  cerr << "InetTransportManager::runCycle(): call of handle() on descriptor " << piter->fd << " returned error " << itrans->getError() << endl;
#else
			  ; // compiler food
#endif
			}
		    }
		  break;

		case POLLHUP :
		case POLLERR :
		case POLLNVAL :

#ifdef LIBOSC_DEBUG
		  cerr << "InetTransportManager::runCycle(): poll() on descriptor " << piter->fd << " got POLLHUP|POLLERR|POLLNVAL. closing" << endl;
#endif
		  // reset revents field
		  piter->revents = 0;

		  itrans = _itrans.find(piter->fd)->second;
		  _itrans.erase(piter->fd);
		  delHandleFromPollfd(piter->fd);
		  delete itrans;

		  // since we deleted objects, we have to restart
		  pindex = 0;

		  break;

		default :
#ifdef LIBOSC_DEBUG
		  cerr << "InetTransportManager::runCycle(): poll() on descriptor " << piter->fd << " got " << piter->revents << endl;
#endif
		  // reset revents field
		  piter->revents = 0;
		} // switch (iter->revents)

	      --changenum;
	    } // while (changenum)
	  break;
	} // switch (poll())
      break;
    } // while (1)
} // runCylce


void InetTransportManager::addInetTransport (InetTransport *const itrans)
{
  int handle = itrans->getHandle();

  if (_itrans.find(handle) != _itrans.end())
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTransportManager::addInetTransport(): InetTransport with socket descriptor " << handle << " already added" << endl;
#endif

      return;
    }

  // add to pollfd struct
  if (! addHandleToPollfd(handle))
    {
      return;
    }

  // add to stl map
  _itrans[handle] = itrans;

} // addInetTransport


void InetTransportManager::delInetTransport (InetTransport *const itrans)
{
  int handle = itrans->getHandle();

  if ((! _pollnum) || _itrans.find(handle) == _itrans.end())
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTransportManager::delInetTransport(): InetTransport with socket descriptor " << handle << " not found for deletion" << endl;
#endif

      return;
    }

  // remove from STL map
  _itrans.erase(handle);

  // remove from _pollfd
  delHandleFromPollfd(handle);

} // delInetTransport


void InetTransportManager::addService (Service *const service)
{
  InetTransport *itrans;

  if ((itrans = dynamic_cast<InetTransport *> (service)))
    {
      addInetTransport(itrans);
    }
  else
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTransportManager::addService(): can't add objects of type other than InetTransport" << endl;
#endif
    }
} // addService


void InetTransportManager::delService (Service *const service)
{
  InetTransport *itrans;

  if ((itrans = dynamic_cast<InetTransport *> (service)))
    {
      delInetTransport(itrans);
    }
  else
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTransportManager::delService(): can't delete objects of type other than InetTransport" << endl;
#endif
    }
} // delService


void
InetTransportManager::getTransports (std::vector<InetTransport *>& transports) const
{
  InetTransportMap::const_iterator iter = _itrans.begin();

  while (iter != _itrans.end())
    {
      transports.push_back(iter->second);
      ++iter;
    }
} // getTransports


bool
InetTransportManager::addHandleToPollfd (int handle)
{
  struct pollfd *pollfd_tmp;

  if (! (pollfd_tmp = (struct pollfd *)realloc(_pollfd, ++_pollnum * sizeof(struct pollfd))))
    {
#ifdef LIBOSC_DEBUG
      cerr << "InetTransportManager::addHandleToPollfd():realloc(): " << strerror(errno) << ". InetTransport not added" << endl;
#endif
      --_pollnum;

      return false;
    }
  else
    {
      _pollfd = pollfd_tmp;
    }

  // set struct pollfd for added InetTransport
  (_pollfd + _pollnum - 1)->fd = handle;
  (_pollfd + _pollnum - 1)->events = POLLIN;
  (_pollfd + _pollnum - 1)->revents = 0;

  return true;
} // addHandleToPollfd


bool
InetTransportManager::delHandleFromPollfd (int handle)
{
  // special case: only one entry
  if (_pollnum == 1)
    {
      free(_pollfd);
      _pollfd = NULL;
      _pollnum = 0;

      return true;
    }

  // find corresponding poll struct
  struct pollfd *iter = _pollfd;
  int pollindex = 0;

  /* iter should not leave the _pollfd array
   * otherwise _itrans and _pollfd are out of sync!
   * would give a great SIGSEGV
   */
  for (; iter->fd != handle; ++iter, ++pollindex);

  // delete our candidate's struct pollfd
  // by shifting the succeeding structs
  // and decrease _pollnum
  if (pollindex < --_pollnum)
    {
      memmove(iter,
	      iter + 1,
	      (_pollnum - pollindex) * sizeof(struct pollfd));
    }

  // free the uneccesary memory
  struct pollfd * pollfd_tmp;

  if (! (pollfd_tmp = (struct pollfd *)realloc(_pollfd, _pollnum * sizeof(struct pollfd))))
    {
      /* if this happens, we have just one time sizeof(struct pollfd)
       * too much memory.  since all other values are consistent
       * and realloc doesn't touch the original memory on failure,
       * all will be pretty o.k. the next time a transport will deleted
       */
#ifdef LIBOSC_DEBUG
      cerr << "InetTransportManager::delHandleFromPollfd():realloc(): " << strerror(errno) << ". however, InetTransport was deleted" << endl;
#endif

      return false;
    }
  else
    {
      _pollfd = pollfd_tmp;
    }

  return true;
} // delHandleFromPollfd


void
InetTransportManager::showState ()
{
  cerr << "InetTransportManager::showState(): InetTransport entries in _itrans: " << _itrans.size() << endl;
  cerr << "InetTransportManager::showState(): InetTransport entries in _pollfd: " << _pollnum << endl;
} // showState


void
InetTransportManager::validatePollfd ()
{
  struct pollfd *iter;
  int pollnum = 0;

  for (iter = _pollfd; pollnum < _pollnum; ++iter, ++pollnum)
    {
      cerr << "InetTransportManager::validatePollfd(): _pollfd[" << pollnum << "]->fd      : " << iter->fd << endl;
      cerr << "InetTransportManager::validatePollfd(): _pollfd[" << pollnum << "]->events  : " << iter->events << endl;
      cerr << "InetTransportManager::validatePollfd(): _pollfd[" << pollnum << "]->revents : " << iter->revents << endl;
    }

}


// EOF
