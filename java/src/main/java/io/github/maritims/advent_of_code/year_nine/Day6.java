package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;

import java.util.stream.Collectors;

public class Day6 extends Day {
    public Day6(Boolean useSampleData) {
        super(2024, 6, useSampleData);
    }

    @Override
    public Integer solvePartOne() {
        var grid             = Grid.fromString(getInputText());
        var startingPoint    = grid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));

        return (int) grid.findExit(startingPoint)
                .waypoints()
                .parallelStream()
                .map(Waypoint::point)
                .distinct()
                .count();
    }

    protected Integer solvePartTwoForFile(String filename) {
        var grid          = Grid.fromString(getInputText(filename));
        var startingPoint = grid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));
        var visitedPoints = grid.findExit(startingPoint)
                .waypoints()
                .parallelStream()
                .map(Waypoint::point)
                .distinct()
                .collect(Collectors.toList());

        return (int) visitedPoints.parallelStream()
                .map(visitedPoint -> grid.clone()
                        .withElementAt(visitedPoint, '#')
                        .findExit(startingPoint)
                        .pathState()
                )
                .filter(pathState -> pathState == TraversalOutcome.PathState.Loop)
                .count();
    }

    @Override
    public Integer solvePartTwo() {
        return solvePartTwoForFile(getResourceName());
    }

}
