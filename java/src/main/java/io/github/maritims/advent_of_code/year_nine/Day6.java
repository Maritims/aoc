package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;

import java.util.LinkedList;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

public class Day6 extends Day {
    public Day6(Boolean useSampleData) {
        super(2024, 6, useSampleData);
    }

    private final LinkedList<GuardPosition> guardPositions = new LinkedList<>();

    @Override
    public Integer solvePartOne() {
        var grid          = Grid.fromString(getInputText());
        var startingPoint = grid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));

        grid.findExit(startingPoint, (point, direction) -> guardPositions.add(new GuardPosition(point, direction)), null);

        return (int) guardPositions.stream()
                .map(GuardPosition::getPoint)
                .distinct()
                .count();
    }

    @Override
    public Integer solvePartTwo() {
        var grid = Grid.fromString(getInputText());

        // Start going through a grid.
        // Place an obstacle right before the exit uncovered in part 1.
        // Did it cause a loop? Great, record that.
        // Clone the grid and try again.
        // Do this for the number of positions recorded in part 1, minus the starting position.

        var points = guardPositions.stream()
                .map(GuardPosition::getPoint)
                .distinct()
                .collect(Collectors.toList());
        var loops = new AtomicInteger(0);

        for (var i = points.size() - 1; i > 0; i--) {
            var currentGrid = grid.clone().withObstacleAt(points.get(i), '#');
            var currentStartingPoint = currentGrid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));
            currentGrid.findExit(currentStartingPoint, null, (point, direction) -> loops.set(loops.get() + 1));
        }

        return loops.get();
    }

    static class GuardPosition {
        private final Point2D point;
        private final int     directionIndex;

        public GuardPosition(Point2D point, int directionIndex) {
            this.point = point;
            this.directionIndex = directionIndex;
        }

        public Point2D getPoint() {
            return point;
        }

        @Override
        public boolean equals(Object o) {
            if (o == null || getClass() != o.getClass()) return false;
            GuardPosition that = (GuardPosition) o;
            return directionIndex == that.directionIndex && Objects.equals(point, that.point);
        }

        @Override
        public int hashCode() {
            return Objects.hash(point, directionIndex);
        }

        @Override
        public String toString() {
            return "GuardPosition{" +
                    "point=" + point +
                    ", directionIndex=" + directionIndex +
                    '}';
        }
    }
}
