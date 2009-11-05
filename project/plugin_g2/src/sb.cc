/**
 * @file sb.cc
 *
 *  Created on: 2009-11-05
 *      Author: jldupont
 */

#include "sb.h"


ServiceBrowser::ServiceBrowser() {

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

	return pipe!=NULL;
}//

std::string
ServiceBrowser::popmsg(void) {

	std::string ret;

	if (NULL==pipe)
		return ret;


}//

void
ServiceBrowser::pushmsg(std::string msg) {

}//
