package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.Collections;

public class Day1 extends Day {
    public Day1(boolean useSampleData) {
        super(2024, 1, useSampleData);
    }

    @Override
    public @NotNull Integer solvePartOne() {
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

    @Override
    public Integer solvePartTwo() {
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

        var totalSimilarityScore = 0;

        for(var i = 0; i < lines.size(); i++) {
            var left = leftParts.get(i);
            var occurrences = Collections.frequency(rightParts, left);
            var similarityScore = left * occurrences;
            totalSimilarityScore += similarityScore;
        }

        return totalSimilarityScore;
    }
}
