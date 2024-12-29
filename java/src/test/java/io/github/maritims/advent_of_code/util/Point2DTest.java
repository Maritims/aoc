package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class Point2DTest {

    public static Stream<Arguments> testEquals() {
        return Stream.of(
                Arguments.arguments(Point2D.at(4, 6), Point2D.at(4, 6), true)
        );
    }

    @ParameterizedTest
    @MethodSource
    void testEquals(Point2D p1, Point2D p2, boolean expectedResult) {
        assertEquals(expectedResult, p1.equals(p2));
        assertEquals(expectedResult, p2.equals(p1));
    }
}