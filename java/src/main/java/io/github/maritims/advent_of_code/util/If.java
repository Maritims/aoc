package io.github.maritims.advent_of_code.util;

import java.util.function.Consumer;
import java.util.function.Predicate;

public class If<T> {
    private final T       value;
    private final boolean condition;

    public If(T value, boolean condition) {
        this.value = value;
        this.condition = condition;
    }

    public static <T> If<T> of(T value) {
        return new If<>(value, true);
    }

    public If<T> when(Predicate<T> predicate) {
        return condition && value != null ? new If<>(value, predicate.test(value)) : this;
    }

    public If<T> then(Consumer<T> action) {
        if(condition && value != null) {
            action.accept(value);
        }
        return this;
    }

    public T thenGetValue() {
        return condition && value != null ? value : null;
    }

    public If<T> otherwise(Consumer<T> action) {
        if(!condition && value != null) {
            action.accept(value);
        }
        return this;
    }

    public T get() {
        return value;
    }
}
