package io.github.maritims.advent_of_code.util;

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

    public static final int[]       ROW_DIRECTIONS = {-1, -1, -1, 0, 0, 1, 1, 1};
    public static final int[]       COL_DIRECTIONS = {-1, 0, 1, -1, 1, -1, 0, 1};
    public static final int[]       horizontalDirections;
    public static final int[]       verticalDirections;
    public static final Direction[] allDirections;

    static {
        var values = values();
        allDirections = new Direction[values.length];
        horizontalDirections = new int[values.length];
        verticalDirections = new int[values.length];

        for (var i = 0; i < values.length; i++) {
            allDirections[i] = values[i];
            horizontalDirections[i] = values[i].x();
            verticalDirections[i] = values[i].y();
        }
    }
}