package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import io.github.maritims.advent_of_code.util.ListUtil;
import io.github.maritims.advent_of_code.util.Pair;

import java.util.ArrayList;
import java.util.List;
import java.util.function.BiFunction;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class Day7 extends Day {
    public Day7(Boolean useSampleData) {
        super(2024, 7, useSampleData);
    }

    @Override
    public Long solvePartOne() {
        var lines   = getInputLines();
        var pattern = Pattern.compile("^(\\d+): ([\\d\\s]+)$");
        var trueEquations = lines.parallelStream()
                .map(line -> pattern.matcher(line).results())
                .flatMap(matchResults -> matchResults.map(matchResult -> Pair.of(
                        Long.parseLong(matchResult.group(1)),
                        ListUtil.toLongList(matchResult.group(2).trim().split("\\s+"))
                )))
                .map(pair -> new Equation(pair.first(), pair.second()))
                .filter(Equation::isTrue)
                .collect(Collectors.toList());

        return trueEquations.stream()
                .mapToLong(Equation::getResult)
                .sum();
    }

    @Override
    public Long solvePartTwo() {
        return 0L;
    }

    interface Operation extends BiFunction<Long, Long, Long> {
    }

    static class Equation {
        private static final List<Operation> operators = List.of(Math::addExact, Math::multiplyExact);
        private final        long            result;
        private final        List<Long>      numbers;

        Equation(long result, List<Long> numbers) {
            this.result = result;
            this.numbers = numbers;
        }

        public long getResult() {
            return result;
        }

        public boolean isTrue() {
            var operatorSpaces    = numbers.size() - 1;
            var totalCombinations = Math.pow(2, operatorSpaces);

            for (var i = 0; i < totalCombinations; i++) {
                var temp                 = i;
                var operatorCombinations = new ArrayList<Operation>();

                for (var j = 0; j < operatorSpaces; j++) {
                    operatorCombinations.add(operators.get(temp % operators.size()));
                    temp /= operators.size();
                }

                var result = numbers.get(0);
                for (var k = 0; k < operatorSpaces; k++) {
                    result = operatorCombinations.get(k).apply(result, numbers.get(k + 1));
                }
                if (result == getResult()) {
                    return true;
                }
            }

            return false;
        }
    }
}
