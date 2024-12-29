package io.github.maritims.advent_of_code.util.tuples;

import java.util.Objects;

public class Pair<TFirst, TSecond> {
    private final TFirst  first;
    private final TSecond second;

    public Pair(TFirst first, TSecond second) {
        this.first = first;
        this.second = second;
    }

    public TFirst first() {
        return first;
    }

    public TSecond second() {
        return second;
    }

    public static <TFirst, TSecond> Pair<TFirst, TSecond> of(TFirst first, TSecond second) {
        return new Pair<>(first, second);
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        Pair<?, ?> pair = (Pair<?, ?>) o;
        return Objects.equals(first, pair.first) && Objects.equals(second, pair.second);
    }

    @Override
    public int hashCode() {
        return Objects.hash(first, second);
    }

    @Override
    public String toString() {
        return "Pair{" +
                "first=" + first +
                ", second=" + second +
                '}';
    }
}
