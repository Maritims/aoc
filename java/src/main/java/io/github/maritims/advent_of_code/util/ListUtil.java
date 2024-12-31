package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;

import java.util.Arrays;
import java.util.List;
import java.util.function.Function;
import java.util.stream.Collectors;

public class ListUtil {

    public static <T> List<T> splitToList(@NotNull String str, String regex, Function<String, T> mapper) {
        return Arrays.stream(str.split(regex))
                .map(mapper)
                .collect(Collectors.toList());
    }

    @NotNull
    public static <T> List<List<T>> splitToListOfLists(@NotNull List<String> lines, String regex, Function<String, T> mapper) {
        return lines.stream()
                .map(line -> splitToList(line, regex, mapper))
                .collect(Collectors.toList());
    }
}
