#include <cstring>
#include <new>

#include "SolidPattern.h"
#include "FloatingPattern.h"

namespace my_regex
{

bool FloatingPattern::Init(const char* string_pattern)
{
   size_t len = strlen(string_pattern);

   delete [] string_pattern_;
   string_pattern_ = new (std::nothrow) char[len+1];
   if(!string_pattern_)  return false;

   strcpy(string_pattern_, string_pattern);


   return true;
}


}  // namespace my_regex