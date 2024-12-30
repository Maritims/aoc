package io.github.maritims.advent_of_code.year_nine;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.params.provider.Arguments.*;

class Day6Test {
    public static Stream<Arguments> solvePartOne() {
        return Stream.of(
                arguments(true, 41),
                arguments(false, 5534)
        );
    }

    public static Stream<Arguments> solvePartTwo() {
        return Stream.of(
                arguments(true, 6),
                arguments(false, 0)
        );
    }

    public static Stream<Arguments> solvePartTwoForFile() {
        return Stream.of(
                arguments("2024/6/sample2.txt", 0),
                arguments("2024/6/sample3.txt", 19),
                arguments("2024/6/sample4.txt", 2)
        );
    }

    @ParameterizedTest
    @MethodSource
    void solvePartOne(boolean useSampleData, int expectedResult) {
        assertEquals(expectedResult, new Day6(useSampleData).solvePartOne());
    }

    @ParameterizedTest
    @MethodSource
    void solvePartTwo(boolean useSampleData, int expectedResult) {
        assertEquals(expectedResult, new Day6(useSampleData).solvePartTwo());
    }

    @ParameterizedTest
    @MethodSource
    void solvePartTwoForFile(String filename, int expectedResult) {
        assertEquals(expectedResult, new Day6(true).solvePartTwoForFile(filename));
    }
}