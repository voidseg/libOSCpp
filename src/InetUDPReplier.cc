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

/*
 * This class was written by Jared Jennings <jjenning@fastmail.fm>
 */

#include "InetUDPReplier.h"
#include "InetUDPMaster.h"
#include <sstream>

using namespace std;

InetUDPReplier::InetUDPReplier(InetUDPMaster *master) {
	_master = master;
	_addrlen = master->_addrlen;
	_addrType = master->_addrType;
	allocSockAddr(_addrType, &_remoteSendAddr, &_addrlen);
}

InetUDPReplier::~InetUDPReplier () {
	free(_remoteSendAddr);
}

InetUDPReplier::InetUDPReplier(InetUDPReplier &from) {
	_master = from._master;
	_addrlen = from._addrlen;
	_addrType = from._addrType;
	allocSockAddr(_addrType, &_remoteSendAddr, &_addrlen);
	memcpy(_remoteSendAddr, from._remoteSendAddr, _addrlen);
}


bool InetUDPReplier::transmit(const std::string &data) {
	return this->_master->transmitTo(this->_remoteSendAddr, data);
}

bool InetUDPReplier::setDestination(const std::string& host, 
		const short port) {
	return setRemoteAddr(host, htons(port), _addrType, _remoteSendAddr);
} 

bool InetUDPReplier::setReturnPort (const short port) {
	return setRemotePort(htons(port), _addrType, _remoteSendAddr);
}


/** Uniquely identifies the other end of this connection (i.e. address and
 * port). Does not uniquely identify this object (i.e. should not contain any
 * representation of this), because we clone InetUDPRepliers sometimes, and we
 * want to know if they're connected to the same other end. */
std::string InetUDPReplier::getName() {
	ostringstream name;
	name << "[UDP replier: " << getHostAddress(_addrType, _remoteSendAddr) 
		<< " port " << (unsigned short) getPortNumber(_addrType, _remoteSendAddr) 
		<< "]";
	return name.str();
}

Transmit *InetUDPReplier::clone() {
	return new InetUDPReplier(*this);
}

// EOF
