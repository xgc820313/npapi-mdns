/**
 * @file np_sb.h
 *
 *  Created on: 2009-11-16
 *      Author: jldupont
 */

#ifndef NP_SB_H_
#define NP_SB_H_

	#include "npapi.h"
	#include "npfunctions.h"
	#include "sb.h"

  class NP_ServiceBrowser: public NPObject {

  public:
	  virtual ~NP_ServiceBrowser();

  protected:
	  NP_ServiceBrowser(NPP npp);

	  void Invalidate(void);
	  bool HasMethod(NPIdentifier name);
	  bool Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
	  bool InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result);
	  bool HasProperty(NPIdentifier name);
	  bool GetProperty(NPIdentifier name, NPVariant *result);
	  bool SetProperty(NPIdentifier name, const NPVariant *value);
	  bool RemoveProperty(NPIdentifier name);
	  bool Enumeration(NPIdentifier **identifier, uint32_t *count);
	  bool Construct(const NPVariant *args, uint32_t argCount, NPVariant *result);


  public:

	  static NPObject* Allocate(NPP npp, NPClass *aClass) {
		  return (NPObject *)new NP_ServiceBrowser(npp);
	  }
	  void Deallocate(void);

	  static NPClass _npclass;

	  /*
	   * NPClass interface
	   */
	  static NPObject *(*_NPAllocateFunctionPtr)(NPP npp, NPClass *aClass);
	  static void _NPDeallocate(NPObject *npobj);
	  static void _NPInvalidate(NPObject *npobj);
	  static bool _NPHasMethod(NPObject *npobj, NPIdentifier name);
	  static bool _NPInvoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount,NPVariant *result);
	  static bool _NPInvokeDefault(NPObject *npobj,const NPVariant *args,uint32_t argCount,NPVariant *result);
	  static bool _NPHasProperty(NPObject *npobj, NPIdentifier name);
	  static bool _NPGetProperty(NPObject *npobj, NPIdentifier name,NPVariant *result);
	  static bool _NPSetProperty(NPObject *npobj, NPIdentifier name,const NPVariant *value);
	  static bool _NPRemoveProperty(NPObject *npobj,NPIdentifier name);
	  static bool _NPEnumeration(NPObject *npobj, NPIdentifier **value,uint32_t *count);
	  static bool _NPConstruct(NPObject *npobj,const NPVariant *args,uint32_t argCount,NPVariant *result);

  protected:
      NPP m_Instance;
      ServiceBrowser sb;

      // API
  public:
      void setSBPath(std::string sbpath);
      void init(void);
  };


#endif /* NP_SB_H_ */
