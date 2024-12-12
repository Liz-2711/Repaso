class MathOperations {
    int add(int a, int b) {
        return a + b;
    }

    int addThree(int a, int b, int c) {
        return a + b + c;
    }

    void showResults() {
        print(add(5, 10));
        print(addThree(5, 10, 15));
    }
}
