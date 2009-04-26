/**\file*********************************************************************
 *                                                                     \brief
 *  18.6 Type identification [support.rtti]
 *
 ****************************************************************************
 */

#ifndef NTL__STLX_TYPEINFO
#define NTL__STLX_TYPEINFO

/**\addtogroup  lib_language_support *** 18 Language support library [language.support]
 * @{ */
/**\defgroup    lib_support_rtti ******* 18.6 Type identification [support.rtti]
 * @{ */

#ifndef STLX__USE_RTTI
  #if defined(_MSC_VER)
    #ifdef _CPPRTTI
      #ifdef NTL_KM
        #pragma message("Kernel mode RTTI support was not implemented yet")
        #define STLX__USE_RTTI 0
      #else
        #define STLX__USE_RTTI 1
      #endif
    #else
      #define STLX__USE_RTTI 0
    #endif
  #elif defined(__GNUC__)
    #ifdef __GXX_RTTI
      #define STLX__USE_RTTI 1
    #else
      #define STLX__USE_RTTI 0
    #endif
  #else
    #error define STLX__USE_RTTI
  #endif
#endif

#include "cstring.hxx" // for std::strcmp
#include "exception.hxx"

#pragma warning(push)
#pragma warning(disable:4820) // '3' bytes padding added after data member 'type_info::mname'

/**
 *	@brief 18.7.1 Class type_info [type.info] (N2857)
 *  @details The class type_info describes type information generated by the implementation. Objects of this class
 *  effectively store a pointer to a name for the type, and an encoded value suitable for comparing two types for
 *  equality or collating order.
 **/
class type_info
{
  public:
    /** type_info destructor */

    virtual ~type_info();

    /**
     *	@brief Compares the current object with \c rhs.
     *	@return true if the two values describe the same type.
     **/
    bool operator==(const type_info& rhs) const
    {
      return std::strcmp(mname+1, rhs.mname+1) == 0;
    }

    /**
     *	@brief Compares the current object with \c rhs.
     *	@return true if the two values describe the different types.
     **/
    bool operator!=(const type_info& rhs) const
    {
      return std::strcmp(mname+1, rhs.mname+1) != 0;
    }

    /**
     *	@brief Compares the current object with \c rhs.
     *	@return true if \c *this precedes \c rhs in the implementation's collation order.
     **/
    bool before(const type_info& rhs) const __ntl_nothrow
    {
      return std::strcmp(mname+1, rhs.mname+1) <= 0;
    }

    /** Hashes this object */
    size_t hash_code() const __ntl_nothrow
    {
      return reinterpret_cast<size_t>(this);
    }

    /**
     *	@brief Returns an implementation-defined NTBS.
     *	@return a null-terminated string representing the human-readable name of the type.
     **/
    const char* name() const  __ntl_nothrow
#if !STLX__USE_RTTI
    {
      return mname;
    }
#else
    ;
#endif
  private:
    type_info(const type_info&) __deleted;
    type_info& operator=(const type_info&) __deleted;
    mutable void* data;
    char    mname[1];
};

#ifndef __ICL
inline type_info::~type_info()
{}
#endif
#pragma warning(pop)

/** @} lib_support_rtti */
/** @} lib_language_support */

namespace std
{
/**\addtogroup  lib_language_support *** 18 Language support library [language.support]
 * @{ */
/**\addtogroup  lib_support_rtti ******* 18.6 Type identification [support.rtti]
 * @{ */

#if STLX__USE_RTTI

/// 18.7.1 Class type_info [type.info] (N2857)
using ::type_info;

#endif//if STLX__USE_RTTI

/**
 *	@brief 18.7.2 Class type_index [type.index] (N2857)
 *  @details The class type_index provides a simple wrapper for type_info which can be used as an index type in
 *  associative containers and in unordered associative containers.
 **/
class type_index
{
public:
  /** Constructs type_index object which holds the type_info object */
  type_index(const type_info& rhs)
    :target(&rhs)
  {}
  ///\name Comparsion operators
  bool operator==(const type_index& rhs) const  { return *target == *rhs.target; }
  bool operator!=(const type_index& rhs) const  { return *target != *rhs.target; }
  bool operator< (const type_index& rhs) const  { return target->before(*rhs.target); }
  bool operator<= (const type_index& rhs)const  { return !rhs.target->before(*target); }
  bool operator> (const type_index& rhs) const  { return rhs.target->before(*rhs.target); }
  bool operator>= (const type_index& rhs)const  { return !target->before(*rhs.target); }
  ///\}

  /** Returns the hash value of the stored type_info object */
  size_t hash_code() const __ntl_nothrow { return target->hash_code(); }
  /** Returns the type name of the stored type_info object */
  const char* name() const __ntl_nothrow { return target->name(); }
private:
  const type_info* target;
};

/// 18.7.3 Class bad_cast [bad.cast] (N2857)
class bad_cast: public exception
{
public:
  bad_cast() __ntl_nothrow
  {}
  bad_cast(const bad_cast&) __ntl_nothrow
  {}
  bad_cast& operator=(const bad_cast&) __ntl_nothrow
  { return *this; }
  ~bad_cast() __ntl_nothrow
  {}

  virtual const char* what() const __ntl_nothrow
  {
    return "bad_cast";
  }
};

/// 18.7.4 Class bad_typeid [bad.typeid] (N2857)
class bad_typeid: public exception {
public:
  bad_typeid() __ntl_nothrow
  {}
  bad_typeid(const bad_typeid&) __ntl_nothrow
  {}
  bad_typeid& operator=(const bad_typeid&) __ntl_nothrow
  { return *this; }
  ~bad_typeid() __ntl_nothrow
  {}

  virtual const char* what() const __ntl_nothrow
  {
    return "bad_typeid";
  }
};

/** @} lib_support_rtti */
/** @} lib_language_support */
}//namespace std

#endif//#ifndef NTL__STLX_TYPEINFO
