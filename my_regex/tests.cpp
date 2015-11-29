
#include <cassert>
#include <cstdio>
#include <cstring>
#include <ctime>

//#include "CmnHdr.h"
//#include <windowsx.h>
//#include <tchar.h>
//#include <commdlg.h>
//#include <cstring>
//#include "Resource.h"
#include <Windows.h>


#include "SolidPattern.h"
#include "FloatingPattern.h"
#include "CLogReader.h"

#define ISOK(b) (b ? "is OK" : "failed")

#ifdef _DEBUG 

template<class SYMBOL>
const char* boyer_moore (const char *string, size_t stringlen, const SYMBOL *pat, size_t patlen);

struct DebugTest
{
static void BMTest()
{
   const char* string = "abedefgabedefgabedefg";
   const char* pat = "abcdefg";
   const char* pos = boyer_moore(string, 21, pat, 7);
   printf("BMTest is OK\n");
}

static void SolidPatternInitTest1()
{
   char* text = "The Boyer–Moore–Horspool? algorithm is a simplification of the?? ???Boyer–Moore algorithm using only the bad character rule.";
   my_regex::SolidPattern fpat;
   
   fpat.Reset(text);
   assert(fpat.AtomsNum() == 4);
   assert(fpat.TailLen() == 0);
   assert(fpat.Offset() == 0);

   const char* p = nullptr;
   size_t len;
   size_t offset;
   fpat.GetAtom(0, &p, len, offset);

   assert(p-text != 0);

   assert(*p == 'T');
   assert(len == 24);
   assert(offset == 0);

   fpat.GetAtom(1, &p, len, offset);

   assert(*p == ' ');
   assert(len == 37);
   assert(offset == 1);
   assert(text[25]==' ');

   fpat.GetAtom(2, &p, len, offset);

   assert(*p == ' ');
   assert(len == 1);
   assert(offset == 2);

   fpat.GetAtom(3, &p, len, offset);

   assert(*p == 'B');
   assert(len == 56);
   assert(offset == 3);

   fpat.GetAtom(100500, &p, len, offset);

   assert(p == '\0');

   printf("SolidPatternInitTest1 is OK\n");
}
static void SolidPatternInitTest2()
{
   char* text = "?????The Boyer–Moore–Horspool? algorithm is a simplification of the? Boyer–Moore algorithm using only the bad character rule.";
   my_regex::SolidPattern fpat;
   fpat.Reset(text);

   assert(fpat.AtomsNum() == 3);
   assert(fpat.TailLen() == 0);
   assert(fpat.Offset() == 5);

   const char* p = nullptr;
   size_t len;
   size_t offset;
   fpat.GetAtom(0, &p, len, offset);

   assert(p-text != 5);

   assert(*p == 'T');
   assert(len == 24);
   assert(offset == 5);

   fpat.GetAtom(1, &p, len, offset);

   assert(*p == ' ');
   assert(len == 37);
   assert(offset == 1);

   fpat.GetAtom(2, &p, len, offset);

   assert(*p == ' ');
   assert(len == 57);
   assert(offset == 1);

   fpat.GetAtom(3, &p, len, offset);

   assert(p == '\0');

   printf("SolidPatternInitTest2 is OK\n");
}
static void SolidPatternInitTest3()
{
   char* text = "The Boyer–Moore–Horspool? algorithm is a simplification of the? Boyer–Moore algorithm using only the bad character rule.???";
   my_regex::SolidPattern fpat;
   fpat.Reset(text);

   assert(fpat.AtomsNum() == 3);
   assert(fpat.TailLen() == 3);

   printf("SolidPatternInitTest3 is OK\n");
}

static void SolidPatternInitTest4()
{
   char* text = "The Boyer–Moore–Horspool algorithm is a simplification of the Boyer–Moore algorithm using only the bad character rule.";
   my_regex::SolidPattern fpat;
   fpat.Reset("??? Boyer–Moore–Horspool");

   assert(fpat.AtomsNum() == 1);
   assert(fpat.TailLen() == 0);

   const char* entry;

   const char* p = nullptr;
   size_t len;
   size_t offset;
   fpat.GetAtom(0, &p, len, offset);

   assert(p != '\0');
   assert(*p == ' ');
   assert(len == 21);
   assert(offset == 3);

   printf("SolidPatternInitTest4 is OK\n");
}

static void SolidPatternInitTest5()
{
   const char* text = "Boyer";
   my_regex::SolidPattern fpat;
   fpat.Reset(text);

   assert(fpat.AtomsNum() == 1);
   assert(fpat.TailLen() == 0);
   assert(fpat.Offset() == 0);

   const char* p = nullptr;
   size_t len;
   size_t offset;
   fpat.GetAtom(0, &p, len, offset);

   assert(p != '\0');
   assert(p != text);
   assert(*p == 'B');
   assert(p[4] == 'r');
   assert(len == 5);
   assert(offset == 0);


   const char* text2 = "????";
   fpat.Reset(text2);

   assert(fpat.AtomsNum() == 0);
   assert(fpat.TailLen() == 4);
   assert(fpat.Offset() == 0);

   fpat.GetAtom(0, &p, len, offset);

   assert(p == '\0');

   printf("SolidPatternInitTest5 is OK\n");
}

static void SolidPatternSearchTest1()
{
   char* text = "The Boyer–Moore–Horspool algorithm is a simplification of the Boyer–Moore algorithm using only the bad character rule.";

   const char* entry;

   my_regex::SolidPattern fpat;
   fpat.Reset("Boyer");
   entry = fpat.FindIn(text);
   assert(entry == text+4);
   assert(*entry == 'B');

   my_regex::SolidPattern fpat3;
   fpat3.Reset("Boyer");
   entry = fpat3.FindIn(text, 5);
   assert(entry == '\0');

   my_regex::SolidPattern fpat2;
   fpat2.Reset(".");
   entry = fpat2.FindIn(text);
   assert(*entry == '.');

   my_regex::SolidPattern fpat4;
   fpat4.Reset("simplificatin");
   entry = fpat4.FindIn(text);
   assert(entry == '\0');

   printf("SolidPatternSearchTest1 is OK\n");
}

static void SolidPatternSearchTest2()
{
   char* text = "The Boyer–Moore–Horspool algorithm is a simplification of the Boyer–Moore algorithm using only the bad character rule.";

   const char* entry;

   my_regex::SolidPattern fpat;
   fpat.Reset("Bo?er");
   entry = fpat.FindIn(text);
   assert(*entry == 'B');

   my_regex::SolidPattern fpat3;
   fpat3.Reset("Bo?er–");
   entry = fpat3.FindIn(text, 5);
   assert(entry == '\0');

   my_regex::SolidPattern fpat4;
   fpat4.Reset("simplificati?n");
   entry = fpat4.FindIn(text);
   assert(*entry == 's');

   printf("SolidPatternSearchTest2 is OK\n");
}

static void SolidPatternSearchTest3()
{
   char* text = "The Boyer–Moore–Horspool algorithm is a simplification of the Boyer–Moore algorithm using only the bad character rule.";

   const char* entry;

   my_regex::SolidPattern fpat;
   fpat.Reset("Boyer?Moore?Horspool");
   entry = fpat.FindIn(text);
   assert(*entry == 'B');
   assert(entry-text == 4);

   my_regex::SolidPattern fpat3;
   fpat3.Reset("Bo?er–");
   entry = fpat3.FindIn(text, 5);
   assert(entry == '\0');

   my_regex::SolidPattern fpat4;
   fpat4.Reset("simp???icati?n");
   entry = fpat4.FindIn(text);
   assert(*entry == 's');

   my_regex::SolidPattern fpat5;
   fpat5.Reset("??simp???icati?n");
   entry = fpat5.FindIn(text);
   assert(entry != '\0');
   assert(*entry != 's');
   assert(*entry == 'a');

   my_regex::SolidPattern fpat6;
   fpat6.Reset("??simp???icati?n????");
   entry = fpat6.FindIn(text);
   assert(entry != '\0');
   assert(entry-38 == text);
   assert(*entry != 's');
   assert(*entry == 'a');

   my_regex::SolidPattern fpat7;
   fpat7.Reset("rule??");
   entry = fpat7.FindIn(text);
   assert(entry == '\0');

   fpat7.Reset("rule?");
   entry = fpat7.FindIn(text);
   assert(entry != '\0');
   assert(*entry == 'r');

   fpat7.Reset("??? Boyer–Moore");
   entry = fpat7.FindIn(text);
   assert(entry != '\0');
   assert(*entry == 'T');

   fpat7.Reset("Boye???oore ");
   entry = fpat7.FindIn(text);
   assert(entry != '\0');
   assert(entry-text == 62);
   assert(*entry == 'B');

   fpat7.Reset("a?g?r?t?m?u");
   entry = fpat7.FindIn(text);
   assert(entry != '\0');
   assert(entry-text == 74);
   assert(*entry == 'a');

   fpat7.Reset("???");
   entry = fpat7.FindIn(text);
   assert(entry != '\0');
   assert(entry-text == 0);
   assert(*entry == 'T');

   printf("SolidPatternSearchTest3 is OK\n");
}

static void FloatingPatternResetTest1()
{
   my_regex::FloatingPattern fp;
   fp.Reset("*qwe*asd*zxc*");
   const char* str_pattern = nullptr;
   fp.ToString(&str_pattern);
   assert(str_pattern != nullptr);
   assert(strlen(str_pattern) == 13);
   assert(fp.MinLength() == 11);

   fp.Reset("***qwe****asd**zxc*");
   str_pattern = nullptr;
   fp.ToString(&str_pattern);
   assert(str_pattern != nullptr);
   assert(strlen(str_pattern) == 13);
   assert(fp.MinLength() == 11);
   assert(str_pattern[0] == '*');
   assert(str_pattern[3] != '*');
   assert(str_pattern[4] == '*');
   assert(str_pattern[5] != '*');
   assert(str_pattern[11] != '*');
   assert(str_pattern[12] == '*');

   fp.Reset("***");
   str_pattern = nullptr;
   fp.ToString(&str_pattern);
   assert(str_pattern != nullptr);
   assert(strlen(str_pattern) == 1);
   assert(fp.MinLength() == 0);
   assert(str_pattern[0] == '*');

   printf("FloatingPatternResetTest1 is OK\n");
}

static void FloatingPatternResetTest2()
{
   my_regex::FloatingPattern fp;
   fp.Reset("*qwe*asd*zxc*");
   assert(fp.left_aligned_ == nullptr);
   assert(fp.right_aligned_ == nullptr);
   assert(fp.free_pat_num_ == 3);

   fp.Reset("***qwe****asd**zxc*");
   assert(fp.left_aligned_ == nullptr);
   assert(fp.right_aligned_ == nullptr);
   assert(fp.free_pat_num_ == 3);

   fp.Reset("***");
   assert(fp.left_aligned_ == nullptr);
   assert(fp.right_aligned_ == nullptr);
   assert(fp.free_pat_num_ == 0);

   fp.Reset("qwe****asd**zxc*");
   assert(fp.left_aligned_ != nullptr);
   assert(fp.right_aligned_ == nullptr);
   assert(fp.free_pat_num_ == 2);

   fp.Reset("*qwe****asd**zxc");
   assert(fp.left_aligned_ == nullptr);
   assert(fp.right_aligned_ != nullptr);
   assert(fp.free_pat_num_ == 2);
   assert(fp.Offset() == 0);
   assert(fp.Tail() == 3);

   fp.Reset("qwe****asd**zxc");
   assert(fp.left_aligned_ != nullptr);
   assert(fp.right_aligned_ != nullptr);
   assert(fp.free_pat_num_ == 1);
   assert(fp.Offset() == 3);

   fp.Reset("???****asd**???");
   assert(fp.left_aligned_ != nullptr);
   assert(fp.right_aligned_ != nullptr);
   assert(fp.free_pat_num_ == 1);
   assert(fp.Offset() == 3);
   assert(fp.Tail() == 3);

   fp.Reset("The?");
   assert(fp.left_aligned_ != nullptr);
   assert(fp.right_aligned_ == nullptr);
   assert(fp.free_pat_num_ == 0);
   assert(fp.Offset() == 4);
   assert(fp.Tail() == 0);

   printf("FloatingPatternResetTest2 is OK\n");
}

static void FloatingPatternSearchTest1()
{
   const char* text = "The Boyer–Moore–Horspool algorithm is a simplification of the Boyer–Moore algorithm using only the bad character rule.";

   my_regex::FloatingPattern fp;
   fp.Reset("The Boyer**");
   const char* res = fp.DoesMatch(text, 30);
   assert(res != nullptr);
   assert(res == text);

   fp.Reset("The");
   res = nullptr;
   res = fp.DoesMatch(text);
   assert(res == nullptr);

   fp.Reset("The?Boye?*er ?ule?");
   res = nullptr;
   res = fp.DoesMatch(text);
   assert(res != nullptr);
   assert(res == text);

   fp.Reset("he?Boye?*er ?ule?");
   res = nullptr;
   res = fp.DoesMatch(text);
   assert(res == nullptr);

   fp.Reset("The?Boye?*er ?ule");
   res = nullptr;
   res = fp.DoesMatch(text);
   assert(res == nullptr);

   fp.Reset("The Boyer*er rule?");
   res = nullptr;
   res = fp.DoesMatch(text, strlen(text) -1);
   assert(res == nullptr);
   assert(res != text);

   printf("FloatingPatternSearchTest1 is OK\n");
}

static void FloatingPatternSearchTest2()
{
   const char* text = "The Boyer–Moore–Horspool algorithm is a simplification of the Boyer–Moore algorithm using only the bad character rule.";

   my_regex::FloatingPattern fp;
   fp.Reset("The Boyer*algorithm*rule*");
   const char* res = fp.DoesMatch(text);
   assert(res != nullptr);
   assert(res == text);

   fp.Reset("The Boyer*algorthm*rule*");
   res = nullptr;
   res = fp.DoesMatch(text);
   assert(res == nullptr);

   fp.Reset("The Boyer*algor*thm*rule*");
   res = nullptr;
   res = fp.DoesMatch(text);
   assert(res != nullptr);

   fp.Reset("The Boyer?");
   res = nullptr;
   res = fp.DoesMatch(text);
   assert(res == nullptr);

   printf("FloatingPatternSearchTest2 is OK\n");
}

static void FileSearchingTest1()
{
   const size_t max_line_size = 256*256;
   FILE *p_file;
   p_file = fopen("small_file.txt", "r");
   assert(p_file);
   char line[max_line_size];
   fgets(line, max_line_size, p_file);
   
   my_regex::FloatingPattern fp;
   fp.Reset("The");
   bool isOK = fp.DoesMatch(line);
   assert(!isOK);

   fp.Reset("The Boyer*algorthm*rule*");
   isOK = fp.DoesMatch(line);
   assert(!isOK);

   fp.Reset("The Boyer*algor*thm**");
   isOK = fp.DoesMatch(line);
   assert(isOK);

   fclose(p_file);

   printf("FileSearchingTest1 is OK\n");
}

static void FileSearchingTest2()
{
   const size_t max_line_size = 256*256;
   FILE *p_file;
   p_file = fopen("small_file.txt", "r");
   assert(p_file);
   char line[max_line_size];

   bool isOK = false;
   my_regex::FloatingPattern fp;
   fp.Reset("*shift*,*P**");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   assert(isOK);     
   assert(feof(p_file));     


   rewind(p_file);


   fp.Reset("?ore*=*moving*");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   assert(isOK);     
   assert(!feof(p_file));     


   rewind(p_file);


   fp.Reset("**********************k********************************=******************************n*************************");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   assert(isOK);     
   assert(!feof(p_file));     


   fp.Reset("*k??=??n*");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   assert(!isOK);     
   assert(feof(p_file));     


   fclose(p_file);

   printf("FileSearchingTest2 is OK\n");
}
static void MapViewSearchingTest1()
{
   const size_t max_line_size = 256*256;

   HANDLE hFile = CreateFile(L"small_file.txt", GENERIC_READ, 0, 
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (hFile == INVALID_HANDLE_VALUE) {
      assert(false);
      //chMB("File could not be opened.");
      //return(FALSE);
   }

   DWORD dwFileSize = GetFileSize(hFile, NULL);

   HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 
      0, /*dwFileSize + sizeof(WCHAR)*/0, NULL);

   if (hFileMap == NULL) {
      //chMB("File map could not be opened.");
      assert(false);
      CloseHandle(hFile);
      //return(FALSE);
   }

   PVOID pvFile = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);

