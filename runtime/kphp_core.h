#pragma once

#include <tuple>
#include <utility>

#include "runtime/allocator.h"
#include "runtime/include.h"
#include "runtime/kphp_type_traits.h"
#include "runtime/profiler.h"
#include "common/algorithms/find.h"
#include "common/type_traits/list_of_types.h"

// order of includes below matters, be careful

#define INCLUDED_FROM_KPHP_CORE

#include "string_decl.inl"
#include "array_decl.inl"
#include "class_instance_decl.inl"
#include "variable_decl.inl"
#include "string_buffer_decl.inl"

#include "string.inl"
#include "array.inl"
#include "class_instance.inl"
#include "variable.inl"
#include "string_buffer.inl"
#include "comparison_operators.inl"

#undef INCLUDED_FROM_KPHP_CORE

#define unimplemented_function(name, args...) ({                              \
  php_critical_error ("unimplemented_function: %s", f$strval (name).c_str()); \
  1;                                                                          \
})

#define SAFE_SET_OP(a, op, b, b_type) ({b_type b_tmp___ = b; a op std::move(b_tmp___);})
#define SAFE_SET_FUNC_OP(a, func, b, b_type) ({b_type b_tmp___ = b; func (a, b_tmp___);})
#define SAFE_INDEX(a, b, b_type) a[({b_type b_tmp___ = b; b_tmp___;})]
#define SAFE_SET_VALUE(a, b, b_type, c, c_type) ({b_type b_tmp___ = b; c_type c_tmp___ = c; (a).set_value (b_tmp___, c_tmp___);})
#define SAFE_PUSH_BACK(a, b, b_type) ({b_type b_tmp___ = b; a.push_back (b_tmp___);})
#define SAFE_PUSH_BACK_RETURN(a, b, b_type) ({b_type b_tmp___ = b; a.push_back_return (b_tmp___);})
#define NOERR(a, a_type) ({php_disable_warnings++; a_type a_tmp___ = a; php_disable_warnings--; a_tmp___;})
#define NOERR_VOID(a) ({php_disable_warnings++; a; php_disable_warnings--;})

#define f$likely likely
#define f$unlikely unlikely

template<typename T, typename ...Args>
void hard_reset_var(T &var, Args &&... args) noexcept {
  new(&var) T(std::forward<Args>(args)...);
}
inline double divide(int lhs, int rhs);

inline double divide(double lhs, int rhs);

inline double divide(const string &lhs, int rhs);

inline double divide(const var &lhs, int rhs);


inline double divide(int lhs, double rhs);

inline double divide(double lhs, double rhs);

inline double divide(const string &lhs, double rhs);

inline double divide(const var &lhs, double rhs);


inline double divide(int lhs, const string &rhs);

inline double divide(double lhs, const string &rhs);

inline double divide(const string &lhs, const string &rhs);

inline double divide(const var &lhs, const string &rhs);


inline double divide(int lhs, const var &rhs);

inline double divide(double lhs, const var &rhs);

inline double divide(const string &lhs, const var &rhs);

inline double divide(const var &lhs, const var &rhs);


inline double divide(bool lhs, bool rhs);

template<class T>
inline double divide(bool lhs, const T &rhs);

template<class T>
inline double divide(const T &lhs, bool rhs);

template<class T>
inline double divide(bool lhs, const array<T> &rhs);

template<class T>
inline double divide(const array<T> &lhs, bool rhs);

template<class T>
inline double divide(bool lhs, const class_instance<T> &rhs);

template<class T>
inline double divide(const class_instance<T> &lhs, bool rhs);


template<class T, class T1>
inline double divide(const array<T> &lhs, const T1 &rhs);

template<class T, class T1>
inline double divide(const T1 &lhs, const array<T> &rhs);


template<class T, class T1>
inline double divide(const class_instance<T> &lhs, const T1 &rhs);

template<class T, class T1>
inline double divide(const T1 &lhs, const class_instance<T> &rhs);


template<class T>
inline double divide(const array<T> &lhs, const array<T> &rhs);

template<class T>
inline double divide(const class_instance<T> &lhs, const class_instance<T> &rhs);

template<class T, class T1>
inline double divide(const array<T> &lhs, const array<T1> &rhs);

template<class T, class T1>
inline double divide(const class_instance<T> &lhs, const class_instance<T1> &rhs);

template<class T, class T1>
inline double divide(const array<T> &lhs, const class_instance<T1> &rhs);

template<class T, class T1>
inline double divide(const class_instance<T1> &lhs, const array<T> &rhs);


template<class T1, class T2>
inline double divide(const Optional<T1> &lhs, const T2 &rhs); //not defined

template<class T1, class T2>
inline double divide(const T1 &lhs, const Optional<T2> &rhs); //not defined

template<class T1, class T2>
inline int spaceship(const T1 &lhs, const T2 &rhs) {
  if (lt(lhs, rhs)) {
    return -1;
  } else if (gt(lhs, rhs)) {
    return 1;
  } else {
    return 0;
  }
}

inline int modulo(int lhs, int rhs);

template<class T1, class T2>
inline int modulo(const T1 &lhs, const T2 &rhs);

inline int int_power(int base, int exp) {
  int res = 1;
  while (exp > 0) {
    if (exp & 1) {
      res *= base;
    }
    base *= base;
    exp >>= 1;
  }
  return res;
}

inline double float_power(double base, int exp) {
  return std::pow(base, exp);
}

