#pragma once

namespace my_regex
{
class FloatingPattern;

class CLogReader
{
public:
           CLogReader() : h_file_(), dw_file_size_(), h_file_map_(), p_file_(), pattern_(), position_() {}
           ~CLogReader();
   bool    Open(const char *file_name);     // �������� �����, false - ������
   void    Close();                         // �������� �����
   bool    SetFilter(const char *filter);   // ��������� ������� �����, false - ������
   bool    GetNextLine(char *buf,           // ������ ��������� ��������� ������, 
                       const int bufsize);  // buf - �����, bufsize - ������������ �����
                                            // false - ����� ����� ��� ������
private:
   HANDLE h_file_;
   DWORD dw_file_size_;
   HANDLE h_file_map_;
   PSTR p_file_;

   FloatingPattern* pattern_;
   const char *position_;     // ��������� ������� ������, ���������� � SetFilter; ��� GetNextLine

   void free_file();
};

}  // namespace my_regex
