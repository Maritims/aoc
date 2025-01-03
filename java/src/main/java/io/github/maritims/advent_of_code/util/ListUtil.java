package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;

import java.util.*;
import java.util.function.Function;
import java.util.function.Predicate;
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

    @NotNull
    public static <T> LinkedHashSet<Pair<T, T>> generateUniquePairSet(@NotNull List<T> items) {
        var pairs = new LinkedHashSet<Pair<T, T>>();

        for(var i = 0; i < items.size(); i++) {
            for(var j = i + 1; j < items.size(); j++) {
                pairs.add(Pair.of(items.get(i), items.get(j)));
            }
        }

        return pairs;
    }

    public static <T> int lastIndexOf(List<T> list, Predicate<T> predicate) {
        for(var i = list.size() - 1; i > 0; i--) {
            var item = list.get(i);
            if(predicate.test(item)) {
                return i;
            }
        }
        return -1;
    }

    public static <T> void swap(LinkedList<T> list, int index1, int index2) {
        if(index1 == -1) {
            throw new IllegalArgumentException("index1");
        }

        if(index2 == -1) {
            throw new IllegalArgumentException("index2");
        }

        var element1 = list.get(index1);
        var element2 = list.get(index2);
        list.set(index1, element2);
        list.set(index2, element1);
    }
}
