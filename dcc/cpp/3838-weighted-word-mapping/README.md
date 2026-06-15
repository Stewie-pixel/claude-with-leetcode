<h2><a href="https://leetcode.com/problems/weighted-word-mapping">Weighted Word Mapping</a></h2> <img src='https://img.shields.io/badge/Difficulty-Easy-brightgreen' alt='Difficulty: Easy' /><hr><p>You are given an array of strings <code>words</code>, where each string represents a word containing lowercase English letters.</p>

<p>You are also given an integer array <code>weights</code> of length 26, where <code>weights[i]</code> represents the weight of the <code>i<sup>th</sup></code> lowercase English letter.</p>

<p>The <strong>weight</strong> of a word is defined as the <strong>sum</strong> of the weights of its characters.</p>

<p>For each word, take its weight modulo 26 and map the result to a lowercase English letter using reverse alphabetical order (<code>0 -&gt; &#39;z&#39;, 1 -&gt; &#39;y&#39;, ..., 25 -&gt; &#39;a&#39;</code>).</p>

<p>Return a string formed by concatenating the mapped characters for all words in order.</p>

<p>&nbsp;</p>
<p><strong class="example">Example 1:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">words = [&quot;abcd&quot;,&quot;def&quot;,&quot;xyz&quot;], weights = [5,3,12,14,1,2,3,2,10,6,6,9,7,8,7,10,8,9,6,9,9,8,3,7,7,2]</span></p>

<p><strong>Output:</strong> <span class="example-io">&quot;rij&quot;</span></p>

<p><strong>Explanation:</strong></p>

<ul>
	<li>The weight of <code>&quot;abcd&quot;</code> is <code>5 + 3 + 12 + 14 = 34</code>. The result modulo 26 is <code>34 % 26 = 8</code>, which maps to <code>&#39;r&#39;</code>.</li>
	<li>The weight of <code>&quot;def&quot;</code> is <code>14 + 1 + 2 = 17</code>. The result modulo 26 is <code>17 % 26 = 17</code>, which maps to <code>&#39;i&#39;</code>.</li>
	<li>The weight of <code>&quot;xyz&quot;</code> is <code>7 + 7 + 2 = 16</code>. The result modulo 26 is <code>16 % 26 = 16</code>, which maps to <code>&#39;j&#39;</code>.</li>
</ul>

<p>Thus, the string formed by concatenating the mapped characters is <code>&quot;rij&quot;</code>.</p>
</div>

<p><strong class="example">Example 2:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">words = [&quot;a&quot;,&quot;b&quot;,&quot;c&quot;], weights = [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]</span></p>

<p><strong>Output:</strong> <span class="example-io">&quot;yyy&quot;</span></p>

<p><strong>Explanation:</strong></p>

<p>Each word has weight 1. The result modulo 26 is <code>1 % 26 = 1</code>, which maps to <code>&#39;y&#39;</code>.</p>

<p>Thus, the string formed by concatenating the mapped characters is <code>&quot;yyy&quot;</code>.</p>
</div>

<p><strong class="example">Example 3:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">words = [&quot;abcd&quot;], weights = [7,5,3,4,3,5,4,9,4,2,2,7,10,2,5,10,6,1,2,2,4,1,3,4,4,5]</span></p>

<p><strong>Output:</strong> <span class="example-io">&quot;g&quot;</span></p>

<p><strong>Explanation:​​​​​​​</strong></p>

<p>The weight of <code>&quot;abcd&quot;</code> is <code>7 + 5 + 3 + 4 = 19</code>. The result modulo 26 is <code>19 % 26 = 19</code>, which maps to <code>&#39;g&#39;</code>.</p>

<p>Thus, the string formed by concatenating the mapped characters is <code>&quot;g&quot;</code>.</p>
</div>

<p>&nbsp;</p>
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= words.length &lt;= 100</code></li>
	<li><code>1 &lt;= words[i].length &lt;= 10</code></li>
	<li><code>weights.length == 26</code></li>
	<li><code>1 &lt;= weights[i] &lt;= 100</code></li>
	<li><code>words[i]</code> consists of lowercase English letters.</li>
</ul>

