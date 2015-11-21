#include <cassert>
#include <cstdio>

#include "SolidPattern.h"


void SolidPatternInitTest1()
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
void SolidPatternInitTest2()
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
void SolidPatternInitTest3()
{
   char* text = "The Boyer–Moore–Horspool? algorithm is a simplification of the? Boyer–Moore algorithm using only the bad character rule.???";
   my_regex::SolidPattern fpat;
   fpat.Reset(text);

   assert(fpat.AtomsNum() == 3);
   assert(fpat.TailLen() == 3);

   printf("SolidPatternInitTest3 is OK\n");
}

void SolidPatternInitTest4()
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


void SolidPatternSearchTest1()
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

void SolidPatternSearchTest2()
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

void SolidPatternSearchTest3()
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

   printf("SolidPatternSearchTest3 is OK\n");
}

void RunTests()
{
   SolidPatternInitTest1();
   SolidPatternInitTest2();
   SolidPatternInitTest3();
   SolidPatternInitTest4();

   SolidPatternSearchTest1();
   SolidPatternSearchTest2();
   SolidPatternSearchTest3();

}