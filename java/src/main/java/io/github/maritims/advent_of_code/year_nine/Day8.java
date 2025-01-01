package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import io.github.maritims.advent_of_code.util.Grid;
import io.github.maritims.advent_of_code.util.Point2D;

import java.util.*;

public class Day8 extends Day {
    public Day8(Boolean useSampleData) {
        super(2024, 8, useSampleData);
    }

    public Day8(String filename) {
        super(2024, 8, filename);
    }

    private Grid                                       grid;
    private HashMap<Character, LinkedHashSet<Point2D>> antennaLocations;

    @Override
    protected void initialize() {
        super.initialize();
        grid = Grid.fromString(getInputText());
        antennaLocations = new HashMap<>();

        for (var row = 0; row < grid.rows(); row++) {
            for (var col = 0; col < grid.cols(); col++) {
                var c     = grid.getValueAt(col, row);
                var point = Point2D.at(col, row);

                antennaLocations.compute(c, (k, v) -> {
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
        
        return 0L;
    }

    @Override
    public Long solvePartTwo() {
        return 0L;
    }
}
