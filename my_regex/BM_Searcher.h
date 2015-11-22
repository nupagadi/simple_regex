#pragma once

#define BM_ALPHABET_LEN 256

namespace my_regex
{

class BM_Searcher
{
public:
   BM_Searcher() : delta2_(), pat_(), patlen_() {}
   ~BM_Searcher() { free(); }

   bool Reset(const char *pat, size_t patlen);

   const char* FindIn(const char *string, size_t stringlen);

private:
   int delta1_[BM_ALPHABET_LEN];
   int* delta2_;
   const char* pat_;
   size_t patlen_;

   void free() { 
      delete [] delta2_;  delta2_ = nullptr;
      pat_ = nullptr;  patlen_ = 0;
   }
};

}  // namespace my_regex