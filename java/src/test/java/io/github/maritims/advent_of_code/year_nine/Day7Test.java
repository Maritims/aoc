package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.MathUtil;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.List;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class Day7Test {
    public static Stream<Arguments> equationIsTrue() {
        return Stream.of(
                Arguments.arguments(new Day7.Equation(156, List.of(15L, 6L)), true)
        );
    }

    @ParameterizedTest
    @MethodSource
    void equationIsTrue(Day7.Equation equation, boolean expectedResult) {
        assertEquals(expectedResult, equation.isTrue(List.of(Math::addExact, Math::multiplyExact, MathUtil::concatenate)));
    }
}
