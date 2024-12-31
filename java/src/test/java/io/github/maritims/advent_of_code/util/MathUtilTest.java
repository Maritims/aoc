package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class MathUtilTest {

    public static Stream<Arguments> concatenate() {
        return Stream.of(
                Arguments.arguments(1L, 2L, 12L),
                Arguments.arguments(1723891L, 2128390L, 17238912128390L)
        );
    }

    @ParameterizedTest
    @MethodSource
    void concatenate(long a, long b, long expectedResult) {
        assertEquals(expectedResult, MathUtil.concatenate(a, b));
    }
}