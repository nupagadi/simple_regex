#include <new>
#include <cstring>
#include <cstdint>
#include <cassert>

#include "SolidPattern.h"

const char* boyer_moore (const char *string, uint32_t stringlen, const char *pat, uint32_t patlen);


namespace my_regex
{

bool SolidPattern::Reset(const char* source)
{
   offset_ = length_ = tail_len_ = atoms_num_ = 0;
   length_ = strlen(source);

   delete [] pattern_;
   pattern_ = new (std::nothrow) char[length_+1];
   if(!pattern_)  return false;

   strcpy(pattern_, source);

   // '?' count
   size_t q_mark_num = 0;
   for(const char* pos = pattern_; *pos; q_mark_num += (*pos++=='?') );

   // Максимально возможное количество участков, разделенных '?'
   // например, в zxc??xs????sdd? три таких участка
   size_t max_elem_num = min(q_mark_num + 1, length_ - q_mark_num);

   if(!max_elem_num)
      return zeroAtomOperation();
   //ЕСЛИ ВОПРОСОВ НОЛЬ!!!!!!!!!!
   //ЕСЛИ БУКВ НОЛЬ!!!!!!!!!!

   // Начала этих участков
   size_t* starts = new (std::nothrow) size_t[max_elem_num];
   if(!starts)  return false;
   // Длины этих участков
   size_t* lens = new (std::nothrow) size_t[max_elem_num];
   if(!lens)  return false;

   const char* pos = pattern_;
   for (size_t i = 0, &j = atoms_num_; pattern_[i]; ++i, ++pos)
   {
      pos = strchr(pos, '?');

      if(pos-pattern_ != i)
      {
         starts[j] = i;
         if(pos)   i = pos - pattern_;
         else      i = length_;
         lens[j] = i - starts[j];
         ++j;
      }
      if(!pos) break;
   }
   offset_ = starts[0];
   tail_len_ = length_ - starts[atoms_num_-1] - lens[atoms_num_-1];

   delete [] p_atoms_;
   p_atoms_ = new (std::nothrow) SolidPatternAtom[atoms_num_];
   if(!p_atoms_)  return false;

   p_atoms_[0].Init(pattern_ + starts[0], lens[0], starts[0]);
   for (size_t i = 1; i < atoms_num_; ++i)
      p_atoms_[i].Init(pattern_ + starts[i], lens[i], starts[i] - starts[i-1] - lens[i-1]);

   delete [] lens;
   delete [] starts;

   return true;
}

void SolidPattern::GetAtom(size_t num, const char** ptr, size_t& len, size_t& offset)
{
   if(num < atoms_num_)
   {
      *ptr = p_atoms_[num].str();
      len = p_atoms_[num].len();
      offset = p_atoms_[num].offset();
   }
   else  *ptr = nullptr;
}

            // TUNE BM!!!!!!!!!!!!!!!!!
const char* SolidPattern::FindIn(const char* str, size_t str_len /*= 0*/)
{
   if (!str_len)   str_len = strlen(str);
   if (str_len < length_) return nullptr;
   if (!atoms_num_)  return str;
   
   const char* entry = str;
   while(true)
   {
      entry = boyer_moore(entry, str_len-(entry-str), p_atoms_[0].str(), p_atoms_[0].len());
      // nothing was found
      if (!entry)  return nullptr;
      // superfluous?
   //    if (entry-str < offset_) return nullptr;
      // tail of pattern goes out of the end of str
      if (str_len - (entry-str) < length_) return nullptr;
      
      if(isEqualPastTheFirst(entry))   return  entry-offset_;
      else  ++entry;
   }

}

            // TUNE BM!!!!!!!!!!!!!!!!!
bool SolidPattern::isEqualPastTheFirst(const char* str)
{
   size_t i = 1;
   for (; i < atoms_num_; ++i)
      if (strncmp(str + (p_atoms_[i-1].str() - p_atoms_[0].str()) + p_atoms_[i-1].len() + p_atoms_[i].offset(), p_atoms_[i].str(), p_atoms_[i].len()))
         return false;

   return true;   
}

bool SolidPattern::zeroAtomOperation()
{
   //offset_ = length_ = tail_len_ = atoms_num_ = 0;
   //length_ = strlen(source);
   
   tail_len_ = length_;

   delete [] p_atoms_;
   p_atoms_ = nullptr;

   return true;
}




bool SolidPatternAtom::Init(const char* str, size_t len, size_t offset)
{
   str_ = str;
   len_ = len;
   offset_ = offset;
   return true;
}

}  // namespace my_regex
