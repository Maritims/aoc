package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.params.provider.Arguments.*;

class LineSegmentTest {

    public static Stream<Arguments> contains() {
        return Stream.of(
                arguments(new LineSegment(new Point(0, 0), new Point(0, 5)), new Point(0, 1), true),
                arguments(new LineSegment(new Point(0, 0), new Point(0, 5)), new Point(0, 6), false)
        );
    }

    public static Stream<Arguments> midpoint() {
        return Stream.of(
                arguments(new LineSegment(new Point(1, 0), new Point(3, 2)), new Point(2, 1))
        );
    }

    @ParameterizedTest
    @MethodSource
    void contains(LineSegment lineSegment, Point p, boolean expectedResult) {
        assertEquals(expectedResult, lineSegment.contains(p));
    }

    @ParameterizedTest
    @MethodSource
    void midpoint(LineSegment lineSegment, Point expectedResult) {
        var midpoint = lineSegment.midpoint();
        assertEquals(expectedResult, midpoint);
    }
}