inline var var_power(const var &base, const var &exp) {
  if (base.is_int() && exp.is_int() && exp.to_int() >= 0) {
    return int_power(base.to_int(), exp.to_int());
  }
  const double float_base = base.to_float();
  if (exp.is_int()) {
    return std::pow(float_base, exp.to_int());
  }

  const double float_exp = exp.to_float();
  if (float_exp == static_cast<int>(float_exp)) {
    return std::pow(float_base, float_exp);
  }

  if (base < 0.0) {
    php_warning("Calculating pow with negative base and double exp will produce zero");
    return 0.0;
  }

  return std::pow(float_base, float_exp);
}

inline int &power_self(int &base, int exp) {
  return base = int_power(base, exp);
}

inline double &power_self(double &base, int exp) {
  return base = float_power(base, exp);
}

inline var &power_self(var &base, const var &exp) {
  return base = var_power(base, exp);
}

template<class T1, class T2>
inline T1 &divide_self(T1 &lhs, const T2 &rhs);


inline int &modulo_self(int &lhs, int rhs);

template<class T1, class T2>
inline T1 &modulo_self(T1 &lhs, const T2 &rhs);


template<class T0, class T>
inline void assign(T0 &dest, const T &from);


template<class T, class = enable_for_bool_int_double<T>>
inline bool f$boolval(const T &val);

inline bool f$boolval(const string &val);

template<class T>
inline bool f$boolval(const array<T> &val);

template<class T>
inline bool f$boolval(const class_instance<T> &val);

template<class ...Args>
bool f$boolval(const std::tuple<Args...> &val);

template<class T>
inline bool f$boolval(const Optional<T> &val);

inline bool f$boolval(const var &val);


template<class T, class = enable_for_bool_int_double<T>>
inline int f$intval(const T &val);

inline int f$intval(const string &val);

template<class T>
inline int f$intval(const array<T> &val);

template<class T>
inline int f$intval(const class_instance<T> &val);

inline int f$intval(const var &val);

template<class T>
inline int f$intval(const Optional<T> &val);


inline int f$safe_intval(const bool &val);

inline const int &f$safe_intval(const int &val);

inline int f$safe_intval(const double &val);

inline int f$safe_intval(const string &val);

template<class T>
inline int f$safe_intval(const array<T> &val);

template<class T>
inline int f$safe_intval(const class_instance<T> &val);

inline int f$safe_intval(const var &val);


template<class T, class = enable_for_bool_int_double<T>>
double f$floatval(const T &val);

inline double f$floatval(const string &val);

template<class T>
inline double f$floatval(const array<T> &val);

template<class T>
inline double f$floatval(const class_instance<T> &val);

inline double f$floatval(const var &val);

template<class T>
inline double f$floatval(const Optional<T> &val);


inline string f$strval(const bool &val);

inline string f$strval(const int &val);

inline string f$strval(const double &val);

inline string f$strval(const string &val);

inline string &&f$strval(string &&val);

template<class T>
inline string f$strval(const array<T> &val);

template<class ...Args>
inline string f$strval(const std::tuple<Args...> &val);

template<class T>
inline string f$strval(const Optional<T> &val);

template<class T>
inline string f$strval(Optional<T> &&val);

template<class T>
inline string f$strval(const class_instance<T> &val);

inline string f$strval(const var &val);

inline string f$strval(var &&val);

template<class T>
inline array<T> f$arrayval(const T &val);

template<class T>
inline const array<T> &f$arrayval(const array<T> &val);

template<class T>
inline array<T> &&f$arrayval(array<T> &&val);

template<class T>
inline array<var> f$arrayval(const class_instance<T> &val);

inline array<var> f$arrayval(const var &val);

inline array<var> f$arrayval(var &&val);

template<class T>
inline array<T> f$arrayval(const Optional<T> &val);

template<class T>
inline array<T> f$arrayval(Optional<T> &&val);

template<class T>
inline array<T> f$arrayval(const Optional<array<T>> &val);

template<class T>
inline array<T> f$arrayval(Optional<array<T>> &&val);


inline bool &boolval_ref(bool &val);

inline bool &boolval_ref(var &val);

inline const bool &boolval_ref(const bool &val);

inline const bool &boolval_ref(const var &val);


inline int &intval_ref(int &val, const char *function);

inline int &intval_ref(var &val, const char *function);

inline const int &intval_ref(const int &val, const char *function);

inline const int &intval_ref(const var &val, const char *function);


inline double &floatval_ref(double &val);

inline double &floatval_ref(var &val);

inline const double &floatval_ref(const double &val);

inline const double &floatval_ref(const var &val);


inline string &strval_ref(string &val, const char *function);

inline string &strval_ref(var &val, const char *function);

inline const string &strval_ref(const string &val, const char *function);

inline const string &strval_ref(const var &val, const char *function);


template<class T>
inline array<T> &arrayval_ref(array<T> &val, const char *function);

inline array<var> &arrayval_ref(var &val, const char *function);

template<class T>
inline array<T> &arrayval_ref(Optional<array<T>> &val, const char *function);

template<class T>
inline const array<T> &arrayval_ref(const array<T> &val, const char *function);

inline const array<var> &arrayval_ref(const var &val, const char *function);

template<class T>
inline const array<T> &arrayval_ref(const Optional<array<T>> &val, const char *function);

template<class T, class = enable_for_bool_int_double<T>>
inline bool f$empty(const T &v);
inline bool f$empty(const string &v);
inline bool f$empty(const var &v);
template<class T>
inline bool f$empty(const array<T> &);
template<class T>
inline bool f$empty(const class_instance<T> &);
template<class T>
inline bool f$empty(const Optional<T> &);


