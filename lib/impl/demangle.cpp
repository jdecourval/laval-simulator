#include "demangle.h"

#ifdef __GNUG__

#include <memory>
#include <cxxabi.h>


std::string demangle(const char* name)
{
    int status = -4;  // some arbitrary value to eliminate the compiler warning
    std::unique_ptr<char, void (*)(void*)> res{abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free};
    return (status == 0) ? res.get() : name;
}

#else

#error your compiler is not yet supported

#endif
