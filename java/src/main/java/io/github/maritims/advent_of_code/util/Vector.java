package io.github.maritims.advent_of_code.util;

public class Vector {
    private final int x;
    private final int y;

    public Vector(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public int x() {
        return x;
    }

    public int y() {
        return y;
    }

    private double magnitude = 0.0;

    public double magnitude() {
        if(magnitude == 0) {
            magnitude = Math.sqrt(x * x + y * y);
        }
        return magnitude;
    }
}