   if (pvFile == NULL) {
      //chMB("Could not map view of file.");
      assert(false);
      CloseHandle(hFileMap);
      CloseHandle(hFile);
      //return(FALSE);
   }
   PSTR pchANSI = (PSTR) pvFile;


   char *pch = pchANSI, *ends = pch;

   my_regex::FloatingPattern fp;
   fp.Reset("*shift*,*P**");
   bool isOK = false;

   while (pch != NULL)
   {
      ends = strstr(pch,"\r\n");

      isOK = fp.DoesMatch(pch, (ends?ends:pch) - pch);

      if(isOK)  break;

      pch = ends ? ends+2 : nullptr;
   }

   assert(isOK);     
   //assert(feof(p_file));     

   //fgets(line, max_line_size, pchANSI);


   UnmapViewOfFile(pvFile);
   CloseHandle(hFileMap);

   // Remove trailing zero character added earlier.
   SetFilePointer(hFile, dwFileSize, NULL, FILE_BEGIN);
   SetEndOfFile(hFile);
   CloseHandle(hFile);

   /*
   FILE *p_file;
   p_file = fopen("small_file.txt", "r");
   assert(p_file);
   char line[max_line_size];

   bool isOK = false;
   my_regex::FloatingPattern fp;
   fp.Reset("*shift*,*P**");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   assert(isOK);     
   assert(feof(p_file));     


   rewind(p_file);


   fp.Reset("?ore*=*moving*");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   assert(isOK);     
   assert(!feof(p_file));     


   rewind(p_file);


   fp.Reset("**********************k********************************=******************************n*************************");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   assert(isOK);     
   assert(!feof(p_file));     


   fp.Reset("*k??=??n*");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   assert(!isOK);     
   assert(feof(p_file));     


   fclose(p_file);
   */
   printf("MapViewSearchingTest1 is OK\n");
}

}; // class DebugTest

