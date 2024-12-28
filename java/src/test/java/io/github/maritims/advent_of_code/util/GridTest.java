package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class GridTest {

    @Test
    void rows() {
        var grid = new Grid<>(new Integer[][]{
                new Integer[]{1, 2, 3, 4},
                new Integer[]{5, 6, 7, 8},
                new Integer[]{9, 10, 11, 12}
        });
        assertEquals(3, grid.rows());
    }

    @Test
    void cols() {
        var grid = new Grid<>(new Integer[][]{
                new Integer[]{1, 2, 3, 4},
                new Integer[]{5, 6, 7, 8},
                new Integer[]{9, 10, 11, 12}
        });
        assertEquals(4, grid.cols());
    }

    @Test
    void isOutOfBounds() {
        var grid = new Grid<>(new Integer[][]{
                new Integer[]{1, 2, 3, 4},
                new Integer[]{5, 6, 7, 8},
                new Integer[]{9, 10, 11, 12}
        });
        assertTrue(grid.isOutOfBounds(new Point2D(4, 0)));
        assertFalse(grid.isOutOfBounds(new Point2D(3, 0)));
    }

    @Test
    void cellAt() {
        var grid = new Grid<>(new Integer[][]{
                new Integer[]{1, 2, 3, 4},
                new Integer[]{5, 6, 7, 8},
                new Integer[]{9, 10, 11, 12}
        });
        assertNull(grid.cellAt(new Point2D(4, 0)));
        assertNotNull(grid.cellAt(new Point2D(3, 0)));
    }
}