template<class T>
bool f$is_numeric(const T &);
template<class T>
inline bool f$is_numeric(const Optional<T> &v);
inline bool f$is_numeric(const string &v);
inline bool f$is_numeric(const var &v);

template<class T>
inline bool f$is_bool(const T &v);
template<class T>
inline bool f$is_bool(const Optional<T> &v);
inline bool f$is_bool(const var &v);

template<class T>
inline bool f$is_int(const T &);
template<class T>
inline bool f$is_int(const Optional<T> &v);
inline bool f$is_int(const var &v);

template<class T>
inline bool f$is_float(const T &v);
template<class T>
inline bool f$is_float(const Optional<T> &v);
inline bool f$is_float(const var &v);


template<class T>
inline bool f$is_scalar(const T &v);
template<class T>
inline bool f$is_scalar(const Optional<T> &v);
inline bool f$is_scalar(const var &v);


template<class T>
inline bool f$is_string(const T &v);
template<class T>
inline bool f$is_string(const Optional<T> &v);
inline bool f$is_string(const var &v);


template<class T>
inline bool f$is_array(const T &v);
template<class T>
inline bool f$is_array(const Optional<T> &v);
inline bool f$is_array(const var &v);


template<class T>
bool f$is_object(const T &);
template<class T>
inline bool f$is_object(const class_instance<T> &v);


template<class T>
inline bool f$is_integer(const T &v);

template<class T>
inline bool f$is_long(const T &v);

template<class T>
inline bool f$is_double(const T &v);

template<class T>
inline bool f$is_real(const T &v);

template<class Derived, class Base>
inline bool f$is_a(const class_instance<Base> &base) {
  return base.template is_a<Derived>();
}

template<class ClassInstanceDerived, class Base>
inline ClassInstanceDerived f$instance_cast(const class_instance<Base> &base, const string &) {
  return base.template cast_to<typename ClassInstanceDerived::ClassType>();
}

inline const char *get_type_c_str(const bool &v);
inline const char *get_type_c_str(const int &v);
inline const char *get_type_c_str(const double &v);
inline const char *get_type_c_str(const string &v);
inline const char *get_type_c_str(const var &v);
template<class T>
inline const char *get_type_c_str(const array<T> &v);
template<class T>
inline const char *get_type_c_str(const class_instance<T> &v);

template<class T>
inline string f$get_type(const T &v);

inline string f$get_class(const bool &v);
inline string f$get_class(const int &v);
inline string f$get_class(const double &v);
inline string f$get_class(const string &v);
inline string f$get_class(const var &v);
template<class T>
inline string f$get_class(const array<T> &v);
template<class T>
inline string f$get_class(const class_instance<T> &v);

template<class T>
inline int f$get_hash_of_class(const class_instance<T> &klass);


inline int f$count(const var &v);

template<class T>
inline int f$count(const Optional<T> &a);

template<class T>
inline int f$count(const array<T> &a);

template<class ...Args>
inline int f$count(const std::tuple<Args...> &a);

template<class T>
inline int f$count(const T &v);


template<class T>
int f$sizeof(const T &v);


inline string &append(string &dest, const string &from);

template<class T>
inline string &append(Optional<string> &dest, const T &from);

template<class T>
inline string &append(string &dest, const T &from);

template<class T>
inline var &append(var &dest, const T &from);

template<class T0, class T>
inline T0 &append(T0 &dest, const T &from);


inline string f$gettype(const var &v);

template<class T>
inline bool f$function_exists(const T &a1);


const int E_ERROR = 1;
const int E_WARNING = 2;
const int E_PARSE = 4;
const int E_NOTICE = 8;
const int E_CORE_ERROR = 16;
const int E_CORE_WARNING = 32;
const int E_COMPILE_ERROR = 64;
const int E_COMPILE_WARNING = 128;
const int E_USER_ERROR = 256;
const int E_USER_WARNING = 512;
const int E_USER_NOTICE = 1024;
const int E_STRICT = 2048;
const int E_RECOVERABLE_ERROR = 4096;
const int E_DEPRECATED = 8192;
const int E_USER_DEPRECATED = 16384;
const int E_ALL = 32767;

inline var f$error_get_last();

inline int f$error_reporting(int level);

inline int f$error_reporting();

inline void f$warning(const string &message);

#define f$critical_error(message) \
  php_critical_error("%s", message.c_str());

inline int f$memory_get_static_usage();

inline int f$memory_get_peak_usage(bool real_usage = false);

inline int f$memory_get_usage(bool real_usage = false);

inline int f$memory_get_total_usage();


template<class T>
inline int f$get_reference_counter(const array<T> &v);

template<class T>
inline int f$get_reference_counter(const class_instance<T> &v);

inline int f$get_reference_counter(const string &v);

inline int f$get_reference_counter(const var &v);


template<class T>
inline T &val(T &x);

template<class T>
inline const T &val(const T &x);

template<class T>
inline T &ref(T &x);

template<class T>
inline const T &val(const Optional<T> &x);

template<class T>
inline T &val(Optional<T> &x);

template<class T>
inline T &ref(Optional<T> &x);


template<class T>
inline typename array<T>::iterator begin(array<T> &x);

template<class T>
inline typename array<T>::const_iterator begin(const array<T> &x);

template<class T>
inline typename array<T>::const_iterator const_begin(const array<T> &x);

inline array<var>::iterator begin(var &x);

inline array<var>::const_iterator begin(const var &x);

inline array<var>::const_iterator const_begin(const var &x);

template<class T>
inline typename array<T>::iterator begin(Optional<array<T>> &x);

