/**
 * @file usocket.cc
 *
 *  Created on: 2009-11-05
 *      Author: jldupont
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "usocket.h"

uSocket::uSocket(int s) {

	status=true;
	code=0;

	sockets[0] = s;
	sockets[1] = 0;
}//

uSocket::uSocket() {

	int result=socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
	status = (result==0);

	if (!status) {
		code=errno;
	}
}//

int uSocket::getPeerSocket(void) {

	return sockets[1];
}//

int uSocket::getLastError(void) {
	return code;
}

std::string
uSocket::popmsg(void) {

	std::string ret;
	char *buff[512]; buff[0] = '\0';

	size_t pos = ibuffer.find_first_of('\n');
	if (std::npos!=pos) {
		//found at least one message
		ret=ibuffer.substr(0, pos);

	}

	// first, attempt to read some bytes from the pipe
	// even if we've got potentially some stuff waiting in the
	// input buffer. It doesn't matter... all messages will get
	// delivered ;-)
	ssize_t bytes = read( sockets[0], (void*) buff, sizeof(buff));

	if (0>bytes) {
		code=errno;
		throw uSocketSocketException(errno);
	}

	ibuffer.append( buff );



}//


void
uSocket::pushmsg(std::string msg) {

	//even send the NULL termination byte
	ssize_t written=write(sockets[0], (const void *) msg.data(),  msg.length() );
	if (-1 == written) {
		code = errno;
		throw uSocketException(errno);
	}
}//

