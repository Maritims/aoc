package io.github.maritims.advent_of_code.util;

public class MathUtil {
    public static long concatenate(long a, long b) {
        var digits = (int) Math.log10(b) + 1;
        return a * (long) Math.pow(10, digits) + b;
    }

    public static long getDigitCount(long number) {
        number = Math.abs(number);
        var count = 0L;
        do {
            number /= 10;
            count++;
        } while (number > 0);
        return count;
    }
}