template<class T>
inline typename array<T>::const_iterator begin(const Optional<array<T>> &x);

template<class T>
inline typename array<T>::const_iterator const_begin(const Optional<array<T>> &x);

template<class T>
inline typename array<T>::iterator end(array<T> &x);

template<class T>
inline typename array<T>::const_iterator end(const array<T> &x);

template<class T>
inline typename array<T>::const_iterator const_end(const array<T> &x);

inline array<var>::iterator end(var &x);

inline array<var>::const_iterator end(const var &x);

inline array<var>::const_iterator const_end(const var &x);

template<class T>
inline typename array<T>::iterator end(Optional<array<T>> &x);

template<class T>
inline typename array<T>::const_iterator end(const Optional<array<T>> &x);

template<class T>
inline typename array<T>::const_iterator const_end(const Optional<array<T>> &x);


inline void clear_array(var &v);

template<class T>
inline void clear_array(array<T> &a);

template<class T>
inline void clear_array(Optional<array<T>> &a);

template<class T>
inline void unset(array<T> &x);

template<class T>
inline void unset(class_instance<T> &x);

inline void unset(var &x);


/*
 *
 *     IMPLEMENTATION
 *
 */

double divide(int lhs, int rhs) {
  return divide(static_cast<double>(lhs), static_cast<double>(rhs));
}

double divide(double lhs, int rhs) {
  return divide(lhs, static_cast<double>(rhs));
}

double divide(const string &lhs, int rhs) {
  return divide(f$floatval(lhs), rhs);
}

double divide(const var &lhs, int rhs) {
  return divide(f$floatval(lhs), rhs);
}


double divide(int lhs, double rhs) {
  return divide(static_cast<double>(lhs), rhs);
}

double divide(double lhs, double rhs) {
  if (rhs == 0) {
    php_warning("Float division by zero");
  }

  return lhs / rhs;
}

double divide(const string &lhs, double rhs) {
  return divide(f$floatval(lhs), rhs);
}

double divide(const var &lhs, double rhs) {
  return divide(f$floatval(lhs), rhs);
}


double divide(int lhs, const string &rhs) {
  return divide(lhs, f$floatval(rhs));
}

double divide(double lhs, const string &rhs) {
  return divide(lhs, f$floatval(rhs));
}

double divide(const string &lhs, const string &rhs) {
  return divide(f$floatval(lhs), f$floatval(rhs));
}

double divide(const var &lhs, const string &rhs) {
  return divide(lhs, f$floatval(rhs));
}


double divide(int lhs, const var &rhs) {
  return divide(lhs, f$floatval(rhs));
}

double divide(double lhs, const var &rhs) {
  return divide(lhs, f$floatval(rhs));
}

double divide(const string &lhs, const var &rhs) {
  return divide(f$floatval(lhs), rhs);
}

double divide(const var &lhs, const var &rhs) {
  return divide(f$floatval(lhs), f$floatval(rhs));
}


double divide(bool lhs, bool rhs) {
  php_warning("Both arguments of operator '/' are bool");
  return divide(static_cast<int>(lhs), static_cast<int>(rhs));
}

template<class T>
double divide(bool lhs, const T &rhs) {
  php_warning("First argument of operator '/' is bool");
  return divide((int)lhs, f$floatval(rhs));
}

template<class T>
double divide(const T &lhs, bool rhs) {
  php_warning("Second argument of operator '/' is bool");
  return divide(f$floatval(lhs), static_cast<int>(rhs));
}

template<class T>
double divide(bool, const array<T> &) {
  php_warning("Unsupported operand types for operator '/' bool and array");
  return 0.0;
}

template<class T>
double divide(const array<T> &, bool) {
  php_warning("Unsupported operand types for operator '/' array and bool");
  return 0.0;
}

template<class T>
double divide(bool, const class_instance<T> &) {
  php_warning("Unsupported operand types for operator '/' bool and object");
  return 0.0;
}

template<class T>
double divide(const class_instance<T> &, bool) {
  php_warning("Unsupported operand types for operator '/' object and bool");
  return 0.0;
}


template<class T, class T1>
double divide(const array<T> &lhs, const T1 &rhs) {
  php_warning("First argument of operator '/' is array");
  return divide(f$count(lhs), rhs);
}

template<class T, class T1>
double divide(const T1 &lhs, const array<T> &rhs) {
  php_warning("Second argument of operator '/' is array");
  return divide(lhs, f$count(rhs));
}


template<class T, class T1>
double divide(const class_instance<T> &, const T1 &rhs) {
  php_warning("First argument of operator '/' is object");
  return divide(1.0, rhs);
}

template<class T, class T1>
double divide(const T1 &lhs, const class_instance<T> &) {
  php_warning("Second argument of operator '/' is object");
  return lhs;
}


template<class T>
double divide(const array<T> &, const array<T> &) {
  php_warning("Unsupported operand types for operator '/' array and array");
  return 0.0;
}

template<class T>
double divide(const class_instance<T> &, const class_instance<T> &) {
  php_warning("Unsupported operand types for operator '/' object and object");
  return 0.0;
}

template<class T, class T1>
double divide(const array<T> &, const array<T1> &) {
  php_warning("Unsupported operand types for operator '/' array and array");
  return 0.0;
}

template<class T, class T1>
double divide(const class_instance<T> &, const class_instance<T1> &) {
  php_warning("Unsupported operand types for operator '/' object and object");
  return 0.0;
}

template<class T, class T1>
double divide(const array<T> &, const class_instance<T1> &) {
  php_warning("Unsupported operand types for operator '/' array and object");
  return 0.0;
}

