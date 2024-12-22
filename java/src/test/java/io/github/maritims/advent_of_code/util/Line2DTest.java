package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class Line2DTest {

    public static Stream<Arguments> contains() {
        return Stream.of(
                Arguments.arguments(new Line2D(new Point2D(0, 0), new Point2D(0, 5)), new Point2D(0, 1), true),
                Arguments.arguments(new Line2D(new Point2D(0, 0), new Point2D(0, 5)), new Point2D(0, 6), false)
        );
    }

    public static Stream<Arguments> getMidpoint() {
        return Stream.of(
                Arguments.arguments(new Line2D(new Point2D(1, 0), new Point2D(3, 2)), new Point2D(2, 1))
        );
    }

    @ParameterizedTest
    @MethodSource
    void contains(Line2D line, Point2D p, boolean expectedResult) {
        assertEquals(expectedResult, line.contains(p));
    }

    @ParameterizedTest
    @MethodSource
    void getMidpoint(Line2D line, Point2D expectedResult) {
        var midpoint = line.getMidpoint();
        assertEquals(expectedResult, midpoint);
    }
}