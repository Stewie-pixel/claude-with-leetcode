<h2><a href="https://leetcode.com/problems/keyboard-row">Keyboard Row</a></h2> <img src='https://img.shields.io/badge/Difficulty-Easy-brightgreen' alt='Difficulty: Easy' /><hr>


<p>Given an array of strings <code>words</code>, return <em>the words that can be typed using letters of the alphabet on only one row of American keyboard like the image below</em>.</p>

<p><strong>Note</strong> that the strings are <strong>case-insensitive</strong>, both lowercased and uppercased of the same letter are treated as if they are at the same row.</p>

<p>In the <strong>American keyboard</strong>:</p>

<ul>
	<li>the first row consists of the characters <code>"qwertyuiop"</code>,</li>
	<li>the second row consists of the characters <code>"asdfghjkl"</code>, and</li>
	<li>the third row consists of the characters <code>"zxcvbnm"</code>.</li>
</ul>
<img alt="" style="width: 800px; max-width: 600px; height: 267px;" src="https://assets.leetcode.com/uploads/2018/10/12/keyboard.png">
<p>&nbsp;</p>
<p><strong class="example">Example 1:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">words = ["Hello","Alaska","Dad","Peace"]</span></p>

<p><strong>Output:</strong> <span class="example-io">["Alaska","Dad"]</span></p>

<p><strong>Explanation:</strong></p>

<p>Both <code>"a"</code> and <code>"A"</code> are in the 2nd row of the American keyboard due to case insensitivity.</p>
</div>

<p><strong class="example">Example 2:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">words = ["omk"]</span></p>

<p><strong>Output:</strong> <span class="example-io">[]</span></p>
</div>

<p><strong class="example">Example 3:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">words = ["adsdf","sfd"]</span></p>

<p><strong>Output:</strong> <span class="example-io">["adsdf","sfd"]</span></p>
</div>

<p>&nbsp;</p>
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= words.length &lt;= 20</code></li>
	<li><code>1 &lt;= words[i].length &lt;= 100</code></li>
	<li><code>words[i]</code> consists of English letters (both lowercase and uppercase).&nbsp;</li>
</ul>