template<class T, class T1>
double divide(const class_instance<T1> &, const array<T> &) {
  php_warning("Unsupported operand types for operator '/' object and array");
  return 0.0;
}


int modulo(int lhs, int rhs) {
  if (rhs == 0) {
    php_warning("Modulo by zero");
    return 0;
  }
  return lhs % rhs;
}

template<class T1, class T2>
int modulo(const T1 &lhs, const T2 &rhs) {
  int div = f$intval(lhs);
  int mod = f$intval(rhs);

  if (neq2(div, lhs)) {
    php_warning("First parameter of operator %% is not an integer");
  }
  if (neq2(mod, rhs)) {
    php_warning("Second parameter of operator %% is not an integer");
  }

  if (mod == 0) {
    php_warning("Modulo by zero");
    return 0;
  }
  return div % mod;
}


template<class T1, class T2>
T1 &divide_self(T1 &lhs, const T2 &rhs) {
  return lhs = divide(lhs, rhs);
}


int &modulo_self(int &lhs, int rhs) {
  return lhs = modulo(lhs, rhs);
}

template<class T1, class T2>
T1 &modulo_self(T1 &lhs, const T2 &rhs) {
  return lhs = modulo(lhs, rhs);
}


template<class T0, class T>
void assign(T0 &dest, const T &from) {
  dest = from;
}

template<class T, class>
bool f$boolval(const T &val) {
  return static_cast<bool>(val);
}

bool f$boolval(const string &val) {
  return val.to_bool();
}

template<class T>
bool f$boolval(const array<T> &val) {
  return !val.empty();
}

template<class T>
bool f$boolval(const class_instance<T> &val) {
  return !val.is_null();
}

template<class ...Args>
bool f$boolval(const std::tuple<Args...> &) {
  return true;
}

template<class T>
bool f$boolval(const Optional<T> &val) {
  return val.has_value() ? f$boolval(val.val()) : false;
}

bool f$boolval(const var &val) {
  return val.to_bool();
}


template<class T, class>
int f$intval(const T &val) {
  return static_cast<int>(val);
}

int f$intval(const string &val) {
  return val.to_int();
}

template<class T>
int f$intval(const array<T> &val) {
  php_warning("Wrong convertion from array to int");
  return val.to_int();
}

template<class T>
int f$intval(const class_instance<T> &) {
  php_warning("Wrong convertion from object to int");
  return 1;
}

int f$intval(const var &val) {
  return val.to_int();
}

template<class T>
inline int f$intval(const Optional<T> &val) {
  return val.has_value() ? f$intval(val.val()) : 0;
}


int f$safe_intval(const bool &val) {
  return val;
}

const int &f$safe_intval(const int &val) {
  return val;
}

int f$safe_intval(const double &val) {
  if (fabs(val) > 2147483648) {
    php_warning("Wrong convertion from double %.6lf to int", val);
  }
  return (int)val;
}

int f$safe_intval(const string &val) {
  return val.safe_to_int();
}

template<class T>
int f$safe_intval(const array<T> &val) {
  php_warning("Wrong convertion from array to int");
  return val.to_int();
}

template<class T>
int f$safe_intval(const class_instance<T> &) {
  php_warning("Wrong convertion from object to int");
  return 1;
}

int f$safe_intval(const var &val) {
  return val.safe_to_int();
}


template<class T, class>
double f$floatval(const T &val) {
  return static_cast<double>(val);
}

double f$floatval(const string &val) {
  return val.to_float();
}

template<class T>
double f$floatval(const array<T> &val) {
  php_warning("Wrong convertion from array to float");
  return val.to_float();
}

template<class T>
double f$floatval(const class_instance<T> &) {
  php_warning("Wrong convertion from object to float");
  return 1.0;
}

double f$floatval(const var &val) {
  return val.to_float();
}

template<class T>
inline double f$floatval(const Optional<T> &val) {
  return val.has_value() ? f$floatval(val.val()) : 0;
}


string f$strval(const bool &val) {
  return (val ? string("1", 1) : string());
}

string f$strval(const int &val) {
  return string(val);
}

string f$strval(const double &val) {
  return string(val);
}

string f$strval(const string &val) {
  return val;
}

string &&f$strval(string &&val) {
  return std::move(val);
}

template<class T>
string f$strval(const array<T> &) {
  php_warning("Convertion from array to string");
  return string("Array", 5);
}

template<class ...Args>
string f$strval(const std::tuple<Args...> &) {
  php_warning("Convertion from tuple to string");
  return string("Array", 5);
}

template<class T>
string f$strval(const Optional<T> &val) {
  return val.has_value() ? f$strval(val.val()) : f$strval(false);
}

template<class T>
string f$strval(Optional<T> &&val) {
  return val.has_value() ? f$strval(std::move(val.val())) : f$strval(false);
}

template<class T>
string f$strval(const class_instance<T> &val) {
  return string(val.get_class());
}

string f$strval(const var &val) {
  return val.to_string();
}

string f$strval(var &&val) {
  return val.is_string() ? std::move(val.as_string()) : val.to_string();
}

template<class T>
array<T> f$arrayval(const T &val) {
  array<T> res(array_size(1, 0, true));
  res.push_back(val);
  return res;
}

template<class T>
const array<T> &f$arrayval(const array<T> &val) {
  return val;
}

template<class T>
array<T> &&f$arrayval(array<T> &&val) {
  return std::move(val);
}

template<class T>
array<var> f$arrayval(const class_instance<T> &) {
  php_warning("Can not convert class instance to array");
  return array<var>();
}

