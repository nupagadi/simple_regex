#pragma once

namespace my_regex
{
// pattern consists of SolidPattern instances, may contain '*'
class FloatingPattern
{
public:
   FloatingPattern() : left_alligned_(), right_alligned_(), free_alligned_(), string_pattern_() {}
   bool Init(const char* string_pattern);

   // for debug
   void ToString(const char** string_pattern) { *string_pattern = string_pattern_; }

   ~FloatingPattern() { delete left_alligned_;  delete right_alligned_;  delete [] free_alligned_; delete [] string_pattern_; }
private:
   SolidPattern *left_alligned_, *right_alligned_;
   SolidPattern* free_alligned_; // array
   char* string_pattern_;
};

}  // namespace my_regex