class NestedMethodCalls {
    int square(int x) {
        return x * x;
    }

    int calculateExpression(int a, int b, int c) {
        return square(a) + square(b) + square(c);
    }

    void displayResult() {
        int result;
        result = calculateExpression(2, 3, 4);
        print(result);
    }
}
