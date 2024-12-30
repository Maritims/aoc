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
        var grid              = Grid.fromString(getInputText());
        var startingPoint     = grid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));
        var traversalOutcome  = grid.findExit(startingPoint.point(), startingPoint.direction());
        var distinctPositions = traversalOutcome.distinctPositions();

        return distinctPositions.size();
    }

    protected Integer solvePartTwoForFile(String filename) {
        var grid                     = Grid.fromString(getInputText(filename));
        var startingPoint            = grid.findFirst('^').orElseThrow(() -> new IllegalStateException("The grid does not contain a guard symbol"));
        var originalTraversalOutcome = grid.findExit(startingPoint.point(), startingPoint.direction());
        var originalWaypoints        = new ArrayList<>(originalTraversalOutcome.waypoints());
        var distinctObstaclePoints   = new LinkedHashSet<Point2D>();

        for (var i = 0; i < originalWaypoints.size() - 1; i++) {
            var currentWaypoint = originalWaypoints.get(i);
            var nextWaypoint    = originalWaypoints.get(i + 1);
            var currentGrid     = grid.clone();

            if (i > 0 && currentGrid.getValueAt(nextWaypoint.point().x(), nextWaypoint.point().y()) != '#') {
                currentGrid.withElementAt(nextWaypoint.point(), 'O');
            }

            var exit = currentGrid.findExit(currentWaypoint.point(), currentWaypoint.direction());
            if (exit.pathState() == TraversalOutcome.PathState.Loop) {
                System.out.println(currentGrid);
                distinctObstaclePoints.add(currentWaypoint.point());
            }
        }

        return distinctObstaclePoints.size();
    }

    @Override
    public Integer solvePartTwo() {
        return solvePartTwoForFile(getResourceName());
    }

}
