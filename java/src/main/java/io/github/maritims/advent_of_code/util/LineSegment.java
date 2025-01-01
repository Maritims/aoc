package io.github.maritims.advent_of_code.util;

/**
 * In geometry, a line segment is a part of a straight line that is bounded by two distinct end points, and contains every point on the line that is between its endpoints.
 */
public class LineSegment {
    private final Point2D p1;
    private final Point2D p2;

    public LineSegment(Point2D p1, Point2D p2) {
        this.p1 = p1;
        this.p2 = p2;
    }

    public Point2D p1() {
        return p1;
    }

    public Point2D p2() {
        return p2;
    }

    private double length = Double.MIN_VALUE;

    public double length() {
        if (length == Double.MIN_VALUE) {
            var dx = p2.x() - p1.x();
            var dy = p2.y() - p1.y();
            length = Math.sqrt(dx * dx + dy * dy);
        }
        return length;
    }

    public LineSegment extend(double factor) {
        // Direction vector
        var dx = p2.x() - p1.x();
        var dy = p2.y() - p1.y();

        var length = length();

        // Unit direction vector
        var ux = dx / length;
        var uy = dy / length;

        var newX1 = p1.x() - factor * ux;
        var newY1 = p1.y() - factor * uy;
        var newP1 = Point2D.at((int) newX1, (int) newY1);

        var newX2 = p2.x() + factor * ux;
        var newY2 = p2.y() + factor * uy;
        var newP2 = Point2D.at((int) newX2, (int) newY2);

        return new LineSegment(newP1, newP2);
    }

    public boolean contains(Point2D p) {
        return p.y() <= Math.max(p1.y(), p2.y()) &&
                p.y() >= Math.min(p1.y(), p2.y()) &&
                p.x() <= Math.max(p1.x(), p2.x()) &&
                p.x() >= Math.min(p1.x(), p2.x());
    }

    public Point2D midpoint() {
        var row = (p1.y() + p2.y()) / 2;
        var col = (p1.x() + p2.x()) / 2;
        return new Point2D(col, row);
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

    public boolean formsCrossWith(LineSegment that) {
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
