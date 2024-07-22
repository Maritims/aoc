#include <stdio.h>

#include "testutils.h"
#include "bitutils.h"

typedef struct {
    uint32_t input;
    uint32_t expectedResult;
} TestCase;

void test_to_little_endian(TestCase *testCases, TestResults *testResults) {
    for(int i = 0; i < sizeof(testCases) / sizeof(testCases[0]); i++) {
        uint32_t result;
        to_little_endian_uint32_t(&testCases[0].input, &result);
        if(result == testCases[i].expectedResult) {
            printf("OK\n");
            testResults->succeeded++;
        } else {
            fprintf(stderr, "Not OK. Expected %#010x but got %#010x\n", testCases[i].expectedResult, result);
            testResults->failed++;
        }
    }
}

int main(void) {
    TestCase testCases[] = {
        { 0x12345678, 0x78563412 },
        { 0x87654321, 0x21436587 }
    };
    TestResults testResults;
    test_to_little_endian(testCases, &testResults);
}