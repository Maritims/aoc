package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;
import org.jetbrains.annotations.NotNull;

import java.util.*;

import static io.github.maritims.advent_of_code.util.CollectionUtil.collectionOfNonNulls;
import static io.github.maritims.advent_of_code.util.CollectionUtil.generateUniquePairSet;

public class Day8 extends Day {
    public Day8(Boolean useSampleData) {
        super(2024, 8, useSampleData);
    }

    public Day8(String filename) {
        super(2024, 8, filename);
    }

    private Grid                                     grid;
    private HashMap<Character, LinkedHashSet<Point>> antennas;

    @Override
    protected void initialize() {
        super.initialize();
        grid = Grid.fromString(getInputText());
        antennas = new HashMap<>();

        for (var row = 0; row < grid.rows(); row++) {
            for (var col = 0; col < grid.cols(); col++) {
                var c = grid.getValueAt(col, row);
                if (c == '.') {
                    continue;
                }

                antennas.computeIfAbsent(c, k -> new LinkedHashSet<>()).add(Point.at(row, col));
            }
        }
    }

    @NotNull
    protected HashSet<Point> getAntiNodes(@NotNull Pair<Point, Point> pair) {
        var p1 = pair.first();
        var p2 = pair.second();
        var v  = p2.subtract(p1);

        return collectionOfNonNulls(
                HashSet::new,
                If.of(p1.subtract(v)).when(value -> !grid.isOutOfBounds(value)).thenGetValue(),
                If.of(p2.add(v)).when(value -> !grid.isOutOfBounds(value)).thenGetValue()
        );
    }

    @Override
    public Long solvePartOne() {
        return antennas.entrySet()
                .parallelStream()
                .filter(entry -> entry.getKey() != '.' && entry.getKey() != '#')
                .map(Map.Entry::getValue)
                .flatMap(value -> generateUniquePairSet(new ArrayList<>(value))
                        .stream()
                        .map(this::getAntiNodes)
                        .flatMap(Collection::parallelStream))
                .distinct()
                .count();
    }

    @Override
    public Long solvePartTwo() {
        return 0L;
    }
}
