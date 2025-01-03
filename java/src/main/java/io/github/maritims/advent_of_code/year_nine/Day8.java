package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;
import org.jetbrains.annotations.NotNull;

import java.util.*;

import static io.github.maritims.advent_of_code.util.ListUtil.generateUniquePairSet;

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
                if (c == '.' || c == '#') {
                    continue;
                }

                antennas.computeIfAbsent(c, k -> new LinkedHashSet<>()).add(Point.at(col, row));
            }
        }
    }

    @SuppressWarnings("LoopConditionNotUpdatedInsideLoop")
    @NotNull
    protected HashSet<Point> getAntiNodes(@NotNull Pair<Point, Point> pair, boolean oneShot) {
        var p1        = pair.first();
        var p2        = pair.second();
        var v         = p2.subtract(p1);
        var antiNodes = new HashSet<Point>();

        if(!oneShot) {
            antiNodes.add(p1);
            antiNodes.add(p2);
        }

        do {
            var antiNode = p1.subtract(v);
            if (grid.isOutOfBounds(antiNode)) {
                break;
            }
            antiNodes.add(antiNode);
            p1 = antiNode;
        } while (!oneShot);

        do {
            var antiNode = p2.add(v);
            if (grid.isOutOfBounds(antiNode)) {
                break;
            }
            antiNodes.add(antiNode);
            p2 = antiNode;
        } while (!oneShot);

        return antiNodes;
    }

    private long getNumberOfUniqueAntiNodePositions(boolean oneShot) {
        return antennas.entrySet()
                .parallelStream()
                .filter(entry -> entry.getKey() != '.' && entry.getKey() != '#')
                .map(Map.Entry::getValue)
                .flatMap(value -> generateUniquePairSet(new ArrayList<>(value))
                        .stream()
                        .map(pair -> getAntiNodes(pair, oneShot))
                        .flatMap(Collection::parallelStream))
                .distinct()
                .count();
    }

    @Override
    public Long solvePartOne() {
        return getNumberOfUniqueAntiNodePositions(true);
    }

    @Override
    public Long solvePartTwo() {
        return getNumberOfUniqueAntiNodePositions(false);
    }
}
