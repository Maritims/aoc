package io.github.maritims.advent_of_code.util;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class ListUtil {
    public static List<Integer> toIntegerList(String[] strings) {
        return Arrays.stream(strings)
                .map(Integer::parseInt)
                .collect(Collectors.toList());
    }
}
