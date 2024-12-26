package io.github.maritims.advent_of_code.util;

import java.util.Objects;

public class Point2D {
    private final int row;
    private final int col;

    public Point2D(int row, int col) {
        this.row = row;
        this.col = col;
    }

    public Integer getRow() {
        return row;
    }

    public Integer getCol() {
        return col;
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        var point2D = (Point2D) o;
        return Double.compare(row, point2D.row) == 0 && Double.compare(col, point2D.col) == 0;
    }

    @Override
    public int hashCode() {
        return Objects.hash(row, col);
    }

    @Override
    public String toString() {
        return String.format("(%d, %d)", row, col);
    }
}
