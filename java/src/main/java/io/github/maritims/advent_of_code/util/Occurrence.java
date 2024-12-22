package io.github.maritims.advent_of_code.util;

public class Occurrence<T> {
    private final T         value;
    private final int       row;
    private final int       col;
    private final Direction direction;

    public Occurrence(T value, int row, int col, Direction direction) {
        this.value = value;
        this.row = row;
        this.col = col;
        this.direction = direction;
    }

    public T getValue() {
        return value;
    }

    public int getRow() {
        return row;
    }

    public int getCol() {
        return col;
    }

    public Direction getDirection() {
        return direction;
    }

    @Override
    public String toString() {
        return "'" + value + "' found at (" + row + ", " + col + ") heading " + getDirection();
    }
}
