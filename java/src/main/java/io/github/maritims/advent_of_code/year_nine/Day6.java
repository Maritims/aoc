package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.*;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;
import java.util.function.Function;
import java.util.stream.IntStream;

public class Day6 extends Day {
    public Day6(Boolean useSampleData) {
        super(2024, 6, useSampleData);
    }

    void foo(int row, int col, char[][] grid) {
        if (grid[row][col] == '#') {

        }
    }

    @Override
    public Integer solvePartOne() {
        var   lines            = getInputLines();
        int[] startingPosition = null;
        for (var row = 0; row < lines.size(); row++) {
            var col = lines.get(row).indexOf('^');
            if (col != -1) {
                startingPosition = new int[]{row, col};
                break;
            }
        }
        if(startingPosition == null) {
            throw new IllegalStateException("Unable to acquire starting position");
        }

        var grid             = ListUtil.toCharacterGrid(lines);
        var rowDirections    = new int[]{-1, 0, 1, 0};
        var colDirections    = new int[]{0, 1, 0, -1};
        var currentDirection = 0;
        var queue            = new LinkedList<int[]>();
        var visited          = new Boolean[grid.length][grid[0].length];
        visited[startingPosition[0]][startingPosition[1]] = true;
        queue.add(startingPosition);

        while (!queue.isEmpty()) {
            var coords  = queue.poll();
            var nextRow = coords[0] + rowDirections[currentDirection];
            var nextCol = coords[1] + colDirections[currentDirection];

            if (nextRow < 0 || nextRow >= grid.length || nextCol < 0 || nextCol >= grid[0].length) {
                // We've found the exit.
                break;
            }

            if (grid[nextRow][nextCol] == '#') {
                // Change direction and try again.
                currentDirection = (currentDirection + 4 + 1) % 4;
                nextRow = coords[0] + rowDirections[currentDirection];
                nextCol = coords[1] + colDirections[currentDirection];

                visited[nextRow][nextCol] = true;
                queue.add(new int[]{nextRow, nextCol});
                continue;
            }

            visited[nextRow][nextCol] = true;
            queue.add(new int[]{nextRow, nextCol});
        }

        return Arrays.stream(visited)
                .mapToInt(row -> Arrays.stream(row).mapToInt(v -> Boolean.TRUE.equals(v) ? 1 : 0).sum())
                .sum();
    }

    @Override
    public Integer solvePartTwo() {
        return 0;
    }
}
