package io.github.maritims.advent_of_code.year_nine.day_one;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class PartTwoTest {

    public static Stream<Arguments> solve() {
        return Stream.of(
                Arguments.arguments(true, 31),
                Arguments.arguments(false, 25358365)
        );
    }

    @ParameterizedTest
    @MethodSource
    void solve(boolean useSampleData, int expectedResult) {
        var sut = new PartTwo(useSampleData);
        var result = sut.solve();
        assertEquals(expectedResult, result);
    }
}