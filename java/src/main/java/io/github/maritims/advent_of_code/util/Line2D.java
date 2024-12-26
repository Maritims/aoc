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
        return p.getRow() <= Math.max(p1.getRow(), p2.getRow()) &&
                p.getRow() >= Math.min(p1.getRow(), p2.getRow()) &&
                p.getCol() <= Math.max(p1.getCol(), p2.getCol()) &&
                p.getCol() >= Math.min(p1.getCol(), p2.getCol());
    }

    public Point2D getMidpoint() {
        var row = (p1.getRow() + p2.getRow()) / 2;
        var col = (p1.getCol() + p2.getCol()) / 2;
        return new Point2D(row, col);
    }

    public int getSlope() {
        return (p2.getCol() - p1.getCol()) / (p2.getRow() - p1.getRow());
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
