package io.github.maritims.advent_of_code.year_nine.day_one;

import io.github.maritims.advent_of_code.util.Part;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

public class PartOne extends Part {
    public PartOne(boolean useSampleData) {
        super(2024, 1, useSampleData);
    }

    @Override
    public @NotNull Integer solve() {
        var lines = getInputLines();
        var leftParts = new ArrayList<Integer>();
        var rightParts = new ArrayList<Integer>();

        for(var line : lines) {
            var parts = line.split("\\s+");
            var left = Integer.parseInt(parts[0]);
            var right = Integer.parseInt(parts[1]);

            leftParts.add(left);
            rightParts.add(right);
        }

        leftParts.sort(Integer::compareTo);
        rightParts.sort(Integer::compareTo);
        var totalDistance = 0;

        for(var i = 0; i < leftParts.size(); i++) {
            var distance = Math.abs(leftParts.get(i) - rightParts.get(i));
            totalDistance += distance;
        }

        return totalDistance;
    }
}
