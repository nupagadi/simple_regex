#pragma once

namespace my_regex
{
class FloatingPattern;

class CLogReader
{
public:
           CLogReader() : h_file_(), dw_file_size_(), h_file_map_(), p_file_(), pattern_(), position_() {}
           ~CLogReader();
   bool    Open(const char *file_name);     // открытие файла, false - ошибка
   void    Close();                         // закрытие файла
   bool    SetFilter(const char *filter);   // установка фильтра строк, false - ошибка
   bool    GetNextLine(char *buf,           // запрос очередной найденной строки, 
                       const int bufsize);  // buf - буфер, bufsize - максимальная длина
                                            // false - конец файла или ошибка
private:
   HANDLE h_file_;
   DWORD dw_file_size_;
   HANDLE h_file_map_;
   PSTR p_file_;

   FloatingPattern* pattern_;
   const char *position_;     // указатель текущей строки, обнуляется в SetFilter; для GetNextLine

   void free_file();
};

}  // namespace my_regex
