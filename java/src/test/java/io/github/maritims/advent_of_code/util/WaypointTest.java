package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.api.Test;

class WaypointTest {
    @Test
    void testEqualsAndHashCode() {
        Point     p1 = new Point(4, 6);
        Point     p2 = new Point(4, 6);
        Direction d1 = Direction.North; // Example enum value
        Direction d2 = Direction.North; // Same enum value

        Waypoint w1 = new Waypoint(p1, d1);
        Waypoint w2 = new Waypoint(p2, d2);

        System.out.println("w1.equals(w2): " + w1.equals(w2)); // Should print true
        System.out.println("w1.hashCode(): " + w1.hashCode());
        System.out.println("w2.hashCode(): " + w2.hashCode());

        assert w1.equals(w2) : "Waypoints should be equal";
        assert w1.hashCode() == w2.hashCode() : "Hash codes should match";
    }
}