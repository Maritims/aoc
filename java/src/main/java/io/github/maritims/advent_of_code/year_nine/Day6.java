package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;

import java.util.ArrayList;
import java.util.HashSet;

public class Day6 extends Day {
    public Day6(Boolean useSampleData) {
        super(2024, 6, useSampleData);
    }

    @Override
    public Integer solvePartOne() {
        var grid             = Grid.fromString(getInputText());
        var startingPoint    = grid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));
        var traversalOutcome = grid.findExit(startingPoint);

        return traversalOutcome.distinctPositions().size();
    }

    @Override
    public Integer solvePartTwo() {
        var grid                     = Grid.fromString(getInputText());
        var startingPoint            = grid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));
        var originalTraversalOutcome = grid.findExit(startingPoint);
        var originalWaypoints        = new ArrayList<>(originalTraversalOutcome.waypoints());
        var distinctObstaclePoints   = new HashSet<Point2D>();

        for (var i = 0; i < originalWaypoints.size(); i++) {
            var currentWaypoint = originalWaypoints.get(i);
            var clonedGrid      = grid.clone();
            if (i < originalWaypoints.size() - 1) {
                clonedGrid = clonedGrid.withElementAt(originalWaypoints.get(i + 1).point(), 'O');
            }

            var exit = clonedGrid.findExit(currentWaypoint.point(), currentWaypoint.direction());
            if (exit.pathState() == TraversalOutcome.PathState.Loop) {
                distinctObstaclePoints.add(currentWaypoint.point());
            }
        }

        return distinctObstaclePoints.size();
    }

}
