package io.github.maritims.advent_of_code.util;

import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.util.List;
import java.util.Optional;

import static java.util.stream.Collectors.*;

public abstract class Part {
    private final String resourceName;

    protected Part(int year, int day, boolean useSampleData) {
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
                    try(var reader = new BufferedReader(new InputStreamReader(is))) {
                        return reader.lines().collect(toList());
                    } catch (IOException e) {
                        throw new RuntimeException(e);
                    }
                })
                .orElse(List.of());

        if(lines.isEmpty()) {
            throw new RuntimeException("no lines were read from resource " + resourceName);
        }

        return lines;
    }

    @NotNull
    public abstract Integer solve();
}
