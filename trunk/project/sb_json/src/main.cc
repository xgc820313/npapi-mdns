/**
 * @file main.cc
 *
 *  Created on: 2009-10-28
 *      Author: jldupont
 */
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>

#include "browser.h"

using namespace std;


DBusConnection *conn=NULL;

void sigtermHandler(int i) {

	if (NULL!=conn) {
		dbus_connection_unref(conn);
	}

	exit(0);
}

void sigpipeHandler(int i) {

	DBGLOG(LOG_INFO, "ServiceBrowser: SIGPIPE received");

	if (NULL!=conn) {
		dbus_connection_unref(conn);
	}

	exit(0);
}

int main(int argc, char *argv[]) {

	signal(SIGTERM, sigtermHandler);
	signal(SIGPIPE, sigpipeHandler);

	BrowserReturnCode code;

	code=browser_setup_dbus_conn( &conn );
	if (BROWSER_OK != code) {
		cout << "{'signal':'connection_error'};\n";
		cout.flush();
		exit(1);
	}

	cout << "{'signal':'connected'};\n";
	cout.flush();

	do {
		if (!dbus_connection_read_write_dispatch(conn, 100 /*timeout*/)) {
			cout << "{'signal':'disconnected'};\n";
			cout.flush();
			break;
		}

	} while(1);

	return 0;
}//
