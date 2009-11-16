/**
 * @file sb.cc
 *
 *  Created on: 2009-11-05
 *      Author: jldupont
 */

#include "sb.h"


ServiceBrowser::ServiceBrowser(std::string _sbpath) {
	sbpath=_sbpath;
	st=NOT_READY;
	pipe=NULL;
}//

ServiceBrowser::~ServiceBrowser() {

	if (NULL!=pipe)
		pclose(pipe);
}//

bool
ServiceBrowser::init(void) {

	// only one communication direction required.
	// I am avoiding sockets intentionally here.
	pipe = popen(sbpath.data(), "r");

	if (NULL==pipe) {
		return false;
	}

	printf("ServiceBrowser::init, pipe opened\n");

	int flags;

	pipeno=fileno(pipe);
	if (pipeno<0) {
		goto fail;
	}

	printf("ServiceBrowser::init, fileno acquired\n");

	flags = fcntl(pipeno, F_GETFL, 0);
	if (flags<0) {
		goto fail;
	}

	printf("ServiceBrowser::init, flags acquired\n");

	if (fcntl(pipeno, F_SETFL, flags | O_NONBLOCK)<0) {
		goto fail;
	}

	printf("ServiceBrowser::init, non-blocking mode set\n");

	st=READY;
	return true;

// ----------------------------------

fail:
	pclose(pipe);
	pipe=NULL;
	return false;
}//

std::string
ServiceBrowser::popmsg(void) {

	if (NULL==pipe || (READY != st))
		return std::string("{'signal':'service_browser_unavailable'};");

	char buf[128];
	ssize_t r = read(pipeno, buf, sizeof(buf)-1);

	if (-1==r && EAGAIN==errno) {
		// no-data available
	} else if (r>0) {
		// data received
		buffer.append(buf, r);
	} else {
		st=CLOSED;
	}

	if (READY!=st) {
		return std::string("");
	}

	size_t pos = buffer.find_first_of('\n');
	if (pos==std::string::npos) {
		return std::string("");
	}

	//printf("\nbuffer: %s \n", buffer.c_str());

	std::string ret(buffer.substr(0, pos));

	if (pos+1 < buffer.size()) {
		std::string newBuf(buffer, pos+1);
		buffer = newBuf;
	} else {
		buffer.clear();
	}

	return ret;
}//