array<var> f$arrayval(const var &val) {
  return val.to_array();
}

array<var> f$arrayval(var &&val) {
  return val.is_array() ? std::move(val.as_array()) : val.to_array();
}

namespace {

template<typename T>
std::enable_if_t<vk::is_type_in_list<T, bool, var>{}, array<T>> false_cast_to_array() {
  return f$arrayval(T{false});
}

template<typename T>
std::enable_if_t<!vk::is_type_in_list<T, bool, var>{}, array<T>> false_cast_to_array() {
  php_warning("Dangerous cast false to array, the result will be different from PHP");
  return array<T>{};
}

} // namespace

template<class T>
array<T> f$arrayval(const Optional<T> &val) {
  switch (val.value_state()) {
    case OptionalState::has_value:
      return f$arrayval(val.val());
    case OptionalState::false_value:
      return false_cast_to_array<T>();
    case OptionalState::null_value:
      return array<T>{};
    default:
      __builtin_unreachable();
  }
}

template<class T>
array<T> f$arrayval(Optional<T> &&val) {
  switch (val.value_state()) {
    case OptionalState::has_value:
      return f$arrayval(std::move(val.val()));
    case OptionalState::false_value:
      return false_cast_to_array<T>();
    case OptionalState::null_value:
      return array<T>{};
    default:
      __builtin_unreachable();
  }
}

template<class T>
array<T> f$arrayval(const Optional<array<T>> &val) {
  if (val.value_state() == OptionalState::false_value) {
    return false_cast_to_array<T>();
  }
  return val.val();
}

template<class T>
array<T> f$arrayval(Optional<array<T>> &&val) {
  if (val.value_state() == OptionalState::false_value) {
    return false_cast_to_array<T>();
  }
  return std::move(val.val());
}

bool &boolval_ref(bool &val) {
  return val;
}

bool &boolval_ref(var &val) {
  return val.as_bool("unknown");
}

const bool &boolval_ref(const bool &val) {
  return val;
}

const bool &boolval_ref(const var &val) {
  return val.as_bool("unknown");
}


int &intval_ref(int &val, const char *) {
  return val;
}

int &intval_ref(var &val, const char *function) {
  return val.as_int(function);
}

const int &intval_ref(const int &val, const char *) {
  return val;
}

const int &intval_ref(const var &val, const char *function) {
  return val.as_int(function);
}


double &floatval_ref(double &val) {
  return val;
}

double &floatval_ref(var &val) {
  return val.as_float("unknown");
}

const double &floatval_ref(const double &val) {
  return val;
}

const double &floatval_ref(const var &val) {
  return val.as_float("unknown");
}


string &strval_ref(string &val, const char *) {
  return val;
}

string &strval_ref(var &val, const char *function) {
  return val.as_string(function);
}

const string &strval_ref(const string &val, const char *) {
  return val;
}

const string &strval_ref(const var &val, const char *function) {
  return val.as_string(function);
}


template<class T>
array<T> &arrayval_ref(array<T> &val, const char *) {
  return val;
}

array<var> &arrayval_ref(var &val, const char *function) {
  return val.as_array(function);
}

template<class T>
array<T> &arrayval_ref(Optional<array<T>> &val, const char *function) {
  if (unlikely(!val.has_value())) {
    php_warning("%s() expects parameter to be array, null or false is given", function);
  }
  return val.ref();
}

template<class T>
const array<T> &arrayval_ref(const array<T> &val, const char *) {
  return val;
}

const array<var> &arrayval_ref(const var &val, const char *function) {
  return val.as_array(function);
}

template<class T>
const array<T> &arrayval_ref(const Optional<array<T>> &val, const char *function) {
  if (unlikely(!val.has_value())) {
    php_warning("%s() expects parameter to be array, null or false is given", function);
  }
  return val.val();
}

template<class T>
const T &convert_to<T>::convert(const T &val) {
  return val;
}

template<class T>
T &&convert_to<T>::convert(T &&val) {
  return std::move(val);
}

template<class T>
T convert_to<T>::convert(const Unknown &) {
  return T();
}

template<class T>
template<class T1, class, class>
T convert_to<T>::convert(T1 &&val) {
  return T(std::forward<T1>(val));
}

template<>
template<class T1, class, class>
bool convert_to<bool>::convert(T1 &&val) {
  return f$boolval(std::forward<T1>(val));
}

template<>
template<class T1, class, class>
int convert_to<int>::convert(T1 &&val) {
  return f$intval(std::forward<T1>(val));
}

template<>
template<class T1, class, class>
double convert_to<double>::convert(T1 &&val) {
  return f$floatval(std::forward<T1>(val));
}

template<>
template<class T1, class, class>
string convert_to<string>::convert(T1 &&val) {
  return f$strval(std::forward<T1>(val));
}

template<>
template<class T1, class, class>
array<var> convert_to<array<var>>::convert(T1 &&val) {
  return f$arrayval(std::forward<T1>(val));
}

template<>
template<class T1, class, class>
var convert_to<var>::convert(T1 &&val) {
  return var{std::forward<T1>(val)};
}


template<class T, class>
inline bool f$empty(const T &v) {
  return v == 0;
}

bool f$empty(const string &v) {
  int l = v.size();
  return l == 0 || (l == 1 && v[0] == '0');
}

template<class T>
bool f$empty(const array<T> &a) {
  return a.empty();
}

template<class T>
bool f$empty(const class_instance<T> &o) {
  return o.is_null();   // false/null внутри инстанса (в php empty(false)=true тоже)
}

bool f$empty(const var &v) {
  return v.empty();
}

