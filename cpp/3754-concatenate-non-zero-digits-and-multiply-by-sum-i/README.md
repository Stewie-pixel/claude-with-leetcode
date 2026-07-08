<h2><a href="https://leetcode.com/problems/concatenate-non-zero-digits-and-multiply-by-sum-i">Concatenate Non-Zero Digits and Multiply by Sum I</a></h2> <img src='https://img.shields.io/badge/Difficulty-Easy-brightgreen' alt='Difficulty: Easy' /><hr><p>You are given an integer <code>n</code>.</p>

<p>Form a new integer <code>x</code> by concatenating all the <strong>non-zero digits</strong> of <code>n</code> in their original order. If there are no <strong>non-zero</strong> digits, <code>x = 0</code>.</p>

<p>Let <code>sum</code> be the <strong>sum of digits</strong> in <code>x</code>.</p>

<p>Return an integer representing the value of <code>x * sum</code>.</p>

<p>&nbsp;</p>
<p><strong class="example">Example 1:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">n = 10203004</span></p>

<p><strong>Output:</strong> <span class="example-io">12340</span></p>

<p><strong>Explanation:</strong></p>

<ul>
	<li>The non-zero digits are 1, 2, 3, and 4. Thus, <code>x = 1234</code>.</li>
	<li>The sum of digits is <code>sum = 1 + 2 + 3 + 4 = 10</code>.</li>
	<li>Therefore, the answer is <code>x * sum = 1234 * 10 = 12340</code>.</li>
</ul>
</div>

<p><strong class="example">Example 2:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">n = 1000</span></p>

<p><strong>Output:</strong> <span class="example-io">1</span></p>

<p><strong>Explanation:</strong></p>

<ul>
	<li>The non-zero digit is 1, so <code>x = 1</code> and <code>sum = 1</code>.</li>
	<li>Therefore, the answer is <code>x * sum = 1 * 1 = 1</code>.</li>
</ul>
</div>

<p>&nbsp;</p>
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= n &lt;= 10<sup>9</sup></code></li>
</ul>

