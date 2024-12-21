package io.github.maritims.advent_of_code.util;

import java.util.List;

public class Monotonicity {
    public static boolean isMonotonic(List<Integer> series) {
        var isIncreasing = true;
        var isDecreasing = true;

        for(var i = 0; i < series.size() - 1; i++) {
            if(series.get(i) > series.get(i + 1)) {
                isIncreasing = false;
            } else if(series.get(i) < series.get(i + 1)) {
                isDecreasing = false;
            }
        }

        return isIncreasing || isDecreasing;
    }
}