template<class T>
bool f$empty(const Optional<T> &a) {
  return a.has_value() ? f$empty(a.val()) : true;
}

int f$count(const var &v) {
  return v.count();
}

template<class T>
int f$count(const Optional<T> &a) {
  auto count_lambda = [](const auto &v) { return f$count(v);};
  return call_fun_on_optional_value(count_lambda, a);
}

template<class T>
int f$count(const array<T> &a) {
  return a.count();
}

template<class ...Args>
inline int f$count(const std::tuple<Args...> &a __attribute__ ((unused))) {
  return (int)std::tuple_size<std::tuple<Args...>>::value;
}

template<class T>
int f$count(const T &) {
  php_warning("Count on non-array");
  return 1;
}


template<class T>
int f$sizeof(const T &v) {
  return f$count(v);
}


template<class T>
bool f$is_numeric(const T &) {
  return std::is_same<T, int>::value || std::is_same<T, double>::value;
}

bool f$is_numeric(const string &v) {
  return v.is_numeric();
}

bool f$is_numeric(const var &v) {
  return v.is_numeric();
}

template<class T>
inline bool f$is_numeric(const Optional<T> &v) {
  return v.has_value() ? f$is_numeric(v.val()) : false;
}


template<class T>
bool f$is_null(const T &) {
  return false;
}

template<class T>
bool f$is_null(const class_instance<T> &v) {
  return v.is_null();
}

template<class T>
inline bool f$is_null(const Optional<T> &v) {
  return v.has_value() ? f$is_null(v.val()) : v.value_state() == OptionalState::null_value;
}

bool f$is_null(const var &v) {
  return v.is_null();
}


template<class T>
bool f$is_bool(const T &) {
  return std::is_same<T, bool>::value;
}

template<class T>
inline bool f$is_bool(const Optional<T> &v) {
  return v.has_value() ? f$is_bool(v.val()) : v.value_state() == OptionalState::false_value;
}

bool f$is_bool(const var &v) {
  return v.is_bool();
}


template<class T>
bool f$is_int(const T &) {
  return std::is_same<T, int>::value;
}

template<class T>
inline bool f$is_int(const Optional<T> &v) {
  return v.has_value() ? f$is_int(v.val()) : false;
}

bool f$is_int(const var &v) {
  return v.is_int();
}


template<class T>
bool f$is_float(const T &) {
  return std::is_same<T, double>::value;
}

template<class T>
inline bool f$is_float(const Optional<T> &v) {
  return v.has_value() ? f$is_float(v.val()) : false;
}

bool f$is_float(const var &v) {
  return v.is_float();
}


template<class T>
bool f$is_scalar(const T &) {
  return std::is_arithmetic<T>::value || std::is_same<T, string>::value;
}

template<class T>
inline bool f$is_scalar(const Optional<T> &v) {
  auto is_scalar_lambda = [](const auto &v) { return f$is_scalar(v);};
  return call_fun_on_optional_value(is_scalar_lambda, v);
}

bool f$is_scalar(const var &v) {
  return v.is_scalar();
}


template<class T>
bool f$is_string(const T &) {
  return std::is_same<T, string>::value;
}

template<class T>
inline bool f$is_string(const Optional<T> &v) {
  return v.has_value() ? f$is_string(v.val()) : false;
}

bool f$is_string(const var &v) {
  return v.is_string();
}


template<class T>
inline bool f$is_array(const T &) {
  return is_array<T>::value;
}

bool f$is_array(const var &v) {
  return v.is_array();
}

template<class T>
inline bool f$is_array(const Optional<T> &v) {
  return v.has_value() ? f$is_array(v.val()) : false;
}

template<class T>
bool f$is_object(const T &) {
  return false;
}

template<class T>
bool f$is_object(const class_instance<T> &v) {
  return !v.is_null();
}


template<class T>
bool f$is_integer(const T &v) {
  return f$is_int(v);
}

template<class T>
bool f$is_long(const T &v) {
  return f$is_int(v);
}

template<class T>
bool f$is_double(const T &v) {
  return f$is_float(v);
}

template<class T>
bool f$is_real(const T &v) {
  return f$is_float(v);
}

const char *get_type_c_str(const bool &) {
  return "boolean";
}

const char *get_type_c_str(const int &) {
  return "integer";
}

const char *get_type_c_str(const double &) {
  return "double";
}

const char *get_type_c_str(const string &) {
  return "string";
}

const char *get_type_c_str(const var &v) {
  return v.get_type_c_str();
}

template<class T>
const char *get_type_c_str(const array<T> &) {
  return "array";
}

template<class T>
const char *get_type_c_str(const class_instance<T> &) {
  return "object";
}


template<class T>
string f$get_type(const T &v) {
  const char *res = get_type_c_str(v);
  return string(res, strlen(res));
}


string f$get_class(const bool &) {
  php_warning("Called get_class() on boolean");
  return string();
}

string f$get_class(const int &) {
  php_warning("Called get_class() on integer");
  return string();
}

string f$get_class(const double &) {
  php_warning("Called get_class() on double");
  return string();
}

string f$get_class(const string &) {
  php_warning("Called get_class() on string");
  return string();
}

string f$get_class(const var &v) {
  php_warning("Called get_class() on %s", v.get_type_c_str());
  return string();
}

template<class T>
string f$get_class(const array<T> &) {
  php_warning("Called get_class() on array");
  return string();
}

template<class T>
string f$get_class(const class_instance<T> &v) {
  const char *result = v.get_class();
  return string(result, (dl::size_type)strlen(result));
}

