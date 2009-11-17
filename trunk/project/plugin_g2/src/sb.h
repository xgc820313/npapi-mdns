/**
 * @file sb.h
 *
 *  Created on: 2009-11-05
 *      Author: jldupont
 */

#ifndef SB_H_
#define SB_H_


	#include <sys/socket.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <spawn.h>
	#include <errno.h>
	#include <string>
	#include "macros.h"
	#include <signal.h>

	using namespace std;

	class ServiceBrowser {

	public:
		typedef enum {
			ERROR=-3,
			CLOSED=-2,
			NOT_READY=-1,
			INVALID=0,
			READY=1
		} State;

	protected:
		pid_t sb_pid;
		int   fd[2];

		State st;
		std::string buffer;
		std::string sbpath;

	public:

		ServiceBrowser();
		ServiceBrowser(std::string _sbpath);
		~ServiceBrowser();

		void setSBPath(std::string _sbpath);
		bool init(void);

		State getState(void) {
			return st;
		}

		std::string popmsg(void);
	};


#endif /* SB_H_ */
