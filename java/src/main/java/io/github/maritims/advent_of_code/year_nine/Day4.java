package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class Day4 extends Day {
    public Day4(Boolean useSampleData) {
        super(2024, 4, useSampleData);
    }

    @NotNull
    private List<Occurrence<String>> findOccurrences(char[][] grid, int row, int col, @NotNull String word) {
        var rows        = grid.length;
        var cols        = grid[0].length;
        var occurrences = new ArrayList<Occurrence<String>>();

        for (var direction = 0; direction < 8; direction++) {
            var i  = 0;
            var rd = row;
            var cd = col;
            int lastRd = rd, lastCd = cd;

            for (i = 0; i < word.length(); i++) {
                if (rd < 0 || rd >= rows || cd < 0 || cd >= cols || grid[rd][cd] != word.charAt(i)) {
                    break;
                }

                lastRd = rd;
                lastCd = cd;

                rd += Direction.ROW_DIRECTIONS[direction];
                cd += Direction.COL_DIRECTIONS[direction];
            }

            if (i == word.length()) {
                var occurrence = new Occurrence<>(word, row, col, lastRd, lastCd, Direction.VALUES.get(direction));
                occurrences.add(occurrence);
            }
        }

        return occurrences;
    }

    @NotNull
    private List<Occurrence<String>> findOccurrences(char[][] grid, @NotNull String word) {
        var occurrences = new ArrayList<Occurrence<String>>();
        var rows        = grid.length;
        var cols        = grid[0].length;

        for (var row = 0; row < rows; row++) {
            for (var col = 0; col < cols; col++) {
                occurrences.addAll(findOccurrences(grid, row, col, word));
            }
        }

        return occurrences;
    }

    @Override
    public Integer solvePartOne() {
        var lines  = getInputLines();
        var grid   = ListUtil.toCharacterGrid(lines);
        var result = findOccurrences(grid, "XMAS");

        return result.size();
    }

    @Override
    public Integer solvePartTwo() {
        var lines = getInputLines();
        var grid  = ListUtil.toCharacterGrid(lines);
        var occurrences = findOccurrences(grid, "MAS")
                .stream()
                .filter(occurrence -> occurrence.getDirection().isDiagonal())
                .map(Occurrence::toLine2D)
                .collect(Collectors.toList());
        var intersections = (int) occurrences.stream()
                .filter(line -> occurrences.stream().anyMatch(line::intersects))
                .count();

        return intersections / 2;
    }
}
