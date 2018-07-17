#include "lib-mmy.h"

int main() {
   dbg("Starting tests...");

   // Test 000.
   assert(1024 == kilobytes(1));
   assert((1024*1024) == megabytes(1));
   assert(((u64)10*1024*1024*1024) == gigabytes(10));

   // log_err("Log error: %d, %s", 42, "string literal");
   // log_warn("Log warning: %d, %s", 12, "another string literal");
   // log_info("Log info: %d, %s", 13, "yet another string literal");
   // dbg("Test msg: %d, %s", 14, "not another bloody string literal");


   // Test 001.


   // Test 002.
   //for(int i = 0; i < 10; i++) {
   //   unsigned long rand = stb_rand();
   //   dbg("rand: %lu\t\t%d", rand, rand >= 0 && rand <= ULONG_MAX);
   //   double frand = stb_frand();
   //   dbg("frand: %f\t\t%d", frand, frand >= 0 && frand < 1);
   //}

   for(int i = 0; i < 1000000; i++) {
      unsigned long rand = stb_rand();
      assert(rand >= 0 && rand < ULONG_MAX);

      double frand = stb_frand();
      assert(frand >= 0 && frand < 1);
   }


   // Test 003.
   assert(mth_min(1, 2) == 1);
   assert(mth_max(1, 2) == 2);


   // Test 004.
   assert(str_len("123") == 3);
   assert(str_len("") == 0);
   assert(str_len("0123456789") == 10);

   assert(str_equal("123", "123"));
   assert(!str_equal("123", "1234"));
   assert(!str_equal("12345", "1234"));

   char *str = "a string";
   char *copy = str_copy(str);
   copy[0] = 'a';
   copy[1] = ' ';
   assert(str_equal(str, copy));
   free(copy);

   assert(str_beginswith(str, "a stri"));
   assert(str_endswith(str, "tring"));

   str = str_copy("Test ");
   str = str_concat(str, "string");
   assert(str_equal(str, "Test string"));
   free(str);

   str = str_copy("SHoUT");
   str_lower(str);
   assert(str_equal(str, "shout"));
   free(str);

   str = str_copy("quIet");
   str_upper(str);
   assert(str_equal(str, "QUIET"));
   free(str);

   str = str_copy("aaalksfjnhekwjbegjabwegij");
   assert(str_isalpha(str));
   str[10] = '#';
   assert(!str_isalpha(str));
   free(str);

   str = str_copy("-190572985672019359876298");
   assert(str_isint(str));
   free(str);

   str = str_copy("eeeeeeeeeeeklhgfgh         ");
   assert(str_equal(str_rstrip(str, ' '), "eeeeeeeeeeeklhgfgh"));
   assert(str_equal(str_lstrip(str, 'e'), "klhgfgh"));
   free(str);

   str = str_copy("(),(abc),");
   assert(str_equal(str_strip(str, "(),"), "abc"));
   free(str);

   str = str_copy("agfecbd");
   str_sort(str);
   assert(str_equal(str, "abcdefg"));
   free(str);

   str = str_copy("This:Is::A:Test:To:Use:With:Split::");
   int size;
   char** split = str_split(str, ':', &size);
   assert(size == 11);
   assert(str_equal(split[0], "This"));
   assert(str_equal(split[5], "To"));
   assert(str_equal(split[8], "Split"));
   assert(str_equal(split[9], ""));
   assert(str_equal(split[10], ""));
   
   assert(str_toint("1234") == 1234);
   assert(str_toint("-12345") == -12345);

   // Have a version of assert for things that should break?
   // shouldBreak(str_toint("12-12"));
   

   // 005. Tests
   #include <stdarg.h>
   #define arr_printf(b, ...) ((b) = arr__printf((b), __VA_ARGS__))
   char *arr__printf(char *buf, const char *fmt, ...) {
       va_list args;
       va_start(args, fmt);
       size_t cap = arr_cap(buf) - arr_len(buf);
       size_t n = 1 + vsnprintf(arr_end(buf), cap, fmt, args);
       va_end(args);
       if (n > cap) {
           arr_fit(buf, n + arr_len(buf));
           va_start(args, fmt);
           size_t new_cap = arr_cap(buf) - arr_len(buf);
           n = 1 + vsnprintf(arr_end(buf), new_cap, fmt, args);
           assert(n <= new_cap);
           va_end(args);
       }
       arr__hdr(buf)->len += n - 1;
       return buf;
   }
   
   int *buf = NULL;
   assert(arr_len(buf) == 0);
   int n = 1024;
   for (int i = 0; i < n; i++) {
       arr_push(buf, i);
   }
   assert(arr_len(buf) == n);
   for (size_t i = 0; i < arr_len(buf); i++) {
       assert(buf[i] == i);
   }
   arr_free(buf);
   assert(buf == NULL);
   assert(arr_len(buf) == 0);

   char *bstr = NULL;
   arr_printf(bstr, "One: %d\n", 1);
   assert(str_equal(bstr, "One: 1\n"));
   arr_printf(bstr, "Hex: 0x%x\n", 0x12345678);
   assert(str_equal(bstr, "One: 1\nHex: 0x12345678\n"));

   // 006. Tests
   HashTable *t = xmalloc(sizeof(HashTable));
   t->len = 0;
   t->cap = 1024;
   t->buf = (HtRecord*)xcalloc(1, sizeof(HtRecord) * t->cap); // memory needs to be initialised to zero, hence calloc

   int *newValue = xmalloc(sizeof(int)); 
   *newValue = 10;
   ht_insert(t, "by the power of grayskull", newValue);

   int *newValue2 = xmalloc(sizeof(int)); 
   *newValue2 = 11;
   ht_insert(t, "this is a test key", newValue2);

   int *newValue3 = xmalloc(sizeof(int));
   *newValue3 = 12;
   ht_insert(t, "wawaweewah", newValue3);

   ht_insert(t, "stringkey", str_copy("This is a stored value!"));

   int *found = ht_search(t, "by the power of grayskull");
   int *found2 = ht_search(t, "this is a test key");
   int *found3 = ht_search(t, "this is another test key");
   int *found4 = ht_search(t, "key");
   int *found5 = ht_search(t, "wawaweewah");
   char *found6 = ht_search(t, "stringkey");

   assert(*found == 10);
   assert(*found2 == 11);
   assert(found3 == 0);
   assert(found4 == 0);
   assert(*found5 == 12);
   assert(str_equal(found6, "This is a stored value!"));

   dbg("End of tests");
}
