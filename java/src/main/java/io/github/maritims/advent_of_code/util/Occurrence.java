package io.github.maritims.advent_of_code.util;

public class Occurrence<T> {
    private final T         value;
    private final int       startRow;
    private final int       startCol;
    private final int       endRow;
    private final int       endCol;
    private final Direction direction;

    public Occurrence(T value, int startRow, int startCol, int endRow, int endCol, Direction direction) {
        this.value = value;
        this.startRow = startRow;
        this.startCol = startCol;
        this.endRow = endRow;
        this.endCol = endCol;
        this.direction = direction;
    }

    public T getValue() {
        return value;
    }

    public int getStartRow() {
        return startRow;
    }

    public int getStartCol() {
        return startCol;
    }

    public int getEndRow() {
        return endRow;
    }

    public int getEndCol() {
        return endCol;
    }

    public Direction getDirection() {
        return direction;
    }

    public Line2D toLine2D() {
        return new Line2D(new Point2D(startCol, startRow), new Point2D(endCol, endRow));
    }

    @Override
    public String toString() {
        return "Occurrence{" +
                "value=" + value +
                ", startRow=" + startRow +
                ", startCol=" + startCol +
                ", endRow=" + endRow +
                ", endCol=" + endCol +
                ", direction=" + direction +
                '}';
    }
}
