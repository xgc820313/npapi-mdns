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
			INVALID=0,
			READY,
			NOT_READY,
			CLOSED
		} State;

	protected:
		FILE *pipe;
		int pipeno;
		State st;
		std::string buffer;
		std::string sbpath;

	public:

		ServiceBrowser(std::string _sbpath);
		~ServiceBrowser();

		bool init(void);

		State getState(void) {
			return st;
		}

		std::string popmsg(void);

	};


#endif /* SB_H_ */
