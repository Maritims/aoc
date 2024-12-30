package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class GridTest {
    private final Grid grid = new Grid(new char[][]{
            {'1', '2', '3', '4'},
            {'5', '6', '7', '8'},
            {'9', 'a', 'b', 'c'}
    });

    @Test
    void rows() {
        assertEquals(3, grid.rows());
    }

    @Test
    void cols() {
        assertEquals(4, grid.cols());
    }

    @Test
    void isOutOfBounds() {
        assertTrue(grid.isOutOfBounds(4, 0));
        assertFalse(grid.isOutOfBounds(3, 0));
    }

    @Test
    void getValueAt() {
        assertThrows(IllegalArgumentException.class, () -> grid.getValueAt(4, 0));
        assertDoesNotThrow(() -> grid.getValueAt(3, 0));
    }
}