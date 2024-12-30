package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;

import java.util.LinkedHashSet;
import java.util.List;
import java.util.Objects;

public class Waypoint {
    @NotNull
    private final Point2D   point;
    @NotNull
    private final Direction direction;

    public Waypoint(@NotNull Point2D point, @NotNull Direction direction) {
        this.point = point;
        this.direction = direction;
    }

    @NotNull
    public Point2D point() {
        return point;
    }

    @NotNull
    public Direction direction() {
        return direction;
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        Waypoint waypoint = (Waypoint) o;
        return Objects.equals(point, waypoint.point) && direction == waypoint.direction;
    }

    @Override
    public int hashCode() {
        return Objects.hash(point, direction);
    }

    @Override
    public String toString() {
        return String.format("%d,%d,%d", point.x(), point.y(), direction.ordinal());
    }

    public static Waypoint to(int x, int y, Direction direction) {
        return new Waypoint(Point2D.at(x, y), direction);
    }
}
