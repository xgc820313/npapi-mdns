/**
 * @file sb.h
 *
 *  Created on: 2009-11-05
 *      Author: jldupont
 */

#ifndef SB_H_
#define SB_H_

	#include <stdio.h>
	#include <spawn.h>
	#include <string>

	using namespace std;

	class ServiceBrowser {

	protected:
		FILE *pipe;

	public:
		static std::string sbpath;

		ServiceBrowser();
		~ServiceBrowser();

		bool init(void);

		std::string popmsg(void);
		void pushmsg(std::string msg);

	};


#endif /* SB_H_ */
