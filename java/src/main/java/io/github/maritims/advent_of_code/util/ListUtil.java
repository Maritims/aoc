package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class ListUtil {
    @NotNull
    public static List<Integer> toIntegerList(@NotNull String[] strings) {
        return Arrays.stream(strings)
                .map(Integer::parseInt)
                .collect(Collectors.toList());
    }

    public static char[][] toCharacterGrid(@NotNull List<String> strings) {
        var rows = strings.size();
        var cols = strings.get(0).length();
        var grid = new char[rows][cols];

        for(var i = 0; i < rows; i++) {
            grid[i] = strings.get(i).toCharArray();
        }

        return grid;
    }
}
