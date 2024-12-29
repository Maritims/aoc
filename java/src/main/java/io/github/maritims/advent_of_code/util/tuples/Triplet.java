package io.github.maritims.advent_of_code.util.tuples;

public class Triplet<TFirst, TSecond, TThird> {
    private final TFirst  first;
    private final TSecond second;
    private final TThird  third;

    public Triplet(TFirst first, TSecond second, TThird third) {
        this.first = first;
        this.second = second;
        this.third = third;
    }

    public TFirst first() {
        return first;
    }

    public TSecond second() {
        return second;
    }

    public TThird third() {
        return third;
    }

    public static <TFirst, TSecond, TThird> Triplet<TFirst, TSecond, TThird> of(TFirst first, TSecond second, TThird third) {
        return new Triplet<>(first, second, third);
    }

    @Override
    public String toString() {
        return "Triplet{" +
                "first=" + first +
                ", second=" + second +
                ", third=" + third +
                '}';
    }
}
