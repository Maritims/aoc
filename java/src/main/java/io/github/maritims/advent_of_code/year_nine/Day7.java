package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import io.github.maritims.advent_of_code.util.ListUtil;
import io.github.maritims.advent_of_code.util.MathUtil;
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

    private List<String> lines;

    @Override
    protected void initialize() {
        super.initialize();
        lines = getInputLines();
    }

    private Long getTotalCalibrationResult(List<Operation> operations) {
        var pattern = Pattern.compile("^(\\d+): ([\\d\\s]+)$");
        var trueEquations = lines.parallelStream()
                .map(line -> pattern.matcher(line).results())
                .flatMap(matchResults -> matchResults.map(matchResult -> Pair.of(
                        Long.parseLong(matchResult.group(1)),
                        ListUtil.splitToList(matchResult.group(2).trim(), "\\s+", Long::parseLong)
                )))
                .map(pair -> new Equation(pair.first(), pair.second(), operations))
                .filter(Equation::isTrue)
                .collect(Collectors.toList());

        return trueEquations.stream()
                .mapToLong(Equation::getResult)
                .sum();
    }

    @Override
    public Long solvePartOne() {
        return getTotalCalibrationResult(List.of(Math::addExact, Math::multiplyExact));
    }

    @Override
    public Long solvePartTwo() {
        return getTotalCalibrationResult(List.of(Math::addExact, Math::multiplyExact, MathUtil::concatenate));
    }

    interface Operation extends BiFunction<Long, Long, Long> {
    }

    static class Equation {
        private final        long            result;
        private final        List<Long>      numbers;
        private final        List<Operation> operations;

        Equation(long result, List<Long> numbers, List<Operation> operations) {
            this.result = result;
            this.numbers = numbers;
            this.operations = operations;
        }

        public long getResult() {
            return result;
        }

        public boolean isTrue() {
            var operatorSpaces    = numbers.size() - 1;
            var totalCombinations = Math.pow(operations.size(), operatorSpaces);

            for (var i = 0; i < totalCombinations; i++) {
                var temp                 = i;
                var operatorCombinations = new ArrayList<Operation>();

                for (var j = 0; j < operatorSpaces; j++) {
                    operatorCombinations.add(operations.get(temp % operations.size()));
                    temp /= operations.size();
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
