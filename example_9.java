class ArrayMaxFinder {
    int findMax(int[5] arr) {
        int max, i;
        max = arr[0];
        i = 1;
        while (i < 5) {
            if (arr[i] > max) {
                max = arr[i];
            }
            i = i + 1;
        }
        return max;
    }
}
