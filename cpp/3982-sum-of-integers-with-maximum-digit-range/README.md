<h2><a href="https://leetcode.com/problems/sum-of-integers-with-maximum-digit-range">Sum of Integers with Maximum Digit Range</a></h2> <img src='https://img.shields.io/badge/Difficulty-Easy-brightgreen' alt='Difficulty: Easy' /><hr><p>You are given an integer array <code>nums</code>.</p>

<p>The <strong>digit range</strong> of an integer is defined as the difference between its <strong>largest</strong> digit and <strong>smallest</strong> digit.</p>

<p>For example, the digit range of 5724 is <code>7 - 2 = 5</code>.</p>

<p>Return the sum of all integers in <code>nums</code> whose <strong>digit range</strong> is equal to the <strong>maximum digit range</strong> among all integers in the array.</p>

<p>&nbsp;</p>
<p><strong class="example">Example 1:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">nums = [5724,111,350]</span></p>

<p><strong>Output:</strong> <span class="example-io">6074</span></p>

<p><strong>Explanation:</strong></p>

<table border="1" bordercolor="#ccc" cellpadding="5" cellspacing="0" style="border-collapse:collapse;">
	<tbody>
		<tr>
			<th style="text-align:center;"><code>i</code></th>
			<th style="text-align:center;"><code>nums[i]</code></th>
			<th style="text-align:center;">Largest</th>
			<th style="text-align:center;">Smallest</th>
			<th style="text-align:center;">Digit Range</th>
		</tr>
		<tr>
			<td style="text-align:center;">0</td>
			<td style="text-align:center;">5724</td>
			<td style="text-align:center;">7</td>
			<td style="text-align:center;">2</td>
			<td style="text-align:center;">5</td>
		</tr>
		<tr>
			<td style="text-align:center;">1</td>
			<td style="text-align:center;">111</td>
			<td style="text-align:center;">1</td>
			<td style="text-align:center;">1</td>
			<td style="text-align:center;">0</td>
		</tr>
		<tr>
			<td style="text-align:center;">2</td>
			<td style="text-align:center;">350</td>
			<td style="text-align:center;">5</td>
			<td style="text-align:center;">0</td>
			<td style="text-align:center;">5</td>
		</tr>
	</tbody>
</table>

<p>The maximum digit range is 5. The integers with this digit range are 5724 and 350, so the answer is <code>5724 + 350 = 6074</code>.</p>
</div>

<p><strong class="example">Example 2:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">nums = [90,900]</span></p>

<p><strong>Output:</strong> <span class="example-io">990</span></p>

<p><strong>Explanation:</strong></p>

<table border="1" bordercolor="#ccc" cellpadding="5" cellspacing="0" style="border-collapse:collapse;">
	<tbody>
		<tr>
			<th style="text-align:center;"><code>i</code></th>
			<th style="text-align:center;"><code>nums[i]</code></th>
			<th style="text-align:center;">Largest</th>
			<th style="text-align:center;">Smallest</th>
			<th style="text-align:center;">Digit Range</th>
		</tr>
		<tr>
			<td style="text-align:center;">0</td>
			<td style="text-align:center;">90</td>
			<td style="text-align:center;">9</td>
			<td style="text-align:center;">0</td>
			<td style="text-align:center;">9</td>
		</tr>
		<tr>
			<td style="text-align:center;">1</td>
			<td style="text-align:center;">900</td>
			<td style="text-align:center;">9</td>
			<td style="text-align:center;">0</td>
			<td style="text-align:center;">9</td>
		</tr>
	</tbody>
</table>

<p>The maximum digit range is 9. Both integers have this digit range, so the answer is <code>90 + 900 = 990</code>.</p>
</div>

<p>&nbsp;</p>
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 100</code></li>
	<li><code>10 &lt;= nums[i] &lt;= 10<sup>5</sup></code></li>
</ul>

