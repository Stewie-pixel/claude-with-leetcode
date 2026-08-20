<h2><a href="https://leetcode.com/problems/cinema-seat-allocation">Cinema Seat Allocation</a></h2> <img src='https://img.shields.io/badge/Difficulty-Medium-orange' alt='Difficulty: Medium' /><hr><p><img alt="" src="https://assets.leetcode.com/uploads/2020/02/14/cinema_seats_1.png" style="width: 400px; height: 149px;" /></p>

<p>A cinema has <code>n</code> rows of seats, numbered from 1 to <code>n</code>. Each row has 10 seats, numbered from 1 to 10.</p>

<p>You are given a 2D integer array <code data-end="170" data-start="155">reservedSeats</code>, where <code data-end="212" data-start="178">reservedSeats[i] = [row<sub>i</sub>, seat<sub>i</sub>]</code> means that seat <code data-end="236" data-start="229">seat<sub>i</sub></code> in row <code data-end="250" data-start="244">row<sub>i</sub></code> is already reserved.</p>

<p>A four-person group must be assigned to four seats in the <strong>same</strong> row. The group can be seated in one of the following seat blocks:</p>

<ul>
	<li>seats <code data-end="423" data-start="411">2, 3, 4, 5</code></li>
	<li>seats <code data-end="444" data-start="432">4, 5, 6, 7</code></li>
	<li>seats <code data-end="465" data-start="453">6, 7, 8, 9</code></li>
</ul>

<p>A block can be used only if <strong>none</strong> of its seats are reserved. Each seat can be assigned to <strong>at most </strong>one group.</p>

<p>Return an integer denoting the <strong>maximum</strong> number of four-person groups that can be assigned.</p>

<p>&nbsp;</p>
<p><strong class="example">Example 1:</strong></p>

<p><img alt="" src="https://assets.leetcode.com/uploads/2020/02/14/cinema_seats_3.png" style="width: 400px; height: 96px;" /></p>

<pre>
<strong>Input:</strong> n = 3, reservedSeats = [[1,2],[1,3],[1,8],[2,6],[3,1],[3,10]]
<strong>Output:</strong> 4
<strong>Explanation:</strong> The figure above shows an optimal allocation of four groups. Seats marked in blue are already reserved, and each set of four contiguous seats marked in orange is assigned to one group.
</pre>

<p><strong class="example">Example 2:</strong></p>

<pre>
<strong>Input:</strong> n = 2, reservedSeats = [[2,1],[1,8],[2,6]]
<strong>Output:</strong> 2
</pre>

<p><strong class="example">Example 3:</strong></p>

<pre>
<strong>Input:</strong> n = 4, reservedSeats = [[4,3],[1,4],[4,6],[1,7]]
<strong>Output:</strong> 4
</pre>

<p>&nbsp;</p>
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 10<sup>9</sup></code></li>
	<li><code>1 &lt;= reservedSeats.length &lt;= min(10 * n, 10<sup>4</sup>)</code></li>
	<li><code>reservedSeats[i] == [row<sub>i</sub>, seat<sub>i</sub>]</code></li>
	<li><code>1 &lt;= row<sub>i</sub> &lt;= n</code></li>
	<li><code>1 &lt;= seat<sub>i</sub> &lt;= 10</code></li>
	<li>All <code>reservedSeats[i]</code> are distinct.</li>
</ul>

