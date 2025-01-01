package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class PointTest {

    public static Stream<Arguments> testEquals() {
        return Stream.of(
                Arguments.arguments(Point.at(4, 6), Point.at(4, 6), true)
        );
    }

    @ParameterizedTest
    @MethodSource
    void testEquals(Point p1, Point p2, boolean expectedResult) {
        assertEquals(expectedResult, p1.equals(p2));
        assertEquals(expectedResult, p2.equals(p1));
    }
}