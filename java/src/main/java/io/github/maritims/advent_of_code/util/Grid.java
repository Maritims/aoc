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

    public TraversalOutcome findExit(@NotNull Point2D startingPoint, Direction currentDirection) {
        var queue            = new ArrayDeque<Point2D>();
        var visitedWaypoints = new LinkedHashSet<Waypoint>();

        queue.add(startingPoint);
        visitedWaypoints.add(Waypoint.to(startingPoint.x(), startingPoint.y(), currentDirection));

        while (!queue.isEmpty()) {
            var currentPoint = queue.poll();
            var nextX        = currentPoint.x() + currentDirection.x();
            var nextY        = currentPoint.y() + currentDirection.y();

            // Is it on the map?
            if (isOutOfBounds(nextX, nextY)) {
                return new TraversalOutcome(visitedWaypoints, TraversalOutcome.PathState.Exit);
            }

            // Is there something in the way?
            // What if we're surrounded on all sides?
            var attemptedDirections = new LinkedHashSet<Direction>();
            while (!isOutOfBounds(nextX, nextY) && grid[nextY][nextX] == '#' || grid[nextY][nextX] == 'O') {
                currentDirection = Direction.get((currentDirection.ordinal() + 1) % 4);

                nextX = currentPoint.x() + currentDirection.x();
                nextY = currentPoint.y() + currentDirection.y();

                if (attemptedDirections.contains(currentDirection)) {
                    return new TraversalOutcome(visitedWaypoints, TraversalOutcome.PathState.Loop);
                }

                attemptedDirections.add(currentDirection);
            }

            // Have we been here before?
            if (visitedWaypoints.contains(Waypoint.to(nextX, nextY, currentDirection))) {
                return new TraversalOutcome(visitedWaypoints, TraversalOutcome.PathState.Loop);
            }

            visitedWaypoints.add(Waypoint.to(nextX, nextY, currentDirection));
            queue.add(Point2D.at(nextX, nextY));
        }

        throw new RuntimeException();
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
