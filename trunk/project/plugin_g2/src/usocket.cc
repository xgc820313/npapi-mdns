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


