/**
 * @file   plugin.cc
 *
 * @date   2009-10-26
 * @author jldupont
 *
 *
 *
 */
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>

#include "npapi.h"
#include "plugin.h"
#include "macros.h"


#define PLUGIN_NAME        "DBus-mdns adapter"
#define PLUGIN_DESCRIPTION PLUGIN_NAME " Serves as 'dbus-mdns ServiceBrowser for discovering HTTP services"
#define PLUGIN_VERSION     "1.0.0.0"
#define PLUGIN_MIME        "application/dbusmdns:dbmdns:dbusmdns"

#define SERVICE_BROWSER_PATH "ServiceBrowserJSON"

// Working directory
std::string wdir;

// The path to the ServiceBrowserJSON executable
std::string sbpath;

// Browser API functions
static NPNetscapeFuncs* sBrowserFuncs = NULL;

static void
fillPluginFunctionTable(NPPluginFuncs* pFuncs)
{
  pFuncs->version = 11;
  pFuncs->size = sizeof(*pFuncs);
  pFuncs->newp          = NPP_New;
  pFuncs->destroy       = NPP_Destroy;
  pFuncs->setwindow     = NULL; //NPP_SetWindow;
  pFuncs->newstream     = NULL; //NPP_NewStream;
  pFuncs->destroystream = NULL; //NPP_DestroyStream;
  pFuncs->asfile        = NULL; //NPP_StreamAsFile;
  pFuncs->writeready    = NULL; //NPP_WriteReady;
  pFuncs->write         = NULL; //NPP_Write;
  pFuncs->print         = NULL; //NPP_Print;
  pFuncs->event         = NULL; //NPP_HandleEvent;
  pFuncs->urlnotify     = NULL; //NPP_URLNotify;
  pFuncs->getvalue      = NPP_GetValue;
  pFuncs->setvalue      = NULL; //NPP_SetValue;
}


NP_EXPORT(NPError)
NP_Initialize(NPNetscapeFuncs* bFuncs, NPPluginFuncs* pFuncs) {
	DBGLOG(LOG_INFO, "NP_Initialize");

	sBrowserFuncs = bFuncs;
	fillPluginFunctionTable(pFuncs);

	return NPERR_NO_ERROR;
}

NP_EXPORT(char*)
NP_GetPluginVersion() {
  return PLUGIN_VERSION;
}

NP_EXPORT(char*)
NP_GetMIMEDescription() {
  return PLUGIN_MIME;
}

/**
 * This function gets called 2times during plugin discovery:
 * - once with "variable=1"
 * - once with "variable=2"
 */
NP_EXPORT(NPError)
NP_GetValue(void* future, NPPVariable aVariable, void* aValue) {

	DBGLOG(LOG_INFO, "NP_GetValue, variable: %i", aVariable);

	switch (aVariable) {
	case NPPVpluginNameString:
		*((char**)aValue) = PLUGIN_NAME;
		break;
	case NPPVpluginDescriptionString:
		*((char**)aValue) = PLUGIN_DESCRIPTION;
		break;
	default:
		return NPERR_INVALID_PARAM;
		break;
	}
	return NPERR_NO_ERROR;
}

NP_EXPORT(NPError)
NP_Shutdown() {
	DBGLOG(LOG_INFO, "NP_Shutdown");
	return NPERR_NO_ERROR;
}

/**
 * New plugin instance
 */
NPError
NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char* argn[], char* argv[], NPSavedData* saved) {

	DBGLOG(LOG_INFO, "NPP_New");

	sBrowserFuncs->setvalue(instance, NPPVpluginWindowBool, (void*)false);

	// set up our our instance data
	InstanceData* instanceData = (InstanceData*)malloc(sizeof(InstanceData));
	if (!instanceData)
		return NPERR_OUT_OF_MEMORY_ERROR;

	memset(instanceData, 0, sizeof(InstanceData));

	instance->pdata = instanceData;

	instanceData->npo=NULL;
	instanceData->sBrowserFuncs = sBrowserFuncs;

	return NPERR_NO_ERROR;
}

NPError
NPP_Destroy(NPP instance, NPSavedData** save) {

	DBGLOG(LOG_INFO, "NPP_Destroy - start");

	InstanceData *instanceData = (InstanceData*)(instance->pdata);
	if (NULL!=instanceData) {
		free(instance->pdata);
		instance->pdata=NULL;
	}

	DBGLOG(LOG_INFO, "NPP_Destroy - end");

	return NPERR_NO_ERROR;
}

NPError
NPP_GetValue(NPP instance, NPPVariable variable, void *value) {

	DBGLOG(LOG_INFO, "NPP_GetValue, variable: %i", variable);

	InstanceData *instanceData = (InstanceData*)(instance->pdata);

	NPError rv = NPERR_NO_ERROR;
	switch(variable) {
	case NPPVpluginNameString:
		value = *((char **)value) = PLUGIN_NAME;
		break;
	case NPPVpluginDescriptionString:
		*((char **)value) = PLUGIN_DESCRIPTION;
		break;
	case NPPVpluginScriptableNPObject:
		if (NULL!=instanceData->npo) {
		  *(NPObject **)value = instanceData->npo;
		} else {
			//instanceData->npo = (*sBrowserFuncs->createobject)(instance, &NPBrowser::_npclass);
			*(NPObject **)value = instanceData->npo;
		}
		break;
	default:
		rv = NPERR_GENERIC_ERROR;
	}
	return rv;
}

namespace {
    class dynamic_library_load_unload_handler {
         public:
              dynamic_library_load_unload_handler(){
            	    Dl_info dl_info;
            	    dladdr((void *) NP_Initialize, &dl_info);
            	    std::string path(dl_info.dli_fname);
            	    wdir = path.substr( 0, path.find_last_of( '/' ) +1 );
            	    sbpath = wdir + SERVICE_BROWSER_PATH;
              }
              ~dynamic_library_load_unload_handler(){
              }
    } dynamic_library_load_unload_handler_hook;
}
