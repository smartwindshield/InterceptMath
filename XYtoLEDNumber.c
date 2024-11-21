#include <stdio.h>
#include <math.h>

int map_to_led_index(double x, double y, int rows, int cols) {
    // Ensure x and y are in the range [-1, 1]
    if (x < -1 || x > 1 || y < -1 || y > 1) {
        printf("Error: Coordinates out of range.\n");
        return -1;
    }

    // Convert relative coordinates to grid indices
    int col_index = (int)round((x + 1) / 2 * (cols - 1)); // Map x to column (0 to cols-1)
    int row_index = (int)round((y + 1) / 2 * (rows - 1)); // Map y to row (0 to rows-1)

    // Handle snaking pattern for LED mapping
    if (row_index % 2 == 1) {
        // Odd rows reverse the column order
        col_index = cols - 1 - col_index;
    }

    // Compute the LED index
    int led_index = row_index * cols + col_index;

    return led_index;
}

int main() {
    // Example usage
    double x = 0.7, y = 1.0; // Relative coordinates (center of rectangle)
    int rows = 5, cols = 10; // LED grid dimensions

    int led_index = map_to_led_index(x, y, rows, cols);

    if (led_index != -1) {
        printf("The LED index for coordinates (%.2f, %.2f) is: %d\n", x, y, led_index);
    }

    return 0;
}
