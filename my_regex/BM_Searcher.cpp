#include <cstdint>
#include <new>

#include "BM_Searcher.h"

void make_delta1(int *delta1, const char *pat, size_t patlen);
void make_delta2(int *delta2, const char *pat, size_t patlen);
const char* boyer_moore(const char *string, size_t stringlen, const char *pat, size_t patlen, int* delta1, int* delta2);

namespace my_regex
{


bool BM_Searcher::Reset(const char *pat, size_t patlen)
{
   free();
   pat_ = pat;  patlen_ = patlen;
   delta2_ = new (std::nothrow) int[patlen];
   if(!delta2_)  return false;

   make_delta1(delta1_, pat, patlen);
   make_delta2(delta2_, pat, patlen);

   return true;
}

const char* BM_Searcher::FindIn(const char *string, size_t stringlen)
{
   return boyer_moore(string, stringlen, pat_, patlen_, delta1_, delta2_);
}


}  // namespace my_regex