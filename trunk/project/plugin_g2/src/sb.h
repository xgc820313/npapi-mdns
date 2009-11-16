/**
 * @file sb.h
 *
 *  Created on: 2009-11-05
 *      Author: jldupont
 */

#ifndef SB_H_
#define SB_H_

	#include <fcntl.h>
	#include <stdio.h>
	#include <spawn.h>
	#include <errno.h>
	#include <string>
	#include "macros.h"

	using namespace std;

	class ServiceBrowser {

	public:
		typedef enum {
			CLOSED=-2,
			NOT_READY=-1,
			INVALID=0,
			READY=1
		} State;

	protected:
		FILE *pipe;
		int pipeno;
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
