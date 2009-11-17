/**
 * @file sb.cc
 *
 *  Created on: 2009-11-05
 *      Author: jldupont
 */

#include "sb.h"

ServiceBrowser::ServiceBrowser() {
	st=NOT_READY;
	fd[0]=-1;
	fd[1]=-1;
	sb_pid=0;
}

ServiceBrowser::ServiceBrowser(std::string _sbpath) {
	sbpath=_sbpath;
	st=NOT_READY;
	fd[0]=-1;
	fd[1]=-1;
	sb_pid=0;
}//

ServiceBrowser::~ServiceBrowser() {

	if (fd[0])
		close(fd[0]);
	if (sb_pid)
		kill(sb_pid, SIGTERM);
}//

void
ServiceBrowser::setSBPath(std::string _sbpath) {
	sbpath=_sbpath;
}

bool
ServiceBrowser::init(void) {

	int rc=socketpair( AF_UNIX, SOCK_STREAM, 0, fd );
	if (rc<0) {
		st=ERROR;
		return false;
	}

	sb_pid = fork();
	if (sb_pid < 0) {
		st=ERROR;
		return false;
	}

	if (sb_pid==0) {
		// child process
		close(fd[0]);

		if (fd[1] != STDIN_FILENO) {
			if (dup2(fd[1], STDIN_FILENO)!=STDIN_FILENO) {
				DBGLOG(LOG_ERR, "ServiceBrowser::init: can't dup2 STDIN");
				exit(0);
			}
		}

		if (fd[1] != STDOUT_FILENO) {
			if (dup2(fd[1], STDOUT_FILENO)!=STDOUT_FILENO) {
				DBGLOG(LOG_ERR, "ServiceBrowser::init: can't dup2 STDOUT");
				exit(0);
			}
		}
		if (execl(sbpath.data(), "", NULL) < 0) {
			DBGLOG(LOG_ERR, "ServiceBrowser: can't execl");
			exit(0);
	    }
	    exit(0);
	}//child

	// THIS IS THE PARENT
	close(fd[1]);

	int flags = fcntl(fd[0], F_GETFL, 0);
	if (flags<0) {
		goto fail;
	}

	printf("ServiceBrowser::init, flags acquired\n");

	if (fcntl(fd[0], F_SETFL, flags | O_NONBLOCK)<0) {
		goto fail;
	}

	printf("ServiceBrowser::init, non-blocking mode set\n");

	st=READY; // at least, we hope...
	return true;

fail:
	close(fd[0]);
	fd[0]=-1;
	st=NOT_READY;
	return false;

	/*
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
	*/

}//

std::string
ServiceBrowser::popmsg(void) {

	if (NULL==pipe || (READY != st))
		return std::string("{'signal':'service_browser_unavailable'};");

	char buf[128];
	ssize_t r = read(fd[0], buf, sizeof(buf)-1);

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

	std::string ret(buffer.substr(0, pos));

	if (pos+1 < buffer.size()) {
		std::string newBuf(buffer, pos+1);
		buffer = newBuf;
	} else {
		buffer.clear();
	}

	return ret;
}//

