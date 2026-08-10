<h2><a href="https://leetcode.com/problems/rearrange-string-to-avoid-character-pair">Rearrange String to Avoid Character Pair</a></h2> <img src='https://img.shields.io/badge/Difficulty-Easy-brightgreen' alt='Difficulty: Easy' /><hr><p>You are given a string <code>s</code> and two <strong>distinct</strong> lowercase English letters <code>x</code> and <code>y</code>.</p>

<p>Rearrange the characters of <code>s</code> to construct a new string <code>t</code> such that:</p>

<ul>
	<li><code>t</code> is a <strong>permutation</strong> of <code>s</code>.</li>
	<li>Every occurrence of <code>y</code> appears <strong>before</strong> every occurrence of <code>x</code> in <code>t</code>.</li>
</ul>

<p>Return <strong>any</strong> valid string <code>t</code>.</p>

<p>A <strong>permutation</strong> is a rearrangement of all the characters of a string.</p>

<p>&nbsp;</p>
<p><strong class="example">Example 1:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">s = &quot;aabc&quot;, x = &quot;a&quot;, y = &quot;c&quot;</span></p>

<p><strong>Output:</strong> <span class="example-io">&quot;cbaa&quot;</span></p>

<p><strong>Explanation:</strong></p>

<p>The string <code>&quot;cbaa&quot;</code> is a permutation of <code>&quot;aabc&quot;</code>, and every occurrence of <code>&#39;c&#39;</code> appears before every occurrence of <code>&#39;a&#39;</code>.</p>
</div>

<p><strong class="example">Example 2:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">s = &quot;dcab&quot;, x = &quot;d&quot;, y = &quot;b&quot;</span></p>

<p><strong>Output:</strong> <span class="example-io">&quot;cabd&quot;</span></p>

<p><strong>Explanation:</strong></p>

<p>The string <code>&quot;cabd&quot;</code> is a permutation of <code>&quot;dcab&quot;</code>, and every occurrence of <code>&#39;b&#39;</code> appears before every occurrence of <code>&#39;d&#39;</code>.</p>
</div>

<p><strong class="example">Example 3:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">s = &quot;axe&quot;, x = &quot;o&quot;, y = &quot;x&quot;</span></p>

<p><strong>Output:</strong> <span class="example-io">&quot;axe&quot;</span></p>

<p><strong>Explanation:</strong></p>

<p>The string <code>&quot;axe&quot;</code> is already valid. Since <code>&#39;o&#39;</code> does not occur in the string, the required condition is automatically satisfied.</p>
</div>

<p>&nbsp;</p>
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 100</code></li>
	<li><code>s</code> consists of lowercase English letters.</li>
	<li><code>x</code> and <code>y</code> are lowercase English letters.</li>
	<li><code>x != y</code></li>
</ul>

