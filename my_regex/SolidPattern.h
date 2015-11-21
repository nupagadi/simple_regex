#pragma once

#define min(a, b) ((a < b) ? a : b)


namespace my_regex
{
// set of letters only, with its length and offset of '?'
class SolidPatternAtom
{
public:
   SolidPatternAtom() : str_(), len_(), offset_() {}
   bool Init(const char* str, size_t len, size_t offset);

   const char* str() { return str_; }
   size_t len() { return len_; }
   size_t offset() { return offset_; }
   
private:
   const char* str_; // letters only
   size_t len_;      // lenght of str
   size_t offset_;   // preceding '?' count

   // non-copyable
   SolidPatternAtom(const SolidPatternAtom&);
   SolidPatternAtom& operator=(const SolidPatternAtom&);
};

class SolidPattern
{
public:
   SolidPattern() : atoms_num_(), p_atoms_(), tail_len_(), offset_(), length_(), pattern_() {}

   bool Reset(const char* source);

   size_t AtomsNum() { return atoms_num_; }
   size_t TailLen() { return tail_len_; }
   size_t Offset() { return offset_; }

   // ptr = nullptr, if out of range
   void GetAtom(size_t num, const char** ptr, size_t& len, size_t& offset);

   //    void ToString() { for(size_t i = 0; i<atoms_num_; ++i) p_atoms_[i]; }

   // if len == 0, null-terminated string is required
   // return nullptr, if no occurrences
   const char* FindIn(const char* str, size_t len = 0);

   ~SolidPattern() { delete [] p_atoms_; p_atoms_ = nullptr; }
private:
   size_t atoms_num_;
   SolidPatternAtom* p_atoms_;
   size_t tail_len_;
   size_t offset_;
   size_t length_;
   char* pattern_;

   // check if str is equal to pattern except its first atom
   bool isEqualPastTheFirst(const char* str);

   // non-copyable
   SolidPattern(const SolidPattern&);
   SolidPattern& operator=(const SolidPattern&);

};

}  // namespace my_regex