#endif

#ifndef _DEBUG

void FileSearchingReleaseTest1()
{
   time_t start = time(nullptr);

   const size_t max_line_size = 256*256;
   FILE *p_file;
   p_file = fopen("animals.txt", "r");
   assert(p_file);
   char line[max_line_size];
   bool isOK = false;
   my_regex::FloatingPattern fp;


   fp.Reset("B237708F-6CFD-4972-9CE1-21AAF2B2E0A0");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   printf("assert#1 %s\n", ISOK(isOK));
   assert(isOK);     
   assert(feof(p_file));     


   rewind(p_file);


   fp.Reset("*98415C59-14AD-4A28-94D1-9551E3775C2C*");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   printf("assert#2 %s\n", ISOK(isOK));
   assert(isOK);     
   assert(!feof(p_file));     


   rewind(p_file);


   fp.Reset("*98415C59*14AD-????-94D1*9551E3775C2C*");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   printf("assert#3 %s\n", ISOK(isOK));
   assert(isOK);     
   assert(!feof(p_file));     


   rewind(p_file);


   fp.Reset("*98415C59-14AD-4A28-94D1??9551E3775C2C*");
   do {
      fgets(line, max_line_size, p_file);
      isOK = fp.DoesMatch(line);
   } 
   while(!isOK && !feof(p_file));

   printf("assert#4 %s\n", ISOK(!isOK));
   assert(!isOK);     
   assert(feof(p_file));     

   fclose(p_file);


   time_t end = time(nullptr);
   printf("%ds elapsed\n", end-start);


   printf("FileSearchingReleaseTest1 is OK\n");
}


