package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static io.github.maritims.advent_of_code.util.Orientation.Diagonal;

public class Day4 extends Day {
    public Day4(Boolean useSampleData) {
        super(2024, 4, useSampleData);
    }

    private final Grid grid = Grid.fromString(getInputText());

    @NotNull
    private List<LineSegment> findLineSegments(@NotNull Grid grid, int row, int col, @NotNull String word) {
        var lineSegments = new ArrayList<LineSegment>();
        var directions   = Direction.getAll();

        for (var direction : directions) {
            var i      = 0;
            var dy     = row;
            var dx     = col;
            int lastDy = dy, lastDx = dx;

            for (i = 0; i < word.length(); i++) {
                if (grid.isOutOfBounds(dx, dy) || grid.getValueAt(dx, dy) != word.charAt(i)) {
                    break;
                }

                lastDy = dy;
                lastDx = dx;

                dy += direction.y();
                dx += direction.x();
            }

            if (i == word.length()) {
                lineSegments.add(new LineSegment(Point2D.at(col, row), Point2D.at(lastDx, lastDy)));
            }
        }

        return lineSegments;
    }

    @NotNull
    private List<LineSegment> findLineSegments(Grid grid, @NotNull String word) {
        var lineSegments = new ArrayList<LineSegment>();

        for (var row = 0; row < grid.rows(); row++) {
            for (var col = 0; col < grid.cols(); col++) {
                lineSegments.addAll(findLineSegments(grid, row, col, word));
            }
        }

        return lineSegments;
    }

    @Override
    public Long solvePartOne() {
        return (long) findLineSegments(grid, "XMAS").size();
    }

    @Override
    public Long solvePartTwo() {
        var lineSegments = findLineSegments(grid, "MAS")
                .parallelStream()
                .filter(lineSegment -> lineSegment.orientation() == Diagonal)
                .collect(Collectors.toList());
        var crosses = (int) lineSegments.parallelStream()
                .filter(line -> lineSegments.stream().anyMatch(line::formsCrossWith))
                .count();

        return (long) (crosses / 2);
    }
}
