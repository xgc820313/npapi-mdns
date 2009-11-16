/*
 * main.cc
 *
 *  Created on: 2009-11-16
 *      Author: jldupont
 *
 *      For testing purposes only
 */
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "sb.h"

int main(int argc, char *argv[]) {

	ServiceBrowser sb("./ServiceBrowserJSON");

	if (!sb.init()) {
		perror("cannot initialize");
		exit(1);
	}

	while(true) {
		if (sb.getState() != ServiceBrowser::READY) {
			perror("Pipe closed!\n");
			exit(1);
		}
		cout << sb.popmsg();
		cout.flush();
	}

	return 0;
}
