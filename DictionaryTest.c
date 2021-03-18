#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Dictionary.h"

int main(){
   Dictionary D = newDictionary();
   char* str;
   char* v;
   char* str1[] = {"axiopisty", "babbittry", "iniquitously", "peccability", "symptomatology"};
   char* str2[] = {"this", "is", "a", "test", "array"};
   
   //test insert()
   for(int i = 0; i < 5; i++){
       insert(D, str1[i], str2[i]);
   }
   
   //test toString()
   str = DictionaryToString(D);
   printf("%s\n", str);
   free(str);
   
   //test size()
   printf("size(D) after insert test = %d\n", size(D));
   printf("\n");
   
   //test lookup()
   for(int i = 0; i < 5; i++){
       v = lookup(D, str1[i]);
       printf("key=%s, value=%s\n", str1[i], v);
   }
   printf("\n");

   //insert(D, "peccability","blah");  //duplicate key error
   
   //test delete()
   for(int i = 3; i < 5; i++){
	   delete(D, str1[i]);
   }
   
   str = DictionaryToString(D);
   printf("%s\n", str);
   free(str);

   //delete(D, "error");  //key not found error

   printf("size(D) after delete() test = %d\n", size(D));
   printf("\n");
   
   //test lookup after delete()
   for(int i = 0; i < 5; i++){
       v = lookup(D, str1[i]);
       printf("key=%s, value=%s\n", str1[i], v);
   }
   printf("\n");
   
   //test makeEmpty()
   makeEmpty(D);
   printf("size(D) after makeEmpty() test= %d\n", size(D));
   
   str = DictionaryToString(D);
   printf("%s\n", str);
   free(str);
   
   //test freeDictionary()
   freeDictionary(&D);

   return EXIT_SUCCESS;
}