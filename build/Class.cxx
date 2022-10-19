// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Class
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/data/home/zhipz/work/scripts/include/Photon_bunches.h"
#include "/data/home/zhipz/work/scripts/include/events.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_Photon_bunches(void *p = 0);
   static void *newArray_Photon_bunches(Long_t size, void *p);
   static void delete_Photon_bunches(void *p);
   static void deleteArray_Photon_bunches(void *p);
   static void destruct_Photon_bunches(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Photon_bunches*)
   {
      ::Photon_bunches *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Photon_bunches >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Photon_bunches", ::Photon_bunches::Class_Version(), "include/Photon_bunches.h", 6,
                  typeid(::Photon_bunches), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Photon_bunches::Dictionary, isa_proxy, 4,
                  sizeof(::Photon_bunches) );
      instance.SetNew(&new_Photon_bunches);
      instance.SetNewArray(&newArray_Photon_bunches);
      instance.SetDelete(&delete_Photon_bunches);
      instance.SetDeleteArray(&deleteArray_Photon_bunches);
      instance.SetDestructor(&destruct_Photon_bunches);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Photon_bunches*)
   {
      return GenerateInitInstanceLocal((::Photon_bunches*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Photon_bunches*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_events(void *p = 0);
   static void *newArray_events(Long_t size, void *p);
   static void delete_events(void *p);
   static void deleteArray_events(void *p);
   static void destruct_events(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::events*)
   {
      ::events *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::events >(0);
      static ::ROOT::TGenericClassInfo 
         instance("events", ::events::Class_Version(), "include/events.h", 4,
                  typeid(::events), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::events::Dictionary, isa_proxy, 4,
                  sizeof(::events) );
      instance.SetNew(&new_events);
      instance.SetNewArray(&newArray_events);
      instance.SetDelete(&delete_events);
      instance.SetDeleteArray(&deleteArray_events);
      instance.SetDestructor(&destruct_events);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::events*)
   {
      return GenerateInitInstanceLocal((::events*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::events*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Photon_bunches::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Photon_bunches::Class_Name()
{
   return "Photon_bunches";
}

//______________________________________________________________________________
const char *Photon_bunches::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Photon_bunches*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Photon_bunches::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Photon_bunches*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Photon_bunches::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Photon_bunches*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Photon_bunches::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Photon_bunches*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr events::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *events::Class_Name()
{
   return "events";
}

//______________________________________________________________________________
const char *events::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::events*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int events::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::events*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *events::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::events*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *events::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::events*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Photon_bunches::Streamer(TBuffer &R__b)
{
   // Stream an object of class Photon_bunches.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Photon_bunches::Class(),this);
   } else {
      R__b.WriteClassBuffer(Photon_bunches::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Photon_bunches(void *p) {
      return  p ? new(p) ::Photon_bunches : new ::Photon_bunches;
   }
   static void *newArray_Photon_bunches(Long_t nElements, void *p) {
      return p ? new(p) ::Photon_bunches[nElements] : new ::Photon_bunches[nElements];
   }
   // Wrapper around operator delete
   static void delete_Photon_bunches(void *p) {
      delete ((::Photon_bunches*)p);
   }
   static void deleteArray_Photon_bunches(void *p) {
      delete [] ((::Photon_bunches*)p);
   }
   static void destruct_Photon_bunches(void *p) {
      typedef ::Photon_bunches current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Photon_bunches

//______________________________________________________________________________
void events::Streamer(TBuffer &R__b)
{
   // Stream an object of class events.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(events::Class(),this);
   } else {
      R__b.WriteClassBuffer(events::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_events(void *p) {
      return  p ? new(p) ::events : new ::events;
   }
   static void *newArray_events(Long_t nElements, void *p) {
      return p ? new(p) ::events[nElements] : new ::events[nElements];
   }
   // Wrapper around operator delete
   static void delete_events(void *p) {
      delete ((::events*)p);
   }
   static void deleteArray_events(void *p) {
      delete [] ((::events*)p);
   }
   static void destruct_events(void *p) {
      typedef ::events current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::events

namespace {
  void TriggerDictionaryInitialization_libClass_Impl() {
    static const char* headers[] = {
"/data/home/zhipz/work/scripts/include/Photon_bunches.h",
"/data/home/zhipz/work/scripts/include/events.h",
0
    };
    static const char* includePaths[] = {
"/data/home/zhipz/root/include",
"/data/home/zhipz/work/scripts",
"/data/home/zhipz/root/include/",
"/data/home/zhipz/work/scripts/build/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libClass dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$/data/home/zhipz/work/scripts/include/Photon_bunches.h")))  Photon_bunches;
class __attribute__((annotate("$clingAutoload$/data/home/zhipz/work/scripts/include/events.h")))  events;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libClass dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/data/home/zhipz/work/scripts/include/Photon_bunches.h"
#include "/data/home/zhipz/work/scripts/include/events.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Photon_bunches", payloadCode, "@",
"events", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libClass",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libClass_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libClass_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libClass() {
  TriggerDictionaryInitialization_libClass_Impl();
}