void MapViewSearchingReleaseTest1()
{
   const size_t max_line_size = 256*256;
   char line[max_line_size];

   time_t start = time(nullptr);

   HANDLE hFile = CreateFile(L"big_file.txt", GENERIC_READ, 0, 
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (hFile == INVALID_HANDLE_VALUE) {
      assert(false);
      //chMB("File could not be opened.");
      //return(FALSE);
      return;
   }

   printf("File opened\n");

   DWORD dwFileSize = GetFileSize(hFile, NULL);
   printf("File size: %d\n", dwFileSize);

   HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 
      0, /*dwFileSize + sizeof(WCHAR)*/0, NULL);

   if (hFileMap == NULL) {
      //chMB("File map could not be opened.");
      assert(false);
      CloseHandle(hFile);
      //return(FALSE);
      return;
   }
   printf("Mapping created\n");

   PVOID pvFile = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);

   if (pvFile == NULL) {
      //chMB("Could not map view of file.");
      assert(false);
      CloseHandle(hFileMap);
      CloseHandle(hFile);
      //return(FALSE);
      return;
   }
   PSTR pchANSI = (PSTR) pvFile;
   //pchANSI[dwFileSize / sizeof(CHAR)] = '\0';

   printf("Mapping started\n");


   char *pch = pchANSI, *ends = pch;

   my_regex::FloatingPattern fp;
   fp.Reset("*98415C59-14AD-4A28-94D1-9551E3775C2C*");
   bool isOK = false;

   while (pch != NULL)
   {
      ends = strstr(pch,"\r\n");
      while(ends == pch)
      {
         pch += 2;
         ends = strstr(pch,"\r\n");
      }

      isOK = fp.DoesMatch(pch, (ends ? ends-pch : dwFileSize));

      if(isOK)  break;

      pch = ends ? ends+2 : nullptr;
   }

   assert(isOK); 
   printf("assert#1 %s\n", ISOK(isOK));


   pch = pchANSI, ends = pch;

   fp.Reset("B237708F-6CFD-4972-9CE1-21AAF2B2E0A0");
   isOK = false;

   while (pch != NULL)
   {
      ends = strstr(pch,"\r\n");
      while(ends == pch)
      {
         pch += 2;
         ends = strstr(pch,"\r\n");
      }

      isOK = fp.DoesMatch(pch, (ends ? ends-pch : dwFileSize));

      if(isOK)  break;

      pch = ends ? ends+2 : nullptr;
   }

   assert(isOK); 
   printf("assert#2 %s\n", ISOK(isOK));


   pch = pchANSI, ends = pch;

   fp.Reset("*98415C59*14AD-????-94D1*9551E3775C2C*");
   isOK = false;

   while (pch != NULL)
   {
      ends = strstr(pch,"\r\n");
      while(ends == pch)
      {
         pch += 2;
         ends = strstr(pch,"\r\n");
      }

      isOK = fp.DoesMatch(pch, (ends ? ends-pch : dwFileSize));

      if(isOK)  break;

      pch = ends ? ends+2 : nullptr;
   }

   assert(isOK); 
   printf("assert#3 %s\n", ISOK(isOK));


   pch = pchANSI, ends = pch;

   fp.Reset("*98415C59-14AD-4A28-94D1??9551E3775C2C*");
   isOK = false;

   while (pch != NULL)
   {
      ends = strstr(pch,"\r\n");
      while(ends == pch)
      {
         pch += 2;
         ends = strstr(pch,"\r\n");
      }

      if(ends) break;

      isOK = fp.DoesMatch(pch, ends-pch);

      if(isOK)  break;

      pch = ends ? ends+2 : nullptr;
   }

   assert(!isOK); 
   printf("assert#4 %s\n", ISOK(!isOK));




   //assert(feof(p_file));     

   //fgets(line, max_line_size, pchANSI);


   UnmapViewOfFile(pvFile);
   CloseHandle(hFileMap);

   // Remove trailing zero character added earlier.
   //SetFilePointer(hFile, dwFileSize, NULL, FILE_BEGIN);
   //SetEndOfFile(hFile);
   CloseHandle(hFile);

   time_t end = time(nullptr);
   printf("%ds elapsed\n", end-start);

   printf("MapViewSearchingReleaseTest1 is OK\n");

}
#endif