template<class T>
inline int f$get_hash_of_class(const class_instance<T> &klass) {
  return klass.get_hash();
}

string &append(string &dest, const string &from) {
  return dest.append(from);
}

template<class T>
string &append(Optional<string> &dest, const T &from) {
  return append(dest.ref(), from);
}

template<class T>
string &append(string &dest, const T &from) {
  return dest.append(f$strval(from));
}

template<class T>
var &append(var &dest, const T &from) {
  return dest.append(f$strval(from));
}

template<class T0, class T>
T0 &append(T0 &dest, const T &from) {
  php_warning("Wrong arguments types %s and %s for operator .=", get_type_c_str(dest), get_type_c_str(from));
  return dest;
}


string f$gettype(const var &v) {
  return v.get_type_str();
}

template<class T>
bool f$function_exists(const T &) {
  return true;
}


var f$error_get_last() {
  return var();
}

int f$error_reporting(int level) {
  int prev = php_warning_level;
  if ((level & E_ALL) == E_ALL) {
    php_warning_level = 3;
  }
  if (0 <= level && level <= 3) {
    php_warning_level = level;
  }
  return prev;
}

int f$error_reporting() {
  return php_warning_level;
}

void f$warning(const string &message) {
  php_warning("%s", message.c_str());
}

int f$memory_get_static_usage() {
  return (int)dl::get_heap_memory_used();
}

int f$memory_get_peak_usage(bool real_usage) {
  if (real_usage) {
    return (int)dl::get_script_memory_stats().max_real_memory_used;
  } else {
    return (int)dl::get_script_memory_stats().max_memory_used;
  }
}

int f$memory_get_usage(bool real_usage __attribute__((unused))) {
  return (int)dl::get_script_memory_stats().memory_used;
}

int f$memory_get_total_usage() {
  return (int)dl::get_script_memory_stats().real_memory_used;
}


template<class T>
int f$get_reference_counter(const array<T> &v) {
  return v.get_reference_counter();
}

template<class T>
int f$get_reference_counter(const class_instance<T> &v) {
  return v.get_reference_counter();
}

int f$get_reference_counter(const string &v) {
  return v.get_reference_counter();
}

int f$get_reference_counter(const var &v) {
  return v.get_reference_counter();
}


template<class T>
T &val(T &x) {
  return x;
}

template<class T>
const T &val(const T &x) {
  return x;
}

template<class T>
T &ref(T &x) {
  return x;
}

template<class T>
const T &val(const Optional<T> &x) {
  return x.val();
}

template<class T>
T &val(Optional<T> &x) {
  return x.val();
}

template<class T>
T &ref(Optional<T> &x) {
  return x.ref();
}


template<class T>
typename array<T>::iterator begin(array<T> &x) {
  return x.begin();
}

template<class T>
typename array<T>::const_iterator begin(const array<T> &x) {
  return x.begin();
}

template<class T>
typename array<T>::const_iterator const_begin(const array<T> &x) {
  return x.begin();
}


array<var>::iterator begin(var &x) {
  return x.begin();
}

array<var>::const_iterator begin(const var &x) {
  return x.begin();
}

array<var>::const_iterator const_begin(const var &x) {
  return x.begin();
}


template<class T>
typename array<T>::iterator begin(Optional<array<T>> &x) {
  if (unlikely(!x.has_value())) {
    php_warning("Invalid argument supplied for foreach(), false or null is given");
  }
  return x.val().begin();
}

template<class T>
typename array<T>::const_iterator begin(const Optional<array<T>> &x) {
  if (unlikely(!x.has_value())) {
    php_warning("Invalid argument supplied for foreach(), false or null is given");
  }
  return x.val().begin();
}

template<class T>
typename array<T>::const_iterator const_begin(const Optional<array<T>> &x) {
  if (unlikely(!x.has_value())) {
    php_warning("Invalid argument supplied for foreach(), false or null is given");
  }
  return x.val().begin();
}


template<class T>
typename array<T>::iterator end(array<T> &x) {
  return x.end();
}

template<class T>
typename array<T>::const_iterator end(const array<T> &x) {
  return x.end();
}

template<class T>
typename array<T>::const_iterator const_end(const array<T> &x) {
  return x.end();
}

array<var>::iterator end(var &x) {
  return x.end();
}

array<var>::const_iterator end(const var &x) {
  return x.end();
}

array<var>::const_iterator const_end(const var &x) {
  return x.end();
}


template<class T>
typename array<T>::iterator end(Optional<array<T>> &x) {
  if (unlikely(!x.has_value())) {
    php_warning("Invalid argument supplied for foreach(), false or null is given");
  }
  return x.val().end();
}

template<class T>
typename array<T>::const_iterator end(const Optional<array<T>> &x) {
  if (unlikely(!x.has_value())) {
    php_warning("Invalid argument supplied for foreach(), false or null is given");
  }
  return x.val().end();
}

template<class T>
typename array<T>::const_iterator const_end(const Optional<array<T>> &x) {
  if (unlikely(!x.has_value())) {
    php_warning("Invalid argument supplied for foreach(), false or null is given");
  }
  return x.val().end();
}


void clear_array(var &v) {
  v.clear();
}

template<class T>
void clear_array(array<T> &a) {
  a.clear();
}

template<class T>
void clear_array(Optional<array<T>> &a) {
  a = false;
}

template<class T>
void unset(array<T> &x) {
  clear_array(x);
}

template<class T>
void unset(class_instance<T> &x) {
  x.destroy();
}

void unset(var &x) {
  x = var();
}

template<typename T>
using future = int;
template<typename T>
using future_queue = int;
