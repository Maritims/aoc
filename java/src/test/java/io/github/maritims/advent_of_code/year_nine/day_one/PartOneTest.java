package io.github.maritims.advent_of_code.year_nine.day_one;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;

class PartOneTest {

    public static Stream<Arguments> solve() {
        return Stream.of(
                Arguments.arguments(true, 11),
                Arguments.arguments(false, 2580760)
        );
    }

    @ParameterizedTest
    @MethodSource
    void solve(boolean useSampleData, Integer expectedResult) {
        var sut = new PartOne(useSampleData);
        var result = sut.solve();
        assertEquals(expectedResult, result);
    }
}