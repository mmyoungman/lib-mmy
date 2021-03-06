#define TEST
#define MEM_DEBUG
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
    assert(mth_min(1, 1) == 1);
    assert(mth_max(1, 2) == 2);
    assert(mth_max(2, 2) == 2);


    // Test 004.
    assert(str_len("123") == 3);
    assert(str_len("") == 0);
    assert(str_len("0123456789") == 10);

    assert(str_equal("123", "123"));
    assert(!str_equal("123", "1234"));
    assert(!str_equal("12345", "1234"));
    assert(!str_equal("12345", "2345"));

    char *str = "a string";
    char *copy = str_copy(str);
    assert(str_equal(str, copy));
    copy[0] = ' ';
    assert(!str_equal(str, copy));
    copy[0] = '\0';
    assert(!str_equal(str, copy));
    str = "\0 string";
    assert(str_equal(str, copy));
    free(copy);

    str = "a string";
    assert(str_beginswith(str, "a stri"));
    assert(str_endswith(str, "tring"));

    str = str_copy("Test ");
    str = str_concat(str, "string");
    assert(str_equal(str, "Test string"));
    free(str);

    str = str_copy("SHoUT");
    str_lower(str);
    assert(str_equal(str, "shout"));
    str_upper(str);
    assert(str_equal(str, "SHOUT"));
    free(str);

    str = str_copy("quIet");
    str_upper(str);
    assert(str_equal(str, "QUIET"));
    str_lower(str);
    assert(str_equal(str, "quiet"));
    free(str);

    str = str_copy("aaalksfjnhekwjbegjabwegij");
    assert(str_isalpha(str));
    str[10] = '#';
    assert(!str_isalpha(str));
    free(str);

    str = "12345";
    assert(str_isint(str));
    str = "-190572985672019359876298";
    assert(str_isint(str));
    str = " 093120";
    assert(!str_isint(str));
    str = "-3095-409373";
    assert(!str_isint(str));
    str = "-3095409373-";
    assert(!str_isint(str));
    str = "-";
    assert(!str_isint(str));
    str = "-0101";
    assert(!str_isint(str));

    str = str_copy("eeeeeeeeeeeklhgfgh         ");
    assert(str_equal(str_rstrip(str, ' '), "eeeeeeeeeeeklhgfgh"));
    assert(str_equal(str_lstrip(str, 'e'), "klhgfgh"));
    free(str);

    str = str_copy("(),(abc),");
    assert(str_equal(str_strip(str, "(),"), "abc"));
    free(str);

    str = str_copy(",((),(abc),");
    assert(str_equal(str_strip(str, "(,"), "),(abc)"));
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
    assert(str_equal(split[1], "Is"));
    assert(str_equal(split[2], ""));
    assert(str_equal(split[3], "A"));
    assert(str_equal(split[4], "Test"));
    assert(str_equal(split[5], "To"));
    assert(str_equal(split[6], "Use"));
    assert(str_equal(split[7], "With"));
    assert(str_equal(split[8], "Split"));
    assert(str_equal(split[9], ""));
    assert(str_equal(split[10], ""));
    free(str);
    free(split);

    str = str_copy("Waa!");
    split = str_split(str, ':', &size); 
    assert(size == 1);
    assert(str_equal(split[0], "Waa!"));
    free(str);
    free(split);

    assert(str_toint("1234") == 1234);
    assert(str_toint("-12345") == -12345);

    str = str_inttostr(1234);
    assert(str_equal(str, "1234"));
    free(str);
    
    str = str_inttostr(-1234);
    assert(str_equal(str, "-1234"));
    free(str);
    
    str = str_inttostr(-12349876);
    assert(str_equal(str, "-12349876"));
    free(str);
    
    shouldAssert(str_toint("12-12"));
    shouldAssert(str_toint("--1212"));
    shouldAssert(str_toint("1212-"));

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
    assert(t->len == 1);

    int *newValue2 = xmalloc(sizeof(int)); 
    *newValue2 = 11;
    ht_insert(t, "this is a test key", newValue2);
    assert(t->len == 2);

    int *newValue3a = xmalloc(sizeof(int));
    *newValue3a = 12;
    int *newValue3b = xmalloc(sizeof(int));
    *newValue3b = 15;
    ht_insert(t, "wawaweewah", newValue3a);
    assert(t->len == 3);
    ht_insert(t, "wawaweewah", newValue3b);
    assert(t->len == 3);

    ht_insert(t, "stringkey", str_copy("This is a stored value!"));
    assert(t->len == 4);

    int *found = ht_search(t, "by the power of grayskull");
    assert(*found == 10);

    int *found2 = ht_search(t, "this is a test key");
    assert(*found2 == 11);

    int *found3 = ht_search(t, "this is another test key");
    assert(found3 == 0);

    int *found4 = ht_search(t, "key");
    assert(found4 == 0);

    int *found5 = ht_search(t, "wawaweewah");
    assert(*found5 == 15);

    char *found6 = ht_search(t, "stringkey");
    assert(str_equal(found6, "This is a stored value!"));

    typedef struct TestStruct {
        int a;
        int b;
    } TestStruct;
    TestStruct *testStruct = xmalloc(sizeof(TestStruct));
    testStruct->a = 13;
    testStruct->b = 14;

    ht_insert(t, "test-struct", testStruct);
    assert(t->len == 5);
    TestStruct *foundStruct = ht_search(t, "test-struct");
    assert(foundStruct->a == 13 && foundStruct->b == 14);

    for(int a = 0; a < 1000; a++) { // to test for memory leaks
        for(int i = 0; i < 10000; i++) {
            char *forLoopKey = str_inttostr(i);
            int *forLoopValue = xmalloc(sizeof(int));
            *forLoopValue = i;
            ht_insert(t, forLoopKey, forLoopValue);
            free(forLoopKey);
        }

        assert(t->len == 10005);

        // To test ht_delete()
        int testKey1 = stb_rand() % 10000;
        int testKey2 = stb_rand() % 10000;
        int testKey3 = stb_rand() % 10000; 

        while(testKey2 == testKey1) testKey2 = stb_rand() % 10000;
        while(testKey3 == testKey1 || testKey3 == testKey2) testKey3 = stb_rand() % 10000;

        char *testStr1 = str_inttostr(testKey1);
        char *testStr2 = str_inttostr(testKey2);
        char *testStr3 = str_inttostr(testKey3);

        ht_delete(t, testStr1);
        assert(t->len == 10004);
        ht_delete(t, testStr1); // delete an entry that doesn't exist
        assert(t->len == 10004); // because nothing deleted
        ht_delete(t, testStr2);
        assert(t->len == 10003);
        ht_delete(t, testStr3);
        assert(t->len == 10002);

        free(testStr1);
        free(testStr2);
        free(testStr3);

        for(int i = 0; i < 10000; i++) {
            char *key = str_inttostr(i);
            int *value = ht_search(t, key);
            free(key);
            if(i == testKey1 || i == testKey2 || i == testKey3) {
                assert(!value);
            }
            else { assert(i == *value); }
        }

        assert(t->len * 2 < t->cap);
        assert(t->cap <= 32768);
    }

    ht_free(t);


    // 007 tests

    int *memory = xmalloc(sizeof(int));
    xfree(memory);
    xfree(memory);

    dbg("End of tests");
}
