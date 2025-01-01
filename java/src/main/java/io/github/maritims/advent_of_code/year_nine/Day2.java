package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import io.github.maritims.advent_of_code.util.CollectionUtil;
import org.jetbrains.annotations.NotNull;

import java.util.List;
import java.util.stream.IntStream;

public class Day2 extends Day {
    public Day2(Boolean useSampleData) {
        super(2024, 2, useSampleData);
    }

    private List<List<Long>> levels;

    @Override
    protected void initialize() {
        super.initialize();
        levels = CollectionUtil.splitToListOfLists(getInputLines(), "\\s", Long::parseLong);
    }

    private boolean isSafe(List<Long> level) {
        var isIncreasing = true;
        var isDecreasing = true;
        var isSafe       = true;

        for (var i = 0; i < level.size() - 1; i++) {
            if (level.get(i) > level.get(i + 1)) {
                isIncreasing = false;
            } else if (level.get(i) < level.get(i + 1)) {
                isDecreasing = false;
            }

            var difference = Math.abs(level.get(i) - level.get(i + 1));
            if (difference == 0 || difference > 3) {
                isSafe = false;
                break;
            }
        }

        return isSafe && (isIncreasing || isDecreasing);
    }

    private boolean couldBeSafe(List<Long> level) {
        return IntStream.range(0, level.size())
                .anyMatch(i -> {
                    var tmp = level.remove(i);
                    var isSafe = isSafe(level);
                    level.add(i, tmp);
                    return isSafe;
                });
    }

    @Override
    public @NotNull Long solvePartOne() {
        return levels.parallelStream()
                .filter(this::isSafe)
                .count();
    }

    @Override
    public Long solvePartTwo() {
        return levels.parallelStream()
                .filter(this::couldBeSafe)
                .count();
    }
}
