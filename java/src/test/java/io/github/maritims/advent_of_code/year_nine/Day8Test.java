package io.github.maritims.advent_of_code.year_nine;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;

class Day8Test {
    public static Stream<Arguments> solvePartOneForFilename() {
        return Stream.of(
                Arguments.arguments("sample1.txt", 2),
                Arguments.arguments("sample2.txt", 4),
                Arguments.arguments("sample3.txt", 4),
                Arguments.arguments("sample4.txt", 14),
                Arguments.arguments("input.txt", 0)
        );
    }

    @ParameterizedTest
    @MethodSource
    void solvePartOneForFilename(String filename, long expectedResult) {
        // arrange
        var sut = new Day8(filename);
        sut.initialize();

        // act
        var result = sut.solvePartOne();

        // assert
        assertEquals(expectedResult, result);
    }

}