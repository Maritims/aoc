package io.github.maritims.advent_of_code.util;

public class Point2D {
    private final int x;
    private final int y;

    public Point2D(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public int x() {
        return x;
    }

    public int y() {
        return y;
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;

        Point2D point2D = (Point2D) o;
        return x == point2D.x && y == point2D.y;
    }

    @Override
    public int hashCode() {
        int result = x;
        result = 31 * result + y;
        return result;
    }

    @Override
    public String toString() {
        return String.format("(%d, %d)", x, y);
    }

    public static Point2D at(int x, int y) {
        return new Point2D(x, y);
    }

    public static int getManhattanDistance(Point2D p1, Point2D p2) {
        return Math.abs(p1.x() - p2.x()) + Math.abs(p1.y() - p2.y());
}
}
