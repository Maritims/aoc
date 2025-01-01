package io.github.maritims.advent_of_code.year_nine;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.params.provider.Arguments.*;

class Day8Test {
    public static Stream<Arguments> solvePartOneForFilename() {
        return Stream.of(
                arguments("sample1.txt", 2),
                arguments("sample2.txt", 4),
                arguments("sample3.txt", 4),
                arguments("sample.txt", 14),
                arguments("input.txt", 320)
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