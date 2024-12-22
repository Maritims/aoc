package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import io.github.maritims.advent_of_code.util.Direction;
import io.github.maritims.advent_of_code.util.ListUtil;
import io.github.maritims.advent_of_code.util.Occurrence;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class Day4 extends Day {
    private static final Logger log = LogManager.getLogger();

    protected Day4(boolean useSampleData) {
        super(2024, 4, useSampleData);
    }

    @NotNull
    private List<Occurrence<String>> findOccurrences(char[][] grid, int row, int col, @NotNull String word) {
        var rows = grid.length;
        var cols = grid[0].length;
        var occurrences = new ArrayList<Occurrence<String>>();

        for (var direction = 0; direction < 8; direction++) {
            var i = 0;
            var rowDirection = row;
            var colDirection = col;

            // Skip the first character since we've already checked it.
            for (i = 0; i < word.length(); i++) {
                if (rowDirection < 0 || rowDirection >= rows || colDirection < 0 || colDirection >= cols || grid[rowDirection][colDirection] != word.charAt(i)) {
                    break;
                }

                rowDirection += Direction.ROW_DIRECTIONS[direction];
                colDirection += Direction.COL_DIRECTIONS[direction];
            }

            if (i == word.length()) {
                // We've reached the end.
                var occurrence = new Occurrence<>(word, row, col, Direction.VALUES.get(direction));
                occurrences.add(occurrence);
            }
        }

        return occurrences;
    }

    @NotNull
    private List<Occurrence<String>> findOccurrences(char[][] grid, @NotNull String word) {
        var occurrences = new ArrayList<Occurrence<String>>();
        var rows = grid.length;
        var cols = grid[0].length;

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
        return 0;
    }
}
