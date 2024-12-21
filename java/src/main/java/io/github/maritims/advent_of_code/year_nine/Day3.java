package io.github.maritims.advent_of_code.year_nine;

import io.github.maritims.advent_of_code.util.Day;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.ArrayList;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class Day3 extends Day {
    private static final Logger log = LogManager.getLogger();

    protected Day3(boolean useSampleData) {
        super(2024, 3, useSampleData);
    }

    @Override
    public Integer solvePartOne() {
        var text    = getInputText();
        var pattern = Pattern.compile("mul\\((\\d+),(\\d+)\\)");
        var matcher = pattern.matcher(text);
        var sum     = 0;

        while (matcher.find()) {
            var leftFactor  = Integer.parseInt(matcher.group(1));
            var rightFactor = Integer.parseInt(matcher.group(2));
            var product     = leftFactor * rightFactor;
            sum += product;
        }

        return sum;
    }

    @Override
    public Integer solvePartTwo() {
        var text     = getInputText();
        var position = 0;
        var sum      = 0;
        var tokens   = new ArrayList<Token>();

        while (position < text.length()) {
            var c = text.charAt(position);
            if (Character.isDigit(c)) {
                var start = position;
                while (position < text.length() && Character.isDigit(text.charAt(position))) {
                    position++;
                }
                tokens.add(new Token(TokenType.NUMBER, text.substring(start, position)));
            } else if (c == ',') {
                tokens.add(new Token(TokenType.COMMA, ","));
                position++;
            } else if (c == '(') {
                tokens.add(new Token(TokenType.LEFT_PARENTHESIS, "("));
                position++;
            } else if (c == ')') {
                tokens.add(new Token(TokenType.RIGHT_PARENTHESIS, ")"));
                position++;
            } else if (position + 5 <= text.length() && text.substring(position, position + 5).equals("don't")) {
                tokens.add(new Token(TokenType.DO_NOT, "don't"));
                position += 5;
            } else if (position + 2 <= text.length() && text.substring(position, position + 2).equals("do")) {
                tokens.add(new Token(TokenType.DO, "do"));
                position += 2;
            } else if (position + 3 <= text.length() && text.substring(position, position + 3).equals("mul")) {
                tokens.add(new Token(TokenType.MUL, "mul"));
                position += 3;
            } else {
                position++;
            }
        }

        var isMultiplicationEnabled = true;
        for (var i = 0; i < tokens.size(); i++) {
            var token = tokens.get(i);

            if (token.getType() == TokenType.DO) {
                isMultiplicationEnabled = true;
                continue;
            }

            if (token.getType() == TokenType.DO_NOT) {
                isMultiplicationEnabled = false;
                continue;
            }

            if (token.getType() == TokenType.MUL && isMultiplicationEnabled) {
                if (tokens.get(i + 1).getType() != TokenType.LEFT_PARENTHESIS) {
                    log.error("Expected {} at {}", TokenType.LEFT_PARENTHESIS, i + 1);
                    continue;
                }

                if (tokens.get(i + 2).getType() != TokenType.NUMBER) {
                    log.error("Expected {} at {}", TokenType.NUMBER, i + 2);
                    continue;
                }

                if (tokens.get(i + 3).getType() != TokenType.COMMA) {
                    log.error("Expected {} at {}", TokenType.COMMA, i + 3);
                    continue;
                }

                if (tokens.get(i + 4).getType() != TokenType.NUMBER) {
                    log.error("Expected {} at {}", TokenType.NUMBER, i + 4);
                    continue;
                }

                if (tokens.get(i + 5).getType() != TokenType.RIGHT_PARENTHESIS) {
                    log.error("Expected {} at {}", TokenType.RIGHT_PARENTHESIS, i + 5);
                    continue;
                }

                var leftFactor  = Integer.parseInt(tokens.get(i + 2).getValue());
                var rightFactor = Integer.parseInt(tokens.get(i + 4).getValue());
                var product     = leftFactor * rightFactor;
                sum += product;
            }
        }

        return sum;
    }

    enum TokenType {
        DO, DO_NOT, MUL, NUMBER, COMMA, LEFT_PARENTHESIS, RIGHT_PARENTHESIS
    }

    static class Token {
        private final TokenType type;
        private final String    value;

        public Token(TokenType type, String value) {
            this.type = type;
            this.value = value;
        }

        public TokenType getType() {
            return type;
        }

        public String getValue() {
            return value;
        }

        @Override
        public String toString() {
            return value;
        }
    }
}
