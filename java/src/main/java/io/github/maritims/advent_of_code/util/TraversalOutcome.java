package io.github.maritims.advent_of_code.util;

import java.util.LinkedHashSet;
import java.util.Objects;
import java.util.stream.Collectors;

public class TraversalOutcome {
    public enum PathState {
        Exit,
        Loop
    }

    private final LinkedHashSet<Waypoint> waypoints;
    private final PathState               pathState;

    public TraversalOutcome(LinkedHashSet<Waypoint> waypoints, PathState pathState) {
        this.waypoints = waypoints;
        this.pathState = pathState;
    }

    public LinkedHashSet<Waypoint> waypoints() {
        return waypoints;
    }

    public PathState pathState() {
        return pathState;
    }

    public LinkedHashSet<Point2D> distinctPositions() {
        return waypoints.stream()
                .map(Waypoint::point)
                .collect(Collectors.toCollection(LinkedHashSet::new));
    }

    @Override
    public String toString() {
        return "TraversalOutcome{" +
                "waypoints=" + waypoints +
                ", pathState=" + pathState +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        TraversalOutcome that = (TraversalOutcome) o;
        return Objects.equals(waypoints, that.waypoints) && pathState == that.pathState;
    }

    @Override
    public int hashCode() {
        return Objects.hash(waypoints, pathState);
    }
}
