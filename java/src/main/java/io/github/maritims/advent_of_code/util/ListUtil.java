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

    @NotNull
    public static List<Long> toLongList(@NotNull String[] strings) {
        return Arrays.stream(strings)
                .map(Long::parseLong)
                .collect(Collectors.toList());
    }
}
