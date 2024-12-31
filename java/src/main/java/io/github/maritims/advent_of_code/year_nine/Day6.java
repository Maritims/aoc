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
        var traversalOutcome = grid.findExit(startingPoint.point(), startingPoint.direction());

        return (int) traversalOutcome.waypoints().stream().map(Waypoint::point).distinct().count();
    }

    protected Integer solvePartTwoForFile(String filename) {
        var grid          = Grid.fromString(getInputText(filename));
        var startingPoint = grid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));
        var loopCounter   = 0;
        var visitedPoints = grid.findExit(startingPoint.point(), startingPoint.direction())
                .waypoints()
                .stream()
                .map(Waypoint::point)
                .distinct()
                .collect(Collectors.toList());

        for (var visitedPoint : visitedPoints) {
            var clonedGrid       = grid.clone().withElementAt(visitedPoint, '#');
            var traversalOutcome = clonedGrid.findExit(startingPoint.point(), startingPoint.direction());
            if(traversalOutcome.pathState() == TraversalOutcome.PathState.Loop) {
                loopCounter++;
            }
        }

        return loopCounter;
    }

    @Override
    public Integer solvePartTwo() {
        return solvePartTwoForFile(getResourceName());
    }

}
