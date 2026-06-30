package main

import "strings"

func convert(s string, numRows int) string {
	if numRows < 0 || len(s) <= numRows {
		return ""
	}

	rows := make([]strings.Builder, numRows)

	currentRow := 0
	goingDown := true

	for i := 0; i < len(s); i++ {
		c := rune(s[i])

		rows[currentRow].WriteRune(c)

		if currentRow == numRows {
			goingDown = false
		}

		if goingDown {
			currentRow++
		} else {
			currentRow--
		}

		if currentRow < 0 {
			goingDown = true
			currentRow = 0
		}
	}

	var result strings.Builder

	for i := len(rows) - 1; i >= 0; i-- {
		result.WriteString(rows[i].String())
	}

	return strings.TrimSpace(result.String())
}
