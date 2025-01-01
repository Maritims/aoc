package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.Function;
import java.util.stream.Collectors;

public class ListUtil {
    @NotNull
    public static <T> List<T> splitToList(@NotNull String str, @NotNull String regex, @NotNull Function<String, T> mapper) {
        return Arrays.stream(str.split(regex))
                .map(mapper)
                .collect(Collectors.toList());
    }

    @NotNull
    public static <T> List<List<T>> splitToListOfLists(@NotNull List<String> lines, @NotNull String regex, @NotNull Function<String, T> mapper) {
        return lines.stream()
                .map(line -> splitToList(line, regex, mapper))
                .collect(Collectors.toList());
    }

    public static <T> List<Pair<T, T>> generateUniquePairs(@NotNull List<T> items) {
        var pairs = new ArrayList<Pair<T, T>>();

        for(var i = 0; i < items.size(); i++) {
            for(var j = i + 1; j < items.size(); j++) {
                pairs.add(Pair.of(items.get(i), items.get(j)));
            }
        }

        return pairs;
    }
}
