/**
 * @file np_sb.cc
 *
 *  Created on: 2009-11-16
 *      Author: jldupont
 */

#include "np_sb.h"

#include <string.h>
#include <stdlib.h>

#include "plugin.h"
#include "macros.h"

#define POPMSG_METHOD    "popmsg"

NPClass NP_ServiceBrowser::_npclass = {
    NP_CLASS_STRUCT_VERSION,
    NP_ServiceBrowser::Allocate,
    NP_ServiceBrowser::_NPDeallocate,
    NP_ServiceBrowser::_NPInvalidate,
    NP_ServiceBrowser::_NPHasMethod,
    NP_ServiceBrowser::_NPInvoke,
    NP_ServiceBrowser::_NPInvokeDefault,
    NP_ServiceBrowser::_NPHasProperty,
    NP_ServiceBrowser::_NPGetProperty,
    NP_ServiceBrowser::_NPSetProperty,
    NP_ServiceBrowser::_NPRemoveProperty,
    NP_ServiceBrowser::_NPEnumeration,
    NP_ServiceBrowser::_NPConstruct
};

void NP_ServiceBrowser::_NPInvalidate(NPObject *obj) {
    ((NP_ServiceBrowser*)obj)->Invalidate();
}
// static
void NP_ServiceBrowser::_NPDeallocate(NPObject *obj) {
    ((NP_ServiceBrowser*)obj)->Deallocate();
    delete ((NP_ServiceBrowser*)obj);
}
bool NP_ServiceBrowser::_NPHasMethod(NPObject *obj, NPIdentifier name) {
	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::_NPHasMethod");
	return ((NP_ServiceBrowser*)obj)->HasMethod(name);
}
bool NP_ServiceBrowser::_NPInvoke(NPObject *obj, NPIdentifier name, const NPVariant *args, uint32_t argCount,NPVariant *result) {
	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::_NPInvoke");
	return ((NP_ServiceBrowser*)obj)->Invoke(name, args, argCount, result);
}
bool NP_ServiceBrowser::_NPInvokeDefault(NPObject *obj,const NPVariant *args,uint32_t argCount,NPVariant *result) {
	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::_NPInvokeDefault");
	return ((NP_ServiceBrowser*)obj)->InvokeDefault(args, argCount, result);
}
bool NP_ServiceBrowser::_NPHasProperty(NPObject *obj, NPIdentifier name) {
	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::_NPHasProperty, name: %i", name);
	return ((NP_ServiceBrowser*)obj)->HasProperty(name);
}
bool NP_ServiceBrowser::_NPGetProperty(NPObject *obj, NPIdentifier name,NPVariant *result) {
	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::_NPGetProperty, name: %i", name);
	return ((NP_ServiceBrowser*)obj)->GetProperty(name, result);
}
bool NP_ServiceBrowser::_NPSetProperty(NPObject *obj, NPIdentifier name,const NPVariant *value) {
	return ((NP_ServiceBrowser*)obj)->SetProperty(name, value);
}
bool NP_ServiceBrowser::_NPRemoveProperty(NPObject *obj,NPIdentifier name) {
	return ((NP_ServiceBrowser*)obj)->RemoveProperty(name);
}
bool NP_ServiceBrowser::_NPEnumeration(NPObject *obj, NPIdentifier **value,uint32_t *count) {
	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::_NPEnumeration");
    return ((NP_ServiceBrowser*)obj)->Enumeration(value, count);
}
bool NP_ServiceBrowser::_NPConstruct(NPObject *obj,const NPVariant *args,uint32_t argCount,NPVariant *result) {
	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::_NPConstruct");
    return ((NP_ServiceBrowser*)obj)->Construct(args, argCount, result);
}



// --------------------------------------------------------------------------------------------
// IMPLEMENTATION
// --------------------------------------------------------------------------------------------



NP_ServiceBrowser::NP_ServiceBrowser(NPP npp) {
	DBGLOG(LOG_INFO, "NP_ServiceBrowser::NP_ServiceBrowser(npp)");
	m_Instance=npp;
}//

NP_ServiceBrowser::~NP_ServiceBrowser() {
	DBGLOG(LOG_INFO, "NP_ServiceBrowser::~NP_ServiceBrowser()");
}

void
NP_ServiceBrowser::Deallocate() {

	DBGLOG(LOG_INFO, "NP_ServiceBrowser::Deallocate()");

}

void NP_ServiceBrowser::Invalidate() {
	DBGLOG(LOG_INFO, "NP_ServiceBrowser::Invalidate()");
}

bool NP_ServiceBrowser::HasMethod(NPIdentifier name) {

	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::HasMethod, name: %i", name );

	InstanceData* instanceData = (InstanceData *) m_Instance->pdata;
	NPUTF8 *nname = (*instanceData->sBrowserFuncs->utf8fromidentifier)(name);

	// only "popmsg" method is supported.
	// **********************************
	bool result=(strcmp(POPMSG_METHOD,    nname)==0);

	(*instanceData->sBrowserFuncs->memfree)(nname);

	return result;
}

bool NP_ServiceBrowser::Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result) {

	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::Invoke");

	InstanceData* instanceData = (InstanceData *)m_Instance->pdata;

	NPUTF8 *nname = (*instanceData->sBrowserFuncs->utf8fromidentifier)(name);

	bool r=(strcmp(POPMSG_METHOD, nname)==0);
	(*instanceData->sBrowserFuncs->memfree)(nname);

	if (!r)
		return false;

	std::string msg;
	char *pstr;

	if (sb.getState() != ServiceBrowser::READY) {
		DBGLOG(LOG_ERR, "Service Browser not available!");
		static char NA[]="{'signal':'not_available'};";
		pstr=(char *) malloc(sizeof(NA)+1);
		strcpy(pstr, NA);
	} else {
		msg=sb.popmsg();
		if (msg.size()==0) {
			NULL_TO_NPVARIANT(*result);
			return true;
		}

		pstr=(char *)malloc(msg.size()+1);
		strcpy(pstr, msg.data());
	}

	STRINGZ_TO_NPVARIANT(pstr, *result);

	return true;
}//

bool NP_ServiceBrowser::InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result) {
	return false;
}

bool NP_ServiceBrowser::HasProperty(NPIdentifier name) {
	return false;
}

bool NP_ServiceBrowser::GetProperty(NPIdentifier name, NPVariant *result) {
	return false;
}

bool NP_ServiceBrowser::SetProperty(NPIdentifier name, const NPVariant *value) {
	return false;
}

bool NP_ServiceBrowser::RemoveProperty(NPIdentifier name) {
	return false;
}

bool NP_ServiceBrowser::Enumeration(NPIdentifier **identifier, uint32_t *count) {
	return false;
}

bool NP_ServiceBrowser::Construct(const NPVariant *args, uint32_t argCount, NPVariant *result) {
	return false;
}


// =========================================================

void NP_ServiceBrowser::setSBPath(std::string sbpath) {
	//DBGLOG(LOG_INFO, "Setting SBPath: %s", sbpath.data());
	sb.setSBPath(sbpath);
}

void NP_ServiceBrowser::init(void) {
	// status can be read later
	sb.init();
	//DBGLOG(LOG_INFO, "NP_ServiceBrowser::init() -> %i", result);
}

