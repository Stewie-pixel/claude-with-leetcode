package main

import "strings"

func convert(s string, numRows int) string {
	if numRows <= 1 || len(s) <= numRows {
		return s
	}

	rows := make([]strings.Builder, numRows)
	currentRow := 0
	goingDown := false

	for _, c := range s {
		rows[currentRow].WriteRune(c)

		if currentRow == 0 || currentRow == numRows-1 {
			goingDown = !goingDown
		}

		if goingDown {
			currentRow++
		} else {
			currentRow--
		}
	}

	var result strings.Builder
	for _, row := range rows {
		result.WriteString(row.String())
	}

	return result.String()
}