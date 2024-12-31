package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;

import java.util.stream.Collectors;

public class Day6 extends Day {
    public Day6(Boolean useSampleData) {
        super(2024, 6, useSampleData);
    }

    public Day6(String filename) {
        super(2024, 6, filename);
    }

    private Grid             grid;
    private Waypoint         startingPoint;
    private TraversalOutcome originalTraversalOutcome;

    @Override
    protected void initialize() {
        super.initialize();
        grid = Grid.fromString(getInputText());
        startingPoint = grid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));
        originalTraversalOutcome = grid.findExit(startingPoint);
    }

    @Override
    public Long solvePartOne() {
        return originalTraversalOutcome.waypoints()
                .parallelStream()
                .map(Waypoint::point)
                .distinct()
                .count();
    }

    @Override
    public Long solvePartTwo() {
        var visitedPoints = originalTraversalOutcome.waypoints()
                .parallelStream()
                .map(Waypoint::point)
                .distinct()
                .collect(Collectors.toList());

        return visitedPoints.parallelStream()
                .map(visitedPoint -> grid.clone()
                        .withElementAt(visitedPoint, '#')
                        .findExit(startingPoint)
                        .pathState()
                )
                .filter(pathState -> pathState == TraversalOutcome.PathState.Loop)
                .count();
    }

}
