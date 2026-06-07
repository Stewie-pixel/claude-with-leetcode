<h2><a href="https://leetcode.com/problems/exactly-one-consecutive-set-bits-pair">Exactly One Consecutive Set Bits Pair</a></h2> <img src='https://img.shields.io/badge/Difficulty-Easy-brightgreen' alt='Difficulty: Easy' /><hr><p>You are given an integer <code>n</code>.</p>

<p>Return <code>true</code> if its binary representation contains <strong>exactly one</strong> pair of <strong>consecutive set</strong> bits, and <code>false</code> otherwise.</p>
The <strong>set</strong> bits in an integer are the <code>1</code>&#39;s present when it is written in <strong>binary</strong>.
<p>&nbsp;</p>
<p><strong class="example">Example 1:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">nums = 6</span></p>

<p><strong>Output:</strong> <span class="example-io">true</span></p>

<p><strong>Explanation:</strong></p>

<ul>
	<li>Binary representation of 6 is <code>110</code>.</li>
	<li>There is exactly one pair of consecutive set bits (<code>&quot;11&quot;</code>). Thus, the answer is <code>true</code>​​​​​​​.</li>
</ul>
</div>

<p><strong class="example">Example 2:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">nums = 5</span></p>

<p><strong>Output:</strong> <span class="example-io">false</span></p>

<p><strong>Explanation:</strong></p>

<ul>
	<li>Binary representation of 5 is <code>101</code>.</li>
	<li>There are no consecutive set bits. Thus, the answer is <code>false</code>​​​​​​​.</li>
</ul>
</div>

<p>&nbsp;</p>
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= n &lt;= 10<sup>5</sup></code></li>
</ul>

