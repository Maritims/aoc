package io.github.maritims.advent_of_code.util;

import java.util.Set;
import java.util.stream.IntStream;

public enum Direction {
    North(0, -1),
    East(1, 0),
    South(0, 1),
    West(-1, 0),
    NorthWest(-1, -1),
    NorthEast(1, -1),
    SouthWest(-1, 1),
    SouthEast(1, 1);

    Direction(int x, int y) {
        this.x = x;
        this.y = y;
    }

    private final int x;
    private final int y;

    public int x() {
        return x;
    }

    public int y() {
        return y;
    }

    public Orientation orientation() {
        if(y == 0 && x != 0) {
            return Orientation.Horizontal;
        }

        if(x == 0 && y != 0) {
            return Orientation.Vertical;
        }

        if(Math.abs(x) == Math.abs(y)) {
            return Orientation.Diagonal;
        }

        throw new IllegalStateException(String.format("Unable to determine orientation for direction %s (%d, %d)", this, x, y));
    }

    private static final Direction[] allDirections;

    static {
        var values = values();
        allDirections = new Direction[values.length];
        IntStream.range(0, values.length).forEach(i -> allDirections[i] = values[i]);
    }

    public static Direction get(int ordinal) {
        return allDirections[ordinal];
    }

    public static Set<Direction> getAll() {
        return Set.of(allDirections);
    }
}