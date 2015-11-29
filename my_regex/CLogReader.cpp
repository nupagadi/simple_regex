#include <new>
#include <Windows.h>
#include "CLogReader.h"
#include "FloatingPattern.h"

namespace my_regex
{
   
bool CLogReader::Open(const char *file_name)
{
   h_file_ = CreateFileA(file_name, GENERIC_READ, 0, 
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (h_file_ == INVALID_HANDLE_VALUE) {
      free_file();
      return false;
   }
   //printf("File opened\n");

   dw_file_size_ = GetFileSize(h_file_, NULL);
   //printf("File size: %d\n", dwFileSize);

   h_file_map_ = CreateFileMapping(h_file_, NULL, PAGE_READONLY, 0, 0, NULL);

   if (h_file_map_ == nullptr) {
      free_file();
      return false;
   }
   //printf("Mapping created\n");

   PVOID pv_file = MapViewOfFile(h_file_map_, FILE_MAP_READ, 0, 0, 0);

   if (pv_file == nullptr) {
      free_file();
      return false;
   }
   p_file_ = (PSTR) pv_file;

   return true;
}

void CLogReader::Close()
{
   UnmapViewOfFile((PVOID)p_file_);    p_file_ = nullptr;
   CloseHandle(h_file_map_);           h_file_map_ = nullptr;
   CloseHandle(h_file_);               h_file_ = nullptr;
}

bool CLogReader::SetFilter(const char *filter)
{
   position_ = p_file_;
   if(!pattern_)  pattern_ = new (std::nothrow) FloatingPattern;
   if(!pattern_)  return false;

   if(!pattern_->Reset(filter))  return false;

   return true;
}

bool CLogReader::GetNextLine(char *buf, const int bufsize)
{
   if(!pattern_)  return false;
   if(!position_)  { position_ = p_file_; return false; }

   const char *ends = position_;   // end of current string

   while (position_)
   {
      // seek for the end of line
      ends = strstr(position_,"\r\n");

      if(pattern_->DoesMatch(position_, (ends ? ends-position_ : dw_file_size_ - (position_-p_file_))))
         break;

      // skipping "\r\n"
      // asserting (ends[0] == '\r' && ends[1] == '\n')
      while (ends && ends[0] == '\r' && ends[1] == '\n') {
         // in case of "\r\n\r\n"
         ends += 2;
      }
      position_ = ends;
   }

   if(!position_) return false;

   // till the end of line or bufsize
   size_t chars_to_copy = min((ends ? ends-position_ : dw_file_size_ - (position_-p_file_)), bufsize-1);

   strncpy(buf, position_, chars_to_copy);
   buf[chars_to_copy] = '\0';

   while (ends && ends[0] == '\r' && ends[1] == '\n') {
      ends += 2;
   }
   position_ = ends;

   return true;
}
                                           

void CLogReader::free_file()
{
   if(p_file_)       UnmapViewOfFile((PVOID)p_file_);
   if(h_file_map_)   CloseHandle(h_file_map_);
   if(h_file_)       CloseHandle(h_file_);
}

CLogReader::~CLogReader()
{ 
   delete pattern_;  pattern_ = nullptr;
   free_file();
}

}  // namespace my_regex