void CLogReaderTest1()
{
   const size_t max_line_size = 256*256;
   char buffer[max_line_size];

   time_t start = time(nullptr);

   my_regex::CLogReader log_reader;
   if(!log_reader.Open("big_file.txt"))
      printf("Error opening file\n");


   log_reader.SetFilter("B237708F-6CFD-4972-9CE1-21AAF2B2E0A0");

   while(log_reader.GetNextLine(buffer, max_line_size))
   {
      printf("line: %s\n", buffer);
   }

   
   log_reader.SetFilter("*98415C59-14AD-4A28-94D1-9551E3775C2C*");

   while(log_reader.GetNextLine(buffer, max_line_size))
   {
      printf("line: %s\n", buffer);
   }


   log_reader.SetFilter("*98415C59*14AD-????-94D1*9551E3775C2C*");

   while(log_reader.GetNextLine(buffer, max_line_size))
   {
      printf("line: %s\n", buffer);
   }


   log_reader.SetFilter("*98415C59-14AD-4A28-94D1??9551E3775C2C*");

   while(log_reader.GetNextLine(buffer, max_line_size))
   {
      printf("line: %s\n", buffer);
   }
   
   time_t end = time(nullptr);
   printf("%ds elapsed\n", end-start);

   log_reader.Close();

   printf("CLogReaderTest1 is OK\n");
}





void RunTests()
{

#ifdef _DEBUG 
   DebugTest::BMTest();
   DebugTest::SolidPatternInitTest1();
   DebugTest::SolidPatternInitTest2();
   DebugTest::SolidPatternInitTest3();
   DebugTest::SolidPatternInitTest4();
   DebugTest::SolidPatternInitTest5();

   DebugTest::SolidPatternSearchTest1();
   DebugTest::SolidPatternSearchTest2();
   DebugTest::SolidPatternSearchTest3();

   DebugTest::FloatingPatternResetTest1();
   DebugTest::FloatingPatternResetTest2();

   DebugTest::FloatingPatternSearchTest1();
   DebugTest::FloatingPatternSearchTest2();

   DebugTest::FileSearchingTest1();
   DebugTest::FileSearchingTest2();
   DebugTest::MapViewSearchingTest1();

#endif

#ifndef _DEBUG

   //FileSearchingReleaseTest1();
//    MapViewSearchingReleaseTest1();
#endif
   CLogReaderTest1();

}

