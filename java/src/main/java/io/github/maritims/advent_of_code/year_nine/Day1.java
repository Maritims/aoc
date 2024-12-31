package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.IntStream;

public class Day1 extends Day {
    public Day1(Boolean useSampleData) {
        super(2024, 1, useSampleData);
    }

    private final List<Integer> leftList  = new ArrayList<>();
    private final List<Integer> rightList = new ArrayList<>();

    @Override
    protected void initialize() {
        super.initialize();
        getInputLines()
                .stream()
                .map(line -> line.split("\\s+"))
                .forEach(parts -> {
                    leftList.add(Integer.parseInt(parts[0]));
                    rightList.add(Integer.parseInt(parts[1]));
                });
        leftList.sort(Integer::compareTo);
        rightList.sort(Integer::compareTo);
    }

    @Override
    public @NotNull Long solvePartOne() {
        return (long) IntStream.range(0, leftList.size())
                .map(i -> Math.abs(leftList.get(i) - rightList.get(i)))
                .sum();
    }

    @Override
    public Long solvePartTwo() {
        return (long) leftList.parallelStream()
                .mapToInt(left -> left * Collections.frequency(rightList, left))
                .sum();
    }
}
