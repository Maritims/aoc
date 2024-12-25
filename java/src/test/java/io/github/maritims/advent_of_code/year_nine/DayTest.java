package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import org.jetbrains.annotations.NotNull;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.params.provider.Arguments.arguments;

public class DayTest<T extends Day> {
    public static Stream<Arguments> solveUsingSampleData() {
        return Stream.of(
                arguments(2024, 1, 11, 31),
                arguments(2024, 2, 2, 4),
                arguments(2024, 3, 161, 48),
                arguments(2024, 4, 18, 9),
                arguments(2024, 5, 143, 123)
        );
    }

    public static Stream<Arguments> solveUsingRealInput() {
        return Stream.of(
                arguments(2024, 1, 2580760, 25358365),
                arguments(2024, 2, 598, 634),
                arguments(2024, 3, 187194524, 127092535),
                arguments(2024, 4, 2646, 2000),
                arguments(2024, 5, 5955, 4030)
        );
    }

    @SuppressWarnings("unchecked")
    @NotNull
    private Class<T> getClassToTest(int year, int day) {
        var className = String.format("%s.Day%d", getClass().getPackage().getName(), day);
        try {
            return (Class<T>) Class.forName(className);
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }

    @NotNull
    private T getInstance(int year, int day, Object... args) {
        var            classToTest = getClassToTest(year, day);
        Constructor<T> constructor;
        try {
            constructor = classToTest.getConstructor(Boolean.class);
        } catch (NoSuchMethodException e) {
            throw new RuntimeException(e);
        }
        try {
            return constructor.newInstance(args);
        } catch (InstantiationException | IllegalAccessException | InvocationTargetException e) {
            throw new RuntimeException(e);
        }
    }

    @DisplayName("Solving using sample input")
    @ParameterizedTest(name = "{0}.{1}: {2} (part 1), {3} (part 2)")
    @MethodSource
    void solveUsingSampleData(int year, int day, int expectedAnswerToPartOne, int expectedAnswerToPartTwo) {
        var sampleInputAnswers = getInstance(year, day, true).solve();
        assertEquals(expectedAnswerToPartOne, sampleInputAnswers[0], "Wrong answer for part 1");
        assertEquals(expectedAnswerToPartTwo, sampleInputAnswers[1], "Wrong answer for part 2");
    }

    @DisplayName("Solving using real input")
    @ParameterizedTest(name = "{0}.{1}: {2} (part 1), {3} (part 2)")
    @MethodSource
    void solveUsingRealInput(int year, int day, int expectedAnswerToPartOne, int expectedAnswerToPartTwo) {
        var realInputAnswers = getInstance(year, day, false).solve();
        assertEquals(expectedAnswerToPartOne, realInputAnswers[0], "Wrong answer for part 1");
        assertEquals(expectedAnswerToPartTwo, realInputAnswers[1], "Wrong answer for part 2");
    }
}
