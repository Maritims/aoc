package io.github.maritims.advent_of_code.util;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class GridTest {
    private final Grid<Integer> grid = new Grid<>(
            List.of(
                    new ArrayList<>(List.of(1, 2, 3, 4)),
                    new ArrayList<>(List.of(5, 6, 7, 8)),
                    new ArrayList<>(List.of(9, 10, 11, 12))
            )
    );

    public static Stream<Arguments> findExit_should_loop() {
        var obstacles = Set.of(
                Point2D.at(4, 0),
                Point2D.at(9, 1),
                Point2D.at(2, 3),
                Point2D.at(7, 4),
                Point2D.at(1, 6),
                Point2D.at(8, 7),
                Point2D.at(0, 8),
                Point2D.at(6, 9)
        );
        return Stream.of(
                Arguments.arguments(10, 10, obstacles, Set.of(Point2D.at(3, 6)), Point2D.at(4, 6), Direction.West),
                Arguments.arguments(10, 10, obstacles, Set.of(Point2D.at(6, 7)), Point2D.at(6, 6), Direction.South),
                Arguments.arguments(10, 10, obstacles, Set.of(Point2D.at(3, 8)), Point2D.at(4, 8), Direction.West),
                Arguments.arguments(10, 10, obstacles, Set.of(Point2D.at(1, 8)), Point2D.at(2, 8), Direction.West),
                Arguments.arguments(10, 10, obstacles, Set.of(Point2D.at(7, 7)), Point2D.at(6, 7), Direction.East),
                Arguments.arguments(10, 10, obstacles, Set.of(Point2D.at(7, 9)), Point2D.at(7 ,8), Direction.South)
        );
    }

    public static Stream<Arguments> findExit_should_exit() {
        var obstacles = Set.of(
                Point2D.at(4, 0),
                Point2D.at(9, 1),
                Point2D.at(2, 3),
                Point2D.at(7, 4),
                Point2D.at(1, 6),
                Point2D.at(8, 7),
                Point2D.at(0, 8),
                Point2D.at(6, 9)
        );
        return Stream.of(
                Arguments.arguments(10, 10, obstacles, Point2D.at(4, 6))
        );
    }

    @Test
    void rows() {
        assertEquals(3, grid.rows());
    }

    @Test
    void cols() {
        assertEquals(4, grid.cols());
    }

    @Test
    void isOutOfBounds() {
        assertTrue(grid.isOutOfBounds(new Point2D(4, 0)));
        assertFalse(grid.isOutOfBounds(new Point2D(3, 0)));
    }

    @Test
    void cellAt() {
        assertNull(grid.cellAt(new Point2D(4, 0)));
        assertNotNull(grid.cellAt(new Point2D(3, 0)));
    }

    @ParameterizedTest(name = "findExit should loop when starting from {4} in direction {5} in a grid with {0} rows, {1} cols and obstacles at {2} and {3}")
    @MethodSource
    void findExit_should_loop(int rows, int cols, Set<Point2D> originalObstacles, Set<Point2D> additionalObstacles, Point2D startingPoint, Direction direction) {
        // arrange
        var grid = new Grid<>(rows, cols, '.').withElementAt(startingPoint, '^');
        originalObstacles.forEach(point -> grid.withElementAt(point, '#'));
        additionalObstacles.forEach(point -> grid.withElementAt(point, 'O'));

        // act
        var traversalOutcome = grid.findExit(startingPoint, direction);

        // assert
        assertEquals(TraversalOutcome.PathState.Loop, traversalOutcome.pathState(), () -> traversalOutcome.waypoints().toString());
    }

    @ParameterizedTest
    @MethodSource
    void findExit_should_exit(int rows, int cols, Set<Point2D> obstacles, Point2D startingPoint) {
        // arrange
        var grid = new Grid<>(rows, cols, '.').withElementAt(startingPoint, '^');
        obstacles.forEach(point -> grid.withElementAt(point, '#'));

        // act
        var traversalOutcome = grid.findExit(startingPoint);

        // assert
        assertEquals(TraversalOutcome.PathState.Exit, traversalOutcome.pathState(), () -> traversalOutcome.waypoints().toString());
        System.out.println(traversalOutcome);
    }
}