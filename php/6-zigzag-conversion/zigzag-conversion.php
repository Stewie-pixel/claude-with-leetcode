<?php

function convert($s, $numRows) {
    if ($numRows < 0 || strlen($s) <= $numRows) {
        return "";
    }

    $rows = array_fill(0, $numRows, "");

    $currentRow = 0;
    $goingDown = true;

    for ($i = 0; $i < strlen($s); $i++) {

        $char = $s[$i];

        $rows[$currentRow] .= $char;

        if ($currentRow == $numRows) {
            $goingDown = false;
        }

        if ($goingDown) {
            $currentRow++;
        } else {
            $currentRow--;
        }

        if ($currentRow < 0) {
            $currentRow = 0;
            $goingDown = true;
        }
    }

    $result = "";

    for ($i = count($rows) - 1; $i >= 0; $i--) {
        $result .= $rows[$i];
    }

    return trim($result);
}

?>