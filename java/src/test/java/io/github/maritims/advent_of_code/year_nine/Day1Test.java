package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.year_nine.day_one.PartTwo;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;

class Day1Test {

    public static Stream<Arguments> solvePartOne() {
        return Stream.of(
                Arguments.arguments(true, 11),
                Arguments.arguments(false, 2580760)
        );
    }

    @ParameterizedTest
    @MethodSource
    void solvePartOne(boolean useSampleData, Integer expectedResult) {
        var sut = new Day1(useSampleData);
        var result = sut.solvePartOne();
        assertEquals(expectedResult, result);
    }

    public static Stream<Arguments> solvePartTwo() {
        return Stream.of(
                Arguments.arguments(true, 31),
                Arguments.arguments(false, 25358365)
        );
    }

    @ParameterizedTest
    @MethodSource
    void solvePartTwo(boolean useSampleData, int expectedResult) {
        var sut = new Day1(useSampleData);
        var result = sut.solvePartTwo();
        assertEquals(expectedResult, result);
    }
}