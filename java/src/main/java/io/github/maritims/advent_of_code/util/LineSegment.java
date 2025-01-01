package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;

/**
 * In geometry, a line segment is a part of a straight line that is bounded by two distinct end points, and contains every point on the line that is between its endpoints.
 */
public class LineSegment {
    @NotNull
    private final Point p1;
    @NotNull
    private final Point p2;

    public LineSegment(@NotNull Point p1, @NotNull Point p2) {
        this.p1 = p1;
        this.p2 = p2;
    }

    @NotNull
    public Point p1() {
        return p1;
    }

    @NotNull
    public Point p2() {
        return p2;
    }

    public boolean contains(@NotNull Point p) {
        return p.y() <= Math.max(p1.y(), p2.y()) &&
                p.y() >= Math.min(p1.y(), p2.y()) &&
                p.x() <= Math.max(p1.x(), p2.x()) &&
                p.x() >= Math.min(p1.x(), p2.x());
    }

    public Point midpoint() {
        var row = (p1.y() + p2.y()) / 2;
        var col = (p1.x() + p2.x()) / 2;
        return new Point(col, row);
    }

    /**
     * <p>The steepness, incline, or grade of a line is the absolute value of its slope: greater absolute value indicates a steeper line. The line trend is defined as follows:</p>
     * <ul>
     *    <li>An "increasing" or "ascending" line goes up from left to right and has positive slope: m > 0</li>
     *    <li>A "decreasing" or "descending" line goes down from left to right and has negative slope: m < 1</li>
     *    <li>A "square" (diagonal) line has unit slope: m = 1</li>
     *    <li>A horizontal line has zero slope: m = 0</li>
     *    <li>A vertical line has undefined or infinite slope.</li>
     * </ul>
     */
    public double slope() {
        if (p2.x() == p1.x()) {
            return Double.POSITIVE_INFINITY;
        }

        var y = p2.y() - p1.y();
        var x = p2.x() - p1.x();
        return (double) y / x;
    }

    public Orientation orientation() {
        var slope = slope();

        if (Double.isInfinite(slope)) {
            return Orientation.Vertical;
        }

        if (slope == 0) {
            return Orientation.Horizontal;
        }

        if (slope > 0 || slope < 0) {
            return Orientation.Diagonal;
        }

        throw new IllegalStateException(String.format("Unable to determine orientation from slope %f", slope));
    }

    public boolean formsCrossWith(@NotNull LineSegment that) {
        if (!midpoint().equals(that.midpoint())) {
            return false;
        }

        var s1 = slope();
        var s2 = that.slope();
        return (s1 == 1 && s2 == -1) || (s1 == -1 && s2 == 1);
    }

    @Override
    public String toString() {
        return String.format("%s -> %s", p1, p2);
    }
}
