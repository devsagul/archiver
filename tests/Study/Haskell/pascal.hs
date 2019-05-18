factorial :: Integer -> Integer
factorial 0 = 1
factorial n = product[1..n]

arrange :: Integer -> Integer -> Integer
arrange k n = product[(k+1)..n]

binominal :: Integer -> Integer -> Integer
binominal k n = if k > (n-k)
                then arrange k n `div` factorial (n-k)
                else arrange (n-k) n `div` factorial k

pascal :: Integer -> [Integer]
pascal k = [binominal m (k-1) | m <- [0..(k-1)]]
