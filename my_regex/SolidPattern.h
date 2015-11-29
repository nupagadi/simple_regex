#pragma once

#define min(a, b) ((a < b) ? a : b)

namespace my_regex
{

class BM_Searcher;

// interface
class ISolidPattern
{
public:
   virtual ~ISolidPattern(){}
   virtual bool Reset(const char* source, size_t len = 0) = 0;
   virtual bool IsEqual(const char* str, size_t str_len) const = 0;
   virtual size_t Length() const = 0;
   virtual const char* FindIn(const char* str, size_t len = 0) const = 0;
};

// set of letters only, with its length and offset of '?'
class SolidPatternAtom
{
public:
   SolidPatternAtom() : str_(), len_(), offset_() {}
   bool Init(const char* str, size_t len, size_t offset);

   const char* str() const { return str_; }
   size_t len() const { return len_; }
   size_t offset() const { return offset_; }
   
private:
   const char* str_; // letters only
   size_t len_;      // lenght of str
   size_t offset_;   // preceding '?' count

   // non-copyable
   SolidPatternAtom(const SolidPatternAtom&);
   SolidPatternAtom& operator=(const SolidPatternAtom&);
};

// set of SolidPatternAtom and distances between them
// pattern of letters and '?' (excluding '*')
class SolidPattern : public ISolidPattern
{
public:
   SolidPattern() : atoms_num_(), p_atoms_(), tail_len_(), offset_(), length_(), pattern_(), searcher_() {}

   bool Reset(const char* source, size_t len = 0);

   bool IsEqual(const char* str, size_t str_len) const;

   size_t AtomsNum() const { return atoms_num_; }
   size_t TailLen() const { return tail_len_; }
   size_t Offset() const { return offset_; }
   size_t Length() const { return length_; }

   // ptr = nullptr, if out of range
   void GetAtom(size_t num, const char** ptr, size_t& len, size_t& offset) const;

   //    void ToString() { for(size_t i = 0; i<atoms_num_; ++i) p_atoms_[i]; }

   // if len == 0, null-terminated string is required
   // return nullptr, if no occurrences
   const char* FindIn(const char* str, size_t len = 0) const;

   ~SolidPattern() { free(); }
private:
   size_t atoms_num_;
   SolidPatternAtom* p_atoms_;
   size_t tail_len_; // following "???"
   size_t offset_;   // preceding "???"
   size_t length_;   // length of pattern_
   char* pattern_;
   BM_Searcher* searcher_;

   // check if str is equal to pattern except its first atom
   bool isEqualPastTheFirst(const char* str) const;
   bool zeroAtomOperation();
   void free();
   bool resetSearher();

   // non-copyable
   SolidPattern(const SolidPattern&);
   SolidPattern& operator=(const SolidPattern&);

};

}  // namespace my_regex
