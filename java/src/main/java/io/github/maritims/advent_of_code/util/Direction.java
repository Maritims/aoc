package io.github.maritims.advent_of_code.util;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public enum Direction {
    NorthWest,
    North,
    NorthEast,
    West,
    East,
    SouthWest,
    South,
    SouthEast;

    public static final List<Direction> VALUES = Arrays.stream(values()).collect(Collectors.toList());

    public static final int[] ROW_DIRECTIONS = {-1, -1, -1, 0, 0, 1, 1, 1};
    public static final int[] COL_DIRECTIONS = {-1, 0, 1, -1, 1, -1, 0, 1};
}
