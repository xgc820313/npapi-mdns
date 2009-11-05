/**
 * @file browser.cc
 *
 * @date 2009-10-28
 * @author jldupont
 *
 * IMPORTANT NOTE:
 * ===============
 * Once the client user of this class sends an "exit & clean"
 * command msg, the said client shouldn't be using the message
 * queues either way.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "browser.h"

#include<iostream>
#include <sstream>

using namespace std;


//{{ PROTOTYPES
	DBusHandlerResult __browser_filter_func (DBusConnection *connection, DBusMessage *message, void *user_data);
	void __browser_handle_message(DBusMessage *msg);
	int __browser_send_request_service_browser_new(DBusConnection *conn);
	void __browser__process_signal(DBusMessage *msg, const char *signalName);
	int __browser_setup_dbus_conn(DBusConnection **conn);
//}}


// ============================================================================================
// ============================================================================================
// ============================================================================================






/**
 * Sets up access to DBus
 *
 * Returns the DBusConnection in *bp
 */
BrowserReturnCode
browser_setup_dbus_conn(DBusConnection **conn) {

	BrowserReturnCode ret=BROWSER_OK;
	DBusError error;

	dbus_error_init(&error);

	*conn = dbus_bus_get (DBUS_BUS_SYSTEM, &error);
	if (NULL==*conn) {
		ret=BROWSER_DBUS_CONN_ERROR;
		goto fail;
	}

	//DBGMSG("> Configuring filter function\n");
	// Configure the filter function
	if (!dbus_connection_add_filter (*conn, __browser_filter_func, NULL, NULL)) {
		ret=BROWSER_DBUS_FILTER_ERROR;
		goto fail;
	}

	dbus_error_init (&error);

	//DBGMSG("> Configuring inteface match rule\n");
	dbus_bus_add_match(*conn, "interface=org.freedesktop.Avahi.ServiceBrowser", &error);
	if (dbus_error_is_set(&error)) {
		ret=BROWSER_DBUS_MATCH_ERROR;
		goto fail;
	}

	//DBGMSG("> Registering Client to Avahi ServiceBrowser\n");
	if (!__browser_send_request_service_browser_new(*conn)) {
		ret = BROWSER_DBUS_SERVICE_ERROR;
		goto fail;
	}

	return BROWSER_OK;

fail:

	if (*conn) {
		dbus_connection_unref(*conn);
	}

	dbus_error_free (&error);
	return ret;
}//

int
__browser_send_request_service_browser_new(DBusConnection *conn) {

	//DBGMSG(" > Preparing DBus method call\n");
	DBusMessage *msg=dbus_message_new_method_call("org.freedesktop.Avahi",
									 "/",
									 "org.freedesktop.Avahi.Server",
									 "ServiceBrowserNew");
	if (NULL==msg)
		return FALSE;

	/*
	 * <method name="ServiceBrowserNew">
	 * 	<arg name="interface" type="i"  direction="in"/>
	 *  <arg name="protocol"  type="i"  direction="in"/>
	 *  <arg name="type"      type="s"  direction="in"/>
	 *  <arg name="domain"    type="s"  direction="in"/>
	 *  <arg name="flags"     type="u"  direction="in"/>
	 *
	 *  <arg name="path"      type="o"  direction="out"/>
	 * </method>
	 */
	int32_t interface = -1; //unspecified
	int32_t protocol  = -1; //unspecified
	uint32_t zero = 0;
	const char *sname  = "_http._tcp";
	const char *domain = "";

	// We must ask Avahi to communicate its results on the DBus
	// There must be at least 1 browser client on the DBus or
	// else Avahi won't generate signals on DBus
	int result = dbus_message_append_args(
			msg,
			DBUS_TYPE_INT32,   &interface,
			DBUS_TYPE_INT32,   &protocol,
			DBUS_TYPE_STRING,  &sname,
			DBUS_TYPE_STRING,  &domain,
			DBUS_TYPE_UINT32,  &zero,
			DBUS_TYPE_INVALID);

	if (!result) {
		DBGLOG(LOG_ERR, "browser: error building message");
		return FALSE;
	}

	dbus_message_set_destination(msg, "org.freedesktop.Avahi");

	//DBGMSG(" > Sending on DBus\n");
	result=dbus_connection_send(conn, msg, NULL);
	if (!result) {
		DBGLOG(LOG_ERR, "browser: error sending message");
	}

	//DBGMSG(" > Releasing message\n");
	dbus_message_unref(msg);

	return result;

}//



// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------




DBusHandlerResult
__browser_filter_func (DBusConnection *connection,
						DBusMessage   *message,
						void          *_unused)
{
	//DBGLOG(LOG_INFO, "ingress_filter_func, conn: %i  message: %i", connection, message);

	if (dbus_message_is_signal (message, DBUS_INTERFACE_LOCAL, "Disconnected")) {
		cout << "{'signal':'disconnected'};\n";
		cout.flush();

	} else {

		// main message handling starts here
		__browser_handle_message(message);
	}

	return DBUS_HANDLER_RESULT_HANDLED;
}//

void
__browser_handle_message(DBusMessage *msg) {

	int mtype = dbus_message_get_type(msg);
	if (DBUS_MESSAGE_TYPE_ERROR==mtype) {
		//const char *ename = dbus_message_get_error_name(msg);
		//printf("--> Error: %s\n", ename);
		return;
	}

	// we only care about signals
	if (DBUS_MESSAGE_TYPE_SIGNAL != mtype) {
		return;
	}

	// signal name e.g. ItemNew , ItemRemove
	const char *signalName = dbus_message_get_member(msg);
	if (NULL==signalName) {
		// paranoia... shouldn't happen
		return;
	}

	if (0==strcmp(signalName, "CacheExhausted")) {
		cout << "{'signal':'cache_exhausted'};\n";
		cout.flush();
	}

	if (0==strcmp(signalName, "AllForNow")) {
		cout << "{'signal':'all_for_now'};\n";
		cout.flush();
	}

	if ((0==strcmp(signalName, "ItemNew")) || (0==strcmp(signalName, "ItemRemove"))) {
		__browser__process_signal(msg, signalName);
	}
}//

/*
    <signal name="ItemNew">
      <arg name="interface" type="i"/>
      <arg name="protocol"  type="i"/>
      <arg name="name"      type="s"/>
      <arg name="type"      type="s"/>
      <arg name="domain"    type="s"/>
      <arg name="flags"     type="u"/>
    </signal>

    <signal name="ItemRemove">
      <arg name="interface" type="i"/>
      <arg name="protocol"  type="i"/>
      <arg name="name"      type="s"/>
      <arg name="type"      type="s"/>
      <arg name="domain"    type="s"/>
      <arg name="flags"     type="u"/>
    </signal>
 */
void
__browser__process_signal(DBusMessage *msg, const char *signalName) {

	dbus_int32_t interface, protocol;
	dbus_uint32_t flags;
	char *name=NULL, *type=NULL, *domain=NULL;

	DBusError error;
	dbus_error_init (&error);

    dbus_message_get_args(
            msg, &error,
            DBUS_TYPE_INT32, &interface,
            DBUS_TYPE_INT32, &protocol,
            DBUS_TYPE_STRING, &name,
            DBUS_TYPE_STRING, &type,
            DBUS_TYPE_STRING, &domain,
            DBUS_TYPE_UINT32, &flags,
            DBUS_TYPE_INVALID);
    if (dbus_error_is_set(&error)) {
    	DBGLOG(LOG_ERR, "browser: error whilst decoding event: (%s)", error.message);
    	return;
    }

	// =============================================

	cout << "{ 'signal':'" << signalName << "', 'interface':'" << interface << "',";
	cout << " 'protocol':'" << protocol << "', 'name':'" << name << "','type':'" << type << "',";
	cout << " 'domain':'" << domain << "', 'flags':'" << flags << "' };\n";
	cout.flush();

}//
