package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;

import java.util.*;

import static java.util.stream.Collectors.*;

public class Day5 extends Day {
    public Day5(Boolean useSampleData) {
        super(2024, 5, useSampleData);
    }

    private Map<Integer, Set<Integer>> rules;
    private List<Update>               updates;

    @Override
    protected void initialize() {
        super.initialize();
        var lines = getInputLines();
        rules = getRules(lines);
        updates = getUpdates(lines, rules);
    }

    protected Map<Integer, Set<Integer>> getRules(List<String> lines) {
        return lines.stream()
                .filter(line -> line.contains("|"))
                .map(line -> line.split("\\|"))
                .collect(groupingBy(pair -> Integer.parseInt(pair[0]), mapping(pair -> Integer.parseInt(pair[1]), toSet())));
    }

    protected ArrayList<Update> getUpdates(List<String> lines, Map<Integer, Set<Integer>> rules) {
        var iterator = lines.iterator();
        var updates  = new ArrayList<Update>();

        while (iterator.hasNext()) {
            var line = iterator.next();
            if (line.isEmpty() || line.contains("|")) {
                continue;
            }

            var pageNumbers = Arrays.stream(line.split(","))
                    .map(Integer::parseInt)
                    .collect(toList());
            var isValid = true;

            for (var i = 0; i < pageNumbers.size() - 1; i++) {
                var acceptedFollowers = rules.get(pageNumbers.get(i));
                if (acceptedFollowers == null) {
                    isValid = false;
                    break;
                }

                var follower = pageNumbers.get(i + 1);
                if (!acceptedFollowers.contains(follower)) {
                    isValid = false;
                    break;
                }
            }

            updates.add(new Update(pageNumbers, isValid));
        }

        return updates;
    }

    @Override
    public Long solvePartOne() {
        return updates.stream()
                .filter(Update::isValid)
                .mapToLong(Update::getMiddlePageNumber)
                .sum();
    }

    @SuppressWarnings("ComparatorMethodParameterNotUsed")
    @Override
    public Long solvePartTwo() {
        return (long) updates.stream()
                .filter(update -> !update.isValid())
                .mapToInt(update -> {
                    var pageNumbers = new ArrayList<>(update.getPageNumbers());
                    pageNumbers.sort((o1, o2) -> rules.getOrDefault(o1, new HashSet<>()).contains(o2) ? -1 : Integer.MAX_VALUE);
                    return pageNumbers.get(pageNumbers.size() / 2);
                })
                .sum();
    }

    public static class Update {
        private final List<Integer> pageNumbers;
        private final boolean       isValid;

        public Update(List<Integer> pageNumbers, boolean isValid) {
            this.pageNumbers = pageNumbers;
            this.isValid = isValid;
        }

        public List<Integer> getPageNumbers() {
            return pageNumbers;
        }

        public Integer getMiddlePageNumber() {
            return pageNumbers.get(pageNumbers.size() / 2);
        }

        public boolean isValid() {
            return isValid;
        }
    }

}
