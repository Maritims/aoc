package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.api.Test;

class DirectionTest {
    @Test
    void testOrdinal() {
        Direction d1 = Direction.North;
        Direction d2 = Direction.get(0); // Should return North

        assert d1.equals(d2) : "Directions should be equal";
    }
}