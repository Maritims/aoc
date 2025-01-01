package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;

import java.util.*;

public class Day8 extends Day {
    public Day8(Boolean useSampleData) {
        super(2024, 8, useSampleData);
    }

    public Day8(String filename) {
        super(2024, 8, filename);
    }

    private Grid                                       grid;
    private HashMap<Character, LinkedHashSet<Point2D>> antennas;

    @Override
    protected void initialize() {
        super.initialize();
        grid = Grid.fromString(getInputText());
        antennas = new HashMap<>();

        for (var row = 0; row < grid.rows(); row++) {
            for (var col = 0; col < grid.cols(); col++) {
                var c     = grid.getValueAt(col, row);
                var point = Point2D.at(col, row);

                antennas.compute(c, (k, v) -> {
                    if (v == null) {
                        v = new LinkedHashSet<>(Set.of(point));
                    } else {
                        v.add(point);
                    }
                    return v;
                });
            }
        }
    }

    @Override
    public Long solvePartOne() {
        // Find a point.
        // Is there one antenna with the same frequency in any direction?
        // If there is another antenna with the same frequency in any direction, get the Manhattan distance between the two.
        // The antinodes will be the same distance away in the same orientation as the antennas.
        var antinodes = new LinkedHashSet<Point2D>();

        for (var entry : antennas.entrySet()) {
            var frequency   = entry.getKey();
            if(frequency == '.' || frequency == '#') {
                continue;
            }

            var uniquePairs = ListUtil.generateUniquePairs(new ArrayList<>(entry.getValue()));

            for (var uniquePair : uniquePairs) {
                var lineSegment         = new LineSegment(uniquePair.first(), uniquePair.second());
                var extendedLineSegment = lineSegment.extend(lineSegment.length());

                if(!grid.isOutOfBounds(extendedLineSegment.p1())) {
                    antinodes.add(extendedLineSegment.p1());
                }

                if(!grid.isOutOfBounds(extendedLineSegment.p2())) {
                    antinodes.add(extendedLineSegment.p2());
                }
            }
        }

        return (long) antinodes.size();
    }

    @Override
    public Long solvePartTwo() {
        return 0L;
    }
}
