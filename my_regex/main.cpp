#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "CLogReader.h"

void RunTests();

int main(int argc,  char** argv)
{
   if (argc != 3) {
      printf("Usage: %s \"<FILE_NAME>\" \"<PATTERN>\"\n", argv[0]);
      return EXIT_FAILURE;
   }

   const size_t max_line_size = 256*256;
   char buffer[max_line_size];

   time_t start = time(nullptr);

   my_regex::CLogReader log_reader;
   if(!log_reader.Open(argv[1])) {
      printf("Error opening file\n");
      return EXIT_FAILURE;
   }

   if(!log_reader.SetFilter(argv[2])) {
      printf("Error making pattern\n");
      return EXIT_FAILURE;
   }
   
   printf("Searching for any \"%s\" in \"%s\"...\n", argv[2], argv[1]);

   bool nothing_was_found = true;
   while(log_reader.GetNextLine(buffer, max_line_size))
   {
      printf("\t%s\n", buffer);
      nothing_was_found = false;
   }
   if(nothing_was_found)
      printf("Nothing was found\n");

   time_t end = time(nullptr);
   printf("%ds elapsed\n", end-start);

   log_reader.Close();

   return EXIT_SUCCESS;

   //    RunTests();
}