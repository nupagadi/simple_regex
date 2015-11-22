#include <cstring>
#include <cassert>
#include <new>

#include "SolidPattern.h"
#include "FloatingPattern.h"

namespace my_regex
{

bool FloatingPattern::Reset(const char* string_pattern)
{
   min_length_ = 0;

   size_t len = strlen(string_pattern);

   delete [] string_pattern_;
   string_pattern_ = new (std::nothrow) char[len+1];
   if(!string_pattern_) { free(); return false; }

   if(!len) { free(); return false; }

   size_t asterisk_num = 0;
   const char* from = string_pattern;
   char* to = string_pattern_;
   while(*from)
   {
      while(*from == '*' && from[1] == '*')  ++from;
      asterisk_num += *from == '*';
      *to++ = *from++;
   }
   *to = '\0';
   free_pat_num_ = asterisk_num ? asterisk_num-1 : 0;
   assert(free_pat_num_ >= 0);

   min_length_ = strlen(string_pattern_);
   if(string_pattern_[0] == '*')  --min_length_;
   if(string_pattern_[strlen(string_pattern_)-1] == '*')  --min_length_;

   delete left_aligned_;  left_aligned_ = nullptr;
   if(string_pattern_[0] != '*')
   {
      left_aligned_ = new (std::nothrow) SolidPattern;
      if(!left_aligned_) { free(); return false; }
   }  

   delete right_aligned_;  right_aligned_ = nullptr;
   if(string_pattern_[strlen(string_pattern_)-1] != '*' && asterisk_num)
   {
      right_aligned_ = new (std::nothrow) SolidPattern;
      if(!right_aligned_) { free(); return false; }
   }

   if(!strcmp(string_pattern_, "*"))  min_length_ = 0;

   delete [] free_aligned_;  free_aligned_ = nullptr;
   free_aligned_ = new (std::nothrow) SolidPattern[free_pat_num_];
   if(!free_aligned_) { free(); return false; }

   
   const char *prev, *pos = prev = string_pattern_;
   
   if(left_aligned_)
   {
      prev = pos;
      pos = strchr(pos, '*');
      if(!pos) pos = prev;  // so (pos-prev) == 0
      if(!left_aligned_->Reset(prev, pos-prev))
      { free(); return false; }
   }
   assert(!asterisk_num || *pos == '*');
   for(size_t i = 0; i < free_pat_num_; ++i)
   {
      prev = ++pos;
      pos = strchr(pos, '*');
      if(!pos) pos = prev;  // so (pos-prev) == 0
      if(!free_aligned_[i].Reset(prev, pos-prev))
      { free(); return false; }
   }
   assert(!asterisk_num || *pos == '*');
   if(right_aligned_)
   {
      prev = ++pos;
      pos = strchr(pos, '*');
      if(!pos) pos = prev;  // so (pos-prev) == 0
      if(!right_aligned_->Reset(prev, pos-prev))
      { free(); return false; }
   }

   return true;
}

const char* FloatingPattern::DoesMatch(const char* str, size_t str_len/* = 0*/)
{
   if (!str_len)   str_len = strlen(str);
   if (str_len < min_length_) return nullptr;
   if(!strcmp(string_pattern_, "*"))  return str;

   if(left_aligned_)
   {
      if(!left_aligned_->IsEqual(str, left_aligned_->Length()))
         return nullptr;
   }

   if(right_aligned_)
   {
      if(!right_aligned_->IsEqual(str + str_len - right_aligned_->Length(), right_aligned_->Length()))
         return nullptr;
   }

   const char* pos = str + Offset();
   for(size_t i = 0; i < free_pat_num_; ++i)
   {
      pos = free_aligned_[i].FindIn(pos, str_len - (pos-str) - Tail());
      if(!pos)  return nullptr;
      pos += free_aligned_[i].Length();
   }

   return str;

}

void FloatingPattern::free()
{
   min_length_ = free_pat_num_ = 0;
   if(!left_aligned_)  delete left_aligned_;  
   if(!right_aligned_)  delete right_aligned_;     
   delete [] free_aligned_;  delete [] string_pattern_; 
   left_aligned_ = right_aligned_ = free_aligned_ = nullptr;
   string_pattern_ = nullptr;
}


}  // namespace my_regex