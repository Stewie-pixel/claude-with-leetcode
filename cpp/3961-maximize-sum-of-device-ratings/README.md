<h2><a href="https://leetcode.com/problems/maximize-sum-of-device-ratings">Maximize Sum of Device Ratings</a></h2> <img src='https://img.shields.io/badge/Difficulty-Medium-orange' alt='Difficulty: Medium' /><hr><p>You are given a 2D integer array <code>units</code> of size <code>m &times; n</code> where <code>units[i][j]</code> represents the capacity of the <code>j<sup>th</sup></code> unit in the <code>i<sup>th</sup></code> device. Each device contains <strong>exactly</strong> <code>n</code> units.</p>

<p>The <strong>rating</strong> of a device is the <strong>minimum</strong> capacity among all its units.</p>

<p>You may perform the following operation any number of times (including zero):</p>

<ul>
	<li>Choose a device <code>i</code> that has <strong>not been</strong> used as a source before.</li>
	<li>Remove <strong>exactly</strong> one unit from device <code>i</code> and add it to <strong>any</strong> different device.</li>
	<li>Then mark device <code>i</code> as used, so it cannot be chosen again as a source.</li>
</ul>

<p>Return the <strong>maximum</strong> possible sum of the ratings of all devices after any number of such operations.</p>

<p><strong>Note:</strong></p>

<ul>
	<li>Devices can receive units from multiple devices, regardless of whether they have been selected.</li>
	<li>The rating of an empty device is 0.</li>
</ul>

<p>&nbsp;</p>
<p><strong class="example">Example 1:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">units = [[1,3],[2,2]]</span></p>

<p><strong>Output:</strong> <span class="example-io">4</span></p>

<p><strong>Explanation:</strong></p>

<ul>
	<li>​​​​​​​​​​​​​​Select device <code>i = 0</code> and transfer <code>units[0][0] = 1</code> to device <code>i = 1</code>.</li>
	<li>After the transfer, the ratings are:
	<ul>
		<li>Device <code>0 = [3]</code>: <code>rating[0] = 3</code></li>
		<li>Device <code>1 = [2, 2, <u>1</u>]</code>: <code>rating[1] = 1</code></li>
	</ul>
	</li>
	<li>Thus, the sum of ratings is <code>3 + 1 = 4</code>.</li>
</ul>
</div>

<p><strong class="example">Example 2:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">units = [[1,2,3],[4,5,6]]</span></p>

<p><strong>Output:</strong> <span class="example-io">6</span></p>

<p><strong>Explanation:</strong></p>

<ul>
	<li>Select device <code>i = 1</code> and transfer <code>units[1][0] = 4</code> to device <code>i = 0</code>.</li>
	<li>After the transfer, the ratings are:
	<ul>
		<li>Device <code>0 = [1, 2, 3, <u>4</u>]</code>: <code>rating[0] = 1</code></li>
		<li>Device <code>1 = [5, 6]</code>: <code>rating[1] = 5</code></li>
	</ul>
	</li>
	<li>Thus, the sum of ratings is <code>1 + 5 = 6</code>.</li>
</ul>
</div>

<p><strong class="example">Example 3:</strong></p>

<div class="example-block">
<p><strong>Input:</strong> <span class="example-io">units = [[5,5,5],[1,1,1]]</span></p>

<p><strong>Output:</strong> <span class="example-io">6</span></p>

<p><strong>Explanation:</strong></p>

<ul>
	<li>No transfers increase the sum of ratings. Thus, the sum of ratings is <code>5 + 1 = 6</code>.</li>
</ul>
</div>

<p>&nbsp;</p>
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= m == units.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= n == units[i].length &lt;= 10<sup>5</sup></code></li>
	<li><code>m * n &lt;= 2 * 10<sup>5</sup></code></li>
	<li><code>1 &lt;= units[i][j] &lt;= 10<sup>5</sup></code></li>
</ul>

