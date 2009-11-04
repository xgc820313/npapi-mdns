/**
 * @file logger.cc
 *
 * @date   2009-09-30
 * @author jldupont
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "macros.h"


#ifdef _DEBUG
/**
 * Crude logging function
 */
void doLog(int priority, const char *message, ...) {

	openlog("npapi-dbus", LOG_PID, LOG_LOCAL1);

	char buffer[2048];
	va_list ap;

	va_start(ap, message);
		vsnprintf (buffer, sizeof(buffer), message, ap);
	va_end(ap);

	syslog(priority, buffer, NULL);

	closelog();

}
#endif
