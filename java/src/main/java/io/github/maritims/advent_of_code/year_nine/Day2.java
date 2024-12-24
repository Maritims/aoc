package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import io.github.maritims.advent_of_code.util.ListUtil;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;

public class Day2 extends Day {
    public Day2(Boolean useSampleData) {
        super(2024, 2, useSampleData);
    }

    private boolean isSafe(List<Integer> level) {
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

    @Override
    public @NotNull Integer solvePartOne() {
        return getInputLines()
                .stream()
                .map(line -> ListUtil.toIntegerList(line.split("\\s")))
                .mapToInt(level -> isSafe(level) ? 1 : 0).sum();
    }

    @Override
    public Integer solvePartTwo() {
        var lines = getInputLines();
        var safeReports = 0;

        for(var line : lines) {
            var level = ListUtil.toIntegerList(line.split("\\s"));

            for(var i = 0; i < level.size(); i++) {
                var thisLevel = new ArrayList<>(level);
                thisLevel.remove(i);

                if(isSafe(thisLevel)) {
                    safeReports++;
                    break;
                }
            }
        }

        return safeReports;
    }
}
