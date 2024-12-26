package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.List;
import java.util.Optional;

import static java.util.stream.Collectors.joining;
import static java.util.stream.Collectors.toList;

public abstract class Day {
    private final int     year;
    private final int     day;
    private final Boolean useSampleData;
    private final String  resourceName;

    public Day(int year, int day, Boolean useSampleData) {
        this.year = year;
        this.day = day;
        this.useSampleData = useSampleData;
        this.resourceName = year + "/" + day + "/" + (useSampleData ? "sample" : "input") + ".txt";
    }

    @NotNull
    private Optional<InputStream> getInputStream() {
        return Optional.ofNullable(getClass().getClassLoader().getResourceAsStream(resourceName));
    }

    @NotNull
    protected List<String> getInputLines() {
        var lines = getInputStream()
                .map(is -> {
                    try (var reader = new BufferedReader(new InputStreamReader(is))) {
                        return reader.lines().collect(toList());
                    } catch (IOException e) {
                        throw new RuntimeException(e);
                    }
                })
                .orElse(List.of());

        if (lines.isEmpty()) {
            throw new RuntimeException("no lines were read from resource " + resourceName);
        }

        return lines;
    }

    @NotNull
    protected String getInputText() {
        return getInputStream()
                .map(is -> new BufferedReader(new InputStreamReader(is)).lines().collect(joining("\n")))
                .orElseThrow(() -> new RuntimeException("no text was read from resource " + resourceName));
    }

    public abstract Integer solvePartOne();

    public abstract Integer solvePartTwo();

    public final Integer[] solve() {
        return new Integer[] { solvePartOne(), solvePartTwo() };
    }

    @Override
    public String toString() {
        return String.format("%d.%d", year, day);
    }
}
