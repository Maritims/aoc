package io.github.maritims.advent_of_code.util;

public class Point {
    private final int x;
    private final int y;

    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public int x() {
        return x;
    }

    public int y() {
        return y;
    }

    public Vector subtract(Point that) {
        return new Vector(x - that.x, y - that.y);
    }

    public Point subtract(Vector vector) {
        return new Point(x - vector.x(), y - vector.y());
    }

    public Point add(Vector vector) {
        return new Point(x + vector.x(), y + vector.y());
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;

        Point point = (Point) o;
        return x == point.x && y == point.y;
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

    public static Point at(int x, int y) {
        return new Point(x, y);
    }
}
