package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.Optional;
import java.util.function.BiConsumer;

public class Grid<T> implements Cloneable {
    private final T[][] grid;

    public Grid(@NotNull T[][] grid) {
        this.grid = grid;
    }

    public int rows() {
        return grid.length;
    }

    public int cols() {
        return grid[0].length;
    }

    public boolean isOutOfBounds(Point2D point) {
        return point.y() < 0 || point.y() >= grid.length || point.x() < 0 || point.x() >= grid[0].length;
    }

    @Nullable
    public T cellAt(Point2D point) {
        if (isOutOfBounds(point)) {
            return null;
        }

        return grid[point.y()][point.x()];
    }

    @NotNull
    public Optional<Point2D> findFirst(@NotNull T t) {
        for(var row = 0; row < grid.length; row++) {
            for(var col = 0; col < grid[0].length; col++) {
                if(t.equals(grid[row][col])) {
                    return Optional.of(new Point2D(col, row));
                }
            }
        }

        return Optional.empty();
    }

    @NotNull
    public static Grid<Character> fromString(@NotNull String str) {
        var lines = str.split("\n");
        var grid  = new Character[lines.length][lines[0].length()];

        for (var row = 0; row < lines.length; row++) {
            for (var col = 0; col < lines[0].length(); col++) {
                grid[row][col] = lines[row].charAt(col);
            }
        }

        return new Grid<>(grid);
    }

    @NotNull
    public Grid<T> withObstacleAt(@NotNull Point2D point, @NotNull T obstacle) {
        if(isOutOfBounds(point)) {
            throw new IllegalArgumentException("point is out of bounds");
        }
        grid[point.y()][point.x()] = obstacle;
        return this;
    }

    @Nullable
    public Point2D findExit(@NotNull Point2D startingPoint, @Nullable BiConsumer<Point2D, Integer> onMove, @Nullable BiConsumer<Point2D, Integer> onAlreadyVisited) {
        var currentDirection = 0;
        var queue = new LinkedList<Point2D>();
        var visited = new LinkedHashSet<String>();

        queue.add(startingPoint);
        visited.add(String.format("%d,%d,%d", startingPoint.x(), startingPoint.y(), currentDirection));
        if(onMove != null) {
            onMove.accept(startingPoint.clone(), currentDirection);
        }

        while(!queue.isEmpty()) {
            var point    = queue.poll();
            var dy       = point.y() + Direction.verticalDirections[currentDirection];
            var dx       = point.x() + Direction.horizontalDirections[currentDirection];
            var newPoint = new Point2D(dx, dy);

            if (isOutOfBounds(newPoint)) {
                // We've found the exit.
                return newPoint;
            }

            if (Character.valueOf('#').equals(cellAt(newPoint))) {
                // Change direction by 90 degrees (N -> E -> S -> W).
                currentDirection = (currentDirection + 4 + 1) % 4;
                dy = point.y() + Direction.verticalDirections[currentDirection];
                dx = point.x() + Direction.horizontalDirections[currentDirection];
                newPoint = new Point2D(dx, dy);
            }

            if(onMove != null) {
                onMove.accept(newPoint, currentDirection);
            }

            var visit = String.format("%d,%d,%d", newPoint.x(), newPoint.y(), currentDirection);
            if(visited.contains(visit) && onAlreadyVisited != null) {
                onAlreadyVisited.accept(newPoint, currentDirection);
                // An obstacle at this point causes a loop.
                return newPoint;
            }
            visited.add(visit);
            queue.add(new Point2D(dx, dy));
        }

        // We're blocked!
        return null;
    }

    @Override
    public String toString() {
        var stringBuilder = new StringBuilder();

        for(var row = 0; row < grid.length; row++) {
            for(var col = 0; col < grid[0].length; col++) {
                stringBuilder.append(grid[row][col]);
            }
            if(row < grid.length - 1) {
                stringBuilder.append('\n');
            }
        }

        return stringBuilder.toString();
    }

    @SuppressWarnings("MethodDoesntCallSuperMethod")
    @Override
    public Grid<T> clone() {
        @SuppressWarnings("unchecked") var array = (T[][]) new Object[rows()][cols()];

        for(var row = 0; row < rows(); row++) {
            if (cols() >= 0) {
                System.arraycopy(grid[row], 0, array[row], 0, cols());
            }
        }

        return new Grid<>(array);
    }
}
