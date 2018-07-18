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
    HashTable *t = ht_create();

    int *newValue = xmalloc(sizeof(int)); 
    *newValue = 10;
    ht_insert(t, "by the power of grayskull", newValue);

    int *newValue2 = xmalloc(sizeof(int)); 
    *newValue2 = 11;
    ht_insert(t, "this is a test key", newValue2);

    int *newValue3a = xmalloc(sizeof(int));
    *newValue3a = 12;
    int *newValue3b = xmalloc(sizeof(int));
    *newValue3b = 15;
    ht_insert(t, "wawaweewah", newValue3a);
    ht_insert(t, "wawaweewah", newValue3b);

    ht_insert(t, "stringkey", str_copy("This is a stored value!"));

    int *found = ht_search(t, "by the power of grayskull");
    int *found2 = ht_search(t, "this is a test key");
    int *found3 = ht_search(t, "this is another test key");
    int *found4 = ht_search(t, "key");
    int *found5 = ht_search(t, "wawaweewah");
    char *found6 = ht_search(t, "stringkey");

    typedef struct TestStruct {
        int a;
        int b;
    } TestStruct;
    TestStruct *testStruct = xmalloc(sizeof(TestStruct));
    testStruct->a = 13;
    testStruct->b = 14;

    ht_insert(t, "test-struct", testStruct);
    TestStruct *foundStruct = ht_search(t, "test-struct");

    assert(*found == 10);
    assert(*found2 == 11);
    assert(found3 == 0);
    assert(found4 == 0);
    assert(*found5 == 15);
    assert(str_equal(found6, "This is a stored value!"));
    assert(foundStruct->a == 13 && foundStruct->b == 14);

    for(int a = 0; a < 50000; a++) {
    //for(int a = 0; a < 10000; a++) {
        dbg("a: %d, len: %d, cap: %d", a, t->len, t->cap);
        // To test ht_grow()
        for(int i = 0; i < 10000; i++) {
            int *forLoopValue = xmalloc(sizeof(int));
            *forLoopValue = i;
            char *forLoopKey = str_inttostr(i);
            ht_insert(t, forLoopKey, forLoopValue);
            free(forLoopKey);
        }

        // To test ht_delete()
        int testKey1 = stb_rand() % 10000;
        int testKey2 = stb_rand() % 10000;
        int testKey3 = stb_rand() % 10000;
        int testKey4 = stb_rand() % 10000;
        int testKey5 = stb_rand() % 10000;

        char *testStr1 = str_inttostr(testKey1);
        char *testStr2 = str_inttostr(testKey2);
        char *testStr3 = str_inttostr(testKey3);
        char *testStr4 = str_inttostr(testKey4);
        char *testStr5 = str_inttostr(testKey5);

        ht_delete(t, testStr1);
        ht_delete(t, testStr1); // delete an entry that doesn't exist
        ht_delete(t, testStr2);
        ht_delete(t, testStr3);
        ht_delete(t, testStr4);
        ht_delete(t, testStr5);

        free(testStr1);
        free(testStr2);
        free(testStr3);
        free(testStr4);
        free(testStr5);

        for(int i = 0; i < 10000; i++) {
            char *key = str_inttostr(i);
            int *value = ht_search(t, key);
            free(key);
            if(i == testKey1 || i == testKey2 || i == testKey3 || i == testKey4 || i == testKey5) {
                assert(value == 0);
            }
            else { assert(i == *value); }
        }
    }

    ht_free(t);

    dbg("End of tests");
}
