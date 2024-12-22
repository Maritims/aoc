package io.github.maritims.advent_of_code.year_nine;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class Day4Test {
    public static Stream<Arguments> solvePartOne() {
        return Stream.of(
                Arguments.arguments(true, 18),
                Arguments.arguments(false, 2646)
        );
    }

    @ParameterizedTest
    @MethodSource
    public void solvePartOne(boolean useSampleData, int expectedResult) {
        var sut = new Day4(useSampleData);
        var result = sut.solvePartOne();
        assertEquals(expectedResult, result);
    }
}
