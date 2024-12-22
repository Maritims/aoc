package io.github.maritims.advent_of_code.util;

public class Line2D {
    private final        Point2D p1;
    private final        Point2D p2;

    public Line2D(Point2D p1, Point2D p2) {
        this.p1 = p1;
        this.p2 = p2;
    }

    public Point2D getP1() {
        return p1;
    }

    public Point2D getP2() {
        return p2;
    }

    public boolean contains(Point2D p) {
        return p.getX() <= Math.max(p1.getX(), p2.getX()) &&
                p.getX() >= Math.min(p1.getX(), p2.getX()) &&
                p.getY() <= Math.max(p1.getY(), p2.getY()) &&
                p.getY() >= Math.min(p1.getY(), p2.getY());
    }

    public Point2D getMidpoint() {
        var x = (p1.getX() + p2.getX()) / 2;
        var y = (p1.getY() + p2.getY()) / 2;
        return new Point2D(x, y);
    }

    public int getSlope() {
        return (p2.getY() - p1.getY()) / (p2.getX() - p1.getX());
    }

    public boolean intersects(Line2D that) {
        var mp1 = getMidpoint();
        var mp2 = that.getMidpoint();
        if(!mp1.equals(mp2)) {
            return false;
        }

        var s1 = getSlope();
        var s2 = that.getSlope();
        return ((s1 == 1 && s2 == -1) || (s1 == -1 && s2 == 1));
    }

    @Override
    public String toString() {
        return String.format("(%s, %s)", p1, p2);
    }
}
