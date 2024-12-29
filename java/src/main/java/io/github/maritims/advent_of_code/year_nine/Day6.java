package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;

import java.util.ArrayList;
import java.util.LinkedHashSet;

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
        var distinctObstaclePoints   = new LinkedHashSet<Point2D>();

        for (var i = originalWaypoints.size() - 1; i > 0; i--) {
            var currentWaypoint  = originalWaypoints.get(i);
            var previousWaypoint = originalWaypoints.get(i - 1);
            var currentGrid      = grid.clone().withElementAt(currentWaypoint.point(), 'O');
            var exit             = currentGrid.findExit(previousWaypoint.point(), previousWaypoint.direction());
            if (exit.pathState() == TraversalOutcome.PathState.Loop) {
                distinctObstaclePoints.add(originalWaypoints.get(i).point());
            }
        }

        return distinctObstaclePoints.size();
    }

}
