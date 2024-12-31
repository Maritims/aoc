package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;

import java.util.*;

public class Grid implements Cloneable {
    private final char[][] grid;

    public Grid(char[][] grid) {
        this.grid = grid;
    }

    public int rows() {
        return grid.length;
    }

    public int cols() {
        return grid[0].length;
    }

    public boolean isOutOfBounds(int x, int y) {
        return y < 0 || y >= rows() || x < 0 || x >= cols();
    }

    public char getValueAt(int x, int y) {
        if (isOutOfBounds(x, y)) {
            throw new IllegalArgumentException(String.format("Point (%d, %d) is not within the grid", x, y));
        }

        return grid[y][x];
    }

    @NotNull
    public Optional<Waypoint> findFirst(char value) {
        for (var row = 0; row < rows(); row++) {
            for (var col = 0; col < cols(); col++) {
                var targetValue = getValueAt(col, row);
                if (value == targetValue) {
                    Direction direction;
                    switch (value) {
                        case '>':
                            direction = Direction.East;
                            break;
                        case '<':
                            direction = Direction.West;
                            break;
                        case 'v':
                            direction = Direction.South;
                            break;
                        default:
                            direction = Direction.North;
                            break;
                    }
                    return Optional.of(new Waypoint(Point2D.at(col, row), direction));
                }
            }
        }

        return Optional.empty();
    }

    @NotNull
    public static Grid fromString(@NotNull String str) {
        var lines = str.split("\n");
        var array = new char[lines.length][lines[0].length()];

        for (var row = 0; row < lines.length; row++) {
            for (var col = 0; col < lines[0].length(); col++) {
                array[row][col] = lines[row].charAt(col);
            }
        }

        return new Grid(array);
    }

    @NotNull
    public Grid withElementAt(@NotNull Point2D point, char value) {
        if (!isOutOfBounds(point.x(), point.y())) {
            grid[point.y()][point.x()] = value;
        }

        return this;
    }

    public TraversalOutcome findExit(@NotNull Point2D point, @NotNull Direction direction) {
        var isLooping        = false;
        var visitedWaypoints = new LinkedHashSet<Waypoint>();
        visitedWaypoints.add(Waypoint.to(point.x(), point.y(), direction));

        while (true) {
            var nextX = point.x() + direction.x();
            var nextY = point.y() + direction.y();

            if (isOutOfBounds(nextX, nextY)) {
                // We've found an exit!
                break;
            }

            // Step forward if there's nothing in the way.
            if (grid[nextY][nextX] != '#') {
                point = Point2D.at(nextX, nextY);
                var visitedWaypoint = Waypoint.to(point.x(), point.y(), direction);
                if (visitedWaypoints.contains(visitedWaypoint)) {
                    // We're in a loop!
                    isLooping = true;
                    break;
                }
                visitedWaypoints.add(Waypoint.to(point.x(), point.y(), direction));
            }
            // Turn 90 degrees to the right if there's something in the way.
            else {
                direction = Direction.get((direction.ordinal() + 1) % 4);
            }
        }

        return new TraversalOutcome(visitedWaypoints, isLooping ? TraversalOutcome.PathState.Loop : TraversalOutcome.PathState.Exit);
    }

    public char[][] deepCloneArray() {
        var array = new char[rows()][cols()];

        for (var row = 0; row < rows(); row++) {
            if (cols() >= 0) {
                System.arraycopy(grid[row], 0, array[row], 0, cols());
            }
        }

        return array;
    }

    @SuppressWarnings("MethodDoesntCallSuperMethod")
    @Override
    public Grid clone() {
        return new Grid(deepCloneArray());
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        Grid grid1 = (Grid) o;
        return Objects.deepEquals(grid, grid1.grid);
    }

    @Override
    public int hashCode() {
        return Arrays.deepHashCode(grid);
    }

    @Override
    public String toString() {
        var stringBuilder = new StringBuilder();

        for (var row = 0; row < rows(); row++) {
            for (var col = 0; col < cols(); col++) {
                stringBuilder.append(getValueAt(col, row));
            }
            stringBuilder.append('\n');
        }

        return stringBuilder.toString();
    }
}
