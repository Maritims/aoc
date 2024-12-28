package io.github.maritims.advent_of_code.util;

public class Occurrence<T> {
    private final T   value;
    private final int startRow;
    private final int startCol;
    private final int endRow;
    private final int endCol;

    public Occurrence(T value, int startRow, int startCol, int endRow, int endCol) {
        this.value = value;
        this.startRow = startRow;
        this.startCol = startCol;
        this.endRow = endRow;
        this.endCol = endCol;
    }

    public LineSegment toLineSegment() {
        return new LineSegment(new Point2D(startCol, startRow), new Point2D(endCol, endRow));
    }

    @Override
    public String toString() {
        return "Occurrence{" +
                "value=" + value +
                ", startRow=" + startRow +
                ", startCol=" + startCol +
                ", endRow=" + endRow +
                ", endCol=" + endCol +
                '}';
    }
}
