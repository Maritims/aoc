#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maritims_md5.h"
#include "test4c.h"

void test_digest(TestResults *testResults) {
	typedef TestCaseForInputWithExpectation(char *, uint8_t *) TestCase;
	TestCase testCases[] = {
		/*{"", (uint8_t[16]){
			0xd4, 0x1d, 0x8c, 0xd9,
			0x8f, 0x00, 0xb2, 0x04,
			0xe9, 0x80, 0x09, 0x98,
			0xec, 0xf8, 0x42, 0x7e}},*/
		{"abc", (uint8_t[16]){
			0x90, 0x01, 0x50, 0x98,
			0x3c, 0xd2, 0x4f, 0xb0,
			0xd6, 0x96, 0x3f, 0x7d,
			0x28, 0xe1, 0x7f, 0x72}},
		{"message digest", (uint8_t[16]){
			0xf9, 0x6b, 0x69, 0x7d,
			0x7c, 0xb7, 0x93, 0x8d,
			0x52, 0x5a, 0x2f, 0x31,
			0xaa, 0xf1, 0x61, 0xd0}},
		{"abcdefghijklmnopqrstuvwxyz", (uint8_t[16]) {
			0xc3, 0xfc, 0xd3, 0xd7,
			0x61, 0x92, 0xe4, 0x00,
			0x7d, 0xfb, 0x49, 0x6c,
			0xca, 0x67, 0xe1, 0x3b}},
		{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", (uint8_t[16]){
			0xd1, 0x74, 0xab, 0x98,
			0xd2, 0x77, 0xd9, 0xf5,
			0xa5, 0x61, 0x1c, 0x2c,
			0x9f, 0x41, 0x9d, 0x9f}},
		{"12345678901234567890123456789012345678901234567890123456789012345678901234567890", (uint8_t[16]){
			0x57, 0xed, 0xf4, 0xa2,
			0x2b, 0xe3, 0xc9, 0x55,
			0xac, 0x49, 0xda, 0x2e,
			0x21, 0x07, 0xb6, 0x7a}}
	};

	for (uint32_t i = 0; i < sizeof(testCases) / sizeof(testCases[0]); i++)
	{
		printf("test_md5_digest(\"%s\"): ", testCases[i].input);
		uint8_t *output = md5_digest(testCases[i].input);

		if (memcmp(output, testCases[i].expectedResult, 16) == 0)
		{
			printf("OK\n");
			testResults->succeeded++;
			free(output);
		}
		else
		{
			printf("Not OK. Expected ");//%x but got %x\n", testCases[i].expectedResult, output);
			for(uint32_t j = 0; j < 16; j++) {
				printf("%02x", testCases[i].expectedResult[j]);
			}
			printf(" but got ");
			for(uint32_t j = 0; j < 16; j++) {
				printf("%02x", output[j]);
			}
			printf("\n");
			free(output);
			return;
		}
	}
}

void test_md5_digest(const char *str, uint8_t expected_result[16]) {
    uint8_t *output = md5_digest(str);
	if (memcmp(output, expected_result, 16) == 0)
	{
		printf("%s(\"%s\") passed\n", __func__, str);
        free(output);
        return;
    }

    printf("%s(\"%s\") failed\n", __func__, str);
    exit(EXIT_FAILURE);
}

int main(void)
{
	test_md5_digest("a", (uint8_t[16]){ 0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8, 0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61});
    printf("All tests passed\n");
}
