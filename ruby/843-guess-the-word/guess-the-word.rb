#    This is Master's API interface.
#    You should not implement it, or speculate about its implementation
#
# class Master
# =begin
#     :type word: String
#     :rtype: Integer
# =end
#     def guess(word)
#         ...
#     end
# end
#

# @param {String[]} words
# @param {Master} master
# @return {Void}
def get_matches(w1, w2)
  matches = 0
  (0...6).each do |i|
    matches += 1 if w1[i] == w2[i]
  end
  matches
end

def find_secret_word(words, master)
  pool = words.dup
  30.times do
    break if pool.empty?
    count = Array.new(6) { Array.new(26, 0) }
    pool.each do |w|
      (0...6).each do |i|
        count[i][w[i].ord - 'a'.ord] += 1
      end
    end
    best_guess = pool[0]
    max_score = -1
    pool.each do |w|
      score = 0
      (0...6).each do |i|
        score += count[i][w[i].ord - 'a'.ord]
      end
      if score > max_score
        max_score = score
        best_guess = w
      end
    end
    matches = master.guess(best_guess)
    return if matches == 6
    next_pool = []
    pool.each do |w|
      next_pool << w if get_matches(best_guess, w) == matches
    end
    pool = next_pool
  end
end