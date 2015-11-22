#pragma once

namespace my_regex
{
// pattern consists of SolidPattern instances, may contain '*'
class FloatingPattern
{
public:
   FloatingPattern() : left_aligned_(), right_aligned_(), free_aligned_(), string_pattern_(), min_length_(), free_pat_num_() {}
   bool Reset(const char* string_pattern);

   const char* DoesMatch(const char* str, size_t len = 0);

   size_t MinLength() { return min_length_; }
   size_t Offset() { return left_aligned_ ? left_aligned_->Length() : 0; }
   size_t Tail() { return right_aligned_ ? right_aligned_->Length() : 0; }

#ifdef _DEBUG 
   void ToString(const char** string_pattern) { *string_pattern = string_pattern_; }
#endif

   ~FloatingPattern() {  }

private:

#ifdef _DEBUG 
public:
#endif

   SolidPattern *left_aligned_, *right_aligned_;
   SolidPattern* free_aligned_; // array
   char* string_pattern_;

   size_t min_length_;
   size_t free_pat_num_;

   void free();
};

}  // namespace my_regex