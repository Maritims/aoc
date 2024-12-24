package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;

import java.util.*;
import java.util.stream.Collectors;

public class Day5 extends Day {
    public Day5(Boolean useSampleData) {
        super(2024, 5, useSampleData);
    }

    @Override
    public Integer solvePartOne() {
        var lines   = getInputLines();
        var rules   = new HashMap<Integer, Set<Integer>>();
        var updates = new ArrayList<ArrayList<Integer>>();

        for (var line : lines) {
            if (line.contains("|")) {
                var pair = line.split("\\|");
                var before = Integer.parseInt(pair[0]);
                var after = Integer.parseInt(pair[1]);
                rules.computeIfAbsent(before, k -> new HashSet<>()).add(after);
            } else if (!line.isEmpty()) {
                var pageNumbers = Arrays.stream(line.split(","))
                        .map(Integer::parseInt)
                        .collect(Collectors.toCollection(ArrayList::new));
                updates.add(pageNumbers);
            }
        }

        var iterator = updates.listIterator();
        var sum = 0;
        while(iterator.hasNext()) {
            var update = iterator.next();
            var isValid = true;

            for(var i = 0; i < update.size() - 1; i++) {
                var a = rules.get(update.get(i));
                if(a == null) {
                    isValid = false;
                    break;
                }

                var b = update.get(i + 1);
                if(!a.contains(b)) {
                    isValid = false;
                    break;
                }
            }

            if(isValid) {
                sum += update.get(update.size() / 2);
            } else {
                iterator.remove();
            }
        }

        return sum;
    }

    @Override
    public Integer solvePartTwo() {
        return 0;
    }
}
