package io.github.maritims.advent_of_code.year_nine;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class Day2Test {

    public static Stream<Arguments> solvePartOne() {
        return Stream.of(
                Arguments.arguments(true, 2),
                Arguments.arguments(false, 598)
        );
    }

    public static Stream<Arguments> solvePartTwo() {
        return Stream.of(
                Arguments.arguments(true, 4),
                Arguments.arguments(false, 634)
        );
    }

    @ParameterizedTest
    @MethodSource
    void solvePartOne(boolean useSampleData, int expectedResult) {
        var sut = new Day2(useSampleData);
        var result = sut.solvePartOne();
        assertEquals(expectedResult, result);
    }

    @ParameterizedTest
    @MethodSource
    void solvePartTwo(boolean useSampleData, int expectedResult) {
        var sut = new Day2(useSampleData);
        var result = sut.solvePartTwo();
        assertEquals(expectedResult, result);
    }
}