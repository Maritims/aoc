package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import io.github.maritims.advent_of_code.util.CollectionUtil;
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

    private List<Equation> equations;

    @Override
    protected void initialize() {
        super.initialize();
        var pattern = Pattern.compile("^(\\d+): ([\\d\\s]+)$");
        var lines   = getInputLines();
        equations = lines.parallelStream()
                .map(line -> pattern.matcher(line).results())
                .flatMap(matchResults -> matchResults.map(matchResult -> Pair.of(
                        Long.parseLong(matchResult.group(1)),
                        CollectionUtil.splitToList(matchResult.group(2).trim(), "\\s+", Long::parseLong)
                )))
                .map(pair -> new Equation(pair.first(), pair.second()))
                .collect(Collectors.toList());
    }

    private Long getTotalCalibrationResult(List<Operation> operations) {
        return equations.parallelStream()
                .filter(equation -> equation.isTrue(operations))
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
        private final long       result;
        private final List<Long> numbers;

        Equation(long result, List<Long> numbers) {
            this.result = result;
            this.numbers = numbers;
        }

        public long getResult() {
            return result;
        }

        public boolean isTrue(List<Operation> operations) {
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
