impl Solution {
    pub fn convert(s: String, num_rows: i32) -> String {
        let num_rows = num_rows as usize;

        if num_rows <= 1 || s.len() <= num_rows {
            return s;
        }

        let mut rows: Vec<String> = vec![String::new(); num_rows];
        let mut current_row: usize = 0;
        let mut going_down = false;

        for c in s.chars() {
            rows[current_row].push(c);

            if current_row == 0 || current_row == num_rows - 1 {
                going_down = !going_down;
            }

            current_row = if going_down {
                current_row + 1
            } else {
                current_row - 1
            };
        }

        rows.concat()
    }
}