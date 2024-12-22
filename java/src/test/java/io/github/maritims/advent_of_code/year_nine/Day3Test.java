package io.github.maritims.advent_of_code.year_nine;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class Day3Test {
    public static Stream<Arguments> solvePartOne() {
        return Stream.of(
                Arguments.arguments(true, 161),
                Arguments.arguments(false, 187194524)
        );
    }

    public static Stream<Arguments> solvePartTwo() {
        return Stream.of(
                Arguments.arguments(true, 48),
                Arguments.arguments(false, 127092535)
        );
    }

    @ParameterizedTest
    @MethodSource
    public void solvePartOne(boolean useSampleData, int expectedResult) {
        var sut = new Day3(useSampleData);
        var result = sut.solvePartOne();
        assertEquals(expectedResult, result);
    }

    @ParameterizedTest
    @MethodSource
    public void solvePartTwo(boolean useSampleData, int expectedResult) {
        var sut = new Day3(useSampleData);
        var result = sut.solvePartTwo();
        assertEquals(expectedResult, result);
    }
}
