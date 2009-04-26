/**\file*********************************************************************
 *                                                                     \brief
 *  new and delete operators
 *
 ****************************************************************************
 */

#ifndef NTL__NT_NEW
#define NTL__NT_NEW

#ifndef NTL_NO_NEW

#include "heap.hxx"
#include "../stlx/new.hxx"

#ifdef __ICL
#pragma warning(disable:522) // function attribute redeclared after called
#endif

namespace ntl
{
  extern "C" volatile std::new_handler __new_handler;
  extern "C" __declspec(selectany) std::new_handler const __new_handler_null_stub = 0;
#ifdef _M_X64
  #pragma comment(linker, "/alternatename:__new_handler=__new_handler_null_stub")
#else
  #pragma comment(linker, "/alternatename:___new_handler=___new_handler_null_stub")
#endif
}

///\name  Single-object forms

__forceinline
void* __cdecl operator new(std::size_t size) throw(std::bad_alloc)
{
  void* ptr;
  for(;;) {
    ptr = ntl::nt::heap::alloc(ntl::nt::process_heap(), size);
    if(ptr)
      return ptr;

    std::new_handler nh = ntl::__new_handler;
  #if STLX__USE_EXCEPTIONS
    if(!nh)
      __ntl_throw(std::bad_alloc());
  #else
    if(!nh)
      std::abort();
  #endif
    nh();
  }
}

__forceinline
void __cdecl operator delete(void* ptr) __ntl_nothrow
{
  ntl::nt::heap::free(ntl::nt::process_heap(), ptr);
}

__forceinline
void* __cdecl operator new(std::size_t size, const std::nothrow_t&) __ntl_nothrow
{
  void* ptr;
  for(;;) {
    ptr = ntl::nt::heap::alloc(ntl::nt::process_heap(), size);
    if(ptr)
      return ptr;

    std::new_handler nh = ntl::__new_handler;
    if(!nh)
      return 0;
#if STLX__USE_EXCEPTIONS == 1
    try {
      nh();
    } 
    catch(const std::bad_alloc&){
      return 0;
    }
#elif STLX__USE_EXCEPTIONS == 2
    __try {
      nh();
    }
    __except(1){
      return 0;
    }
#else
    return 0;
#endif
  } 
}

__forceinline
void __cdecl
  operator delete(void* ptr, const std::nothrow_t&) __ntl_nothrow
{
  ntl::nt::heap::free(ntl::nt::process_heap(), ptr);
}


///\name  Array forms

__forceinline
void* __cdecl operator new[](std::size_t size) throw(std::bad_alloc)
{
  return operator new(size);
}

__forceinline
void __cdecl operator delete[](void* ptr) __ntl_nothrow
{
  ntl::nt::heap::free(ntl::nt::process_heap(), ptr);
}

__forceinline
void* __cdecl operator new[](std::size_t size, const std::nothrow_t&) __ntl_nothrow
{
  return operator new(size, std::nothrow);
}

__forceinline
void __cdecl operator delete[](void* ptr, const std::nothrow_t&) __ntl_nothrow
{
  ntl::nt::heap::free(ntl::nt::process_heap(), ptr);
}

#ifdef __ICL
#pragma warning(default:444)
#endif

#endif//#ifndef NTL_NO_NEW

#endif//#ifndef NTL__NT_NEW
