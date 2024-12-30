package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;

class DirectionTest {
    public static Stream<Arguments> orientation() {
        return Direction.getAll()
                .stream()
                .map(direction -> Arguments.arguments(direction, direction.orientation()));
    }

    @Test
    void testOrdinal() {
        Direction d1 = Direction.North;
        Direction d2 = Direction.get(0); // Should return North

        assert d1.equals(d2) : "Directions should be equal";
    }

    @ParameterizedTest
    @MethodSource
    void orientation(Direction direction, Orientation expectedResult) {
        assertEquals(expectedResult, direction.orientation());
    }
}