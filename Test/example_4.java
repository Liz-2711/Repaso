class RefExample {
    void incrementBy(ref int x, int y) {
        x = x + y;
    }

    void applyIncrements() {
        int base;
        int i;
        int[5] values;
        base = 10;
       
        i = 0;
        while (i < 5) {
            incrementBy( base, i);
            values[i] = base;
            i = i + 1;
        }
        print(values[4]);
    }
}
