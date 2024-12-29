package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Grid<T> implements Cloneable {
    private final        List<ArrayList<T>> grid;

    public Grid(@NotNull List<ArrayList<T>> grid) {
        this.grid = grid;
    }

    public Grid(int rows, int cols, T defaultElement) {
        this.grid = IntStream.range(0, rows)
                .mapToObj(row -> IntStream.range(0, cols)
                        .mapToObj(col -> defaultElement)
                        .collect(Collectors.toCollection(ArrayList::new))
                )
                .collect(Collectors.toCollection(ArrayList::new));
    }

    public int rows() {
        return grid.size();
    }

    public int cols() {
        return grid.get(0).size();
    }

    public boolean isOutOfBounds(@NotNull Point2D point) {
        return point.y() < 0 || point.y() >= grid.size() || point.x() < 0 || point.x() >= grid.get(0).size();
    }

    @Nullable
    public T cellAt(Point2D point) {
        if (isOutOfBounds(point)) {
            return null;
        }

        return grid.get(point.y()).get(point.x());
    }

    @NotNull
    public Optional<Point2D> findFirst(@NotNull T t) {
        for (var row = 0; row < grid.size(); row++) {
            for (var col = 0; col < grid.get(0).size(); col++) {
                if (t.equals(grid.get(row).get(col))) {
                    return Optional.of(new Point2D(col, row));
                }
            }
        }

        return Optional.empty();
    }

    @NotNull
    public static Grid<Character> fromString(@NotNull String str) {
        var lines = str.split("\n");
        var grid = Arrays.stream(lines)
                .map(line -> line.chars()
                        .mapToObj(c -> (char) c)
                        .collect(Collectors.toCollection(ArrayList::new))
                )
                .collect(Collectors.toList());

        return new Grid<>(grid);
    }

    @NotNull
    public Grid<T> withElementAt(@NotNull Point2D point, @NotNull T element) {
        if (!isOutOfBounds(point)) {
            grid.get(point.y()).set(point.x(), element);
        }

        return this;
    }

    @NotNull
    public TraversalOutcome findExit(@NotNull Point2D startingPoint, Direction currentDirection) {
        //var currentDirection = 0;
        var queue            = new LinkedList<Point2D>();
        var visitedWaypoints = new LinkedHashSet<Waypoint>();

        queue.add(startingPoint);
        var visitedWaypoint = new Waypoint(startingPoint, currentDirection);
        visitedWaypoints.add(visitedWaypoint);

        while (!queue.isEmpty()) {
            var point    = queue.poll();
            var dy       = point.y() + currentDirection.y();
            var dx       = point.x() + currentDirection.x();
            var newPoint = new Point2D(dx, dy);

            if (isOutOfBounds(newPoint)) {
                // We've found the exit.
                return new TraversalOutcome(visitedWaypoints, TraversalOutcome.PathState.Exit, new Waypoint(newPoint, currentDirection));
            }

            var cellValueAtNewPoint = cellAt(newPoint);
            if (Character.valueOf('#').equals(cellValueAtNewPoint) || Character.valueOf('O').equals(cellValueAtNewPoint)) {
                // Change direction by 90 degrees (N -> E -> S -> W).
                var directionIndex = (currentDirection.ordinal() + 4 + 1) % 4;
                currentDirection = Direction.get(directionIndex);
                dy = point.y() + currentDirection.y();
                dx = point.x() + currentDirection.x();
                newPoint = new Point2D(dx, dy);
            }

            visitedWaypoint = new Waypoint(newPoint, currentDirection);
            if (visitedWaypoints.contains(visitedWaypoint)) {
                // An obstacle at this point causes a loop.
                return new TraversalOutcome(visitedWaypoints, TraversalOutcome.PathState.Loop, visitedWaypoint);
            }
            visitedWaypoints.add(visitedWaypoint);
            queue.add(new Point2D(dx, dy));
        }

        // We're blocked!
        throw new IllegalStateException("We were unable to move anywhere!");
    }

    @NotNull
    public TraversalOutcome findExit(@NotNull Point2D startingPoint) {
        return findExit(startingPoint, Direction.North);
    }

    public List<ArrayList<T>> deepCloneInnerGrid() {
        return grid.stream()
                .map(ArrayList::new)
                .collect(Collectors.toCollection(ArrayList::new));
    }

    @SuppressWarnings("MethodDoesntCallSuperMethod")
    @Override
    public Grid<T> clone() {
        return new Grid<>(deepCloneInnerGrid());
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        Grid<?> grid1 = (Grid<?>) o;
        return Objects.equals(grid, grid1.grid);
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(grid);
    }
}
