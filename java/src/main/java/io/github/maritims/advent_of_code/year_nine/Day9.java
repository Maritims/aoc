package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import io.github.maritims.advent_of_code.util.Pair;

import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Day9 extends Day {
    public Day9(Boolean useSampleData) {
        super(2024, 9, useSampleData);
    }

    private List<Optional<Integer>> originalDisk;

    @Override
    protected void initialize() {
        super.initialize();

        var text = getInputText();
        originalDisk = IntStream.range(0, text.length())
                .mapToObj(i -> Pair.of(i, text.charAt(i) - '0'))
                .flatMap(pair -> {
                    var i = pair.first();
                    var c = pair.second();
                    return Collections.nCopies(c, i % 2 == 0 ? Optional.of(i / 2) : Optional.<Integer>empty()).stream();
                })
                .collect(Collectors.toList());
    }

    @Override
    public Long solvePartOne() {
        var disk  = new ArrayList<>(originalDisk);
        var left  = 0;
        var right = disk.size() - 1;

        while (left < right) {
            while (left < right && disk.get(left).isPresent()) {
                left++;
            }

            while (left < right && disk.get(right).isEmpty()) {
                right--;
            }

            var tmp = disk.get(left);
            disk.set(left, disk.get(right));
            disk.set(right, tmp);

            left++;
            right--;
        }

        var checksum = IntStream.range(0, disk.size())
                .map(i -> disk.get(i).map(x -> x * i).orElse(0))
                .sum();

        return (long) checksum;
    }

    @Override
    public Long solvePartTwo() {
        return 0L;
    }

    enum BlockType {
        File,
        FreeSpace
    }

    static class Block {
        private final BlockType type;
        private final int       freeSpace;
        private final int       dataSize;

        Block(BlockType type, int freeSpace, int dataSize) {
            this.type = type;
            this.freeSpace = freeSpace;
            this.dataSize = dataSize;
        }

        public BlockType getType() {
            return type;
        }

        public int getFreeSpace() {
            return freeSpace;
        }

        public int getDataSize() {
            return dataSize;
        }
    }
}
